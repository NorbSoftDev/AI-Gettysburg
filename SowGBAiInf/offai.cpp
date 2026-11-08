//*************************************************************************************************************
//*************************************************************************************************************
// File: offai.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "sowgbaiinf.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"
#include "../sowmod/xtables.h"

#include "SOWWLMOD.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DIRECT_DIST2 (500*500)
#define DISTBUFFER 5
#define DIST2OFFCATCHUP (125 * 125)
#define DISTARTYHOLD 200
#define DISTOFFSCARED 80
#define DIST2WAGONCATCH (200 * 200)
#define DIST2COURARRIVE (15 * 15)
#define DISTCOURKILL 50

#define TICKS_WHEEL (15 * TICSPERMIN)
#define COUR_RECALCTICKS (TICSPERSEC/6)

#define RETREATPERCENTAGE 21
#define PLAYTRIGGERDISTANCE 500
#define STANCE_DANGER_DISTANCE   150  // distance brigade trigger stance

//*************************************************************************************************************
//*************************************************************************************************************
// Function: OppositeQuad
//*************************************************************************************************************
//*************************************************************************************************************
int OppositeQuad( int enemyquad ) 
{
	//---quad opposite the past
	int quad;

	quad = ( enemyquad + 2); 
	if (quad>3)
		quad -= 4;

	return quad;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ArtyWithoutAmmos
//*************************************************************************************************************
//*************************************************************************************************************
bool ArtyWithoutAmmos( CXUnit wagon, CXOff base, CXVec& location )
{
	CXOff suboff;
	int* ammo;
	int munition = 0;
	const int minimalammo = 50;

	ammo = &munition;

	if (base.Rank()==eRankReg && base.IsArt()
		&& base.NeedAmmo(ammo)
		&& base.Standing()
		&& base.TargCloseYds()>DISTARTYHOLD)
	{				
		if (munition>minimalammo)  //I move only below of 50 ammo from the totals
		{
			if ( wagon.Loc().DistYds(base.GetLocAhead(3))>0 && wagon.ValidDest(base.GetLocAhead(3)) ) 
			{			
				location = base.GetLocAhead(3);
				return true;
			}
		}

	}

	if (base.Rank()==eRankBrig && base.UnitType()==eUnitArt)
	{
		for (int i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			if (ArtyWithoutAmmos( wagon, suboff, location ))
				return true;
		}
	}

	if (base.Rank()<eRankBrig && base.NumArty())
	{
		for (int i=0;i<base.NumArty();i++)
		{
			suboff = base.Arty(i);
			if (ArtyWithoutAmmos( wagon, suboff, location ))
				return true;
		}
	}

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: BrigadeEngaged
//*************************************************************************************************************
//*************************************************************************************************************
bool BrigadeEngaged( CXOff base ) 
{
	CXUnit unit;


	for (int i = 0;i<base.NumSubs();i++)
	{
		unit = base.Sub(i);
		if (!unit.EnemyClose() || unit.EnemyCloseYds()>STANCE_DANGER_DISTANCE)
			continue;

			return true;
	}

	for (int i = 0;i<base.NumArty();i++)
	{
		unit = base.Arty(i);
		if (!unit.EnemyClose() || unit.EnemyCloseYds()>STANCE_DANGER_DISTANCE)
			continue;

			return true;
	}

	return false;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: OriginalMenNumber
//*************************************************************************************************************
//*************************************************************************************************************
int OriginalMenNumber( CXOff base ) 
{
	int i = 0;
	CXOff suboff;
	int total = 0;

	for (i=0;i<base.NumSubs();i++)
	{
		suboff = base.Sub(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.StartMen();
		else                    //no regiment? I go below until I find them
			total += OriginalMenNumber( suboff );
	}
	
	for (i=0;i<base.NumDetach();i++)
	{
		suboff = base.Det(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.StartMen();
		else                    //no regiment? I go below until I find them
			total += OriginalMenNumber( suboff );
	}

	for (i=0;i<base.NumArty();i++)
	{
		suboff = base.Arty(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.StartMen();
		else                    //no regiment? I go below until I find them
			total += OriginalMenNumber( suboff );
	}

	return total;


}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ActualMenNumber
//*************************************************************************************************************
//*************************************************************************************************************
int ActualMenNumber( CXOff base ) 
{
	int i = 0;
	CXOff suboff;
	int total = 0;

	for (i=0;i<base.NumSubs();i++)
	{
		suboff = base.Sub(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.NumMen();
		else                    //no regiment? I go below until I find them
			total += ActualMenNumber( suboff );
	}
	
	for (i=0;i<base.NumDetach();i++)
	{
		suboff = base.Det(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.NumMen();
		else                    //no regiment? I go below until I find them
			total += ActualMenNumber( suboff );
	}

	for (i=0;i<base.NumArty();i++)
	{
		suboff = base.Arty(i);
		if (suboff.Rank()==eRankReg)  //regiment? I count the men
			total += suboff.NumMen();
		else                    //no regiment? I go below until I find them
			total += ActualMenNumber( suboff );
	}

	return total;


}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CampaignRetreatCalculation
//*************************************************************************************************************
//*************************************************************************************************************
bool CampaignRetreatCalculation( CXOff base ) 
{
	int original = 0;
	int ratio = 0;
	SXComm comm;
	int retreatfactor = RETREATPERCENTAGE + base.Personality();  //retreat factor percentage + personality


	if (CXUtil::Ask(eUAIsCampaign)!=base.Rank(eRankSide))  //campaign battle and only AI side check
		return false;


	if (base.TicksForOffSDK(1)==2)   //memo of original value; i use the memo 1 to know if I have already initialized it or no
		original = base.TicksForOffSDK(2);  //If I memo it the routed or prisoner units which can disappears from the oob are already stored also starting from a save and I avoid to amke again the calculation
	else
	{
		original = OriginalMenNumber(base);
		base.TicksForOffSDK(1,2);
		base.TicksForOffSDK(2,original);
	}


	if (original>0)
		ratio = 100 - ((ActualMenNumber(base)*100)/original);
	else
		ratio = 100;

	if (ratio>retreatfactor)  //I retreat
	{
		comm.Val(base.Rank(eRankSide));
		comm.Comm(eComResign);
		CXUtil::AddComm(&comm);
	}

	return false;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitBrigThink( int , CXOff base ) 
{
	int i;

	bool   bOK;
	bool   bMove;
	bool   bTrouble;
	bool   bMoveAll;
	CXPos  spot;
	CXUnit targ;
	CXUnit wtarg;
	SXComm comm;
	
	int dist;
	int cdist;
	int tdist;

	targ = base.GetSaveClosest( dist );

	if ( base.OffThink() )
		return true;

	if ( base.Orders() )
	 	return true;

	base.CurrTarg(base.HasTargets());

	bMove    = base.Moving();
	bMoveAll = base.Moving( true );
	bTrouble = base.InTrouble();

	base.CheckEnemySee();
	base.CheckEnemyEng();


	// Fighting position
	if ( !base.HasStance() && ( !base.Attached() || base.Leader() == NULL ) && ( base.Fighting() || base.UnderCloseFire() ) )
	{		

		comm.Comm(eComOrders);

		comm.Param( 1, base.AssignedOrders() );

		base.OrdersState(eOrdersFighting);

		base.Orders( &comm );		
	}
	//else if ( !base.HasStance() && ( !base.Leader() || ( base.Leader() && base.Leader().PlyrCtrl() ) ) && ( base.UnderCloseFire() || base.Fighting() ) )
	//{

	//	comm.Comm( eComOrders );

	//	comm.Param( 1, base.AssignedOrders() );

	//	base.OrdersState(eOrdersFighting);

	//	base.Orders( &comm );		
	//}
	// throwing this is for waterloo until it becomes unneccessary
	else if ( !base.HasStance() && BrigadeEngaged(base) )
	{
		comm.Comm( eComOrders );

		comm.Param( 1, base.AssignedOrders() );

		base.OrdersState(eOrdersFighting);

		base.Orders( &comm );		
	}
 	else if ( !bMove && base.IQuit() )
		base.SupportUnits( false );
	else if ( base.CurrTarg() && ( !bMove || !base.PrevTarg() ) )
	{
		base.SupportUnits( true, dist < 40 );
	}
	else if ( !bMoveAll && !base.CurrTarg() && targ )
	{

		wtarg = NULL;
		if ( base.HasOrders() && !base.OrdersAllowWheel() )
		{
		}																					 // this is odd here because it should be looking at the range of it's subs
		else if ( base.WheelTime() <= 0 && dist > base.MaxRangeYds() + DISTBUFFER )
			wtarg = base.CheckWheel();

		if ( wtarg ) //brigade in a fort doesn't wheel for avoid to leave the fort if no enemy is around
		{
			base.WheelTime(true,TICKS_WHEEL);

			base.WheelToTarget( wtarg );

			comm.Comm(eComForm);
			comm.Val(base.FightForm());

			comm.SetBit(eCBAll);

			comm.Morale(MORAL_CANTWHEEL);
					
			base.Command( &comm ); 
		}
		else
			base.GetOrders( dist, targ );
	}
//leave road marching if too near to the enemy
	else if ( bMoveAll && base.UsingRoad() && targ && 
		dist<(STANCE_DANGER_DISTANCE*2) &&
		!base.Leader().PlyrCtrl() )
	{
		comm.Comm(eComUseRoad);
		comm.SetBit(eCBAll);
		base.Command( &comm );
	}
	else if ( bTrouble && !base.Attached() && base.Leader() && !bMoveAll && !base.CurrTarg() && !targ )
	{
		base.WantAttach(true,true);

		comm.Vec(true, base.Leader().Loc());
		comm.Comm(eComMoveRoad);

		comm.SetBit(eCBAll);

		base.Command( &comm ); 
	}
	else if ( base.WantAttach() && base.Leader() )
	{
		if ( base.Attached() )
			base.WantAttach(true,false);
		else if ( base.Loc().Dist2Yds( base.Leader().Loc() ) <= CXUtil::Vis2() )
		{
			comm.Comm(eComAttach);

			comm.DelBit(eCBAll);

			base.Command( &comm ); 

			base.WantAttach(true,false);
		}
		else if ( !bMoveAll && !targ && !base.CurrTarg() )
		{
			comm.Vec(true, base.Leader().Loc());
			comm.Comm(eComMoveRoad);

			comm.SetBit(eCBAll);

			base.Command( &comm ); 
		}
	}
	else if ( base.ReEval() && !targ && !base.CurrTarg() )
	{
		base.ReEvalTactics();
	}
	// check to see if our objective is finished
	else if ( !targ && !base.CurrTarg() && ( base.TACType() == eTacLoc || base.TACType() == eTacQuadLoc ) && !bTrouble )
	{

		if ( base.Loc().Dist2Yds( base.TACLoc() ) <= 1 )
			base.ObjectiveCompleted();
		else if ( !bMoveAll && !base.PlyrCtrl() )
		{
			comm.Comm(eComOrders);
			
			comm.Param( 1, base.TACOrders() );
		
			base.Command( &comm ); 

			comm.Comm(eComOrderDest);			
			comm.Vec (true,base.TACLoc());

			comm.SetBit(eCBChngDest);

			// must set this after giving the orders
			base.Command( &comm ); 

			if ( base.EnemyCloseYds() < 0 ) // no enemies
			{
				comm.DelBit(eCBChngDest);

				comm.Comm(eComUseRoad);			

				comm.SetBit(eCBAll);

				base.Command( &comm ); 
			}
		}
	}
	else if ( !targ && !base.CurrTarg() && base.TACType() != eTacReserve && base.TACObj() >= 0 && !bTrouble )
	{
	
		if ( !bMoveAll )
		{
			if ( base.TACOBJDone() )
				base.ObjectiveCompleted();
			// move towards objective
			else if ( base.Loc().Dist2Yds( base.TACOBJLoc() ) > base.TACOBJRad2() )
			{
				comm.Comm(eComOrders);
				
				comm.Param( 1, base.TACOrders() );
		
				base.Command( &comm ); 

				comm.Comm(eComOrderDest);			
				comm.Vec(true,base.TACOBJLoc());

				comm.SetBit(eCBChngDest);

				// must set this after giving the orders
				base.Command( &comm ); 

				if ( base.EnemyCloseYds() < 0 ) // no enemies
				{
					comm.DelBit(eCBChngDest);

					comm.Comm(eComUseRoad);			

					comm.SetBit(eCBAll);

					base.Command( &comm ); 
				}
			}
		}
		// check to see if someone else has finished our objective
		else if ( bMove && ( base.TACType() == eTacQuadVP || base.TACType() == eTacVP ) && base.TACOBJLoc() == base.Dst() && base.TACOBJDone() )
			base.ObjectiveCompleted();
	}
	else
	{
		bOK   = false;
		cdist = 0;

		if ( !bMove )
		{
			for ( i = 0; i < base.NumSubs(); ++i )
			{
				tdist = base.Loc().Dist2Yds( base.Sub(i).Loc() );
			
				if ( tdist < cdist || i == 0 )
					cdist = tdist;
			}
			
			if ( cdist > DIST2OFFCATCHUP )
			{
				if ( base.SupportUnits( false ) )
					bOK = true;
			}
		}
	}
	base.PrevTarg(base.CurrTarg());



	return true;

	

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CArtyOffThink( int ticks, CXOff base )
{
	bool   bCanMove = true;
	SXComm comm;
	CXUnit sold;
    CXVec dest = base.Dst();

	int dist;

	if ( base.OffThink() )
		return true;

	sold = base.GetClosest( dist, true );

	if ( base.HasOrders() && base.ORDTACMod() < 0 )
		bCanMove = false;

	if ( bCanMove && !base.HasTargets() && !base.Moving( true ) && base.Leader() && base.Leader().RunningPlay() )
		base.MoveToBestGround( 1, true );
	else if ( bCanMove && !base.HasTargets() && !base.Moving( true ) && base.ForceArtyMove() )
		base.MoveToBestGround( 4, false );
	else if ( sold && dist < DISTARTYHOLD && base.Moving( true ) && !base.UnLimbered() && base.GetQuad(dest)!=OppositeQuad(base.GetQuad(sold)) ) //I don't ckeck if I'm moving on the oppostive quad respect my enemy, avoid stop during redeploymnets
	{
		comm.Comm(eComHalt);
		comm.Unit(base.GetID());

		comm.SetBit(eCBAll);

		comm.DelBit(eCBChngDest);

		CXUtil::AddComm( &comm );

		comm.Comm(eComForm);
		comm.Val(base.FightForm());

		comm.SetBit(eCBAll);

		CXUtil::AddComm( &comm ); 
	}
	else
		CUnitBrigThink( ticks, base );

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CCavOffThink( int ticks, CXOff base )
{
	CXVec  loc;
	SXComm comm;


	if ( base.OffThink() )
		return true;

	if ( base.NumSubs() == 0 && base.NumDetach() > 0 ) // follow at least 1 of our scout units
	{
		loc = base.Det(0).GetLocBehind( 50 );

		if ( loc != base.Dst() && base.State() == eUnitStand )
		{
			base.SetDest( loc );

			comm.Comm(eComMove);
			comm.Vec(true, loc);

			base.Command( &comm );

			comm.Comm(eComEndDir);
			comm.Vec(true,base.Det(0).Dir());

			base.Command( &comm );
		}	
	}
	else
	{
		CUnitBrigThink( ticks, base );
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CSoldAmmoThink( int /*ticks*/, CXUnit base )
{
	int temp = 0;
	int dist2;

	CXVec friendtosupply;
	bool   bEmerg = false;
	SXComm comm;
	CXUnit sold;

	if ( base.FriendEmpt() && base.EnemyClose().Valid() && base.EnemyCloseYds() < DISTOFFSCARED )
		bEmerg = true;
	else if ( base.FriendCloseYds() > base.EnemyCloseYds() && base.EnemyClose().Valid() && base.EnemyCloseYds() < DISTOFFSCARED )
		bEmerg = true;

	if ( !bEmerg && !base.Moving() )
		bEmerg = CXUtil::HexIsFire( base.Loc() );

	if ( base.Ammo() > 0 && base.State() != eUnitRun )
	{
		base.FriendBeg();
		
		while ( base.FriendNext( sold, dist2 ) )
		{
			if ( !sold.Standing() || sold.IsOfficer() )
				continue;

			if ( sold.UnitType() == eUnitArt )
			{
				if ( sold.NeedAmmo() )
					temp = sold.Ammo() + 1;
				else
					temp = 0;
			}
			else
			{		
				temp  = base.Leader().GetMod( eGameMaxAmmoGiven );
				temp *= sold.NumMen();
			}
			
			if ( sold.Ammo() < temp )
				base.GiveMeAmmo( sold );
		}
	}
	dist2 = base.Loc().Dist2Yds( base.Leader().Loc() );

	if ( bEmerg && base.State() != eUnitRun ) // use for retreat
	{
		comm.Comm(eComRetreat);

		base.Command( &comm );
	}
//-----test ammo wagon resupply friends
	else if (!base.Moving() && base.Ammo()>0 && ArtyWithoutAmmos(base, base.Leader(), friendtosupply) )
	{
		comm.Comm(eComMoveRoad);
		comm.Vec( true, friendtosupply );

		base.Command( &comm );

	}
//----------------------------------------
	else if ( dist2 > DIST2WAGONCATCH && !base.Moving() )
	{
		comm.Comm(eComMoveRoad);
		comm.Vec( true, base.Leader().Loc() );

		base.Command( &comm );
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MyThink
//*************************************************************************************************************
//*************************************************************************************************************

bool CSoldAmmoMyThink( int /*ticks*/, CXUnit base )
{
	int temp = 0;
	int dist2;

	CXUnit sold;

	if ( base.Ammo() > 0 )
	{
		base.FriendBeg();
		
		while ( base.FriendNext( sold, dist2 ) )
		{
			if ( !sold.Standing() || sold.IsOfficer() )
				continue;

			if ( sold.UnitType() == eUnitArt )
			{
				if ( sold.NeedAmmo() )
					temp = sold.Ammo() + 1;
				else
					temp = 0;
			}
			else
			{		
				temp  = base.Leader().GetMod( eGameMaxAmmoGiven );
				temp *= sold.NumMen();
			}
			
			if ( sold.Ammo() < temp )
				base.GiveMeAmmo( sold );
		}
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MyThink
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitBrigMyThink( int, CXOff base )
{
	base.OffThink();

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitDivThink( int , CXOff base ) 
{
	int i;
	int troub = 0;
	int count = 0;

	bool  bCour   = base.CourTime() == 0 || base.CourTime() + 15 * TICSPERMIN < CXUtil::GetTime();
	bool  bDone   = true;
	bool  bMoving = true;

	SXComm comm;

	int dist;
	int mdist = 0;

	CXOff art;
	CXOff off;
	CXOff best = NULL;

	CXUnit targ = NULL;
	CXUnit temptarg;



	if ( base.OffThink() )
		return true;


// rally action if the officer is standing
	if ( base.Standing() )
		base.Rally();


//evaluation of single brigades under the division
	for ( i = 0; i < base.NumSubs(); i++ )
	{
		off = base.Sub(i);

		bMoving &= off.Moving( true );  //brigade is all moving	


		if ( off.EngagingEnemy() || off.UnderCloseFire() )  //if the brigade is under close fire or engaging the enemy
		{
			if ( off.InTrouble() )    //I count if the brigade is in trouble
				troub++;

			temptarg = off.GetClosest( dist );   //get the target closest and his distance

			if ( bCour && !off.HasOrders() && base.HasPlay() )    //can send a courier, the brigade has not orders but the division has play in process
			{

				base.CourTime(true,CXUtil::GetTime());  //set a courier time

				comm.Param(1, base.PLAYLOrdersCall(0) );  //compose order for teh brigade on the base of play of division
	
				comm.Comm(eComOrders);
				comm.Unit(off.GetID());
				base.SendOrdersByCour( off, 1, &comm, true );  //send order by courier
			}
			else if ( bCour && !off.HasOrders() && base.HasOrders() && !off.PlyrCtrl() )  //can send a courier, the brigade has not orders but the division has orders and not in player control
			{

				base.CourTime(true,CXUtil::GetTime()); //set a courier time

				comm.Param(1, base.ORDCall() );	 //compose order for teh brigade on the base of orders of division
				comm.Comm(eComOrders);
				comm.Unit(off.GetID());
				base.SendOrdersByCour( off, 1, &comm, true ); //send order by courier
			}
			// Throwing this in to allow brigs to defend themselves
			else if ( bCour && !off.HasOrders() && !base.HasOrders() && !base.HasPlay() && !off.PlyrCtrl() && !base.ReEval() &&
				        base.TACType() == eTacReserve )  //can send a courier, the brigade has not orders, the division also and she has not play, she'sn't under player control and she's not revaluating the tactics, but his mission is tactreserve
			{

				base.CourTime(true,CXUtil::GetTime()); //set a courier time
				comm.Param( 1, base.AssignedOrders() ); //assign autonomous orders  (change of stance)
				comm.Comm(eComOrders);
				comm.Unit(off.GetID());
				base.SendOrdersByCour( off, 1, &comm, true ); //send order by courier
			}

			if ( temptarg )   //there is a target
			{
				count++;

				if ( !best || dist < mdist )  //I evaluate if it is the best (nearest) between all the possibile targets and the nearest of brigades
				{
					best  = off;
					targ  = temptarg;
					mdist = dist;
				}
			}
		}
	}


	for ( i = 0; i < base.NumArty(); ++i )  //I loop on the artillery batteries of the division
	{
		art = base.Arty(i);

		if ( art.UnLimbered() && art.HasTargets() && art.Fighting() )  //if the battery is unlimbered and has targets and is fighting
		{
			temptarg = art.GetClosest( dist );   //select the nearest enemy

			if ( temptarg )
			{
				count++;
				if ( !best || dist < mdist )  //I evaluate if it is the best (nearest) between all the possibile targets and the nearest of batteries
				{
					best  = art;
					targ  = temptarg;
					mdist = dist;
				}
			}
		}
	}


	// reduced this to just 1 brig in trouble
	if ( base.Leader() && troub > 0 && ( base.HelpTime() == 0 || base.HelpTime() + 30 * TICSPERMIN < CXUtil::GetTime() ) )  //if there is units in trouble, the division is attached and i had help time
	{
		base.HelpTime(true,CXUtil::GetTime());  // I set the help time

		comm.Val(base.GetID());                 //sending request help with courier
		comm.Vec(true,base.Loc());
		comm.Comm(eComHelp);
		comm.Unit(base.Leader().GetID());

		base.SendOrdersByCour( base.Leader(), 1, &comm, true );

		char msg[256];

		sprintf(msg,"DIV:%s REQUESTING HELP from %s", base.Name(), base.Leader().Name() );
		CXUtil::AddLog(msg);
	}


	if ( count == 0 || count == base.NumSubs() + base.NumArty() )   //if enemies are zero or equal to our subunits
	{
		if ( count == 0 )                   //if zero
		{
			base.DelPlay();                //delete the division play	
		
			if ( base.ReEval() )            //if the division is revaluate, I exectue a tactics revaluation
				base.ReEvalTactics();
			else if ( base.TACType() != eTacReserve && base.TACObj() >= 0 )    //if it is not in tacreserve and there is tactical objectives, I evaluate the objectives
				base.EvalObjective();
			else if ( base.TACType() == eTacQuad && !base.Moving() )     // I have tactical quadrant and not moving, I evaluate quadrant    
				base.EvalQuad();
			else if ( ( base.TACType() == eTacLoc || base.TACType() == eTacQuadLoc || base.TACType() == eTacHold ) && !base.Moving() )  //if the tacttype is location,location quadrant or holding and not moving I evaluate the location
				base.EvalLoc();

		}
		
		return true;

	}


//	if ( base.HasOrders() && (!base.ORDTACDivPlay() || !base.GetOrders()->divplay) )  
//	if ( base.HasOrders() && !base.GetOrders()->divplay )
	if (!base.ORDTACDivPlay())  //division play permitted only if set order and the order permit a play
	{
		// no play if our orders say not to
		base.PSTATDone(true,true); // set this to true to prevent executeplay from running

	}
//	else if ( base.GetOrders()->divplay && !base.HasPlay() && base.HasTargets() )  //if division has not play
	// I changed this because the div is doing nothing if it has not gotten orders yet
	else if ( !base.HasPlay() && count > 0 )  //if division has not play
	{
		comm.Comm(eComHalt);             //halt for all
		comm.Unit(base.GetID());
		comm.DelBit(eCBAll);

		CXUtil::AddComm(&comm);

		comm.SetBit(eCBAll);

		for ( i = 0; i < base.NumSubs(); ++i )  //check of brigade subunits 
		{
			off = base.Sub(i);

			if ( off == best )   //if the best I don't work with her
				continue;
		
			if ( off.HasTargets() || ( off.EngagingEnemy() && off.Moving() ) )   //if has targets or moving engaging the enemy
			{              //nothing to do
			}
			else
			{
				comm.Comm(eComHalt);              //otherwise I stop and I execute the orders
				comm.Unit(off.GetID());

				if ( !off.PlyrCtrl() )
					off.Command( &comm );

				comm.Comm(eComOrders);
				
				comm.Param(1,"");

				off.Command( &comm );
			}
		}
		base.RunPlay( best, targ );   //  I run the play for the best to his target

		comm.Comm(eComMove);
		comm.Vec(true,base.PSTATLRct(eSpotBotMid));
		comm.Unit(base.GetID());

		comm.DelBit(eCBAll);

		CXUtil::AddComm( &comm );

		comm.Comm(eComRun);

		CXUtil::AddComm( &comm );
	}


	for ( i = 0; i < eSpotMax && !base.PSTATDone(); ++i )
	{
		// don't worry about checking cav here, we exclude them in runplay

		if ( base.PSTATLValid(i) )
		{
			bDone = false;
			
			if ( !base.PSTATLOff(i).Moving() && !base.PSTATLOff(i).InTrouble() )
//			if ( !base.PSTATLOff(i).Moving() && !base.PSTATLOff(i).InTrouble() )
			{
				base.ExecutePlay( true, i, base.PSTATLOff(i) );

				base.PSTATLValid(i,true,false);
			}
			else if ( base.PSTATLOff(i).EngagingEnemy() || base.PSTATLOff(i).HasTargets() )
				base.PSTATLValid(i,true,false);
		}

		if ( base.PSTATRValid(i) )
		{
			bDone = false;

			if ( !base.PSTATROff(i).Moving() && !base.PSTATROff(i).InTrouble() )  	
//			if ( !base.PSTATROff(i).Moving() && !base.PSTATROff(i).InTrouble() )
			{
				base.ExecutePlay( false, i, base.PSTATROff(i) );

				base.PSTATRValid(i,true,false);
			}
			else if ( base.PSTATROff(i).EngagingEnemy() || base.PSTATROff(i).HasTargets() )
				base.PSTATRValid(i,true,false);
		}
	}
	base.PSTATDone(true,bDone);


	if ( base.PSTATDone() )
		base.CheckReserves();

	if ( base.GetClosest( dist ) )
		base.OffScared();


	return true;



}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitCorpThink( int , CXOff base ) 
{
	int dist;

	if (!base.Leader())  //campaign retreat calculation
		CampaignRetreatCalculation( base );

	if ( base.OffThink() )
		return true;

//	if ( Standing() )
//		Rally();

	if ( base.ReEval() )
		base.ReEvalTactics();

	if ( base.GetClosest( dist ) )
		base.OffScared();

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitArmyThink( int, CXOff base ) 
{
	int i;
	int j;
	int k;
	int type;
	int nums[ 50 ]; // corpnum
	int subs[ 50 ]; // m_subs index
	int objs[ 50 ]; // m_subs index

	bool bMod = false;
	bool bMaj;

	int dist;
	int fdist;
	int mdist;

	CXRect rect;

	SXArmyPlay aplay = base.ArmyPlay();


	if (!base.Leader())   //retreat campaign calculation only overall leader
		CampaignRetreatCalculation( base );


	if ( base.OffThink() )
		return true;

	if ( base.ReEval() )   //no play set, I define one on the base of battle type after a tactics revaluation
	{
		base.ReEvalTactics();  //revaluate tactics permit to select the grand tactics (plays possible)

	//there're predefined tactics for specific sandbox, otherwise is used the grand tactics table
		if ( aplay == NULL && base.NumComplete() == 0 && !base.PlyrCtrl() && !CXUtil::IsMulti() )
		{
			type = CXUtil::OpenType();

			if ( type == eTypeAtt1 || type == eTypeAtt2 )
			{
				// attack
				if ( base.Rank(eRankSide) == type )
					aplay = CXUtil::GetAttackPlay(); 
				else
					aplay = CXUtil::GetDefendPlay();
			}
			else if ( type == eTypeMeet )
			{
				base.GetMeetAttackQuad( rect );

				aplay = CXUtil::GetMeetingPlay( rect ); 
			}
		}
	}
	
	if ( aplay != NULL )   //there is a play
	{
		for ( i = 0; i < 50; i++ )  //initialization of arrays
		{
			subs[i] = -1;
			nums[i] = -1;
			objs[i] = -1;
		}

		for ( i = 0; i < base.NumComplete(); i++ )  //I check if I have already corps with play assigned 
		{
//nums contains the link between a corps and the play linked
//  subs the corps of the army with a switch for the play yes or no
			nums[ base.PLAYCorpNum(aplay,i) ] = base.Complete(i); // corpnum is 1 based
			subs[ base.Complete(i) ] = 1;
		}

		// we need to see if there are any majors and if there are we have to modify the play to encompasses them
		if ( base.NumComplete() == 0 )  //still not corps assigned to play
		{

//plays array is filled on the base of the possible value used in the grandtactics table, filtered on the base of type of game to play
// this permit to select a possibile variable options
			for ( i = 0; i < base.PLAYSize(aplay); i++ )  //loop on my play list
			{
				if ( base.PLAYFTime(aplay,i) < CXUtil::GetTime() )  //every play has a start moment defined at the start of match, I use only those valid in this moment
				{
					CXRect xrect = base.PLAYRect(aplay,i);  //every play in the grand tactics has the characteristics of rect to create

					switch ( base.StartQuad() )  //start quad is always where there is the army leader is the reference point; I draw the borders of the ohter quadrants
					{
						case 1:
							rect.TopX( true, CXUtil::MaxCoord() - xrect.TopY() );
							rect.BotX( true, CXUtil::MaxCoord() - xrect.BotY() );
							rect.TopY( true, xrect.BotX() ); 
							rect.BotY( true, xrect.TopX() );
							break;
					
						case 2:
							rect.TopX( true, xrect.TopY() );
							rect.BotX( true, xrect.BotY() );
							rect.TopY( true, CXUtil::MaxCoord() - xrect.BotX() ); 
							rect.BotY( true, CXUtil::MaxCoord() - xrect.TopX() );
							break;
					
						case 3:
							rect.TopX( true, CXUtil::MaxCoord() - xrect.TopX() );
							rect.BotX( true, CXUtil::MaxCoord() - xrect.BotX() );
							rect.TopY( true, CXUtil::MaxCoord() - xrect.TopY() ); 
							rect.BotY( true, CXUtil::MaxCoord() - xrect.BotY() );
							break;
						default:
							rect = xrect;
			
					}
					rect.Norm();

					CXUtil::AreMajorsInRect( rect, objs, base.Rank(eRankSide) );  //check objectives inside the various rects.
				}
			}
			i = 0;

			while ( objs[i] >= 0 )				//I verify there is at least 1 objective insides the quadrants of this play
				bMod |= ( objs[i++] == 0 );
		}

		// now assign plays -> it loops until all the corps have a play assigned
		for ( i = base.NumComplete(); i < base.PLAYSize(aplay); i++ )
		{
			if ( base.PLAYFTime(aplay,i) < CXUtil::GetTime() )  //same checks as above
			{
				CXRect xrect = base.PLAYRect(aplay,i);

				switch ( base.StartQuad() )
				{
					case 1:
						rect.TopX( true, CXUtil::MaxCoord() - xrect.TopY() );
						rect.BotX( true, CXUtil::MaxCoord() - xrect.BotY() );
						rect.TopY( true, xrect.BotX() ); 
						rect.BotY( true, xrect.TopX() );
						break;
				
					case 2:
						rect.TopX( true, xrect.TopY() );
						rect.BotX( true, xrect.BotY() );
						rect.TopY( true, CXUtil::MaxCoord() - xrect.BotX() ); 
						rect.BotY( true, CXUtil::MaxCoord() - xrect.TopX() );
						break;
				
					case 3:
						rect.TopX( true, CXUtil::MaxCoord() - xrect.TopX() );
						rect.BotX( true, CXUtil::MaxCoord() - xrect.BotX() );
						rect.TopY( true, CXUtil::MaxCoord() - xrect.TopY() ); 
						rect.BotY( true, CXUtil::MaxCoord() - xrect.BotY() );
						break;
					default:
						rect = xrect;

				}

				if ( i == 0 && bMod )   //still no corps assigned assigned and objective present in these plays I set this full rect
					rect.Set( 0, 0, CXUtil::MaxCoord(), CXUtil::MaxCoord() );

				rect.Norm();

			  // we've already assigned this corp
				if ( nums[ base.PLAYCorpNum(aplay,i) ] >= 0 )
					k = nums[ base.PLAYCorpNum(aplay,i) ];
				// we need to assign a corp
				else
				{
					k     = -1;
					mdist = 0;
					bMaj  = CXUtil::AreMajorsInRect( rect, objs, base.Rank(eRankSide) ); //objective inside this
					
					for ( j = 0; j < base.NumSubs(); ++j )  //loop on the corps
					{
						if ( subs[j] == -1 )  //nothing assigned to this subs array
						{
							fdist = base.Sub(j).Loc().Dist2Yds( rect.Mid() );  //distance from the middle area of rect

							// make sure that my corp gets the major vp
							if ( objs[0] >= 0 && base.Sub(j).InCorp() && !CXUtil::IsMulti() )
							{
								if ( bMaj )
								{
									k = j;
									break;
								}
							}
						 	else if ( k == -1 || fdist < mdist )  //I assign this quadrant to the nearest corps from his center
							{
								k     = j;
								mdist = fdist;
							}
						}
					}

					if ( k == -1 )
						continue;

					nums[ base.PLAYCorpNum(aplay,i) ] = k;  //memo the corps index inside the army in the array where the index is the corp number for this play in the grand tactics table
					subs[ k ] = 1;  //set filled in the sub units array
				}
				base.CompleteAdd( k );  //mark the sub unit as already completed

				if ( k < base.NumSubs() && k >= 0 )   //at least 1 corps with a play ready, I start the play in the rect assigned
					base.RunArmyPlay( base.Sub(k), rect );
			}
		}
	}
	else if ( base.GetClosest( dist ) )
		base.OffScared();
	else if ( base.Standing() )
		base.Rally();

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CUnitSideThink( int , CXOff base ) 
{
	int dist;

	if (!base.Leader())  //campaign retreat calculation
		CampaignRetreatCalculation( base );

	if ( base.OffThink() )
		return true;

	if ( base.GetClosest( dist ) )
		base.OffScared();
	else if ( base.Standing() )
		base.Rally();

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

bool CSoldCour1Think( int, CXCour base )
{
	int i;
	int dist2;

	CXUnit sold;
	SXComm comm;
	SXComm screen;

	char msg[256];

	std::string temp;
	std::string str;

	if ( base.TargId() < 0 )
		return false;

	sold = CXUtil::GetSold( base.TargId() );	

	dist2 = base.Loc().Dist2Yds( sold.Loc() );

	if ( dist2 < DIST2COURARRIVE )
	{
		sprintf(msg, "MESSENGER ARRIVED,%s", (LPCTSTR)sold.Name() );

		CXUtil::AddLog(msg);

		// orders string message
		if ( strlen( base.Orders() ) > 0 )
		{
			comm.BuildCommand( base.Orders() );

			comm.Unit(base.TargId());

			if ( sold.PlyrCtrl() || sold.Locked() )
			{
				switch ( comm.Comm() )
				{
					case eComMove:
					case eComMoveDir:
					case eComMoveRoad:
					case eComMovePath:
					case eComForceMove:
						comm.Comm(eComOrderDest);
						CXUtil::AddComm( &comm );
						comm.Comm(eComMove);
						break;
					case eComForm:
					case eComUseRoad:
						break;
					default:
						CXUtil::AddComm( &comm );
						break;
				}
			}
			else
				CXUtil::AddComm( &comm );

			sprintf( msg, "COURIER MESSAGE,%s\n", base.Orders() );
			CXUtil::AddLog( msg );
		}
		// command list message
		else if ( base.NumComm() > 0 && base.HasComm() )
		{
			for ( i = 0; i < base.NumComm(); ++i )
			{
				SXComm xcomm = base.Comm(i);

				xcomm.Unit(base.TargId());

				if ( xcomm.Comm() == eComAddCourTxt )
				{
					// do nothing
				}
				else if ( sold.Locked() && xcomm.ChkBit(eCBFromPlyr) )
				{
					// do nothing
				}
				else if ( sold.AppSameRank() || (sold.TCOn() && !xcomm.ChkBit(eCBFromPlyr)) )
				{
					switch ( xcomm.Comm() )
					{
						case eComMove:
						case eComMoveDir:
						case eComMoveRoad:
						case eComMovePath:
						case eComForceMove:
							xcomm.Comm(eComOrderDest);
							CXUtil::AddTimeComm( &xcomm, base.StartTime() );
							xcomm.Comm( eComMove );
							break;
						case eComForm:
						case eComUseRoad:
							break;
						default:
							CXUtil::AddTimeComm( &xcomm, base.StartTime() );
					}
				}
				else
					CXUtil::AddTimeComm( &xcomm, base.StartTime() );
			}
			comm = base.Comm(0);
		}	
		CXUtil::TriggerEvent( -1, base.TargId(), eEvtCourier );

		// if this is a player character, let's show the message window
		// but if it's from the player in single player, let's hide it
		if ( CXUtil::IsMulti() || !comm.ChkBit(eCBFromPlyr) )
		{
			if ( (sold.IsMe() || comm.ChkBit(eCBFromPlyr)) && comm.Comm() != eComLoadLayout && (sold.MyGuys() || sold.IsMe()) )
			{
				// turn off for strat level 2
				if ( CXUtil::IsMulti() || CXUtil::ScenType() == eScenSand )
					base.ShowScreen(false,comm.ChkBit(eCBFromPlyr));
				else if ( CXUtil::StratAI() % 10 != 2 || base.NumComm() != 1 || base.Comm(0).Comm() != eComTactics )
					base.ShowScreen(false,comm.ChkBit(eCBFromPlyr));
			}
		}
		base.DelComm();

		base.NumComm(true,0);
		base.DelOrders();

		base.TargId(true, -1);

		comm.Comm(eComHalt);

		base.Command( &comm );

		return false;
	}
	
	if ( base.EnemyCloseYds() < DISTCOURKILL * 4 && !base.PathEmpty() )
	{
		base.RecalcTics(true,0);

		sold = CXUtil::GetSold( base.TargId() );	

		comm.Comm(eComMove);

		comm.Vec(true,sold.Loc());

		base.Command( &comm );

		comm.Comm(eComRun);

		base.Command( &comm );
	}

	// make sure that we're going to the right place
	if ( !base.Moving() )
		base.RecalcTics(true,COUR_RECALCTICKS + 5);
	else if ( base.Dst() != sold.Loc() )
	{
		base.SetDest( sold.Loc() );

		base.RecalcTics(true,base.RecalcTics()+1);
	}

	if ( base.RecalcTics() > COUR_RECALCTICKS )
	{
		base.RecalcTics(true,0);

		sold = CXUtil::GetSold( base.TargId() );	

		comm.Comm(eComMove);

		comm.Vec(true,sold.Loc());

		if ( base.NumCour() % 2 == 0 && sold.Loc().Dist2Yds(base.Loc()) > DIRECT_DIST2 )
			comm.Comm(eComMoveRoad);
			
		base.Command( &comm );

		comm.Comm(eComRun);

		base.Command( &comm );
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Callers
//*************************************************************************************************************
//*************************************************************************************************************

int SowUnitBrigThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CUnitBrigThink( ticks, unit );		

	return res;
}

int SowUnitDivThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CUnitDivThink( ticks, unit );		

	return res;
}

int SowUnitCorpThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CUnitCorpThink( ticks, unit );		

	return res;
}

int SowUnitArmyThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CUnitArmyThink( ticks, unit );		

	return res;
}

int SowUnitSideThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CUnitSideThink( ticks, unit );		

	return res;
}

int SowArtyOffThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CArtyOffThink( ticks, unit );		

	return res;
}

int SowCavOffThink( int ticks, void *punit )
{
	int res;

	CXOff unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CUnitBrigMyThink( ticks, unit );		
	else
		res = CCavOffThink( ticks, unit );		

	return res;
}

int SowSoldAmmoThink( int ticks, void *punit )
{
	int res;

	CXUnit unit((CUnit *)punit);

	if ( unit.PlyrCtrl() )
		res = CSoldAmmoMyThink( ticks, unit );		
	else
		res = CSoldAmmoThink( ticks, unit );		

	return res;
}

int SowSoldCourThink( int ticks, void *punit )
{
	int res;

	CXCour unit((CUnit *)punit);

	res = CSoldCour1Think( ticks, unit );		

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

