//*************************************************************************************************************
//*************************************************************************************************************
// File: cavai.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "sowgbaiinf.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"

#include "user-declarations.h"
#include "SOWWLMOD.h"

#define AI_GUARDYDS 50
#define GREATERTHREAT 2000
#define CAV_RECALCTICKS (TICSPERSEC/6)
#define DISTARTYHOLD 200
#define DIST2AMMOLOAD     160  // 40 yards squared
#define DISTCAV1AUTOMELEE 100

#define BUFF_CHECK(val) CXUtil::MultSpec(val,14,10)
#define COL_CHECK(val) CXUtil::MultSpec(val,11,10)

extern bool ObstacleAheadMe(CXUnit base);
extern bool GetCavCommand(CXOff lead, int state, SXComm& comm, CXUnit unit);
extern bool GetInfCommand(CXOff lead, int state, SXComm& comm, CXUnit unit);
extern bool IsColumn(CXUnit base);
extern bool CheckHaltLine(CXUnit base);
extern bool IsLine(CXUnit base);
extern bool StopOverlap(CXUnit base);
extern bool AdvanceFallbackTargetAI(CXUnit base, SEnemy* enemy);
extern bool StandTargetNoAmmoAI(CXUnit base, CXUnit closest, CXUnit soldmel);
extern bool MarchTargetAI(CXUnit base, SEnemyExtension*);

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckAllTargets
//*************************************************************************************************************
//*************************************************************************************************************

static bool CheckAllTargets(CXUnit base, SEnemy* enemy, CXUnit& closest, CXUnit& soldmel, int& mindist)
{
	int quad = 0;
	int dist;

	bool hdist;

	CXUnit sold;

	soldmel = NULL;
	closest = NULL;
	mindist = 0;

	base.TargBeg();
	//loop on all the possibile targets
	while (base.TargNext(sold, dist))
	{
		bool hdist = base.DoesShareHex(sold);

		// someone is meleeing us, we need to fight back
		if ( ( hdist || ( sold.Closest() == base && sold.State() == eUnitMelee ) ) && !sold.IsArt() &&
		     sold.State() != eUnitRetreat )
		{
			if ( base.MountState() == eMountMounted && base.GetUnitTypeVal(UVAL_CanCharge))
			{
				base.SetClosest(sold);

				base.ICommand(eComState, eUnitMelee);
			}
			else
				base.ICommand(eComRetreat);

			return false;
		}
		// can't fire at meleeing targets
		else if ( sold.State() == eUnitMelee && base.State() != eUnitMelee && base.State() != eUnitCharge )
		{
			soldmel = sold;

			continue;
		}
		else if ( !sold.CanFight() )
			continue;

		quad = base.GetQuad( sold );

		if ( !closest || dist < mindist )
		{
			closest = sold;
			mindist = dist;
		}

		// check closest in quad
		if ( !enemy[quad].closest || dist < enemy[quad].cdist )
		{
			enemy[quad].closest = sold;
			enemy[quad].cdist   = dist;
		} 
		// total
		// total
		int u = sold.NumMen();

		if ( sold.UnitType() == eUnitArt )
			u *= 100;

		enemy[quad].men  += u;
		enemy[quad].dist += u * dist;
	}

	sold = base.MainTarg();

	if ( sold )
	{
		if (!sold.CanTarget() || !sold.CanFight())
			base.SetMainTarg(NULL);
		else if ( !base.TargFind( sold, dist ) )
		{
			closest   = sold;
			base.SetClosest(NULL);
		}
		else
		{
			closest   = sold;
			base.SetClosest(sold);
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ChargeAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool ChargeAI(CXUnit base, int& mindist)
{
	bool bRes = true;
	bool boldgbsetting = base.GetUnitTypeVal(UVAL_CanCharge);

	SXComm comm;

	if ( boldgbsetting || base.State() == eUnitShoot || !base.EnemyClose() )
		return true;

	CXOff lead = base.Leader();

	// see if it's arty and if we should charge it
	if ( base.EnemyClose().UnitType() == eUnitArt && (!lead || (lead && mindist <= lead.ChargeArtyDistYds())) && !boldgbsetting )
	{
		if ( !lead || (lead && lead.CanICharge(base)) )
		{
			comm.Comm(eComCharge);

			base.Command(&comm);

			bRes = false;
		}
	}
	else if ( !base.EnemyClose().IsArt() && mindist <= DISTCAV1AUTOMELEE && !boldgbsetting )
	{
		if ( !lead || (lead && lead.CanICharge(base)) )
		{
			base.SetClosest(base.EnemyClose());

			comm.Comm(eComCharge);

			base.Command(&comm);

			bRes = false;
		}
		// if there is someone in charge distance, but we are too tired to charge, just ditch it and go rest
		else if ( lead )
		{
			comm.Comm(eComGetAway);

			base.Command(&comm);

			bRes = false;
		}
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandTargetAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool StandTargetAmmoAI(CXUnit base, SEnemy* enemy, CXUnit closest, const int& mindist, SEnemyExtension*)
{
	int i;
	int quad;
	int temp;
	int myquad = base.GetQuad(NULL);

	bool  bCanWheel  = false;
	bool  bMustWheel = false;
	SXComm comm;

	int threat = 0;

	if ( ( base.State() != eUnitStand && base.State() != eUnitShoot ) || base.Ammo() < 1 || closest == NULL )
		return true;

	// Make sure we are dismounted
	if ( base.MountState() != eMountUnmounted )
	{
		base.ICommand( eComSwitchCmn );

		return false;
	}

	// Make sure we can fire from the current formation
	if ( base.State() == eUnitStand && !base.IsFormType(eFTFight) )
	{
		comm.Comm(eComFormType);
		comm.Val(eFTFight);

		base.Command(&comm);

		return false;
	}

	// Getting our first target
	if ( base.Closest() == NULL )
	{
		base.WheelToTarget( closest );

		base.JustWheeled(1);

		base.SetClosest(closest);

		if ( base.UnderRecentFire() )
			base.ICommand( eComRun );

		return false;
	}
	base.SetClosest(closest);

	if ( base.MainTarg() )
	{
		// this should already be the target, but we'll double check
		closest = base.MainTarg();

		if (!closest.CanTarget())
			base.SetMainTarg(NULL);
		else
		{
			if ( !base.WheelToTarget( closest ) && base.State() == eUnitStand )
				base.ICommand( eComFire );
			else
				base.JustWheeled(1);

			return true;
		}
	}
	temp = base.GetLevel(eGameWheelDist);

	quad      = -1;
	bCanWheel = true;

	// Figure Averages
	for ( i = 0; i < eQuadMax; i++ )
	{
		if ( enemy[i].men == 0 )
			continue;

		enemy[i].dist	/= enemy[i].men;

		bCanWheel &= ( enemy[i].cdist > temp );

		enemy[i].threat = enemy[i].men - enemy[i].dist - enemy[i].cdist;

		if ( enemy[i].men > 0 && ( quad == -1 || enemy[i].threat > threat ) )
		{
			quad   = i;
			threat = enemy[i].threat;
		}
	}
	// check to see if we should wheel, we already have a target

	// No threat	
	if ( enemy[myquad].men < 1 || enemy[myquad].cdist > base.MaxRangeYds() )
		bMustWheel = true;

	// If we're too shaken to wheel, fallback
	if ( !base.CanWheel() )
	{
		if ( bMustWheel || CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics))
			base.ICommand( eComFallback );
		else if ( base.State() == eUnitStand )
			base.ICommand( eComFire );

		return false;
	}
	auto lead = base.Leader();

	// Check with our leader
	if ( lead && GetCavCommand(lead, eSTStandShooting, comm, base ) )
	{
		base.Command( &comm );

		return false;
	}

	// if we can wheel, but don't have to, make sure the threat is great enough
	if ( !bMustWheel && bCanWheel && enemy[myquad].threat + GREATERTHREAT > enemy[ quad ].threat )
		quad = myquad;
#ifdef _DEBUG
//	else if ( !bMustWheel && bCanWheel )
//	{
//		CWStdioFile file;
//		CFileStatus stat;
//
//		CWString str;
//
//		if ( !CWFile::GetStatus( "wheeling.txt", stat ) )
//		{
//			file.Open( "wheeling.txt", CWFile::modeCreate | CWFile::modeWrite );
//
//			file.WriteString( "FRONT,MAX,RIGHT,BACK,LEFT,DIFF\n" );
//		}
//		else
//		{
//			file.Open( "wheeling.txt", CWFile::modeWrite );
//
//			file.SeekToEnd();
//		}
//		str.Format( "%3.1f,%3.1f,%3.1f,%3.1f,%3.1f,%3.1f\n", enemy[0].threat, enemy[quad].threat, enemy[1].threat, enemy[2].threat, enemy[3].threat, enemy[quad].threat - enemy[0].threat );
//
//		file.WriteString( str );
//
//		file.Close();
//	}
#endif

	if ( quad == myquad && base.State() == eUnitShoot && CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics))
		base.WheelToTarget( enemy[myquad].closest );
	else if ( quad != myquad && ( bCanWheel || bMustWheel ) )
		base.WheelToTarget( enemy[quad].closest );

	if ( base.State() == eUnitStand )
		base.ICommand( eComFire );

	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandNoTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool StandNoTargetAI(CXUnit base, CXUnit closest, CXUnit soldmel, SEnemy*)
{
	int i;
	int temp;

	bool  bRes = true;
	bool  boldgbsetting = base.GetUnitTypeVal(UVAL_CanCharge);
	SXComm comm;

	int dist2;

	CXUnit ammo;

	if ( base.State() != eUnitStand || base.CurrTarg() || base.Closest() != NULL || closest != NULL )
		return bRes;

	auto lead = base.Leader();

	// only target is in melee
	if ( soldmel != NULL )
	{
		if ( lead && GetCavCommand( lead, eSTStandTargetOnlyMelee, comm, base ) )
		{
			if ( comm.Comm() == eComCharge )
			{
				base.SetClosest(soldmel);

				if ( !boldgbsetting )
					base.Command( &comm );
			}
			else
				base.Command( &comm );

			return false;
		}
	}

	if ( base.Ammo() == 0 || (lead && lead.ShouldGetAmmo( base )) )
	{
		CXVec loc;
		
		bool bFound = base.Leader().FindClosestSpec(eUnitAmmo, base.Loc(), loc);

		if ( bFound && base.Loc().Dist2Yds(loc) > DIST2AMMOLOAD)
		{
			base.ICommand(eComMove, loc);
			base.ICommand(eComFormType, eFTMarch);

			temp = base.GetMod(eGameRunForAmmo);

			if (lead)
				temp += lead.GetMod(eGameRunForAmmo);

			if ( temp >= 0 )
				base.ICommand(eComRun);

			bRes = false;
		}
	}
	else if ( lead && GetCavCommand( lead, eSTStandNoTarget, comm, base ) )
	{
		base.Command( &comm );

		bRes = false;
	}
	else if ( base.UnderFire1() && base.NumTargs() == 0 )
	{
		base.KillTime(0); // so we don't do this again based on the same kill
	
		if ( lead && GetCavCommand( lead, eSTStandNoTargetFire, comm, base ) )
		{
			base.Command( &comm );

			bRes = false;
		}
		else if ( base.EnemyClose().Valid() )
		{
			dist2 = base.EnemyCloseYds(); // not really squared

			if ( dist2 <= DISTCAV1AUTOMELEE )
				base.TargAdd( base.EnemyClose(), dist2 );

			bRes = ChargeAI(base.EnemyClose(), dist2 );

			if ( bRes ) // we did nothing, let's not just stand here
			{
				i = CXUtil::Ran( 98, 0, 100 );

				if ( i <= base.GetMod(eGameRetreatPct) )
				{
					base.ICommand( eComGetAway, 1 );
				
					bRes = false;	
				}
			}
		}
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CavRaidAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool CavRaidAI(CXUnit base)
{
	bool bNeed = true;

	CXUnit sold;
	CXUnit best;
	
	int dist;
	int bdist  = 0.0;
	int wdist2 = base.MaxRangeYds();

	wdist2 *= wdist2;

	if ( base.FollowTarg() )
	{
		if (!base.EnemyFind(base.FollowTarg(), dist))
			base.SetFollow(NULL);
		else if (base.FollowTarg().IsOfficer())
			base.SetFollow(NULL);
	}
	auto lead = base.Leader();

	if (base.FollowTarg() && lead && !base.FollowTarg().HasInfOrArtFriends( wdist2, lead.Personality() ) )
	{
		if (base.FollowTarg().UnitType() == eUnitInf || base.FollowTarg().UnitType() == eUnitCav )
		{
			if ( lead && lead.ShouldAttack( true, base.FollowTarg(), base, false ) )
			 	bNeed = false;
		}
		else
			bNeed = false;
	}
		
	if ( bNeed )
	{
		base.EnemyBeg();

		sold = NULL;

		while (base.EnemyNext(sold, dist))
		{
			if ( best && bdist < dist )
				continue;

			if ( sold.IsOfficer() )
				continue;

			if ( lead && !sold.HasInfOrArtFriends( wdist2, lead.Personality() ) )
			{
				if ( sold.UnitType() == eUnitInf || sold.UnitType() == eUnitCav )
				{
					if ( lead.ShouldAttack( true, sold, base, false ) )
					{
						best  = sold;
						bdist = dist;
					}
				}
				else
				{
					best  = sold;
					bdist = dist;
				}
			}
		}

		if ( best )
			bNeed = false;

		base.SetFollow(best);
	}
	return !bNeed;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: FollowAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool FollowAI(CXUnit base)
{
	bool bRes = true;
	CXVec loc;

	SXComm comm;

	if ( base.FollowTarg() )
	{
		if ( base.NumTargs() == 1 && base.FollowType() == eFollowRaid )
			base.SetFollow( base.TargClose());

		if ( base.FollowType() == eFollowGuard )
			loc = base.FollowTarg().GetLocBehind( AI_GUARDYDS );
		else
			loc = base.FollowTarg().Loc();

		if ( base.FollowTarg().Hidden() )
		{
			base.SetFollow(NULL);
			return bRes;
		}
	}
	else if ( base.State() == eUnitStand && (base.FollowType() == eFollowScout || 
		        base.FollowType() == eFollowRaid || base.FollowType() == eFollowScreen ) )
	{
		// I've made it, let's get a new spot
		if ( base.Loc() == base.FollowLoc() && base.Leader() )
		{
			loc = base.FollowLoc();
			base.Leader().GetScoutLoc(base, loc, false);

			base.SetFollowLoc(loc);
		}
		loc = base.FollowLoc();
	}
	else
		loc = base.Dst();
	
	if ( base.State() == eUnitStand && base.Loc() != loc )
		base.FollowTics(CAV_RECALCTICKS + 5);
	else if ( base.Dst() != loc )
		base.FollowTics(base.FollowTics() + 1);

	if ( base.FollowTics() > CAV_RECALCTICKS )
	{
		base.FollowTics(0);

		base.ICommand(eComMove, loc);

		if (base.FollowType() == eFollowGuard )
			base.ICommand(eComEndDir, base.FollowTarg().Dir());
		else if ( !base.FollowTarg() )
			base.ICommand( eComUseRoad );
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GuardAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool GuardAI(CXUnit base)
{
	bool bNeed = false;

	CUnit* sold = NULL;

	if (base.State() == eUnitCharge || base.State() == eUnitMelee)
		bNeed = true;
	else if (!base.FollowTarg())
		bNeed = true;
	else if (base.FollowTarg().Rank() == eRankBrig || !base.FollowTarg().IsOfficer())
	{
		sold = base.FollowTarg().GetCharger();

		if (sold)
		{
			base.SetClosest(sold);

			base.ICommand(eComState, eUnitCharge);

			bNeed = true;
		}
	}
	return bNeed;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: FollowTargAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool FollowTargAI(CXUnit base)
{
	bool   bRes = true;
	CXUnit sold;

	int dist;
	int distr = 0;
	int fdist = 0;

	sold = base.EnemyClose();
	dist = base.EnemyCloseYds();

	if ( sold )
		distr = base.Loc().DistYds( sold.Loc() );

	// if we can't see who we were following
	if ( base.FollowTarg() && base.FollowType() != eFollowGuard && !base.EnemyFind(base.FollowTarg(), fdist ) )
	{
		base.SetFollow(NULL);
		bRes     = false;
	}
	else if (base.FollowType() == eFollowGuard && GuardAI(base) )
	{
	}
	else if (base.FollowType() == eFollowRaid && CavRaidAI(base) )
	{
		// cavraidai can change m_follow
		if (base.FollowTarg())
			distr = base.Loc().DistYds( base.FollowTarg().Loc() );

		if (base.FollowTarg() && distr < base.MaxRangeYds() && base.State() != eUnitCharge && 
			  base.CanCharge( base.FollowTarg() ) && !base.GetUnitTypeVal(UVAL_CanCharge))
		{
			base.SetClosest(base.FollowTarg());
			base.ICommand(eComState, eUnitCharge );	
			bRes = false;
		}
		else if ( base.State() != eUnitRun && base.Moving() )
		{
			base.ICommand( eComRun );
			bRes = false;
		}
		else if ( base.UnderRecentFire() && !base.Moving() )
		{
			base.ICommand( eComGetAway );
			bRes = false;
		}
	}
	else if (base.FollowType() == eFollowScreen )
	{
		if ( sold && distr < base.LongRangeYds() && !base.Moving() )
		{
			// get outta here
			base.ICommand( eComGetAway );
			bRes = false;
		}
		else if ( base.Moving() && sold && distr < base.MaxRangeYds() )
		{
			// Stop, I'm close enough
			base.ICommand( eComHalt );
			bRes = false;
		}
		else if (base.FollowTarg() && !base.TargClose() && base.Moving() && base.State() != eUnitRun )
			base.ICommand( eComRun );
		else if ( !base.FollowTarg() && sold )
			base.SetFollow(sold);
	}
	else if (base.FollowType() == eFollowScout || base.FollowType() == eFollowRaid )
	{
		if (sold && dist < 300 || base.UnderFire())
		{
			// get outta here
			base.ICommand(eComGetAway);
			bRes = false;
		}
		else if (base.FollowTarg() && fdist < 500)
		{
			// Stop, I'm close enough
			base.ICommand(eComHalt);
			bRes = false;
		}
		else if (!base.FollowTarg())
			base.SetFollow(sold);
	}	
	return bRes;
}

//*************************************************************************************************************

static bool MoraleAI(CXUnit base)
{
	int  i;
	bool bRes = true;
	bool bDanger;
	bool bRouteCheck = base.CheckRouting();    //checking on the possibility to go in rout
	SXComm comm;

	bDanger = ((base.UnderFire() || base.CurrTarg()) && (base.NumTargs() || base.Blocked()));

	// Routed units
	if (base.Morale() <= MORAL_ROUTED)   //if the morale is under this level the unit goes in rout
	{
		if (base.State() != eUnitRouted)
			base.ICommand(eComRoute, 1);

		bRes = false;
	}
	else if (bRouteCheck)   //checking on the possibility to go in rout
	{
		base.RouteUnit();
		bRes = false;
	}
	// Broken units
	else if (base.Morale() <= MORAL_BROKEN)     //if the morale is under this level the unit ca go in retreat
	{
		if ((base.State() != eUnitRetreat || !base.Moving()) && bDanger)  //if the unit is in danger and is not retreating or moving she goes in retreat
		{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand(eComRetreat, 1);
		}
		base.SetClosest(NULL);

		bRes = false;
	}
	// Check fallback, retreat after drops in morale
	else if (base.PrevMor() > base.Morale() && bDanger)    //decrease of morale and danger, check on the percentage of falling back and retreating 
	{
		if (base.State() != eUnitRetreat)
		{
			i = CXUtil::Ran(98, 0, 100);  //random perc. to determine the fallback or retreat (from unitattributes.csv)

			if (base.State() != eUnitFallback && base.State() != eUnitMelee && base.State() != eUnitCharge && base.State() != eUnitRun &&
				i <= base.GetMod(eGameFallbackPct) && base.CurrTarg())
			{
				if (IsColumn(base))   //is column retreat
				{
					base.SetClosest(NULL);
					base.SetMainTarg(NULL);
					base.ICommand(eComRetreat);
					return false;
				}
				if (base.Split() == -1)				  //normal fallback
					base.ICommand(eComFallback, 1);
				if (base.Split() != -1)				//skirmishers company return to the parent
				{
					base.SetClosest(NULL);
					base.SetMainTarg(NULL);
					base.ICommand(eComUnSplit);
				}
				bRes = false;
			}
			else if (i <= base.GetMod(eGameRetreatPct))
			{
				base.SetClosest(NULL);
				base.SetMainTarg(NULL);
				if (base.Split() == -1)				  //new skirmishers
					base.ICommand(eComRetreat, 1);
				if (base.Split() != -1)				//skirmishers company return to the parent
					base.ICommand(eComUnSplit);
				bRes = false;
			}
		}
	}

	if (!bRes)
	{
		base.SetClosest(NULL);
		base.SetMainTarg(NULL);
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MeleeAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool MeleeAI(CXUnit base)
{
	bool bRes = true;
	SXComm comm;
	bool bHalt = false;

	// If Meleeing
	if (base.State() == eUnitMelee || base.State() == eUnitCharge)    //only if melee or charge
	{
		bRes = false;

		if (!base.Closest().Valid())      //the closest unit is not valid I stop
		{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand(eComHalt);
			bHalt = true;
		}
		else if (base.State() == eUnitCharge && !base.CanCharge(base.Closest()))  //i'm charging but i can charge the closest, I stop
		{
			base.SetMainTarg(NULL);
			base.SetClosest(NULL);
			base.ICommand(eComHalt);
			bHalt = true;
		}
		else if (base.State() == eUnitMelee && !base.CanCharge(base.Closest(), true))   //i'm meleing but i can charge the closest, I stop
		{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand(eComHalt);
			bHalt = true;
		}
		else if (base.State() == eUnitMelee && base.Closest().Valid() && base.Closest().State() == eUnitRetreat) //i'm meleing but the closest is retreating i ask to leader if I have to stop
		{
			// this is really for cav
			if (base.Leader().Valid() && !base.Leader().CanICharge(base))  //i stop the charge
			{
				base.SetMainTarg(NULL);
				base.SetClosest(NULL);
				base.ICommand(eComHalt);
				bHalt = true;
			}
			else if (base.TargClose().Valid() && !base.TargClose().Retreating() && base.DoesShareHex(base.TargClose()))  //if during the pursuit I overlap with another I switch melee on him
			{
				base.SetClosest(base.TargClose());
			}
			else   //i halt
			{
				base.SetMainTarg(NULL);
				base.SetClosest(NULL);
				base.ICommand(eComHalt);
				bHalt = true;
			}
		}
		if (bHalt && base.EnemyClose() && IsLine(base) && !CheckHaltLine(base))  //after a melee I form in columns for avoid contacts deploying in line, I check the space around me
		{
			comm.Comm(eComFormType);
			comm.Val(eFTAssault);
			base.Command(&comm);
		}
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MarchNoTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool MarchNoTargetAI(CXUnit base)
{
	bool bRes = true;
	SXComm comm;
	int diff = INT_MAX;

	if (base.CurrTarg())   //no targets
		return bRes;

	//------angle with the enemy close
	if (base.EnemyClose() && base.EnemyCloseYds() <= (BUFF_CHECK(base.MaxRangeYds())))
		diff = base.Pos().GetAngleDiff(base.EnemyClose().Loc(), true);
	//----------------------------------------

	if (base.State() == eUnitFallback)    //falling back if no more danger, i simply let her run
	{
		if (!base.UnderFire() || base.NumTargs() == 0)
			base.ICommand(eComGetAway);  //try the getaway at place of run to avoid strange path  
		//			base.ICommand( eComJustRun );  //just run stop the fallback and transform it in a run

		bRes = false;
	}
	else if (base.State() == eUnitAdvance)   //if advancing but no target if I can stop here i stop.
	{
		if (base.HaltTics() <= 0 && base.ValidDest(base.Loc()))
			base.ICommand(eComHalt);

		bRes = false;
	}
	else if (!base.Marching())   //if no marching I go out
		return bRes;
	else if (base.IsFormType(eFTMarch) && (diff <= 90 || diff >= 270) && base.EnemyClose().Valid() && base.EnemyCloseYds() < BUFF_CHECK(base.MaxRangeYds()))  //i form in fighting formation if enemy is near
	{
		// Check with our leader
		//this function should order to march in a formation different from that of march
		if (base.Leader().Valid() && GetInfCommand(base.Leader(), eSTMarchNoTarget, comm, base))  //determine formation to use
		{
			if (comm.Comm() != eComMax)
				base.Command(&comm);
		}
		else
			base.ICommand(eComFormType, eFTFight);  //if not stance I use the nomrla fighting formation

		if (base.UnderRecentFire())  //under recent fire mean i received a kill in the previous frame
			base.ICommand(eComRun);

		bRes = false;
	}
	else if (!base.ValidSpot(base.Dst()))    //if the destination is not a valid spot (cant halt here)
	{
		if (base.EnemyClose().Valid() && base.EnemyCloseYds() < COL_CHECK(base.MaxRangeYds()))  //if there is a valid enemy close, then I halt if i can here
		{
			if (base.HaltTics() <= 0 && base.ValidDest(base.Loc()))
				base.ICommand(eComHalt);
		}
		// if the enemy isn't close, just keep marching there, hopefully it will be ok when we get there
		bRes = false;
	}
	else if (base.EnemyClose() && (base.EnemyCloseYds() <= (BUFF_CHECK(base.MaxRangeYds())) || base.Leader().CurrTarg()) && ObstacleAheadMe(base))    //if near the enemy and path blocked I check the options
	{
		//I repath or check the path inside the buffer zone: so my only options are change direction or stop behind the friend
		bRes = false;
	}
	else if (StopOverlap(base))
	{
		bRes = false;
	}
	else if (base.IsSprinting() && base.State() == eUnitRun)  //if I did already a cycle running I return to walk
	{
		base.IsSprinting(0);
		base.ICommand(eComWalk);
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

static bool Think(int /*ticks*/, CXUnit base)
{
	int mindist;

	CXUnit soldmel;
	CXUnit closest;

	SEnemy enemy[ eQuadMax ];

	memset( enemy, 0, sizeof enemy );

	if ( base.RunTimer() != 0 && base.RunTimer() < CXUtil::GetTime() && base.MountState() == eMountUnmounted )
	{
		base.RunTimer(0);
		base.SwitchCmn();
	}

	if ( base.State() == eUnitRouted )
		return true;

	if ( !MoraleAI(base) ) // routed will always go here
		return true;

	if ( base.State() == eUnitRetreat )
		return true;

	// If Meleeing
	if ( !MeleeAI(base) )
		return true;

	if ( base.FollowType() != eFollowNone && base.Morale() <= MORAL_CANTWHEEL )
	{
		base.SetFollow(NULL);
		base.FollowType(eFollowNone);

		if ( !base.Attached() )
			base.ICommand( eComAttach );
	}

	if ( (base.FollowType() != eFollowNone && !base.TargClose() && !base.Blocked() ) || (base.FollowType() == eFollowRaid && base.NumTargs() == 1 ) )
		FollowAI(base);

	if ( base.FollowType() != eFollowNone )
		if ( !FollowTargAI( base ) )
			return true;

	// If no targets
	if ( !base.CurrTarg() )
	{
		if (base.AttackMarch())
			return true;

		if ( !StandNoTargetAI( base, NULL, NULL, enemy ) )
			return true;

		if ( !MarchNoTargetAI(base) )
			return true;

		if ( base.State() == eUnitShoot )
			base.ICommand( eComHalt );

		base.SetClosest(NULL);

	 	return true;
	}
	
	if ( !CheckAllTargets( base, enemy, closest, soldmel, mindist ) )
		return true;

	if (base.MainTarg().Valid() && closest && mindist < DISTARTYHOLD)
		base.SetMainTarg(NULL);

	if ( base.AttackMarch() )
		return true;

	if ( base.FormCloseYds() > 0 && base.FormCloseYds() >= mindist )
	{
		base.ICommand( eComGetAway );
		return true;
	}

	// run again to see if we can charge
	if ( !StandNoTargetAI( base, closest, soldmel, enemy ) )
		return true;

	if ( !AdvanceFallbackTargetAI( base, enemy ) )
		return true;

	if ( !ChargeAI( base, mindist ) )
		return true;

	if ( !StandTargetNoAmmoAI( base, closest, soldmel ) )
		return true;

	if ( !StandTargetAmmoAI( base, enemy, closest, mindist, NULL ) )
		return true;

	if ( !MarchTargetAI(base, NULL) )
		return true;

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MyThink
//*************************************************************************************************************
//*************************************************************************************************************

static bool MyThink(int, CXUnit base)
{
	CXUnit soldmel;
	CXUnit closest;
	SXComm comm;

	int mindist;
	SEnemy enemy[eQuadMax];

	memset(enemy, 0, sizeof enemy);

	if (base.State() == eUnitRouted)
		return true;

	if (!MoraleAI(base))
		return true;

	if (base.State() == eUnitRetreat)
	{
		if (!base.ValidDest(base.Dst()))
		{
			CXPos pos = base.DeP();

			if (base.GetValidDest(base.DeP(), &pos.Loc(), true))
				base.SetDest(pos);
		}
		return true;
	}

	// If Meleeing
	if (!MeleeAI(base))
		return true;

	// If no targets
	if (!base.CurrTarg())
	{
		if (base.State() == eUnitShoot)
			base.ICommand(eComHalt);

		base.SetClosest(NULL);

		return true;
	}

	if (!CheckAllTargets(base,enemy, closest, soldmel, mindist))
		return true;

	if (base.State() == eUnitStand && base.CurrTarg() && base.Ammo() > 0)
	{
		if (!base.CanFormFire())
			base.ICommand(eComFormType, eFTFight);
		else
		{
			base.SetClosest(closest);

			base.ICommand(eComFire);
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SowAIFunc
//*************************************************************************************************************
//*************************************************************************************************************

int SowCavAIFunc(int ticks, void* punit)
{
	int res;

	CXUnit unit((CUnit*)punit);

	if (unit.PlyrCtrl())
		res = MyThink(ticks, unit);
	else
		res = Think(ticks, unit);

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

