//*************************************************************************************************************
//*************************************************************************************************************
// File: soldai.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "sowgbaiinf.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"

#include "user-declarations.h"
#include "SOWWLMOD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define AI_ARTYRAD 125
#define ARTGREATERTHREAT 4000
#define CLOSESUPPORT 250


extern bool GetArtCommand( CXOff lead, int state, SXComm &comm, CXUnit unit );

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckAllTargets
//*************************************************************************************************************
//*************************************************************************************************************

static bool CheckAllTargets( CXUnit base, SEnemy *enemy, CXUnit &closest, CXUnit &soldmel, int &mindist, SEnemyExtension *enemyextension )
{
	int quad = 0;
	int dist;
	int myquad = base.GetQuad(NULL); 
	int diff;

	CXUnit sold;
	CXUnit main = NULL;
	
	mindist = 0;
	soldmel = NULL;
	closest = NULL;

	base.TargBeg();

	while ( base.TargNext( sold, dist ) )
	{
		if ( sold == base.MainTarg() )
			main = sold;

		// We need to add something here to capture the gun
		// can't fire at meleeing targets
		if ( sold.State() == eUnitMelee && sold.Closest() != base )
		{
			soldmel = sold;

			continue;
		}
		else if ( !sold.CanFight() )
			continue;

		if ( base.MainTarg().Valid() && sold == base.MainTarg() )
		{
			// always add the target
		}
		else
		{
			switch ( base.FireMode() )  //selective target  on the base of target mode
			{
				case eFConserve: 
					if ( dist > base.DistArtyConsv() )
						continue;
					break;

				case eFTroops: 
					if ( sold.UnitType() != eUnitInf && sold.UnitType() != eUnitCav )
						continue;
					break;
//----added specific target cavalry-------------------------
				case eFCav: 
					if ( sold.UnitType() != eUnitCav )
						continue;
					break;
//--------------------------------------------------------
				case eFArty:
					if ( sold.UnitType() != eUnitArt )
						continue;
					break;
			}
		}
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

// frontal target----------------------

		diff = base.Pos().GetAngleDiff(sold.Loc(),true);  //check angle of frontal enemies to avoid lateral movement
		if ( diff <= 45 || diff >= 315) 
		{
			if (!sold.CanTarget())
				continue;
			
			if ( !enemyextension[myquad].closesttarget || dist < enemyextension[myquad].closesttargetdist)
			{
				enemyextension[myquad].closesttarget = sold;
				enemyextension[myquad].closesttargetdist  = dist;
			}
		}
//----------------------------------------------------
		// total
		enemy[quad].men  += sold.NumMen();
		enemy[quad].dist += sold.NumMen() * dist;
	}

//priority to the frontal targets
	if (enemyextension[myquad].closesttarget && mindist>=CLOSESUPPORT )  
	{
		closest=enemyextension[myquad].closesttarget;
		mindist=enemyextension[myquad].closesttargetdist;
	}
//------------------------

	if ( base.MainTarg() )
	{
		sold = base.MainTarg();

		if ( !sold.CanTarget() )
		{
			base.SetMainTarg(NULL);
			base.SetClosest(NULL);
		}
		else if ( !main )
		{
//			closest   = sold;
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
		}
		else
		{
			closest   = sold;
			base.SetClosest(sold);
		}
	}
	else if (base.Closest())
		base.SetClosest(NULL);  //to be sure to not remain fixed to a target out of range

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ShouldRunAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool ShouldRunAI( CXUnit base, CXUnit , const int & )
{
	int sup = 0;
	int dist;
	SXComm comm;

	CXUnit sold;
	
	int panicdistance = base.DistArtyPanic();


	if (!base.EnemyClose())  //I try this change for avoid a targeting effect can generate a too near fallback
		return true;

	// Check with our leader
	if (base.Leader().Valid())   //ask for the charge order
	{
		//this function should order to conitnue the attacking march or not
		if ( GetArtCommand(base.Leader(), eSTStandArtilleryFallBack, comm, base ) )
		{	
			panicdistance = comm.Val();
		}
	}

	if (base.EnemyCloseYds() > panicdistance)  //no need to make other checks if the enemy is far in any case.
		return true;

	base.FriendBeg();
//if enemy too near and not support from friend the guns retreat
	while ( base.FriendNext( sold, dist ) )
	{
		if ( sold.CanFight() && (sold.UnitType() == eUnitCav || sold.UnitType() == eUnitInf) && dist < base.EnemyCloseYds() )  //teh friend need to be nearest the enemy
			++sup;
	}	



	if ( base.EnemyCloseYds() < panicdistance && (base.MorUnitBon() == 0 || sup == 0) && !base.EnemyClose().Routed() && !base.EnemyClose().Retreating() )
	{
		switch ( base.LimState() )
		{
			case eLimUnLimbered:
			case eLimUnLimbering:
				base.Limber();
				return false;
				break;
			case eLimLimbered:
				base.ICommand(eComHalt);
				base.ICommand( eComRetreat );
				return false;
				break;
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MoraleAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool MoraleAI( CXUnit base )
{
	int  i;
	bool bRes = true;
	bool bDanger;
	bool bRouteCheck = base.CheckRouting();
																 	 
	bDanger = (base.UnderFire() || base.CurrTarg()) && ( base.NumTargs() || base.Blocked() );

	// Routed units
	if ( base.Morale() <= MORAL_ROUTED )
	{
		if ( base.State() != eUnitRouted )
			base.ICommand( eComRoute, 1 );

		bRes = false;
	}
	else if ( bRouteCheck )
	{
		base.RouteUnit();
		bRes = false;
	}
	// Broken units
	else if ( base.Morale() <= MORAL_BROKEN )
	{
		if ( (base.State() != eUnitRetreat || !base.Moving()) && bDanger )
			base.ICommand( eComRetreat, 1 );

		base.SetClosest(NULL);

		bRes = false;
	}
	// Check fallback, retreat after drops in morale
	else if ( base.PrevMor() > base.Morale() && bDanger )
	{
		if ( base.State() != eUnitRetreat )
		{
			i = CXUtil::Ran( 98, 0, 100 );

			if ( base.State() != eUnitFallback && base.State() != eUnitMelee && base.State() != eUnitCharge &&
				   i <= base.GetMod(eGameFallbackPct) && base.CurrTarg() )
			{
				base.ICommand( eComFallback, 1 );
				
				bRes = false;	
			}
			else if ( i <= base.GetMod(eGameRetreatPct) )
			{
				base.ICommand( eComRetreat, 1 );
			
				bRes = false;	
			}
		}
	}

	if ( !bRes )
		base.SetClosest(NULL);

	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandTargetAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool StandTargetAmmoAI( CXUnit base, SEnemy *enemy, CXUnit closest, SEnemyExtension * )
{
	int i;
	int quad;
	int temp;
	int myquad;

	bool  bCanWheel  = false;
	bool  bMustWheel = false;
	SXComm comm;
	int maindist = 0;
	int threat = 0;

	if ( ( base.State() != eUnitStand && base.State() != eUnitShoot ) || base.Ammo() < 1 || closest == NULL )
		return true;

//retreat or limbering if the enemy is too near and I'm without morale bonus
	if ( base.TargCloseYds() < AI_ARTYRAD && base.MorUnitBon() == 0 && !closest.Routed() && !closest.Retreating() )
	{
		switch ( base.LimState() )
		{
			case eLimUnLimbering:
				base.Limber();
			case eLimLimbered:
				base.ICommand( eComRetreat );
				return false;
				break;
		}
	}

	// make sure that we are unlimbered
	if ( base.LimState() != eLimUnLimbered )
	{
		switch ( base.LimState() )
		{
			case eLimLimbered : 
				base.Limber();
				break;
		}
		return false;
	}

	// Make sure we can fire from the current formation
	if ( base.State() == eUnitStand && !base.CanFormFire() )
	{
		base.ICommand( eComFormType, eFTFight );

		return false;
	}



	// Getting our first target
	if ( !base.Closest().Valid() )   
	{
		base.WheelToTarget( closest );

		base.SetClosest(closest);
		base.SetMainTarg(closest);

		if ( base.UnderRecentFire() )
			base.ICommand( eComRun );

		return false;
	}


	if ( base.MainTarg() )
	{
		// this should already be the target, but we'll double check
		closest = base.MainTarg();

		if ( !closest.CanTarget() )
			base.SetMainTarg(NULL);
		else
		{
			base.TargFind(closest,maindist);
			if ( !base.WheelToTarget( closest ) && base.State() == eUnitStand && maindist<=base.MaxRangeYds())
				base.ICommand( eComFire );

			return true;
		}
	}

    

	temp = base.GetMod( eGameWheelDist );

	quad      = -1;
	myquad    = base.GetQuad(NULL);
	bCanWheel = true;

	// Figure Averages
	for ( i = 0; i < eQuadMax; i++ )
	{
		if ( enemy[i].men == 0 )
			continue;

		enemy[i].dist /= enemy[i].men;

		bCanWheel &= ( enemy[i].cdist > temp );

		enemy[i].threat = enemy[i].men - enemy[i].dist - enemy[i].cdist;

		if ( enemy[i].men > 0 && ( quad == -1 || enemy[i].threat > threat ) )
		{
			quad   = i;
			threat = enemy[i].threat;
		}
	}

	if ( quad == -1 )
		quad = myquad;

	// choose ammo on the base of distance
	base.ChooseAmmo( enemy[myquad].cdist, enemy[myquad].closest );
	
	if ( base.Ammo() <= 0 && base.TargCloseYds() < base.DistArtyPanic() )
	{
		base.ICommand( eComRetreat );

		base.SetClosest(NULL);

		return false;
	}
	// No threat	
	// check to see if we should wheel, we already have a target
	if ( enemy[myquad].men < 1 || enemy[myquad].cdist > base.MaxRangeYds() )
		bMustWheel = true;


	// If we're too shaken to wheel
	if ( !base.CanWheel() ) //if frontal target I force to remain fixed
	{
		if ( base.State() == eUnitStand && base.TargCloseYds()<=base.MaxRangeYds())
			base.ICommand( eComFire );

		return false;
	}


	// Check with our leader
	if ( base.Leader().Valid() && GetArtCommand( base.Leader(), eSTStandShooting, comm, base ) )
	{
		if (comm.Comm() != eComMax)
			base.Command( &comm );

		return false;
	}

	// if we can wheel, but don't have to, make sure the threat is great enough
	if ( !bMustWheel && bCanWheel && enemy[myquad].threat + ARTGREATERTHREAT > enemy[ quad ].threat )
		quad = myquad;

	if ( quad == myquad && base.State() == eUnitShoot && CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics) && enemy[myquad].closest)
	{
		base.WheelToTarget( enemy[myquad].closest );
	}
	else if ( quad != myquad  && ( bCanWheel || bMustWheel ) && enemy[quad].closest)
	{

		base.ChooseAmmo( enemy[quad].cdist, enemy[quad].closest );

		base.WheelToTarget( enemy[quad].closest );
	}

	if ( base.State() == eUnitStand && base.TargCloseYds()<=base.MaxRangeYds())
		base.ICommand( eComFire );



	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MeleeAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool MeleeAI( CXUnit base )
{
	bool bRes = true;

	// If Meleeing
	if ( base.State() == eUnitMelee || base.State() == eUnitCharge )
	{
		bRes = false;

		if ( !base.Closest().Valid() )
		{
			base.ICommand( eComHalt );

			base.SetClosest(NULL);
		}
		else if ( base.State() == eUnitCharge && !base.CanCharge( base.Closest() ) )
		{
			base.ICommand( eComHalt );

			base.SetClosest(NULL);
		}
		else if ( base.State() == eUnitMelee && !base.CanCharge( base.Closest(), true ) )
		{
			base.ICommand( eComHalt );

			base.SetClosest(NULL);
		}
		else if ( base.State() == eUnitMelee && base.Closest() && base.Closest().State() == eUnitRetreat )
		{
			// this is really for cav
			if ( base.Leader().Valid() && !base.Leader().CanICharge(base) )
			{
				base.ICommand( eComHalt );

				base.SetClosest(NULL);
			}
		}
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandTargetNoAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool StandTargetNoAmmoAI( CXUnit base, CXUnit closest, CXUnit soldmel)
{
	bool  bRes = true;
	SXComm comm;

	// Just do above check to see if we have to melee
	if ( ( base.State() != eUnitStand && base.State() != eUnitShoot ) || base.Ammo() >= 1 )
		return true;  	


	// See if we can charge someone who is already meleeing
	// or we have a target but no ammo
	if ( ( soldmel != NULL || closest != NULL ) && base.Leader().Valid() && GetArtCommand( base.Leader(), eSTStandTargetNoAmmo, comm, base ) )
	{
		if ( comm.Comm() == eComCharge )
		{
			if ( closest )
				base.SetClosest(closest);
			else
				base.SetClosest(soldmel);
		}
		if (comm.Comm() != eComMax)
			base.Command( &comm );

		bRes = false;
	}
	// No one to charge out of ammo
	else if ( closest == NULL )
	{
		if ( base.State() == eUnitShoot )
			base.ICommand( eComHalt );

		base.SetClosest(NULL);

		bRes = false;
	}
	// No one to charge out of ammo
	else if ( closest != NULL )
	{		
		base.ICommand( eComRetreat );

		base.SetClosest(NULL);

		bRes = false;
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MarchTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool MarchTargetAI( CXUnit base )
{
	bool bRes = true;

	if ( !base.CurrTarg() || !base.Marching() )
		return bRes;

//I retreat if enemy near during the movement
	if ( base.TargCloseYds() < AI_ARTYRAD && !base.TargClose().Routed() && !base.TargClose().Retreating() )
	{
		switch ( base.LimState() )
		{
			case eLimUnLimbering:
				base.Limber();
			case eLimLimbered:
				base.ICommand( eComRetreat );
				return false;
				break;
		}
	}

	if ( base.Closest() == NULL && base.LimState()==eLimLimbered)  // I stop only if limbered 
	{
		if ( base.UnderRecentFire() && base.TargCloseYds() < CLOSESUPPORT )  //avoid stop for counter artillery losses
			bRes = false;
		else if ( base.TargCloseYds() < AI_ARTYRAD )
			bRes = false;
	}
	

	if ( !bRes && base.CanHaltTerrain() )
		base.ICommand( eComHalt );

	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandNoTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

static bool StandNoTargetAI( CXUnit base, CXUnit closest )
{
	bool  bRes = true;
	SXComm comm;

	if ( base.State() != eUnitStand || base.CurrTarg() || base.Closest() != NULL || closest != NULL )
		return bRes;

	if ( base.Leader().Valid() && GetArtCommand( base.Leader(), eSTStandNoTarget, comm, base ) )
	{
		if (comm.Comm() != eComMax)
			base.Command( &comm );

		bRes = false;
	}
	else if ( base.UnderFire() )
	{
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

static bool Think( int /*ticks*/, CXUnit base )
{
	int mindist;

	CXUnit soldmel = NULL;
	CXUnit closest = NULL;

	SEnemy enemy[ eQuadMax ];

	memset( enemy, 0, sizeof enemy );

//  davide---------------------
	SEnemyExtension enemyextension[ eQuadMax ];

	memset( enemyextension, 0, sizeof enemyextension );

//-----------------------------


	// Check formation
	if ( base.LimState() == eLimLimbered && base.FormTypeIndex(eFTMarch) != -1 )
	{
		if ( !base.IsFormType(eFTMarch) && !base.IsFormType(eFTFight) )
			base.ICommand( eComFormType, eFTMarch );
	}

	if ( base.State() == eUnitRouted )
		return true;

	if ( !MoraleAI(base) )
		return true;

	if ( base.State() == eUnitRetreat )    //for the retreating unit i find a valid destination point for the retreat
	{
		if ( !base.ValidDest( base.Dst() ) )
		{
			CXVec vec = base.Dst();

			if ( base.GetValidDest( base.DeP(), &vec, true ) )
				base.SetDest(vec);
		}
		return true;
	}

	// If no targets
	if ( !base.CurrTarg() )
	{
		if ( base.AttackMarch() )
			return true;

		if ( base.AmmoType() == eAACan )   //If no targets and till loading canister, I switch to new ammunition
		{
			if ( base.AmmoCmd() >= 0 )
				base.AmmoType( base.AmmoCmd() );
			else
			{
				base.AmmoType( eAASolid );
				if (base.Ammo()<1)
				{
					base.AmmoType( eAAShell );
					if (base.Ammo()<1)
					{
						base.AmmoType( eAAShrap );
					}
				}
			}
		}

		if ( !StandNoTargetAI( base, NULL ) )
			return true;


		if ( base.State() == eUnitShoot )
			base.ICommand( eComHalt );

		base.SetClosest(NULL);

	 	return true;
	}


	if ( !CheckAllTargets( base, enemy, closest, soldmel, mindist, enemyextension ) )
		return true;


	if ( base.AttackMarch() )
		return true;


	if ( base.MainTarg() )
	{
		CXUnit sold = base.MainTarg();

		if ( closest && mindist < DISTARTYHOLD )
			base.SetMainTarg(NULL);
		else if ( !sold.CanTarget() || !sold.CanFight() )
			base.SetMainTarg(NULL);
	}

	if ( base.FormCloseYds() > 0 && base.FormCloseYds() >= mindist )
	{
		base.ICommand( eComGetAway );
		return true;
	}


	if (base.Ammo()<1 || mindist>base.MaxRangeYds())  //zero ammunition or my ammunition short range I try to change; i'll remain with zero ammo if really no ammo is left
	{
		if (closest)
			base.ChooseAmmo(mindist,closest);
		base.SetMainTarg(NULL);
		base.SetClosest(NULL);
	}

	if (base.Ammo()>=1 && base.State()==eUnitShoot && mindist>base.MaxRangeYds()) //I have ammo but they are short and the only one remaining, I stop to shot.
	{
		base.ICommand(eComState,eUnitStand);
	}


	if ( !StandTargetNoAmmoAI( base, closest, soldmel ) )
		return true;

	if ( !ShouldRunAI( base, closest, mindist ) )
		return true;

	if ( !StandTargetAmmoAI( base, enemy, closest, enemyextension ) )
		return true;

	if ( !MarchTargetAI(base) )
		return true;

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MyThink
//*************************************************************************************************************
//*************************************************************************************************************

static bool MyThink( int /*ticks*/, CXUnit base )
{
	int mindist;
	
	CXUnit soldmel = NULL;
	CXUnit closest = NULL;

	SEnemy enemy[ eQuadMax ];

	memset( enemy, 0, sizeof enemy );

//  davide---------------------
	SEnemyExtension enemyextension[ eQuadMax ];

	memset( enemyextension, 0, sizeof enemyextension );

//-----------------------------

	if ( base.State() == eUnitRouted )
		return true;

	if ( !MoraleAI(base) )
		return true;


	if ( base.State() == eUnitRetreat )    //for the retreating unit i find a valid destination point for the retreat
	{
		base.ValidatePath();
		return true;
	}

	//if ( base.State() == eUnitRetreat )
	//{
	//	if ( !base.ValidDest( base.Dst() ) )
	//	{
	//		CXVec vec;

	//		if ( base.GetValidDest( base.DeP(), &vec, true ) )
	//			base.SetDest(vec);
	//	}
	//	return true;
	//}

	// If Meleeing
	if ( !MeleeAI(base) )
		return true;

	// If no targets
	if ( !base.CurrTarg() )
	{
		if ( base.State() == eUnitShoot )
			base.ICommand( eComHalt );

		base.SetClosest(NULL);

	 	return true;
	}
	
	if ( !CheckAllTargets( base, enemy, closest, soldmel, mindist, enemyextension ) )
		return true;

	if (base.Ammo()>=1 && base.State()==eUnitShoot && mindist>base.MaxRangeYds()) //I have ammo but they are short and the only one remaining, I stop to shot.
	{
		base.ICommand(eComState,eUnitStand);
	}

	if ( base.State() == eUnitStand && base.CurrTarg() && base.Ammo() > 0 )
	{
		if ( !base.CanFormFire() )
			base.ICommand( eComFormType, eFTFight );
		else if (base.TargCloseYds()<=base.MaxRangeYds())
		{
			base.SetClosest(closest);
	 
	 		base.ICommand( eComFire );
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SowAIFunc
//*************************************************************************************************************
//*************************************************************************************************************

int SowArtAIFunc( int ticks, void *punit )
{
	int res;

	CXUnit unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = MyThink( ticks, unit );		
	else
		res = Think( ticks, unit );		

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************


