//*************************************************************************************************************
//*************************************************************************************************************
// File: unitai.cpp
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

#define FOLLOW_TICS      15
#define AI_CANTFOLLOW    3
#define AI_GREATERTHREAT 2000
#define DISTINF1AUTOMELEE 15 //infantry autocharge
#define COLUMN_CHECK 1.1
#define INFLINEATTACK 3
#define BAYONETBREAK 3
#define CAVWEIGHT 10
#define SUPPORTEDFALLBACK 20  //fallback distance for supported skirmishers
#define AI_GUARDYDS_INF   30

extern bool GetInfCommand( CXOff lead, int state, SXComm &comm, CXUnit unit );
extern bool CheckHaltLine( CXUnit base );
extern bool IsColumn( CXUnit base );
extern bool IsLine( CXUnit base );
extern bool IsSkirmisher( CXUnit base );
extern int FearCheck( CXUnit base, CXUnit sold );

#define BUFF_CHECK(val) CXUtil::MultSpec(val,14,10)
#define COL_CHECK(val) CXUtil::MultSpec(val,11,10)

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StopOverlapTarget
//*************************************************************************************************************
//*************************************************************************************************************

bool StopOverlapTarget(CXUnit base)
{
	//  use for check if I can stop in marchnear target
	// if true, i'm touching a friend so I cannot stop

	if (!base.FriendClose())  //no friend near no problems
		return false;

	if (base.FriendClose().IsOfficer() || base.FriendClose().IsArt() || IsSkirmisher(base))
		return false;

	if (base.FriendClose().Marching())  //no friend near no problems
		return false;

	if (base.DoesShareHex(base.FriendClose()))
	{
		return true;
	}
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StopOverlap
//*************************************************************************************************************
//*************************************************************************************************************

bool StopOverlap( CXUnit base )
{
	int diff = 0;

	if (!base.FriendClose())  //no friend near no problems
		return false;

	diff = base.Pos().GetAngleDiff(base.FriendClose().Loc(),true);   //I check before the enemy close just to avoid enemy beg if not necessary

	if ( !base.FriendClose().IsArt() && !base.FriendClose().IsOfficer() && 
		   !base.FriendClose().Marching() && !IsSkirmisher(base.FriendClose()) && 
		   !base.FriendClose().Retreating()	&& base.DoesShareHex(base.FriendClose()) && 
		   (diff< 90 || diff > 270 ))
	{
		base.Repath();
		return true;
	}
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SkirmFallback
//*************************************************************************************************************
//*************************************************************************************************************
int SkirmFallback( CXUnit base )
{

	// this routine determine dinamically the fallback distance of skirmishers (originally set in the drills.csv)
 	int dist;
	CXUnit sold;

	int enemydistxskirmish = base.FormCloseYds();	//temp variabile for dynamic calculation of fallback distance


	if ( base.EnemyClose().IsArt() || base.EnemyClose().FormCloseYds() >0)
		enemydistxskirmish = 0;   //artillery or other skirmishers not generate fallback
	else if (base.EnemyClose().UnitType()==eUnitCav)  //against cavlary max distance
		enemydistxskirmish = base.MaxRangeYds();
	else if (  base.EnemyClose().Retreating() ) //no in the enemy is retreating
		enemydistxskirmish = 0;
	else if (base.EnemyClose().Standing() || base.EnemyClose().State()==eUnitFallback)  //if the enemy is not moving is not a immediate danger I avoid skirmishers' dance
	{
		enemydistxskirmish = base.LongRangeYds();
	}

	if ( enemydistxskirmish != 0 && enemydistxskirmish >= base.EnemyCloseYds() )
	{
		//if solid friend is nearest that the nearest enemy no fallback until very near
		base.FriendBeg();
	//if enemy too near and not support from friend the guns retreat
		while ( base.FriendNext( sold, dist ) )
		{
			if ( !sold.IsOfficer() && !sold.Retreating() && sold.State()!=eUnitFallback && 
				sold.CanFight() && (sold.UnitType() == eUnitCav || sold.UnitType() == eUnitInf) && dist < base.EnemyCloseYds() 
				&& !IsSkirmisher(sold))  
				{
					enemydistxskirmish = SUPPORTEDFALLBACK;
					if (base.EnemyClose().UnitType()==eUnitCav)
						enemydistxskirmish *= 5;
					break;
				}
		}
	}
	return enemydistxskirmish;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: EnemyAhead
//*************************************************************************************************************
//*************************************************************************************************************

bool EnemyAhead( CXUnit base )
{
	CXUnit sold;
	int dist;
	int diff;
	SXComm comm;
	bool bRes = false;

//check If I have enemies ahead of me for avoid to wheel to nearest target when the enemy is frontal but only
// out of range for few yards

	if (!base.EnemyClose() || base.EnemyCloseYds()>=BUFF_CHECK(base.MaxRangeYds()))
		return bRes;

	diff = base.Pos().GetAngleDiff(base.EnemyClose().Loc(),true);   //I check before the enemy close just to avoid enemy beg if not necessary

	if (diff<= 45 || diff >= 315)  //frontal arc 
	{
		bRes = true;
		base.SetClosest(base.EnemyClose());
		return bRes;
	}

	base.EnemyBeg();

	while ( base.EnemyNext( sold, dist ) )
	{
		if (sold.IsOfficer())  //no officers
			continue;
		
		if (dist>=BUFF_CHECK(base.MaxRangeYds()))  //no enemies over the buffer distance
			continue;

		if (!sold.CanFight())  //no enemy which cannot fight
			continue;

		if (sold.UnitType()!=eUnitInf && sold.UnitType()!=eUnitCav)  //only infantry and cavalry
			continue;
		
		if (sold.UnitType()==eUnitInf && (IsSkirmisher(sold)))  //ignore skirmsihers
			continue;

		if (sold.State()==eUnitRetreat && sold.State()==eUnitRouted)  //ignore routed enemies
			continue;

		diff = base.Pos().GetAngleDiff(sold.Loc(),true);  //if it is in the frontal arc
		if ((diff<= 45 || diff >= 315))
		{
			bRes = true;
			base.SetClosest(sold);
			return bRes;
		}
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ObstacleAheadMe
//*************************************************************************************************************
//*************************************************************************************************************

bool ObstacleAheadMe( CXUnit base )
{
	CXUnit sold;
	int dist;
	int diff;

	CXUnit closest;
	int mindist = 0;

//I check if I have friend ahead of me; I decide if stop and repath
	base.FriendBeg();

	while ( base.FriendNext( sold, dist ) )  //routine to avoid march over a friend
	{
		
		if (sold.IsOfficer())  //no officer
			continue;		

		if (sold.UnitType()!=eUnitInf && sold.UnitType()!=eUnitCav)  //only cavlary and infantry
			continue;

		if (sold.UnitType()==eUnitInf && IsSkirmisher(sold))  //no skirmishers
			continue;

		if (sold.State()!=eUnitStand && sold.State()!=eUnitShoot && !sold.Retreating() && sold.State()!=eUnitFallback
			&& sold.State()!=eUnitAdvance)   //no moving friends
			continue;
		
		diff = base.Pos().GetAngleDiff(sold.Loc(),true);

		if (IsLine(base))  //line
		{
			if ((diff<= 90 || diff >= 270))   // valid arc
			{
				if (closest==NULL || dist<mindist)
				{
					closest = sold;
					mindist = dist;
				}
			}
		}
		else  //columns
		{
			if ((diff<= 50 || diff >= 310))   // valid arc
			{
				if (closest==NULL || dist<mindist)
				{
					closest = sold;
					mindist = dist;
				}
			}
		}
	}

	if (closest!=NULL)
	{
		if (mindist<=base.LongRangeYds() || base.State()==eUnitRun || IsLine(base))  // if too near I use check path
		{
			if (base.CheckPath() && base.ValidDest(base.Loc()))
				base.ICommand(eComHalt);
		}
		else if (mindist>base.LongRangeYds())  //if there'is space I repath
			base.Repath();
		return true;
	}
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsShooting
//*************************************************************************************************************
//*************************************************************************************************************

bool IsShooting( CXUnit base )
{
	int temp = 0;
	int threat = 0;
	int i;


	if ( base.State() == eUnitShoot )
    {
        // check to see if our units are getting targets; at last half of sprites are shooting

        for ( i = 1; i < base.NumSprites(); ++i )
        {
            if ( !base.IsValidShot(i) )
                continue;

            ++temp;

            if ( base.IsShotTarg(i) )
                ++threat;
        }

        if ( threat > temp/2 )
        {
			return true;
        }
		return false;

    }

	return true;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsCavalryDangerous
//*************************************************************************************************************
//*************************************************************************************************************
bool IsCavalryDangerous( CXUnit base )
{
//check to see if the cavalry unit is in dangerous state (valid for foes and friend checks)
	bool bRes = false;
	
	if ( base.State() == eUnitMarch || base.State() == eUnitStand || base.State() == eUnitMelee ||
		base.State() == eUnitShoot ||  base.State() ==eUnitRun || base.State() == eUnitCharge )
		bRes = true;

	return bRes;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: BayonetSituation
//*************************************************************************************************************
//*************************************************************************************************************

bool BayonetSituation(CXUnit base)
{
//---this routine is used for evaluate if the infantry unit can resist to a bayonet assault or retreat under the morale
//--- pressure

	CXUnit sold;

	if (!base.TargClose() || base.TargCloseYds()>60)  //for avoid to lost CPU I check only if I have 1 target really near
		return true;

	sold = base.GetCharger();

	if (sold==NULL || sold.NumMen()==0)
		return true;

	if (sold.UnitType()!=eUnitInf || sold.State()!=eUnitMelee || IsSkirmisher(sold))  //no valid cases
		return true;

	//-----infantry flight under a melee in very unfavourable situation
	if (FearCheck(base,sold)>=BAYONETBREAK)
	{
		base.SetClosest(NULL);
		base.SetMainTarg(NULL);
		base.ICommand( eComRetreat, 1 );
		if ( !base.ValidDest( base.GetLocAhead(300) ) )
		{
			CXVec vec = base.GetLocAhead(300);

			if ( base.GetValidDest( base.DeP(), &vec, true ) )
				base.SetDest(vec);
		}
		else
			base.SetDest(base.GetLocAhead(300));
		return false;
	}	
	//---------------------------------------------------	
	  return true;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: WheelingThreatEvaluation
//*************************************************************************************************************
//*************************************************************************************************************
int WheelingThreatEvaluation(int i, CXUnit base, SEnemy *enemy, int myquad )
{


//frontal danger remains like this
	if (i==myquad)
		return enemy[i].threat;

	if (enemy[i].closest.State()==eUnitFallback)   //if the target is falling half
		return enemy[i].threat / 2;
	else if (enemy[i].closest.State()==eUnitMelee) //if the target is in melee 0
		return  0;
	else if (enemy[i].closest.Retreating())  //the target is retreating 0
		return 0;


	if (IsSkirmisher(enemy[i].closest) && base.Closest())  //skirmishers half
		return enemy[i].threat / 2;

	return enemy[i].threat;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MarchingTacticalMovements
//*************************************************************************************************************
//*************************************************************************************************************

bool MarchingTacticalMovements( CXUnit base)
{
	SXComm comm;
	CXOff lead = base.Leader();

//I manage here the reaction to a tactical movement near a target

	if (base.Closest().UnitType()==eUnitCav)  //no advance against cavalry
		return false;

	// Check with our leader
	if (base.Leader().Valid())   //ask for the charge order
	{
		//this function should order to conitnue the attacking march or not
		if ( GetInfCommand(base.Leader(), eSTMarchTargetOnlyMelee, comm, base ) )
		{	
			if (comm.Comm() != eComMax)
				base.Command( &comm );
			return true;
		}
		else if (base.Leader().PlyrCtrl() && base.TargCloseYds() < DISTINF1AUTOMELEE &&
			!base.Blocked() && base.CanCharge(base.TargClose(),true) 
			&& FearCheck(base.TargClose(),base)>=INFLINEATTACK)  //player controlled autocharge
		{
			base.SetClosest(base.TargClose());
			base.ICommand(eComCharge);
			return true;
		}
		else if (base.Leader().PlyrCtrl() && base.TargCloseYds()>(base.GetUnitTypeVal(UVAL_BaseVolleyYds)+base.GetMod(eGameVolleyYds)))  //player controlled I march until a volley distance
			return true;
	}
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandingTacticalMovements
//*************************************************************************************************************
//*************************************************************************************************************

void StandingTacticalMovements( CXUnit base)
{
	SXComm comm;

//management of tactical standing fire

	// Check with our leader
	//this function should order also the advance fire or volley release or charge on the basis of stances.cpp schema
	if ( base.Closest() && base.Leader().Valid() && GetInfCommand(base.Leader(), eSTStandShooting, comm, base ) )
	{	
		if (comm.Comm() != eComMax)
			base.Command( &comm );
		return;
	}
	else if ( base.Leader().Valid() && base.Leader().PlyrCtrl() && base.TargCloseYds() < DISTINF1AUTOMELEE
		&& !base.Blocked() && base.CanCharge(base.TargClose(),true)  
		&& FearCheck(base.TargClose(),base)>=INFLINEATTACK)  //player controlled autocharge
	{
		base.SetClosest(base.TargClose());
		base.ICommand(eComCharge);
		return;
	}
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckAllTargets
//*************************************************************************************************************
//*************************************************************************************************************

bool CheckAllTargets( CXUnit base, SEnemy *enemy, CXUnit &closest, CXUnit &soldmel, int &mindist, SEnemyExtension *enemyextension )
{
	int quad = 0;
	int dist;
	int myquad = base.GetQuad(NULL); 
	int diff = 0;

	bool  hdist;
	CXUnit sold;


	soldmel = NULL;
	closest = NULL;
	mindist = 0;

	base.TargBeg();

	while ( base.TargNext( sold, dist ) )
	{
		hdist = base.DoesShareHex(sold);

		// someone is meleeing us, we need to fight back
		if ( ( hdist || ( sold.Closest() == base && sold.State() == eUnitMelee ) ) && !sold.IsArt() &&
		     sold.State() != eUnitRetreat && base.CanCharge(sold, true))  
		{
					base.SetClosest(sold);

				base.ICommand( eComState, eUnitMelee );

			return false;
		}
		// can't fire at meleeing targets
		else if ( sold.State() == eUnitMelee )
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
			if ( !enemyextension[myquad].closesttarget || dist < enemyextension[myquad].closesttargetdist)
			{
				diff = base.Pos().GetAngleDiff(sold.Loc(),true);  //check angle of frontal enemies to avoid too much lateral movement
				if ( diff <= 45 || diff >= 315) 
				{
						enemyextension[myquad].closesttarget = sold;
						enemyextension[myquad].closesttargetdist  = dist;
				}
			}
		// total
		int u = sold.NumMen();

		if ( sold.UnitType() == eUnitArt )
			u *= base.GetUnitTypeVal(UVAL_ArtWeight);
		else if ( sold.UnitType() == eUnitCav )
			u *= base.GetUnitTypeVal(UVAL_CavWeight);
		else if ( sold.UnitType() == eUnitInf )
			u *= base.GetUnitTypeVal(UVAL_InfWeight);
		enemy[quad].men  += u;
		enemy[quad].dist += u * dist;
		if ( sold.UnitType() == eUnitArt )
			u *= 100;

		enemy[quad].men  += u;
		enemy[quad].dist += u * dist;
	}


	base.SetMainTarg(NULL);
	if (enemyextension[myquad].closesttarget.Valid())  //if nearest target already in shooting range; frontal arc has the priority
	{
		base.SetClosest(enemyextension[myquad].closesttarget);
	}
	else
	{
		base.SetClosest(NULL);
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MoraleAI
//*************************************************************************************************************
//*************************************************************************************************************

bool MoraleAI( CXUnit base )
{
	int  i;
	bool bRes = true;
	bool bDanger;
	bool bRouteCheck = base.CheckRouting();    //checking on the possibility to go in rout
	SXComm comm;

	bDanger = ( (base.UnderFire() || base.CurrTarg()) && ( base.NumTargs() || base.Blocked() ) );  

	// Routed units
	if ( base.Morale() <= MORAL_ROUTED )   //if the morale is under this level the unit goes in rout
	{
		if ( base.State() != eUnitRouted)
			base.ICommand( eComRoute, 1 );

		bRes = false;
	}
	else if ( bRouteCheck )   //checking on the possibility to go in rout
	{
		base.RouteUnit();
		bRes = false;
	}
	// Broken units
	else if ( base.Morale() <= MORAL_BROKEN )     //if the morale is under this level the unit ca go in retreat
	{
		if ( (base.State() != eUnitRetreat || !base.Moving()) && bDanger )  //if the unit is in danger and is not retreating or moving she goes in retreat
		{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand( eComRetreat, 1 );
		}
		base.SetClosest(NULL);    

		bRes = false;
	}
	// Check fallback, retreat after drops in morale
	else if ( base.PrevMor() > base.Morale() && bDanger )    //decrease of morale and danger, check on the percentage of falling back and retreating 
	{
		if ( base.State() != eUnitRetreat )
		{
			i = CXUtil::Ran( 98, 0, 100 );  //random perc. to determine the fallback or retreat (from unitattributes.csv)

			if ( base.State() != eUnitFallback && base.State() != eUnitMelee && base.State() != eUnitCharge && base.State() != eUnitRun &&
				   i <= base.GetMod(eGameFallbackPct) && base.CurrTarg() )
			{
				if (IsColumn(base))   //is column retreat
				{
					base.SetClosest(NULL);
					base.SetMainTarg(NULL);	
					base.ICommand( eComRetreat );
					return false;
				}
				if (base.Split()==-1)				  //normal fallback
					base.ICommand( eComFallback, 1 );
				if (base.Split()!=-1)				//skirmishers company return to the parent
				{
					base.SetClosest(NULL);
					base.SetMainTarg(NULL);	
					base.ICommand(eComUnSplit);	
				}
				bRes = false;	
			}
			else if ( i <= base.GetMod(eGameRetreatPct) )
			{
				base.SetClosest(NULL);
				base.SetMainTarg(NULL);
				if (base.Split()==-1)				  //new skirmishers
					base.ICommand( eComRetreat, 1 );
				if (base.Split()!=-1)				//skirmishers company return to the parent
					base.ICommand(eComUnSplit);			
				bRes = false;	
			}
		}
	}

	if ( !bRes )
	{
		base.SetClosest(NULL);
		base.SetMainTarg(NULL);
	}

	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GuardAI
//*************************************************************************************************************
//*************************************************************************************************************

bool GuardAI(CXUnit base)
{
	bool bNeed = false;

	CXUnit sold;
	CXUnit follow = base.FollowTarg();

	if (base.State() == eUnitCharge || base.State() == eUnitMelee)
		bNeed = true;
	else if (!follow.Valid())
		bNeed = true;
	else if (follow.Rank() == eRankBrig || !follow.IsOfficer())
	{
		sold = follow.GetCharger();

		if (sold.Valid())
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
// Function: FollowScreen
//*************************************************************************************************************
//*************************************************************************************************************

bool FollowScreen( CXUnit base )
{
	// manage battalion screening in skirmishers formation
	CXUnit sold;

	// logic keep itself between the nearest enemy to the leader and the leader
	// don't go too far from the leader
	// if a target present follow the normal skirmsh logic

	int dist = 0;


	//--if not already in skirmish I set it
    if (!IsSkirmisher(base))
	{
		base.ICommand(eComFormType,eFTSkirmish);
		return true;
	}


//  check target priority - my target or the leader target
	if (base.TargClose())
	{
		return false;
	}
	else if (base.Leader())
		sold = base.Leader().GetSaveClosest( dist );

	if (!sold)
		base.SetFollow(NULL);	

	if ( sold.Valid() && ( !base.FollowTarg() || base.FollowTarg().GetID() != sold.GetID() ) )  //I set the follow
	{
		base.SetFollow(sold);
		return true;
	}
	else if (base.FollowTarg() && !base.Moving())
	{

		base.ICommand( eComMove, base.FollowTarg().Loc() );
		return true;

	}
	else if (!base.FollowTarg())//no target
	{
		if (base.Leader() && base.ValidDest(base.Leader().GetLocAhead(300)) ) //keppe them always ahead of leader
			base.ICommand( eComMove, base.Leader().GetLocAhead(300) );
		if ( base.State() != eUnitRun ) //start to run
			base.ICommand( eComRun );
		return true;
	}
	return false;  //returning false I apply the usual skirmishers logic
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: FollowAI
//*************************************************************************************************************
//*************************************************************************************************************

bool FollowAI( CXUnit base )
{
	bool bRes = true;
	CXVec loc;

	CXUnit follow = base.FollowTarg();

	if ( follow.Valid() )
	{
		if ( base.NumTargs() == 1 && base.FollowType() == eFollowRaid )
			base.SetFollow( base.TargClose() );

		loc = follow.GetLocBehind( AI_GUARDYDS_INF );

		if ( follow.Hidden() )
		{
			base.SetFollow(NULL);
			return bRes;
		}
	}
	else
		loc = base.Dst();
	
	int followtics = base.FollowTics();

	if ( base.State() == eUnitStand && base.Loc() != loc )
		followtics = FOLLOW_TICS + 5;
	else if ( base.Dst() != loc )
	{
		followtics++;
		base.SetDest( loc );
	}

	if ( followtics > FOLLOW_TICS )
	{
//		base.SetDest( loc );

		followtics = 0;

		base.ICommand( eComMove  , loc );
		base.ICommand( eComEndDir, follow.Dir() );
	}
	base.FollowTics(followtics);

	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MeleeAI
//*************************************************************************************************************
//*************************************************************************************************************

bool MeleeAI( CXUnit base )
{
	bool bRes = true;
	SXComm comm;
	bool bHalt = false;

	// If Meleeing
	if ( base.State() == eUnitMelee || base.State() == eUnitCharge )    //only if melee or charge
	{
		bRes = false;

		if ( !base.Closest().Valid() )      //the closest unit is not valid I stop
		{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand( eComHalt );
			bHalt = true;
		}
		else if ( base.State() == eUnitCharge && !base.CanCharge( base.Closest() ) )  //i'm charging but i can charge the closest, I stop
		{
			base.SetMainTarg(NULL);
			base.SetClosest(NULL);
			base.ICommand( eComHalt );
			bHalt = true;
		}
		else if ( base.State() == eUnitMelee && !base.CanCharge( base.Closest(), true ) )   //i'm meleing but i can charge the closest, I stop
		{
				base.SetClosest(NULL);
				base.SetMainTarg(NULL);
				base.ICommand( eComHalt );
			    bHalt = true;    
		}
		else if ( base.State() == eUnitMelee && base.Closest().Valid() && base.Closest().State() == eUnitRetreat ) //i'm meleing but the closest is retreating i ask to leader if I have to stop
		{
			// this is really for cav
			if ( base.Leader().Valid() && !base.Leader().CanICharge(base) )  //i stop the charge
			{
				base.SetMainTarg(NULL);
				base.SetClosest(NULL);
				base.ICommand( eComHalt );
				bHalt = true;
			}
			else if ( base.TargClose().Valid() && !base.TargClose().Retreating() && base.DoesShareHex(base.TargClose()))  //if during the pursuit I overlap with another I switch melee on him
			{
				base.SetClosest(base.TargClose());
			}
			else   //i halt
			{
				base.SetMainTarg(NULL);
				base.SetClosest(NULL);
				base.ICommand( eComHalt );
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
// Function: StandTargetNoAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

bool StandTargetNoAmmoAI( CXUnit base, CXUnit closest, CXUnit soldmel )
{
	bool  bRes = true;
	SXComm comm;

	// Just do above check to see if we have to melee
	if ( ( base.State() != eUnitStand && base.State() != eUnitShoot ) || base.Ammo() >= 1 )  //if I till have ammo I go out
		return true;

	// See if we can charge someone who is already meleeing
	// or we have a target but no ammo
	if ( ( soldmel != NULL || closest != NULL ) &&
		base.Leader().Valid() && GetInfCommand(base.Leader(), eSTStandTargetNoAmmo, comm, base ) )
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
// Function: CoverTargetAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

bool CoverTargetAmmoAI( CXUnit base, CXUnit closest )
{
	if ( base.State() != eUnitTakeCover || closest == NULL )  //no closest or not covering, I go out
		return true;

	// see if it's arty and if we should charge it
	if ( closest || base.NumTargs() > 0 )
	{
		base.ICommand( eComState, eUnitStand );
		return false;
	}
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandTargetAmmoAI
//*************************************************************************************************************
//*************************************************************************************************************

bool StandTargetAmmoAI( CXUnit base, SEnemy *enemy, CXUnit closest, const int &mindist, SEnemyExtension *enemyext )
{
	int i;
	int quad;
	int myquad = base.GetQuad( NULL );
	int threat = 0;

	bool bWasNull   = true;
	bool bCanWheel;
	bool bMustWheel = false;
	bool bJustWheel = base.JustWheeled();
	bool bHoldFire = false;
	bool badshot = false;
	int pressed = 0;

	base.JustWheeled(0); //I reset justwheeled

	SXComm comm;
	CXOff  lead = base.Leader();	

	if ( ( base.State() != eUnitStand && base.State() != eUnitShoot ) || base.Ammo() < 1 )
		return true;

	// see if it's arty and if we should charge it
	if ( base.Closest()==NULL && closest!=NULL && ( closest.UnitType() == eUnitArt || closest.UnitType() == eUnitAmmo ) 
		&& base.CanCharge() && mindist <= lead.ChargeArtyDistYds() )  //I ask to the leader the permission to charge artillery
	{
		if ( lead.CanICharge(base) )
		{
			base.SetClosest(closest);

			base.ICommand( eComCharge );

			return false;
		}
	}

	// Make sure we can fire from the current formation
	if ( base.Leader().Valid() && base.Closest() && base.IsFormType(eFTAssault) && GetInfCommand(base.Leader(), eSTStandTarget, comm, base ) )   
	{	
		if (comm.Comm() != eComMax)
			base.Command( &comm );
		return false;
	}
	// Make sure we can fire from the current formation
	if ( base.State() == eUnitStand && !base.CanFormFire())
	{
		if (!base.Closest() && closest)   //avoid to deploy in line with enemies along the flanks, minimize contact situations: I wheel the column eventually
		{
			base.WheelToTarget(closest);
			return false;
		}
		base.ICommand( eComFormType, eFTFight );
		base.ICommand( eComRun );
		return false;
	}
	quad      = -1;
	bCanWheel = true;

	// Figure Averages
	for ( i = 0; i < eQuadMax; i++ )
	{
		if ( enemy[i].men == 0 )     
			continue;

		enemy[i].dist /= enemy[i].men;
		enemy[i].threat = enemy[i].men * 100 - enemy[i].dist - enemy[i].cdist;

		//routine for evaluate the lateral danger 
		enemy[i].threat = WheelingThreatEvaluation(i,base,enemy,myquad); 

		// no threat I bypass the quadrant
		if (enemy[i].threat==0)
			continue;

		if ( enemy[i].men > 0 && ( quad == -1 || enemy[i].threat > threat ) )  //I memorize the more dangerous quadrant
		{
			quad   = i;
			threat = enemy[i].threat;

			if (enemy[i].cdist < base.GetMod( eGameWheelDist ))  //if pressed >=2 it means I'm too much closed from different sides for wheel
				pressed += 1;                              
		}
	}
	bCanWheel = base.CanWheel();  //  direct check against the morale\formation possibility to wheel

	if ( quad == -1 )    //no danger I remain on myquad
		quad = myquad;

	//--if I have a danger different from my frontal quadrant or i'm being flanked
	// Check No lateral threat	
	if ( !bMustWheel && quad != myquad )    
		bMustWheel = true;

	if ( bMustWheel && bCanWheel && enemy[myquad].threat + AI_GREATERTHREAT > enemy[ quad ].threat )  //frontal quadrant is too great 
		quad = myquad;

	if ( !bMustWheel && base.Closest() )  //I have a target front to me
		bWasNull = false;
	else
		bMustWheel = true;  //I force to do something else because I have no frontal target

	// if I'm not shooting since much time i return to stand, ohterwise is mean i'm shooting also without main target
	if (base.VolleyStatus()==0 && base.State() == eUnitShoot)
	{
		if ( CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics))  //not shooting at all I stand
		{
			if (base.Closest() && enemyext[myquad].closesttargetdist>base.LongRangeYds())  //If I have a frontal arc target perhaps is simply too much far and lateral to be well aimed
			{
				badshot = true;
				bMustWheel = true; 
			}
			else
			  base.ICommand( eComState, eUnitStand );   //i stand
		}
		else if ( !bJustWheel && base.Closest() &&   //a target but it seems I'm shooting bad because of distance, I force something
			enemyext[myquad].closesttargetdist>base.LongRangeYds() //enemy probably at too long distance
			&& !IsShooting(base) )  //shooting bad I force action
		{
			badshot = true;
			bMustWheel = true; 
		}
		else if (!base.IsFlanked() && !bJustWheel && base.Closest() && StopOverlapTarget(base) ) //check for overlap
		{
			int diff = base.Pos().GetAngleDiff(base.Closest().Loc(),true);
			if (diff!=0)
			{
				base.ICommand( eComState, eUnitStand );   //i stand
				badshot = true;  //force to make something
				bWasNull = false; //force to make something
				bMustWheel = true;
			}
		}
	}

	if (!bMustWheel && base.IsFlanked())  //if flanked I check what to do -> usually fallback
		bMustWheel = true; 

//----check for engage distance  -> check the distance to start the fire or I advance in line
	if ( ( !bMustWheel || badshot ) && !bWasNull && base.Closest() && base.Leader().Valid())  
	{
		bHoldFire = GetInfCommand(base.Leader(), eSTStandTarget, comm, base );  //I can hold the fire if the target is marching against me or I can advance a little if he's standing
		if (comm.Comm()!=eComMax)  //if I have order to advance in line
		{
			base.Command(&comm);
			return false;
		}
	}

	// orders not to wheel //not wheel for low stance or no stance 
	if ( bMustWheel && !base.IsFlanked() && base.Leader().Valid()
		&& !GetInfCommand(base.Leader(), eSTStandNoWheel, comm, base ) ) 
	{
	 	bMustWheel = false;
		bWasNull = false;
	}

	// If I have no reason for wheel
	if ( !bMustWheel )
	{
		// if there is a target 
		if ( !bWasNull )  //only for maintarget 
		{	
			if (!bHoldFire)  //check if I hold my fire
			{
				if (base.State()!= eUnitShoot)  //I start to fire if I have a maintarg and the enemy is at range
				{
					base.ICommand( eComFire );
					base.JustWheeled(1);
				}
				StandingTacticalMovements(base); // tactical movements if I make not wheeling and the target locaiton is in a valid spot
			}
		} 
		return false;
	}

	// I have reason for wheel and I can wheel
	if ( bMustWheel && bCanWheel && !bJustWheel)
	{
		// I wheel to the target in the other quadrant or in myquad it depends from the context quad can be = to myquad
		if (enemy[quad].closest!=NULL)
		{
			if ( enemyext[myquad].closesttarget && base.IsFlanked() && base.State()==eUnitShoot && IsLine(base))  //flanked with enemy before me; i'm shooting so I fallback
			{
				if (base.TargClose() && base.TargClose().UnitType()==eUnitInf && FearCheck(base,base.TargClose())>=BAYONETBREAK)  //retreat if fear check not passed
				{
					base.ICommand(eComRetreat);
					return false;
				}
				base.ICommand(eComFallback);  //otherwise fallback
				return false;
			}
			else if ( enemyext[myquad].closesttarget && base.State()==eUnitStand)  //I stand I start to shoot because i have a target ahead me, see next cycle what happen
			{
				if (base.VolleyStatus()==1)  //nullify the volley
					base.ICommand(eComVolleyOff);

				base.JustWheeled(1);
				base.ICommand(eComFire);
				return false;
			}
			else if ( enemyext[myquad].closesttarget && base.State()==eUnitShoot && badshot && pressed<3)  //I'm shooting bad I align better but not if I'm pressed from 3 quads; sure context will change after this
			{
				base.SetClosest(enemyext[myquad].closesttarget);	

				if (base.WheelToTarget(base.Closest()))				
					base.ICommand(eComRun);

				base.JustWheeled(1);
				return false;
			}
			else if ( enemyext[myquad].closesttarget && !IsSkirmisher(enemyext[myquad].closesttarget))  //closest not skirmishers before me I remain here in any case so I stop here and I not make the nexts
			{
				if (base.State()==eUnitStand)  //if standing I fire
				{
					if (base.VolleyStatus()==1)
						base.ICommand(eComVolleyOff);

					base.ICommand(eComFire);
					base.JustWheeled(1);
				}
				return false;
			}
			else if (pressed<2 && !EnemyAhead(base))  //check if I can wheel with no enemy in the frontal quadrant
			{
				base.SetClosest(enemy[quad].closest);	
				base.WheelToTarget(base.Closest());
				base.ICommand(eComRun);
				base.JustWheeled(1);
			}
			else if ( pressed<2 && base.Closest() && !enemyext[myquad].closesttarget && enemy[quad].closest && base.State()==eUnitStand && 
				base.Leader().Valid() && GetInfCommand(base.Leader(), eSTStandTarget, comm, base ) && comm.Comm()!= eComMax	)  //in extreme case when i cannot wheel but i'm underfire I move back
			{
					base.Command(&comm);
					return false;
			}
			else if ( !enemyext[myquad].closesttarget && enemy[quad].closest && (base.IsFlanked() || base.UnderFire1()) )  //in extreme case when i cannot wheel but i'm underfire I move back
			{
				if (base.TargClose() && base.TargClose().UnitType()==eUnitInf && FearCheck(base,base.TargClose())>=BAYONETBREAK)  //retreat if fear check not passed
				{
					base.ICommand(eComRetreat);
					return false;
				}
				base.ICommand(eComFallback);  //otherwise fallback
				return false;
			}

		}
		else if (enemyext[myquad].closesttarget)  //just to be sure I make something in all the case I start fire
		{
			if (base.VolleyStatus()==1)
				base.ICommand(eComVolleyOff);
			base.JustWheeled(1);
			base.ICommand(eComFire);
			return false;
		}
		return false; 
	}  

	//I have reason for wheel but I cannot wheel --> this happen only if the unit cantwheel for true
	if ( bMustWheel && !bCanWheel )
	{
		base.SetClosest(NULL);
		base.SetMainTarg(NULL);
		base.ICommand( eComRetreat, 1 );		
		return false;
	}  
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: AdvanceFallbackTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

bool AdvanceFallbackTargetAI( CXUnit base, SEnemy *enemy )
{
	int myquad = base.GetQuad( NULL );

	bool bNoRealTarg = false;

	if ( base.State() != eUnitFallback && base.State() != eUnitAdvance )   //only for advancing or falling back
		return true;

	if ( CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics) || enemy[myquad].men == 0 || base.IsFlanked())  //checking the time for reacquire a new target or no enemy around for know if no target remains
		bNoRealTarg = true;

	if ( base.Leader().StopOnDefTerr(base) && base.OnDefTerr() && base.NewDefTerr() && base.CanWheel() )  //if I'm on defensive ground I stop
		base.ICommand( eComHalt );
	else if ( bNoRealTarg && base.State() == eUnitAdvance )   //if advancing and no real target I stop
	{
		base.ICommand( eComHalt );

		base.SetClosest(NULL);
	}
	else if ( bNoRealTarg && base.State() == eUnitFallback )  //if falling back and no real target I start to run
	{
		base.ICommand( eComGetAway );  //getaway for avoid strange paths
	}
	// if we are advancing or falling back, we should stop ai processing here
	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MarchTargetAI
// returning false usually means we're gonna stop
//*************************************************************************************************************
//*************************************************************************************************************

bool MarchTargetAI( CXUnit base, SEnemyExtension * )
{
	bool bRes = true;
	int diff;
	
	if ( !base.CurrTarg() || !base.Marching() ) //I proceed only if I marching with a target
		return bRes;

	if (IsLine(base) || base.Blocked() )  //test path near the fire area
	{
		if (base.CheckPath() && base.ValidDest(base.Loc()))
		{
			base.ICommand(eComHalt);
			return bRes;
		}
	}

	if ( base.Leader().StopOnDefTerr(base) && base.OnDefTerr() && base.NewDefTerr() )  //I stopped if on defensive ground
		bRes = false;

//if closest is not the nearest and the closest is under minimal distance I leave base.closest() 
	if (base.Closest() && base.TargClose().Valid() && base.Closest().GetID()!=base.TargClose().GetID())
	{
		diff = base.Pos().GetAngleDiff(base.TargClose().Loc(),true);
		
		if ( ( diff < 45 || diff > 315) && base.TargCloseYds()<=(base.MaxRangeYds()/2))  //checks valid only with enemy not behind me	
			base.SetClosest(NULL);
	}

	if (!base.Closest() && base.TargClose())  //stop the march with no target ahead of me
	{
		diff = base.Pos().GetAngleDiff(base.TargClose().Loc(),true);

		if ( diff < 90 || diff > 270)    //checks valid only with enemy not behind me
		{		
			if ( base.IsFormType(eFTMarch)) 
				bRes = false;
			if ( base.UnderRecentFire() && base.GetMod(eGameMarchUnderFire) < 0 )  
				bRes = false;
		}
	}
	else
	{
		//------------------------------------------------------------------------------
		if ( base.IsFormType(eFTMarch))   //if in marching formation i stop
			bRes = false;
		else if (base.Closest()) // no skimishers davide
		{ 
			bRes = (MarchingTacticalMovements(base));                 //i check what alternative to stop I have
		}
	}

//----------------------------------------------------------------------------

	if ( !bRes && !base.CanFormFire() && base.Closest() && base.EnemyCloseYds() < COL_CHECK(base.MaxRangeYds()) )  //form fighting formation if enemy near
	{
		base.ICommand( eComFormType, eFTFight );

		if ( base.UnderRecentFire() )
			base.ICommand( eComRun );

		bRes = false;
	}

	if (!bRes && StopOverlapTarget(base))
	    bRes = true;	

	if ( !bRes && base.HaltTics() <= 0 && base.ValidDest( base.Loc() ))  //I halt if the ticks are zero already and I can halt here
	{
		base.ICommand( eComHalt );
		base.ICommand( eComRun  );
    bRes = false;	
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MarchNoTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

bool MarchNoTargetAI( CXUnit base )
{
	bool bRes = true;
	SXComm comm;
	int diff = INT_MAX;

	if ( base.CurrTarg() )   //no targets
		return bRes;

//------angle with the enemy close
	if (base.EnemyClose() && base.EnemyCloseYds()<=(BUFF_CHECK(base.MaxRangeYds())))
		diff = base.Pos().GetAngleDiff(base.EnemyClose().Loc(),true);
//----------------------------------------

	if ( base.State() == eUnitFallback )    //falling back if no more danger, i simply let her run
	{
		if ( !base.UnderFire() || base.NumTargs() == 0 )
			base.ICommand( eComGetAway );  //try the getaway at place of run to avoid strange path  
//			base.ICommand( eComJustRun );  //just run stop the fallback and transform it in a run

		bRes = false;
	}
	else if ( base.State() == eUnitAdvance )   //if advancing but no target if I can stop here i stop.
	{
		if ( base.HaltTics() <= 0 && base.ValidDest(base.Loc()) )
			base.ICommand( eComHalt );

		bRes = false;
	}
	else if ( !base.Marching() )   //if no marching I go out
		return bRes;
	else if ( base.IsFormType(eFTMarch) && (diff<=90 || diff>=270) && base.EnemyClose().Valid() && base.EnemyCloseYds() < BUFF_CHECK(base.MaxRangeYds()) )  //i form in fighting formation if enemy is near
	{
		// Check with our leader
		//this function should order to march in a formation different from that of march
		if ( base.Leader().Valid() && GetInfCommand(base.Leader(), eSTMarchNoTarget, comm, base ) )  //determine formation to use
		{	
			if (comm.Comm() != eComMax)
				base.Command( &comm );
		}
		else
			base.ICommand( eComFormType, eFTFight );  //if not stance I use the nomrla fighting formation

		if ( base.UnderRecentFire() )  //under recent fire mean i received a kill in the previous frame
			base.ICommand( eComRun );

		bRes = false;
	}
	else if ( !base.ValidSpot( base.Dst() ) )    //if the destination is not a valid spot (cant halt here)
	{
		if ( base.EnemyClose().Valid() && base.EnemyCloseYds() < COL_CHECK(base.MaxRangeYds()) )  //if there is a valid enemy close, then I halt if i can here
		{
			if ( base.HaltTics() <= 0 && base.ValidDest(base.Loc()) )
				base.ICommand( eComHalt );
		}
		// if the enemy isn't close, just keep marching there, hopefully it will be ok when we get there
		bRes = false;
	}
	else if ( base.EnemyClose() && (base.EnemyCloseYds()<=(BUFF_CHECK(base.MaxRangeYds())) || base.Leader().CurrTarg()) && ObstacleAheadMe(base) )    //if near the enemy and path blocked I check the options
	{
		//I repath or check the path inside the buffer zone: so my only options are change direction or stop behind the friend
		bRes = false;
	}
	else if (StopOverlap(base))
	{
		bRes = false;
	}
	else if ( base.IsSprinting() && base.State() == eUnitRun )  //if I did already a cycle running I return to walk
	{
		base.IsSprinting(0);
		base.ICommand( eComWalk );
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: StandNoTargetAI
//*************************************************************************************************************
//*************************************************************************************************************

bool StandNoTargetAI(CXUnit base, CXUnit closest, CXUnit soldmel)
{
	bool  bRes = true;
	SXComm comm;
	int diff;

	//  i avoid this routine if not standing or taking cover
	if ((base.State() != eUnitStand && base.State() != eUnitTakeCover) || base.CurrTarg() || base.Closest() != NULL || closest != NULL)
		return bRes;

	// only target is in melee so I ask to leader if I can charge 
	if (soldmel != NULL && base.Leader().Valid())
	{
		if (GetInfCommand(base.Leader(), eSTStandTargetOnlyMelee, comm, base))
		{
			if (comm.Comm() == eComCharge)
				base.SetClosest(soldmel);

			if (comm.Comm() != eComMax)
				base.Command(&comm);

			return false;
		}
	}

	//without ammo I go to resupply
	if (base.Ammo() == 0 || base.Leader().ShouldGetAmmo(base))
	{
		base.ICommand(eComResupply);

		bRes = false;
	}
	else if (base.UnderFire() && base.WantCover() && base.NumTargs() == 0 && base.IsHighGrnd())   //I check for go in cover for no enemy near //cover only if on high ground 
	{
		if (base.EnemyClose().Valid() && base.EnemyCloseYds() * base.EnemyCloseYds() > base.MaxRangeYds() * base.MaxRangeYds())
			base.ICommand(eComOnlyLayDown);
		else if (!base.EnemyClose().Valid())  //laydown
			base.ICommand(eComOnlyLayDown);
		else if (base.EnemyClose().Valid() && base.Blocked())   
			base.WheelToTarget(base.EnemyClose());

		bRes = false;
	}
	//enemy near but blocked so is a potential target but I could not shot at him, bad situation case of overlapping
	else if (base.Blocked() && base.NumTargs() == 0 && base.EnemyClose().Valid())
	{
		if (base.EnemyCloseYds() <= base.MaxRangeYds() && base.FriendClose() && base.FriendCloseYds() <= base.LongRangeYds())   //enemy close with friend closer
		{
			diff = base.Pos().GetAngleDiff(base.EnemyClose().Loc(), true);
			if (base.EnemyClose().State() != eUnitMelee && (diff <= 135 || diff >= 225)) //if the enemy is in the frontal arc or melee i react
			{

			}
			else if (base.EnemyClose().State() != eUnitMelee && (diff > 135 && diff < 225))  //if the enemy behind me I wheel
				bRes = !base.WheelToTarget(base.EnemyClose());
		}
		else
			bRes = !base.WheelToTarget(base.EnemyClose());  //I wheel in the optimal situation
	}

	// Get a command if we didn't do anything else
//this instruction control also the preparing of volley fire, the order of placements from the leader (line,forward,flank, wheeling)
	if (bRes && base.Leader().Valid() && GetInfCommand(base.Leader(), eSTStandNoTarget, comm, base))  //stance logic -> I ask to the leader
	{
		if (comm.Comm() == eComWheelDir)
			base.WheelToTarget(comm.Unit());
		else if (comm.Comm() != eComMax)
			base.Command(&comm);

		bRes = false;
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SkirmishersLogicNoTarget
//*************************************************************************************************************
//*************************************************************************************************************

bool SkirmishersLogicNoTarget( CXUnit base )
{
     bool bRes = true; 
	 SXComm comm;
    int enemydistxskirmish = 0; //enemy distance for skirmishers fallback 
	int HaltDist = 0;

////I form skirmish formation with the company
	if (base.Split()!=-1 && !IsSkirmisher(base) )
	{
		base.ICommand(eComFormType,eFTSkirmish);
		base.ICommand(eComMove,base.GetLocAhead(150));
		base.ICommand(eComRun);
		return false;
	}
//----routine for volunteer jagers-- no able to fight in line only skirmishing
	if (base.FormTypeIndex(eFTFight)==base.FormTypeIndex(eFTAltSkirmish))
	{
		if (!base.IsFormType(eFTAltSkirmish) && base.EnemyClose() && base.EnemyCloseYds()<(base.MaxRangeYds()*3))  //form in skirmish
		{	
			base.ICommand(eComFormType,eFTAltSkirmish);
			base.ICommand(eComRun);
			return false;
		}
		else if (base.IsFormType(eFTAltSkirmish) && ( !base.EnemyClose() || base.EnemyCloseYds()>=(base.MaxRangeYds()*3)))  //return in line
		{
			base.ICommand(eComFormType,eFTLine);
			base.ICommand(eComRun);
			return false;
		}
	}
//---------------------------------------------------------------------------------	

//

	if (!IsSkirmisher(base))  // only skirmishers troops can continue
		return true;

	if  (base.FormCloseYds()>0 && base.EnemyClose())   //evaluation of skirmishers fallback distance
	{
		enemydistxskirmish = SkirmFallback(base);
	}

	
	if ( base.EnemyClose() && base.FormCloseYds() > 0 && enemydistxskirmish >= base.EnemyCloseYds())   //skirmishers routine (enemy close at place of mindist)
	{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand( eComGetAway );
			return false;
	}

	if (base.Marching())   //marching
	{
	    
		if (base.EnemyClose().Valid())   //the halt distance is halved for art and retreating troops
		{
			HaltDist = base.EnemyCloseYds();           //I approach more the artillery
			if (base.EnemyClose().IsArt() || base.EnemyClose().Retreating() )
				HaltDist /= 2;
		}
		
		if (base.EnemyClose() && base.EnemyCloseYds()<HaltDist)  //halt at the max distance
		{
			bRes = false;
			base.ICommand(eComHalt);
		}
		else if ( !base.ValidSpot( base.Dst() ) )    //if the destination is not a valid spot
		{
			// if the enemy isn't close, just keep marching there, hopefully it will be ok when we get there
			bRes = false;
		}
		else if ( !base.ValidWalk( base.Loc() ) )  //if in this moment I'm walking in a not valid place I repath the path 
		{
			base.Repath();   
			bRes = false;
		}
		else if ( base.State()!=eUnitRun )  //run if not
		{
			base.ICommand(eComRun);
			bRes = false;
		}
	}

	if (base.Standing())   //standing situation
	{
		if (base.Leader().Valid())   //skirmsihers stance
		{
			if (GetInfCommand(base.Leader(), eSTStandSkirmishNoTarget, comm, base ))
			{
				if (comm.Comm() != eComMax)
					base.Command(&comm);
				if (comm.Comm()==eComMove)
					base.ICommand(eComRun); //skirmisher move running
				bRes = false;
			}
		}

	}

	return bRes;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: SkirmishersLogicTarget
//*************************************************************************************************************
//*************************************************************************************************************
bool SkirmishersLogicTarget( CXUnit base, int &mindist )
{
   bool bRes = false;
   int enemydistxskirmish = 0; //enemy distance for skirmishers fallback
   SXComm comm; 
   int diff;

////I form skirmish formation with the company
	if (base.Split()!=-1 && !IsSkirmisher(base) )
	{
		base.ICommand(eComFormType,eFTSkirmish);
		return false;
	}


	if (!IsSkirmisher(base))
		return true;


	if  (base.FormCloseYds()>0 && base.EnemyClose())
	{
		enemydistxskirmish = SkirmFallback(base);
	}

	
	if ( base.EnemyClose() && base.FormCloseYds() > 0 && enemydistxskirmish >= base.EnemyCloseYds())   //skirmishers routine (enemy close at place of mindist)
	{
			base.SetClosest(NULL);
			base.SetMainTarg(NULL);
			base.ICommand( eComGetAway );
			return false;
	}

	if (base.State()==eUnitFallback)
	{
		base.SetClosest(NULL);
		base.SetMainTarg(NULL);
		base.ICommand( eComGetAway );
		return false;
	}


	if (base.Marching())
	{

		if ( base.Leader().StopOnDefTerr(base) && base.OnDefTerr() && base.NewDefTerr() )  //I stopped if on defensive ground
			bRes = false;
		else if (mindist<base.MaxRangeYds())  //alt at the max distance
			bRes = false;
	}

	if (base.VolleyStatus()==1)   //to be sure no volley from the skirmishers
	{
		comm.Comm(eComVolleyOff);
		base.Command(&comm); //close the volley
	}

	if (base.Standing())
	{
		if (base.Leader().Valid())   //ask for the fire orders
		{
			if (!base.Closest())
				base.SetClosest(base.TargClose());
			if (GetInfCommand(base.Leader(), eSTStandSkirmishTarget, comm, base ))
			{
				if (comm.Comm() != eComMax)
					base.Command(&comm);
				bRes = true;
			}
		}

	}

	if (bRes && base.Moving())
	{
		comm.Comm(eComRun);
		base.Command(&comm); //skirmisher move running
	}
	if (!bRes && base.State()==eUnitStand && base.TargClose())
	{

		if (!base.Closest())
			base.SetClosest(base.TargClose());
		diff = base.Pos().GetAngleDiff(base.Closest().Loc(),true);  //check angle for avoid lateral movement
		if (diff>30 && diff<330)
			base.WheelToTarget(base.Closest());
		else 
		{
			comm.Comm(eComFire);
			base.Command(&comm); //I start to fire
		}
		bRes=true;
	}

	if (!bRes && base.State()==eUnitShoot)
	{
		if ( CXUtil::GetTime() - base.ShootTime() > base.GetUnitTypeVal(UVAL_ReacquireTics))  //not shooting at all I stand
		{
			base.ICommand( eComState, eUnitStand );
		}
		bRes = true;
	}

	if ( !bRes && base.HaltTics() <= 0 && base.ValidDest( base.Loc() ) )  //I halt if the ticks are zero already and I can halt here
	{
		base.ICommand( eComHalt );
		base.ICommand( eComRun  );
		bRes = false;	
	}

	return bRes;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool Think( int /*ticks*/, CXUnit base )
{
	int mindist;
	
	CXUnit soldmel;  
	CXUnit closest;
	CXVec Location;
  
	SEnemy enemy[ eQuadMax ];

	memset( enemy, 0, sizeof enemy );

//  davide---------------------
	SEnemyExtension enemyextension[ eQuadMax ];

	memset( enemyextension, 0, sizeof enemyextension );

//-----------------------------

	if ( base.State() == eUnitRouted )   //units routed make nothing
		return true;

	if ( !MoraleAI(base) ) // routed will always go here
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
	
	// If Meleeing
	if ( !MeleeAI(base) )   //checking unit meleing or charging
		return true;


 //reattachment of units with the different type of followtype (example guard) if the morale is too low i stop the action and reattach
	if ( base.FollowType() != eFollowNone && base.Morale() <= AI_CANTFOLLOW )        
	{
		base.FollowStop();

		if ( !base.Attached() )
			base.ICommand( eComAttach );

	}


//----routine for the guard, executed till the unit itself is not in fight
	if ( base.FollowType() == eFollowGuard && !base.TargClose() && !base.Blocked() )
		FollowAI(base);

//---routine for battalions skirmishers screens
	if ( base.FollowType() == eFollowScreen && FollowScreen(base))	
		return true;

//If i'm still acting as follow specializer I continue to do it
	if ( base.FollowType() == eFollowGuard && GuardAI(base) )
		return true;

	// If no targets
	if ( !base.CurrTarg() )
	{

		//--change state if still shooting
		if ( base.State() == eUnitShoot )
			base.ICommand(eComState,eUnitStand);

		if ( base.AttackMarch() )
			return true;

		//  new skirmishers----------------------
		if (!SkirmishersLogicNoTarget(base))
			return true;

		if (IsSkirmisher(base))  //skirmishers logic stop here
			return true;
		//----new skirmishers---------------------

		// no target but standing
		if ( !StandNoTargetAI( base, NULL, NULL ) )
			return true;

		//no target marching
		if ( !MarchNoTargetAI(base ) )
			return false;

		base.SetClosest(NULL);
		base.SetMainTarg(NULL);

		return true;
	}

	// devastating bayonet assault
	if (!BayonetSituation(base))
		return true;

	if ( !CheckAllTargets( base, enemy, closest, soldmel, mindist, enemyextension ) )  //list of all the targets
		return true;

	if ( base.AttackMarch() )   //attack march routine
		return true;

	if ( !CoverTargetAmmoAI( base, closest ) )   //routine for cover troops
		return true;


//  new skirmishers----------------------

	if (!SkirmishersLogicTarget(base, mindist))
		return true;

	if (IsSkirmisher(base)) //skirmishers logic stop here
		return true;
//----new skirmishers---------------------

	// run again to see if we can charge
	if ( !StandNoTargetAI( base, closest, soldmel ) )
		return true;


	if ( !AdvanceFallbackTargetAI( base, enemy ) )   //routine for process advancing and falling back
		return true;


	if ( !StandTargetNoAmmoAI( base, closest, soldmel ) )   //when standing without ammo
		return true;	


	if ( !StandTargetAmmoAI( base, enemy, closest, mindist, enemyextension) )  // standing with Ammo
		return true;
	

	if ( !MarchTargetAI(base, enemyextension) )  //march in a target in range
		return false;

	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MyThink
//*************************************************************************************************************
//*************************************************************************************************************

bool MyThink( int /*ticks*/, CXUnit base )
{
	int mindist;
	
	CXUnit soldmel;
	CXUnit closest;
	SXComm comm;

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

	if (!BayonetSituation(base))
		return true;

	if ( !CheckAllTargets( base, enemy, closest, soldmel, mindist, enemyextension ) )
		return true;

	if ( base.State() == eUnitStand && base.CurrTarg() && base.Ammo() > 0 )
	{
		if ( !base.CanFormFire() )
			base.ICommand( eComFormType, eFTFight );
		else
		{
			base.SetClosest(closest);
	 
	 		base.ICommand( eComFire );
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SowInfAIFunc
//*************************************************************************************************************
//*************************************************************************************************************

int SowInfAIFunc( int ticks, void *punit )
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


