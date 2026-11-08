//*************************************************************************************************************
//*************************************************************************************************************
// File: offcmds.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "sowgbaiinf.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"
#include "../sowmod/xtables.h"

#include "SOWWLMOD.h"

#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define BUFF_CHECK(val) CXUtil::MultSpec(val,11,10)
#define DISTCAV1AUTOMELEE 100
#define DISTINF1AUTOMELEE 15
#define INFCOLUMNATTACK 2
#define INFLINEATTACK 3
#define INFLINEADVANCE 1
#define CAVATTACK 1
#define CAVWEIGHT 10
#define BUFFER_DIST 5

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckAdvancePath
//*************************************************************************************************************
//*************************************************************************************************************

bool CheckAdvancePath( CXUnit base, int dist )
{
	int i;

	for (i=5;i<=dist;i+=5)
	{
		if (!base.ValidSpot(base.GetLocAhead(i)))
			return false;
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckBackPath
//*************************************************************************************************************
//*************************************************************************************************************

static bool CheckBackPath( CXUnit base, int dist )
{
	int i;

	for (i=5;i<=dist;i+=5)
	{
		if (!base.ValidSpot(base.GetLocBehind(i)))
			return false;
	}

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckHaltLine
//*************************************************************************************************************
//*************************************************************************************************************

bool CheckHaltLine( CXUnit base )
{
//check if I have space for square
	int i; 
	CXVec rect[4];


	for (i=0;i<=base.MaxRangeYds();i=i+10)
	{
		rect[0] = base.GetLocLeft(i);
		rect[1] = base.GetLocLeft(base.MaxRangeYds()-i);
		rect[2] = base.GetLocRight(i);
		rect[3] = base.GetLocRight(base.MaxRangeYds()-i);
		if (!base.CanHaltUnitRect(rect))
			return false;
	}

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsColumn
//*************************************************************************************************************
//*************************************************************************************************************

bool IsColumn( CXUnit base )
{
//check if the formation is one of column types
	if (base.IsFormType(eFTAssault) || base.IsFormType(eFTColumnHalf) || base.IsFormType(eFTColumnFull) 
		|| base.IsFormType(eFTMarch) )
		return true;

	return false;
}

//new infantry
//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsLine
//*************************************************************************************************************
//*************************************************************************************************************

bool IsLine( CXUnit base )
{
//check if the formation is one of line types
	if (base.IsFormType(eFTFight) || base.IsFormType(eFTLine))
		return true;

	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CanGunRedeploy
//*************************************************************************************************************
//*************************************************************************************************************

static bool CanGunRedeploy( CXOff off, CXUnit unit, CXVec Dest )
{
	int i;
	CXUnit subunit;

//--not deploy in one location where there is already a gun of the same battery
// avoid overlapping of batteries
	for (i=0;i<off.NumSubs();i++)
	{
		subunit = off.Sub(i);
		if (subunit.GetID()==unit.GetID())
			continue;
		if (subunit.Loc().DistYds(Dest)<=5)
			return false;
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsSkirmisher
//*************************************************************************************************************
//*************************************************************************************************************

bool IsSkirmisher(CXUnit base)
{
	//check if the formation is one of skirmishers types
	if (base.IsFormType(eFTSkirmish) || base.IsFormType(eFTAltSkirmish))
		return true;

	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CanRedeployLine
//*************************************************************************************************************
//*************************************************************************************************************

static bool CanRedeployLine( CXOff off, CXUnit unit, CXVec Dest )
{
	int i;
	CXUnit subunit;

//--not deploy in one destination where there is already a unit of same group
	for (i=0;i<off.NumSubs();i++)
	{
		subunit = off.Sub(i);
		if (subunit.GetID()==unit.GetID())
			continue;
		if (IsSkirmisher(subunit) && subunit.Marching())
			continue;
		if (subunit.Dst().DistYds(Dest)<=subunit.LongRangeYds())	
			return false;
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CanGunRedeployLine
//*************************************************************************************************************
//*************************************************************************************************************

static bool CanGunRedeployLine( CXOff off, CXUnit unit, CXVec Dest )
{
	int i;
	CXUnit subunit;

//--not deploy in one destination where there is already a unit of same group
	for (i=0;i<off.NumSubs();i++)
	{
		subunit = off.Sub(i);
		if (subunit.GetID()==unit.GetID())
			continue;
		if (subunit.Dst().DistYds(Dest)<=5)
			return false;
	}
	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CavalryBlocked
//*************************************************************************************************************
//*************************************************************************************************************

static bool CavalryBlocked( CXUnit base )
{
//  in the case the view is blocked for the cavlary I check if it is ground or friend related
// for avoid graphical not motivated stop

	CXUnit sold;
	int dist;
	int diff;

	if (!base.Blocked())
		return false;

	if (!base.FriendClose())
		return false;

//I check if I have friend to near to me
	base.FriendBeg();

	while ( base.FriendNext( sold, dist ) )
	{
		if (sold.IsOfficer())  //no officers
			continue;
		
		if (sold.UnitType()!=eUnitInf && sold.UnitType()!=eUnitCav)  //no units different from cavalry and infantry
			continue;
		
		if (sold.UnitType()==eUnitInf && IsSkirmisher(sold) )  //no skirmishers
			continue;

		if (sold.UnitType()==eUnitCav && sold.Marching())  //cavlary moving is not checked
			continue;

		if (sold.State()==eUnitMelee || sold.State()==eUnitCharge)
			continue;

		diff = base.Pos().GetAngleDiff(sold.Loc(),true);   //if friend is before me

		if ( IsLine(sold) || base.DoesShareHex(sold))
		{
			if (diff<=90 && diff>=270)
				return true;
		}
		if ( IsLine(base) )
		{
			if (diff<=60 && diff>=300)
				return true;
		}
		if (diff<=45 && diff>=315)
			return true;
	}
	return false;
}

//new infantry
//*************************************************************************************************************
//*************************************************************************************************************
// Function: FearCheck
//*************************************************************************************************************
//*************************************************************************************************************

int FearCheck( CXUnit base, CXUnit sold )
{
	int defender;
	int attacker;
	int diff = base.Pos().GetAngleDiff(sold.Loc(),true); 
	int sidemultiplier = 1;
	int attackertypemultiplier = 1;
	int defendertypemultiplier = 1;
	int attackerrunmultiplier = 1;
	int defenderrunmultiplier = 1;
	int result = 0;

//fear check -> calculation for pre-melee reaction
//--- base = defender
//--- sold  = attacker

	if (diff>80 && diff<280)
		sidemultiplier = 2;
	if (diff>135 && diff<225)
		sidemultiplier = 3;

	if (base.UnitType()==eUnitCav)  //cavalry multiplier
	{
		defendertypemultiplier *= CAVWEIGHT;
		if (base.State()==eUnitRun || base.State()==eUnitCharge)  //run charge is a additional factor
			 defenderrunmultiplier *= 4;
	}

	if (sold.UnitType()==eUnitCav) //cavalry multiplier
	{
		attackertypemultiplier *= CAVWEIGHT;
		if (sold.State()==eUnitRun || sold.State()==eUnitCharge) //run charge is a additional factor
			 attackerrunmultiplier *= 4;
	}
	defender = ((base.NumMen()*defendertypemultiplier)/100);
	attacker = ((sold.NumMen()*attackertypemultiplier)/100);

	defender =  defender + base.Morale() + (base.MorUnitBon()/100) + (base.MorOffBon()/100)
				+ base.VolleyStatus() 
				- (base.UnderFire() + base.UnderFire1() + base.UnderRecentFire()) //fire penalty
				+ (base.IsHighGrnd()*4) + (base.OnDefTerr()*2) - base.IsFlanked() + defenderrunmultiplier;

	attacker = attacker + sold.Morale() + (sold.MorUnitBon()/100) + (sold.MorOffBon()/100) 
			   - (sold.UnderFire() + sold.UnderFire1() + sold.UnderRecentFire())  //fire penalty
			   - base.VolleyStatus() + sold.CanRun() + (sold.IsHighGrnd()*2) - sold.IsFlanked() + attackerrunmultiplier;

	if (base.Leader())  //leader personality in the calculation
		defender += base.Leader().Personality();
	if (sold.Leader())
		attacker += sold.Leader().Personality();

	if (IsSkirmisher(base) || base.IsArt())  //skirmishes and art are valid only 1/5
		defender /= 5;
	if (IsSkirmisher(sold) || sold.IsArt()) //skirmishes and art are valid only 1/5
		attacker /= 5;

	attacker *= sidemultiplier;

	if (base.State()==eUnitMelee || base.State()==eUnitFallback)
		defender /= 5;

	if (defender<=0)  //for avoid divide by zero exception
		defender = 1;

	result = attacker/defender;

	if ((attacker%defender)*2>=defender)  //commercial rounding
		result += 1;

	return result;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: FriendAheadMe
//*************************************************************************************************************
//*************************************************************************************************************

static bool FriendAheadMe( CXUnit base, bool &overlap, bool &left, bool &right, bool &behind)
{
	CXUnit sold;
	int dist;
	int diff;
	bool bRes = false;

//I check if I have friend to near to me for wheel
	base.FriendBeg();

	while ( base.FriendNext( sold, dist ) )
	{
		
		if (sold.IsOfficer())  //no officer
			continue;
	
		if (dist>base.MaxRangeYds())  //out of max weapon range
			continue;
		
		if (sold.UnitType()!=eUnitInf && sold.UnitType()!=eUnitCav)  //only infantry or cavalry
			continue;

		if (sold.State()!=eUnitStand && sold.State()!=eUnitShoot && !sold.Retreating() && sold.State()!=eUnitFallback    //don't check moving friends
			&& sold.State()!=eUnitAdvance)
			continue;

		if (IsSkirmisher(sold) || sold.Split()!=-1)  //no skirmishers
			continue;

		diff = base.Pos().GetAngleDiff(sold.Loc(),true);

		//if (!sold.Retreating() && base.DoesShareHex(sold) && (IsSquareType(sold,true,false)
		//	|| (sold.State()==eUnitStand && sold.EnemyCloseYds()<base.EnemyCloseYds()) ))
		//{
		//	overlap = true;
		//}

		if (!sold.Retreating() && base.DoesShareHex(sold) && sold.State()==eUnitStand )
		{
			if (IsLine(sold) && IsColumn(base))
				overlap = true;
			else if (IsColumn(sold) && IsColumn(base) && sold.EnemyCloseYds()<base.EnemyCloseYds())
				overlap = true;
			else if (IsLine(sold) && IsLine(base) && base.Blocked())
				overlap = true;
			else if (IsLine(sold) && IsLine(base) && sold.EnemyCloseYds()<base.EnemyCloseYds())
				overlap = true;
		}

		if (!bRes && (diff<= 90 || diff >= 270))  //friend very near and ahead, view blocked high possibility of collision
		{				
			if (IsLine(base) && base.Blocked())
				bRes = true;
		}
		if (!bRes &&  (diff<= 30 || diff >= 330))
		{
			if (dist<=(base.LongRangeYds()/2))
				bRes = true;  //if in the frontal arc is always blocking
		}
		if (!bRes &&  (diff<= 45 || diff >= 315))
		{
			if (IsLine(base) || base.Blocked())
				bRes = true;  //if in the frontal arc is always blocking
			if (IsLine(sold) && dist<=(base.LongRangeYds()/2)) 
				bRes = true;  //if in the frontal arc is always blocking
		}
		if (!bRes && (diff<= 70 || diff >= 290))
		{			
			if ((IsLine(sold) || IsLine(base)) && (dist<=(base.LongRangeYds()/2) || base.Blocked()))
				bRes = true;
		}

		if (!right && (diff< 315 && diff >= 225))
			right = true;
		if (!left && (diff> 45 && diff <= 135))
			left = true;
		if (!behind && (diff< 225 && diff > 135))
			behind = true;
	}
	return bRes;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: ReturnSkirmParent
//*************************************************************************************************************
//*************************************************************************************************************

CXUnit ReturnSkirmParent( CXUnit base )
{
	int i;
	CXOff off = base.Leader();
	//return the parent of skirm unit

	if (off==NULL || off.NumSubs()==0)
		return NULL;

	for ( i = 0; i < off.NumSubs(); ++i )
	{
		CXUnit sub = off.Sub(i);

		if (sub.GetID()==base.Split())    //I check if there're splitted unit and what of them has base as parent
			return sub;
	}
	return NULL;
}

//-------------new skirmishers----------------------
//*************************************************************************************************************
//*************************************************************************************************************
// Function: SplitCompany
//*************************************************************************************************************
//*************************************************************************************************************

bool SplitCompany( CXUnit base, int skirmlimit )
{
	int i;
//	const int skirmlimit = 3;
	bool bLim = false;
	int j = 0;

	CXOff off = base.Leader();
	//check if the battalion has already a detached company

	if (off==NULL || off.NumSubs()==0)
		return true;

	if (base.NumMen()<=450)  //only big battalions can have skirmishers
		return true;

	if (!base.CanRun())  //only if the parent can run I don't need tired skimishers
		return true;

	for ( i = 0; i < off.NumSubs(); ++i )
	{
		CXUnit sub = off.Sub(i);

		if (base.GetID()==sub.Split())    //I check if there're splitted unit and what of them has base as parent
			return true;

		if (sub.Split()!=-1) //I count the skirm units inside the brigade
			j++;
	}

	if (j>=skirmlimit)
		bLim = true;

	return bLim;
}

//-------------new skirmishers----------------------
//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetCommand
//*************************************************************************************************************
//*************************************************************************************************************

bool GetInfCommand( CXOff lead, int state, SXComm &comm, CXUnit unit )
{
	bool btarg;

	CXVec pt;
	CXUnit targ;
	CXVec dloc;
	bool enemytoonear = false;
	int diff = 0;
	bool pathblocked = false;
	bool overlap = false;
	bool right = false;
	bool left = false;
	bool behind = false;
	bool keeporders = false;

	int dist   = 1000000;

	const TStance *stance;

	std::list<CXUnit> targs;

	comm.Comm( eComMax );

	if ( !lead.HasStance() || lead.PlyrCtrl() )
		return false;

	stance = lead.GetStance();

//------ratio check------------------------
//----checking of force to make a melee attack
	int ratio = 0;
	
	if (unit.Closest())
		ratio = FearCheck(unit.Closest(),unit);
//----------------------------------------------
	keeporders = lead.GetOrders()->keeporders;

	if ( state == eSTStandNoTarget )                //situation stand no target for infantry
	{
		if ( unit.Morale() <= MORAL_CANTWHEEL || unit.PlyrCtrl() )
			return false;

		btarg = lead.HasTargets();

		if ( !btarg && lead.OrdersState() != eOrdersStay )
			targ = lead.GetSaveClosest( dist );

		// let the brigade move as one in this case
		if ( !lead.CurrTarg() && !lead.Moving( true ) && targ.Valid() )
			return false;

		bool SkirmishersOn = false;

		//new infantry : i want avoid manouvers differents from the normal advance with the enemy too near
		if (unit.EnemyClose() && unit.EnemyCloseYds()<=(BUFF_CHECK(unit.MaxRangeYds())))   
			enemytoonear = true;

		if ( unit.EnemyCloseYds()<(CXUtil::MultSpec(unit.MaxRangeYds(),stance->staymult,10)) || (lead.EnemyClose() && IsSkirmisher(lead.EnemyClose())))  //skirmishers activation
			 SkirmishersOn = true;
		if (  SkirmishersOn && enemytoonear )  //minimal distance check
			 SkirmishersOn = false;
		if (  SkirmishersOn && ( keeporders || stance->SkirmNumber==0 ) )  //no skirmishers for hold to the last
		 	 SkirmishersOn = false;
//-------new skirmishers----------------------------------------------------------------------------------------------------
		if (SkirmishersOn && lead.EnemyClose() && lead.EnemyCloseYds()<(lead.MaxRangeYds() * 4) && !lead.IsFormType(eFTAltSkirmish))  //if the brigade is already in big skirmishers formation i don't detach single companies
		{
			 if (unit.Split()==-1) 
			 {
				//conditions for deploy a the skirmishers company (formation,morale,distance from the enemy, dimension,attitude, no already split unit)
				if ( !IsSkirmisher(unit) && stance->SkirmAttitude == 1 && unit.Morale() >= stance->SkirmSplit
					&& !SplitCompany(unit,stance->SkirmNumber) && stance->SkirmDimension!=0 )
				{
					comm.Comm(eComSplit);
					comm.Val((unit.NumMen()/stance->SkirmDimension));
					unit.Command(&comm);
//					return true;
				}
			 }
		}
///new volley-------------------
		if ( unit.VolleyStatus() == 0 && !unit.TargClose() && unit.Morale() >= stance->volley && !IsSkirmisher(unit) )     // prepare a volley if I have the correct conditions
		{
			comm.Comm(eComVolleyOn);
			return true;
		}
//------new volley
//------------new skirmishers-----------------------------------------------------
		// leader's max range is based on his subordinates
		if ( btarg || ( targ.Valid() && dist < lead.MaxRangeYds() + (lead.Personality() * 15) ) )
		{
			//check of friend ahead me
			pathblocked = FriendAheadMe(unit,overlap,left,right,behind);

			if ( unit.EnemyClose() )
				diff = unit.Pos().GetAngleDiff(unit.EnemyClose().Loc(),true);  //check angle 

			if ( overlap && IsLine(unit)) //extreme case march formation for leave space
			{
				if ( (diff<=135 || diff>=315) && !right)
					comm.Comm(eComRFlank);
				else if ((diff<=45 || diff>=225) && !left)
					comm.Comm(eComLFlank);
				else if (diff>135 && diff<225)
				{
					comm.Unit(unit.EnemyClose().GetID());
					comm.Comm(eComWheelDir);
				}
				else 
				{
					comm.Vec (true, unit.GetLocBehind(10)); //too near light movement back
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
					unit.Command(&comm);
					comm.Comm(eComRun);
				}
			}
			else if ( overlap && (!right || !left || !behind) 
				&& IsColumn(unit)) //extreme case march formation for leave space
			{
				int leaddistance = unit.Loc().DistYds(lead.Loc());
				if (!right && leaddistance < (lead.Personality() * 15) && unit.ValidWalk(unit.GetLocRight(50)))
				{
					comm.Vec (true, unit.GetLocRight(50)); //too near left movement
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
				else if (!left && leaddistance < (lead.Personality() * 15) && unit.ValidWalk(unit.GetLocLeft(50)))
				{
					comm.Vec (true, unit.GetLocLeft(50)); //too near left movement
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
				else if (!behind)
				{
					comm.Vec (true, unit.GetLocBehind(20)); //too near light movement back
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
			}
			//for priority to forward movement
			else if ( !pathblocked && !keeporders && (diff<=60 || diff >= 300) && unit.EnemyClose().Valid()
				&& unit.EnemyCloseYds()>unit.LongRangeYds()
				&& unit.Morale() >= stance->moveforward  && unit.ValidDest(unit.GetLocAhead(unit.LongRangeYds())) )  //move forward, respect current position
			{
				comm.Vec (true,unit.GetLocAhead(unit.LongRangeYds()));
				comm.Comm(eComMove);
			}
			// Maintain line			
			else if ( !pathblocked && unit.Morale() >= stance->maintainline 
				&& (!unit.EnemyClose() || unit.EnemyCloseYds()>=unit.MaxRangeYds() || IsColumn(unit) )
				&& lead.GetLineDest( unit, pt )    //maintain line; the location is on the line according to the leader front
				&&  CanRedeployLine(lead,unit,pt))
			{
				comm.Vec (true, pt);
				comm.Comm(eComMove);
			}
			// Flanking
			else if ( !pathblocked && !keeporders &&
				!IsLine(unit) && //check for the line
				unit.Morale() >= stance->flanking && lead.GetFlankDest( unit, pt, targ ) &&    //flanking movement; the location is on the flank of target
				 CanRedeployLine(lead,unit,pt) )
			{
				comm.Vec (true,pt);
				comm.Comm(eComMove);
			}
			else if ( unit.EnemyClose().Valid() )  // same as above
			{
				int leaddistance = unit.Loc().DistYds(lead.Loc());

				diff = unit.Pos().GetAngleDiff(unit.EnemyClose().Loc(),true);  //check angle 
				// this will cause a wheel to target here
				if (!overlap && !keeporders && (unit.EnemyCloseYds()>(unit.MaxRangeYds()+BUFFER_DIST) || !pathblocked) 
					&& !lead.IsFormType(eFTMarch) && !unit.IsFormType(stance->brigform))  //return to fight formation
				{
					int brigform = stance->brigform;
					comm.Comm(eComFormType);    //formation for the regiment coherent with that of brigade
					comm.Val(brigform);
				}
				else if (unit.IsFlanked() && unit.Blocked() && overlap)  //if flanked I move back
					comm.Comm(eComGetAway);
				else if ( pathblocked && !keeporders && !right && !overlap && IsColumn(unit)
					&& leaddistance<=unit.MaxRangeYds() && unit.ValidWalk(unit.GetLocRight(unit.LongRangeYds())))
				{
					comm.Vec (true, unit.GetLocRight(unit.LongRangeYds())); 
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
				else if ( pathblocked && IsColumn(unit) && !keeporders && !left  && !overlap
					&& leaddistance<=unit.MaxRangeYds() && unit.ValidWalk(unit.GetLocLeft(unit.LongRangeYds())))
				{
					comm.Vec (true, unit.GetLocLeft(unit.LongRangeYds())); 
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
				else if (diff!=0 && unit.EnemyCloseYds()<= (unit.MaxRangeYds()*2) &&
					( !unit.FriendClose() || unit.FriendCloseYds()>unit.LongRangeYds() || IsColumn(unit)  
					|| (diff>135 && diff<225) ) )
				{
					comm.Unit(unit.EnemyClose().GetID());
					comm.Comm(eComWheelDir);
				}
				else if ( pathblocked && !keeporders && IsColumn(unit) && unit.UnderFire1() && unit.EnemyCloseYds()<= unit.MaxRangeYds()
					&& unit.ValidWalk(unit.GetLocBehind(unit.LongRangeYds())))
				{
					comm.Vec (true, unit.GetLocBehind(unit.LongRangeYds())); 
					comm.Dir (true, unit.Dir());
					comm.Comm(eComMoveDir);
				}
			}
		}
	}
	else if ( state == eSTStandShooting )   //situation when unit is shooting
	{
		if (!unit.TargFind(unit.Closest(),dist))  //target distance
			dist = INT_MAX;

		if ( lead.ShouldGetAmmo( unit ) )   //need ammo or i move in fort or i get away
			comm.Comm(eComGetAway);
		else if ( unit.Morale() <= stance->retreat && unit.MorChange() < 0 )   //loss of morale retreat in fort or back
			comm.Comm(eComRetreat);
		else if ( unit.VolleyStatus() != 1 && unit.OnDefTerr() && lead.StopOnDefTerr(unit) && lead.NewDefTerr()  )  //i check volley status because I risk in case of false to not shot
			return false;
		else if ( unit.Morale() <= stance->fback && unit.MorChange() < 0 )  //check of fallback retreat in fort or fallback
			comm.Comm(eComFallback);
//-----new infantry------------------------------------
		else if ( unit.State()==eUnitShoot && unit.VolleyStatus() == 1 )//release of volley
		{
			int targetdist = INT_MAX;
			unit.TargFind(unit.Closest(),targetdist);

			if (targetdist<(unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds)))  //release of volley, automatic or forced
				return true;
			else if (!keeporders || unit.UnderFire1())  //stay condition permit to hold the fire if not immediatly necessary
				comm.Comm(eComVolleyOff);
		}
		else if ( ratio>=INFLINEATTACK && lead.CanICharge(unit, stance->chargemelee) 
			&& dist < stance->InfantryCharge && unit.Closest().UnitType()!=eUnitCav )  //check infantry charge from shooting condition
			comm.Comm(eComCharge);
		else if ( (ratio>INFLINEADVANCE	|| unit.Closest().State()==eUnitFallback ) && !keeporders
			&& !unit.IsHighGrnd() && unit.Morale() >= stance->moveforward
			&&  unit.TargCloseYds()>=unit.LongRangeYds() && !unit.Blocked())  //advancing fire
		{
			diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
			if ( diff <= 20 || diff >= 340) 
			{
				comm.Comm(eComAdvance);
			}
		}
//-----new infantry---------------------------------------
		else if ( !unit.PlyrCtrl() )   //not make them if the unit is under player control or in fort or in square
		{
			if ( !FriendAheadMe(unit,overlap,left,right,behind) )
			{
				targ = unit.Closest();
				
				unit.TargFind( targ, dist );

				lead.GetOrders( dist, targ, unit );
			}
		}
	}
	else if ( state == eSTStandTargetNoAmmo )   //no ammo situation with target
	{
		if ( unit.Closest() && lead.CanICharge( unit, stance->chargenoammo ) && unit.Closest().UnitType()!=eUnitCav )  //charge
			comm.Comm(eComCharge);
		else 
			comm.Comm(eComGetAway);
	}
	else if ( state == eSTStandTargetOnlyMelee )    //no shooting only melee situation
	{
		if ( unit.Closest() && lead.CanICharge(unit))   //charge
			comm.Comm(eComCharge);
	}
// ---  new square-----------
	else if ( state == eSTStandCavalryDanger )   //cavalry danger situation
	{
		//if the unit is not already in a square formation and not in skirmishers formation
		if (!IsSkirmisher(unit))  
		{
			int diffenemy = 0;  //enemy close angle
			diff = 0;
			if (!unit.TargClose() && unit.FindClosestFeature(SKD_DBON,stance->fortrange,dloc))
				diff = unit.Pos().GetAngleDiff(dloc,true);  //search of nearest good defensive ground
				
			if (unit.EnemyClose())
				diffenemy = unit.Pos().GetAngleDiff(unit.EnemyClose().Loc(),true);
			//check of friend ahead me
			pathblocked = FriendAheadMe(unit,overlap,left,right,behind);

			//the split check is for avoid in any case skimrisher unit can			
			if (unit.Split()==-1 && unit.NumMen()>stance->SquareMen && unit.Morale()>=stance->CavDanger && ( unit.EnemyCloseYds() > stance->SquareToEnemy || unit.EnemyCloseYds()==0 ) )  //is possible form square (men number and morale and good location)
			{
				if (unit.VolleyStatus()==1 && IsLine(unit) && (diffenemy<90 || diffenemy>270))  //fire the volley
					return false;
				else if ( unit.IsFormType(eFTMarch) && ( unit.GetCharger() || overlap ))
					comm.Comm(eComRetreat);   //getaway
				else if ( unit.IsFormType(eFTMarch) )  //move back in column, if not possible I retreat rapidly
				{
					if (!unit.TargClose())
					{
						comm.Comm(eComMoveDir);
						comm.Dir(true,unit.Dir());
						comm.SetBit(eCBForce);
						CXVec vec = unit.GetLocBehind(unit.MaxRangeYds());
						comm.Vec(true,vec);
						unit.Command(&comm);
						comm.Comm(eComRun);
					}
					else
						comm.Comm(eComRetreat);   //getaway
				}
				else if (!unit.TargClose() && (!unit.OnDefTerr() || ( unit.EnemyCloseYds() > stance->SquareToEnemy*2 ))
					&& (diffenemy<90 || diffenemy>270) && CheckBackPath(unit,unit.MaxRangeYds()))			//I cannot form square here so I get Away back if I can
				{			
					comm.Comm(eComMoveDir);
					comm.Dir(true,unit.Dir());
					comm.SetBit(eCBForce);
					CXVec vec = unit.GetLocBehind(unit.MaxRangeYds());
					comm.Vec(true,vec);
					unit.Command(&comm);
					comm.Comm(eComRun);
				}
			}
			else if ( dloc && (diff >= 135 && diff <= 225) && !unit.TargClose() && CheckBackPath(unit,stance->fortrange))  //I move in a best ground behind me
			{
				comm.Comm(eComMoveDir);
				comm.Dir(true,unit.Dir());
				comm.SetBit(eCBForce);
				comm.Vec(true,dloc);
				unit.Command(&comm);
				comm.Comm(eComRun);
			}
			else if (unit.Morale() <= stance->retreat && unit.MorChange() < 0)  //no good morale
			{
				comm.Comm(eComFormType);  //no more in square
				comm.Val(eFTMarch);
				unit.Command(&comm);
				comm.Comm(eComRetreat);   //retreat
			}
			else if (unit.Morale() < stance->CavDanger)  //no morale sufficient
			{
				comm.Comm(eComFormType);  //no more in square
				comm.Val(eFTMarch);
				unit.Command(&comm);
				comm.Comm(eComRetreat);   //getaway
			}
			else if ( unit.IsFormType(eFTMarch) && ( overlap || unit.EnemyCloseYds() <= stance->SquareToEnemy || unit.GetCharger()  ) )
				comm.Comm(eComRetreat);   //getaway
			else if (!unit.TargClose() && unit.NumMen()<=stance->SquareMen && CheckBackPath(unit,unit.MaxRangeYds()))  //go back if no target present
			{
				comm.Comm(eComMoveDir);
				comm.Dir(true,unit.Dir());
				comm.SetBit(eCBForce);
				CXVec vec = unit.GetLocBehind(150);
				comm.Vec(true,vec);
				unit.Command(&comm);
				comm.Comm(eComRun);
			}
		}
		else if (IsSkirmisher(unit) && unit.Split()!=-1)  //split company of skirmishers 
		{
			if (!unit.GetCharger()) // if not charged unsplit -> if charged she will get away for the skirmisher getaway logic in THINK routine
				comm.Comm(eComRetreat);
		}
	}
	else if ( state == eSTStandCavalryDangerEnd )  //the cavalry danger ended but I'm still in square
	{
		if (lead.IsFormType(eFTSquare)) //in a brig class square do nothing
		{
			return false;
		}		
		
		if ( unit.Morale() <= stance->retreat && unit.MorChange() < 0 )  //retreat check for morale loss
		{
			comm.Comm(eComFormType);  //no more in qua
			comm.Val(eFTMarch);
			unit.Command(&comm);
			comm.Comm(eComRetreat);   //retreat
		}
		else if (unit.EnemyClose() &&  unit.EnemyCloseYds() <= stance->SquareToEnemy )   //enemy too near to do change formation
		{
			return false;
		}
		else if ( !keeporders && !unit.TargClose() && unit.WantCover() && !unit.IsHighGrnd() && !unit.OnDefTerr()  )  //if wantcover I move to a better location
		{
			if (unit.FindClosestFeature(SKD_DBON,stance->fortrange,dloc))  //I move in a best ground
			{
				comm.Comm(eComFormType);
				comm.Val(eFTAssault);
				unit.Command(&comm);
				comm.Comm(eComMove);
				comm.Vec(true,dloc);
			}
			else // i return in line
			{
				comm.Comm(eComFormType);
				comm.Val(eFTFight);
			}
		}
		else if ( !keeporders && ( !unit.EnemyClose() || ( unit.UnderFire1() && unit.TargClose().Valid() ) 
			|| ( unit.EnemyCloseYds() > (stance->SquareToEnemy * 2) ) ) )   //new routine
		{
			comm.Comm(eComFormType);
			comm.Val(eFTFight);
		}
	}
	else if ( state == eSTMarchCavalryDanger )  //the cavalry danger happen during the movement
	{
		if (unit.IsFormType(eFTAssault) && unit.TargClose() &&  unit.TargCloseYds() <= stance->SquareToEnemy )  //if I'm marching in assault formation I don't stop in the face of enemy
			return false;
		else								//in general I halt to form the square
		{
			CXVec vec = unit.Loc();
			if (!unit.ValidWalk(unit.Loc()))   //if not valid pace I repath
			{
				unit.Repath();
				return true;
			}
			else if (unit.HaltTics() <= 0 && unit.ValidDest( unit.Loc() ) )  //halt
				comm.Comm(eComHalt);
			else if ( !unit.ValidDest( vec ) && unit.GetValidDest( unit.DeP(), &vec, true ) )  //new valid destination
			{
				comm.Comm(eComMove);
				comm.Vec(true,vec);
				unit.Command(&comm);
				comm.Comm(eComRun);
			}
		}
	}
//-----new square end
//--------------new forts----------------------------------------
	else if ( state == eSTStandFortMorale )   //morale check when inside a fort
	{
		if (unit.Morale()<stance->MoraleBesieged)  //check if the morale is sufficient  
		{
		   unit.SetClosest(NULL);
		   unit.SetMainTarg(NULL);
		   comm.Comm(eComRetreat);
		}
	}
	else if ( state == eSTStandFortTarget )   //action when inside a fort with target
	{
		if (unit.Ammo()<1)   //no ammo I leave the fort
		{
		   unit.SetClosest(NULL);
		   unit.SetMainTarg(NULL);
		   comm.Comm(eComGetAway);
		}
	}
	else if ( state == eSTStandFortNoTarget )   //action when inside a fort without target
	{
		if (unit.EnemyClose() && unit.EnemyCloseYds()<=(BUFF_CHECK(unit.MaxRangeYds())))
		{
			enemytoonear = true;
		}

		btarg = lead.HasTargets();
		if (unit.Ammo()==0 && lead.ShouldGetAmmo(unit))  //replenish ammunition
		{
			comm.Comm(eComResupply);
		}
		//the stance ask me to leave the fort but only if no enemy is too near
		else if (!btarg && !enemytoonear && stance->LeaveFort==1 && unit.Morale() >= stance->maintainline
			&& lead.GetLineDest( unit, pt ) )  //leader has not target I check if he wants me out
		{
			comm.Comm(eComMove);
			comm.Vec(true,pt);
		}
	}
	else if ( state == eSTStandEnterFort )   //action for enter in a fort
	{
		if (unit.EnemyClose() && unit.EnemyCloseYds()<unit.MaxRangeYds())  //I cannot enter if enemy too near
			return false;
	}
//----new forts ----------------------------------------
//------new skirmishers---------------------------------
	else if ( state == eSTStandSkirmishTarget )   //skirmishers with target
	{
		
		if (!unit.TargFind(unit.Closest(),dist))
			dist = INT_MAX;


		if (unit.Ammo()==0 && lead.ShouldGetAmmo(unit))  //no ammunition if split I unsplit otherwise I go to search ammo
		{
			if (unit.Split()==-1)
			{
				comm.Comm(eComResupply);
			}
			else
			{
				comm.Comm(eComUnSplit);
			}
		}
		else if ( unit.Split()!=-1 && ( stance->SkirmAttitude != 1 || unit.Morale() < stance->SkirmSplit || lead.IsFormType(eFTAltSkirmish) ))  //skirmishers renter in the battalion
		{
				comm.Comm(eComUnSplit);
		}
		else if (unit.Closest() && dist<= unit.MaxRangeYds())  //target near I start to shoot
		{
			diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle and wheel
			if (diff>30 && diff<330)
				unit.WheelToTarget(unit.Closest());
			else 
			{
				comm.Comm(eComFire);
			}
			return true;
		}
		else if (!unit.Closest())  //no frontal target I search best defensive position
		{
			if (unit.TargClose())  //wheel to the target
			{
				unit.WheelToTarget(unit.TargClose());
				return true;
			}
			else if (!unit.IsHighGrnd() && !unit.OnDefTerr())  //best ground in alternative
			{
				if (unit.FindClosestFeature(SKD_DBON,stance->fortrange,dloc))
				{
					comm.Comm(eComMove);
					comm.Vec(true,dloc);
				}
			}

		}
	}
	else if ( state == eSTStandSkirmishNoTarget )   //situation for skirmishers without targets
	{

		CXUnit parentunit;
		int parentdist = 0;

		//stop the pursuit if too far from the parent
		parentunit = ReturnSkirmParent(unit); 
		if (parentunit)
			parentdist = unit.Loc().DistYds(parentunit.Loc());
		else if (unit.Leader())
			parentdist = unit.Loc().DistYds(unit.Leader().Loc());
		//---------------new skirmishers------------------------------------------------
		if (unit.Split()!=-1 && !unit.CanRun())  //too tired to run
			comm.Comm(eComUnSplit);  
		else if (unit.Split()!=-1 && ( !unit.EnemyClose() || unit.EnemyCloseYds()>=(unit.MaxRangeYds() * 6)))  //enemy too distant, I reenter in the battalion
			comm.Comm(eComUnSplit);
		else if ( unit.Split()!=-1 && ( stance->SkirmAttitude != 1 || unit.Morale() < stance->SkirmSplit ))  //skirmishers renter in the battalion
			comm.Comm(eComUnSplit);
		else if (unit.Ammo()==0 && lead.ShouldGetAmmo(unit))  //no ammunition if split I unsplit otherwise I go to search ammo
		{
			if (unit.Split()==-1)
				comm.Comm(eComResupply);
			else
				comm.Comm(eComUnSplit);
		}
		else if (parentdist < (unit.MaxRangeYds() * 3) && !unit.Blocked() 
			&& !unit.TargClose() && unit.EnemyClose() 
			&& unit.EnemyCloseYds()<(unit.MaxRangeYds() * 3) && lead.GetForwardDest(unit,pt,unit.EnemyClose()) )  //skirmishers attack own enemy close frontally
		{
			comm.Comm(eComMove);
			comm.Vec(true,pt);
		}
		else if (parentdist < (unit.MaxRangeYds() * 3) && !unit.TargClose()
			&& unit.EnemyClose() && unit.EnemyCloseYds()<(unit.MaxRangeYds() * 2)
			&& lead.GetFlankDest(unit,pt,unit.EnemyClose()) )  //lateral attack
		{
			comm.Comm(eComMove);
			comm.Vec(true,pt);
		}
		else if (parentdist < (unit.MaxRangeYds() * 3) && !unit.Blocked() 
			&& !unit.TargClose() && lead.EnemyClose() && lead.EnemyCloseYds()<(unit.MaxRangeYds() * 3)
			&& lead.GetForwardDest(unit,pt,lead.EnemyClose()) )  //skirmishers has not own proper target, they attack the target of leader
		{
			comm.Comm(eComMove);
			comm.Vec(true,pt);
		}
		else if (parentdist < (unit.MaxRangeYds() * 3) && !unit.TargClose() 
			&& lead.EnemyClose() && lead.EnemyCloseYds()<(unit.MaxRangeYds() * 2) 
			&& lead.GetFlankDest(unit,pt,lead.EnemyClose()) )  //lateral attack
		{
			comm.Comm(eComMove);
			comm.Vec(true,pt);
		}
		else if (parentdist >= (unit.MaxRangeYds() * 3) && parentunit && unit.ValidWalk(parentunit.GetLocAhead(100)) )
		{
			comm.Comm(eComMove);
			comm.Vec(true,parentunit.GetLocAhead(100));
		}
		else if (parentdist >= (unit.MaxRangeYds() * 3) && unit.Leader() && unit.ValidWalk(unit.Leader().GetLocAhead(100)))
		{
			comm.Comm(eComMove);
			comm.Vec(true,unit.Leader().GetLocAhead(100));
		}
		//---------------new skirmishers----------------------------------------------
	}	
//----- new infantry-----------------------------------------------------
	else if ( state == eSTMarchTargetOnlyMelee )   //offensive movement
	{

		if (unit.Morale()>=stance->moveforward )  //check before I have the necessary morale 
		{
			if (!unit.EnemyFind(unit.Closest(),dist))
				dist = INT_MAX;

			//check of friend ahead me
			pathblocked = FriendAheadMe(unit,overlap,left,right,behind);

			if (unit.IsFormType(eFTAssault) && lead.CanICharge(unit, stance->chargemelee) 
				&& dist<stance->InfantryCharge && unit.MorChange() >= 0  && unit.Closest().UnitType()!=eUnitCav
				&& ratio>=INFCOLUMNATTACK)  //attack column sufficiently near I charge 
			{
				comm.Comm(eComCharge);
			}
			else if (IsLine(unit) &&  dist<= (unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds)))  //i stop the line under the volley distance
			{
				if (unit.HaltTics() <= 0 && unit.ValidDest( unit.Loc() ) )
					comm.Comm(eComHalt);
			}
			else if ( !pathblocked && !keeporders   //approach with the column
				&& dist > stance->InfantryCharge && unit.IsFormType(eFTAssault)
				&& unit.ValidDest(unit.Closest().Loc())
				&& unit.Closest().UnitType()!=eUnitCav )
			{

				if (unit.TargCloseYds()> (unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds)))  //in assault column I move against the enemy till the infantry range
				{
					diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
					if ( diff <= 10 || diff >=350)  //already on teh good path
						return true;
					else if ( diff <= 30 || diff >= 330) 
					{
						comm.Comm(eComMove);
						comm.Vec(true,unit.Closest().Loc());
					}
				}
				else if (lead.CanICharge(unit, stance->chargemelee) &&   unit.MorChange() >= 0 && ratio>=INFCOLUMNATTACK) //for the final push I must be sure to have the advantage
				{
					diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
					if ( diff <= 10 || diff >=350)  //already on teh good path
						return true;
					else if ( diff <= 30 || diff >= 330) 
					{
						comm.Comm(eComMove);
						comm.Vec(true,unit.Closest().Loc());
					}

				}
			}
			else if (!pathblocked && !keeporders && IsLine(unit)
				&& unit.TargCloseYds()> (unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds))
				&& unit.Closest().UnitType()!=eUnitCav && unit.ValidDest(unit.Closest().Loc()))  //in line I move to align with the target
			{
				diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
				if ( diff <= 10 || diff >=350)  //already on the good path
					return true;
				else if ( diff <= 30 || diff >= 330) 
				{
					comm.Comm(eComMove);
					comm.Vec(true,unit.Closest().Loc());
				}
			}
			else if (!pathblocked && IsLine(unit) && unit.TargCloseYds() > unit.MaxRangeYds())  //I continue to march
				return true;
		}

	}
	else if ( state == eSTMarchNoTarget )   //situation of march without a target
	{
			if (lead.IsFormType(eFTMarch))
				return false;
		
			int brigform = stance->brigform;
		    comm.Comm(eComFormType);    //formation for the regiment coherent with that of brigade
			comm.Val(brigform);
	}
	else if ( state == eSTStandTarget )   //situation of standing with target
	{
		if ( unit.Morale()>=stance->moveforward )  //check before I have the necessary morale (square are immobile) and the target is not in a fort
		{		
			bool lineadvance = ( unit.VolleyStatus()==1 || unit.State()==eUnitStand  
				|| IsSkirmisher(unit.Closest()));     // linear advance to shot a volley , if not shooting or for free herself from the skirmishers

			dist = unit.TargCloseYds();		

			//check of friend ahead me
			pathblocked = FriendAheadMe(unit,overlap,left,right,behind);


			//i'm in column i check if I can attack
			if (!pathblocked && !keeporders && ratio>=INFCOLUMNATTACK
				&& unit.IsFormType(eFTAssault) && unit.Closest().UnitType()!=eUnitCav
				&& lead.CanICharge(unit, stance->chargemelee) 
				&&  unit.MorChange() >= 0  
				&& unit.ValidDest(unit.Closest().Loc()) )
			{
				
				if (dist > stance->InfantryCharge)   //move
				{
					diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
					if ( diff <= 45 || diff >= 315) 
					{				
						comm.Comm(eComMove);
						comm.Vec(true,unit.Closest().Loc());
					}
				}
				else
					comm.Comm(eComCharge);  //charge
			}
			else if (!pathblocked && !keeporders
				&& IsLine(unit) 
				&& lineadvance	&&  !unit.Closest().Marching()
				&& dist > (unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds)) 
				&& unit.Closest().UnitType()!=eUnitCav
				&& unit.ValidDest(unit.Closest().Loc()) )
			{
				diff = unit.Pos().GetAngleDiff(unit.Closest().Loc(),true);  //check angle for avoid lateral movement
				if ( ( diff <= 30 || diff >= 330) && !unit.UnderFire1() )
				{
					comm.Comm(eComMove);
					comm.Vec(true,unit.Closest().Loc());
				}
				else if ( diff > 30 && diff <= 45) 
				{
					comm.Comm(eComLObliq);
				}
				else if ( diff < 330 && diff >= 315) 
				{
					comm.Comm(eComRObliq);
				}
				else if (!overlap && ( diff < 315 && diff >= 270) )
				{
					comm.Comm(eComWRightFwd);
				}
				else if (!overlap && ( diff > 45 && diff <= 90) ) 
				{
					comm.Comm(eComWLeftFwd);	
				}
				else if (!overlap && !behind && ( diff < 270 && diff >= 225) )
				{
					comm.Comm(eComWLeftBck);
				}
				else if (!overlap && !behind && ( diff > 90 && diff <= 135) ) 
				{
					comm.Comm(eComWRightBck);	
				}
			}
			else if (pathblocked && !keeporders
				&& IsLine(unit) 
				&& lineadvance	&&  !unit.TargClose().Marching()
				&& dist > (unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds))
				&& unit.TargClose().UnitType()!=eUnitCav
				&& unit.ValidDest(unit.TargClose().Loc()) )
			{
				diff = unit.Pos().GetAngleDiff(unit.TargClose().Loc(),true);  //check angle for avoid lateral movement
				if (!left && ( diff >= 1 && diff <= 70) )
				{
					comm.Comm(eComLFlank);
				}
				else if (!right && ( diff <= 360 && diff >= 290)) 
				{
					comm.Comm(eComRFlank);
				}
				else if (!overlap && ( diff < 315 && diff >= 270) )
				{
					comm.Comm(eComWRightFwd);
				}
				else if (!overlap && ( diff > 45 && diff <= 90) ) 
				{
					comm.Comm(eComWLeftFwd);					
				}
				else if (!overlap && !behind && ( diff < 270 && diff >= 225) )
				{
					comm.Comm(eComWLeftBck);
				}
				else if (!overlap && !behind && ( diff > 90 && diff <= 135) ) 
				{
					comm.Comm(eComWRightBck);	
				}
			}
		}
		else  //short distnace fire from standing position
		{
			int targdist = 0;
			if (!unit.TargFind(unit.Closest(),targdist))
				targdist = INT_MAX;
			
		//enemy is not standing\shooting (marching against me), there is not charge and is over the minimal distance
			//prepare for short distance fire
			if ( IsLine(unit) && unit.Closest().Marching()
				&& targdist>(unit.GetUnitTypeVal(UVAL_BaseVolleyYds)+unit.GetMod(eGameVolleyYds)) && !unit.GetCharger()
				&& !unit.UnderFire1())
			{
				return true;
			}
		}
	}
	else if ( state == eSTStandNoWheel )   //stand and not wheel for hold to the last order
	{
		if ( lead.GetOrders()->regwheel )
			return true;

		return false;

	}
//--------new infantry-----------------------------
	return comm.Comm() != eComMax;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetCavCommand
//*************************************************************************************************************
//*************************************************************************************************************

bool GetCavCommand( CXOff lead, int state, SXComm &comm, CXUnit unit )
{
	int  dist;
	bool bMoved = false;

// ---- new cavalry----
	bool enemytoonear = false;
	bool keeporders = false;
// ---- new cavalry-----

	CXVec pt;
	CXVec dir;

	CXUnit targ;

	const TStance *stance;

	comm.Comm(eComMax);

	if ( !lead.HasStance() || lead.PlyrCtrl() )
		return false;

	stance = lead.GetStance();
	keeporders = lead.GetOrders()->keeporders;

	if ( state == eSTStandNoTargetFire )
	{
		if ( unit.TCOn() )
			return false;

		if ( unit.Morale() <= stance->retreat + 1 )
		{
			comm.Comm(eComGetAway);

			bMoved = true;
		}
//------new cavalry------------------------------------
		else if ( unit.UnderFire1() || unit.IsFlanked() || unit.UnderRecentFire())  //if in unpleasant situation move back out of range
		{
			comm.Comm(eComMoveDir);
			CXVec vec = unit.GetLocBehind(150);
			comm.Vec(true,vec);
			comm.Dir(true,unit.Dir());
			comm.SetBit(eCBForce);
			unit.Command(&comm);
			comm.Comm( eComRun );
			bMoved = true;
		}
//------end new cavalry--------------------------------
	}
	else if ( state == eSTStandNoTarget )
	{
		int  diff = 180;

		if ( unit.Morale() <= MORAL_CANTWHEEL || unit.TCOn() )
			return false;

		if ( lead.HasTargets() )
		{
			//new cavalry : i want avoid manouvers differents from the normal advance with the enemy too near
			if (unit.EnemyClose() && unit.EnemyCloseYds()<=(BUFF_CHECK(stance->CavRunDist)))
			{
				enemytoonear = true;
				diff = unit.Pos().GetAngleDiff(unit.EnemyClose().Loc(),true);  //check angle for avoid lateral movement
			}

			//advancing against enemy
			if ( !keeporders && enemytoonear && unit.Morale() >= stance->moveforward &&
				(diff<= 60 || diff >= 300) 
				&& !unit.Blocked() && unit.ValidDest(unit.GetLocAhead(50)) )
			{
				comm.Vec (true,unit.GetLocAhead(50));
				comm.Comm(eComMove);
				bMoved = true;
			}
			//rotate agains the nearest enemy
			if (enemytoonear && diff > 60 && diff < 300 )
			{
				comm.Unit(unit.EnemyClose().GetID());
				comm.Comm(eComWheelDir);
				bMoved = true;
			}

			// Flanking
			// targ is empty here, so the officer will find the best target
			if ( !enemytoonear && !bMoved && unit.Morale() >= stance->flanking && lead.GetFlankDest( unit, pt, targ ) )
			{
				comm.Vec (true,pt);
				comm.Comm(eComMove);

				bMoved = true;
			}

			// Maintain line			
			if ( !enemytoonear && !bMoved && unit.Morale() >= stance->maintainline && lead.GetMaintainDest( unit, pt, dir ) )
			{
				comm.Vec (true, pt);
				comm.Dir (true, dir);
				comm.Comm(eComMoveDir);
				bMoved = true;
			}
			
			//antioverlap -- I move if Overlap with a friend standing
			if (!bMoved && unit.FriendClose() && unit.DoesShareHex(unit.FriendClose()) 
				&& ( unit.FriendClose().UnitType()==eUnitInf || unit.FriendClose().UnitType()==eUnitCav )
				&& !IsSkirmisher(unit.FriendClose()) && unit.FriendClose().Standing() 
				&& ( unit.EnemyCloseYds()<unit.FriendClose().EnemyCloseYds() ))
			{
				comm.Vec (true, unit.GetLocBehind(50)); //too near light movement back
				comm.Dir (true, unit.Dir());
				comm.Comm(eComMoveDir);
			}
		}
	}
	else if ( state == eSTStandShooting )
	{
		if ( lead.ShouldGetAmmo( unit ) )
			comm.Comm( eComGetAway );
		else if ( unit.Morale() <= stance->retreat && unit.MorChange() < 0 )
			comm.Comm( eComRetreat );
		else if ( unit.OnDefTerr() && lead.StopOnDefTerr(unit) && lead.NewDefTerr() )
			return false;
		else if ( unit.Morale() <= stance->fback && unit.MorChange() < 0 )
			comm.Comm( eComFallback );
		else if ( !unit.PlyrCtrl() && unit.Closest().Valid() )
		{
			targ = unit.Closest();
			
			unit.TargFind( targ, dist );

			lead.GetOrders( dist, targ, unit );
		}
	}
	else if ( state == eSTStandTargetNoAmmo )
	{
		if ( lead.CanICharge( unit, stance->chargenoammo / 2 ) )
			comm.Comm( eComCharge );
		else
			comm.Comm( eComGetAway );
	}
	else if ( state == eSTStandTargetOnlyMelee )
	{
		if ( lead.CanICharge( unit, stance->chargemelee / 2 ) )
			comm.Comm( eComCharge );
	}
//-------new cavalry------------------------------
	else if ( state == eSTAttackTarget )    //can I charge to the target
	{
		dist = INT_MAX;

		if (unit.Closest() && !unit.EnemyFind(unit.Closest(),dist))
			dist = INT_MAX;	

		CXUnit charger = unit.GetCharger();
		bool avoidcharge = false;

		//If I give charge with a target close different from the frontal one I risk to charge a square
		if (unit.Closest() && unit.TargClose() && unit.TargClose().GetID()!=unit.Closest().GetID() 
			&& unit.TargCloseYds() < stance->CavChargDist)
			avoidcharge = true;

	    //-----I use avoidcharge also for case when the enemy is 45° in my face
		if (!unit.Closest())
		{
		    avoidcharge = true;
			dist = unit.TargCloseYds();
			unit.SetMainTarg(NULL);
		}
//to charge particular type of targets, unit has to be already in running state		

		if (  !avoidcharge && unit.CanCharge(unit.Closest(),true) && lead.CanICharge( unit, stance->CavCharge )) 
		{
			if (unit.TargClose() && unit.DoesShareHex(unit.TargClose()))   //if happen to have contact
			{
				unit.SetMainTarg(NULL);
				unit.SetClosest(unit.TargClose());
				comm.Comm( eComState );
				comm.Val( eUnitMelee );
			}
			else if (IsSkirmisher(unit))  //skirmishers can charge only skirmishers or artillery no distance check is a uncoordinate charge
			{
				if (IsSkirmisher(unit.Closest()) || unit.Closest().IsArt())
					comm.Comm( eComCharge );
			}
			else if ( unit.Closest().Valid() && dist < stance->CavChargDist)  //charge target
				comm.Comm( eComCharge );
			else if (charger!=NULL && charger.GetID() == unit.Closest().GetID()) //countercharge from marching
				comm.Comm( eComCharge );
		}
		else   //I cannot charge
		{
			if (unit.TargClose() && unit.DoesShareHex(unit.TargClose()))   //if happen to have contact
			{
				unit.SetMainTarg(NULL);
				unit.SetClosest(unit.TargClose());
				comm.Comm( eComState );
				comm.Val( eUnitMelee );
			}
			else if (charger!=NULL) //countercharge from marching IF i CANNOT COUNTER CHARGE i GET AWAY
			{
				unit.SetClosest(NULL);
				unit.SetMainTarg(NULL);
				comm.Comm( eComGetAway );					
			}
			else if (unit.TargClose() && unit.TargCloseYds() < (DISTCAV1AUTOMELEE/2)
				&& !IsSkirmisher(unit) ) //to avoid to not charge enemy too near to not be charged
			{
				if (unit.CanCharge(unit.TargClose(),true) && lead.CanICharge( unit, stance->CavCharge ))  //can i charge ? ground or fatigue
				{
					unit.SetMainTarg(NULL);
					unit.SetClosest(unit.TargClose());
					comm.Comm( eComCharge );
				}
				else  //getaway
				{
					unit.SetClosest(NULL);
					unit.SetMainTarg(NULL);
					comm.Comm( eComGetAway );	
				}
			}
			else if (dist < stance->CavChargDist && unit.Closest() )   //move back for avoid to remain in the face of enemy
			{						
				unit.SetClosest(NULL);
				unit.SetMainTarg(NULL);
				comm.Comm(eComMoveDir);
				CXVec vec = unit.GetLocBehind(150);
				comm.Vec(true,vec);
				comm.SetBit(eCBForce);
				comm.Dir(true,unit.Dir());
				unit.Command(&comm);
				comm.Comm( eComRun );
			}
			else   //I stop
			{
				unit.SetClosest(NULL);
				unit.SetMainTarg(NULL);
				comm.Comm( eComHalt );
			}
		}
	}
	else if ( state == eSTMarchTargetOnlyMelee )    //can I start to trot\gallop
	{
		if (!unit.EnemyFind(unit.Closest(),dist))
			dist = INT_MAX;

		int ratio = FearCheck(unit.Closest(),unit);
		int diff ;

		bool stopenemytoonear = false;
		bool pathagain = false;
		bool pathblocked = false;

		if (unit.TargClose() && unit.TargClose().GetID()!=unit.Closest().GetID())
		{
			diff = unit.Pos().GetAngleDiff(unit.TargClose().Loc(),true);
			if (diff<=45 && diff>=315)
				pathblocked = true;
		}

		//If I give charge with a target close different from the frontal one I risk to charge a square
		if (pathblocked && unit.TargCloseYds() <= stance->CavChargDist && unit.TargCloseYds() < dist)
			stopenemytoonear = true;

		//If I give charge with a target close different from the frontal one I risk to charge a square
		if (pathblocked && unit.TargCloseYds() > stance->CavChargDist && unit.TargCloseYds() < dist)
			pathagain = true;

		if (unit.TargClose() && unit.DoesShareHex(unit.TargClose()))   //if happen to have contact
		{
			unit.SetMainTarg(NULL);
			unit.SetClosest(unit.TargClose());
			comm.Comm( eComState );
			comm.Val( eUnitMelee );
		}
		else if (dist > stance->CavRunDist && unit.IsFormType(eFTMarch))  //if still in march formation I change if I can
		{
			comm.Comm( eComFormType );
			comm.Val( eFTFight );
		}
		else if (stopenemytoonear || !unit.ValidDest(unit.Loc()) || unit.Blocked())
			comm.Comm( eComHalt );
		else if (pathagain)  //flanking effect
		{
			unit.Repath();
			return true;
		}
		else if ( !keeporders  
			&& lead.CanICharge( unit, stance->CavCharge ) && (ratio>= CAVATTACK || unit.Closest().Retreating()) 
			&& unit.ValidDest(unit.Closest().Loc()) && !CavalryBlocked(unit) )
		{
			comm.Comm(eComMove);
			comm.Vec(true,unit.Closest().Loc());
			if (dist < stance->CavRunDist && unit.State()!=eUnitRun)
			{
				unit.Command(&comm);
				comm.Comm( eComRun );
			}
		}
	}
	else if ( state == eSTStandTarget )    //with a target to charge i'm standing
	{
		if (!unit.EnemyFind(unit.Closest(),dist))
			dist = INT_MAX;
		int diff;

		bool stopenemytoonear = false;
		bool pathblocked = false;

		if (unit.EnemyClose() && unit.EnemyCloseYds()<=(BUFF_CHECK(stance->CavRunDist)))
			enemytoonear = true;

		if (unit.TargClose() && unit.TargClose().GetID()!=unit.Closest().GetID())
		{
			diff = unit.Pos().GetAngleDiff(unit.TargClose().Loc(),true);
			if (diff<=45 && diff>=315)
				pathblocked = true;
		}

		//If I give charge with a target close different from the frontal one I risk to charge a square
		if (pathblocked && unit.TargCloseYds() <= stance->CavChargDist && unit.TargCloseYds() < dist)
			stopenemytoonear = true;

		int ratio = FearCheck(unit.Closest(),unit);

		 //for particualr type of target need to be sure to be in fighintg formation or over a specific distance
		if (unit.TargClose() && unit.DoesShareHex(unit.TargClose()))   //if happen to have contact
		{
			unit.SetMainTarg(NULL);
			unit.SetClosest(unit.TargClose());
			comm.Comm( eComState );
			comm.Val( eUnitMelee );
		} 
		else if ( !unit.IsFormType(eFTFight) && !IsSkirmisher(unit.Closest()) && !unit.Closest().IsArt() && dist > stance->CavChargDist)
		{
			comm.Comm( eComFormType );
			comm.Val( eFTFight );
		}
		else if (stopenemytoonear)
		{
			unit.WheelToTarget(unit.TargClose());
			return true;
		}
		else if ( !keeporders 
			&& (ratio>=CAVATTACK || unit.Closest().Retreating()) && lead.CanICharge( unit, stance->CavCharge )
			&& unit.ValidDest(unit.Closest().Loc()) && !CavalryBlocked(unit) )  //check attack start
		{	
			comm.Comm( eComMove );
			comm.Vec(true,unit.Closest().Loc());
		}
	}
	else if ( state ==eSTMeleeTargetSquare )    //Cavalry melee to manage if I'm enter in melee with a square
	{
		int ratio = FearCheck(unit.Closest(),unit);
		//a formation during the "formation forming" is in moving state so is without the melee\fire bonus
		if (ratio<CAVATTACK && !unit.Closest().Retreating())
		{
			unit.SetClosest(NULL);
			unit.SetMainTarg(NULL);				
			comm.Comm( eComGetAway );	
		}
	}
	else if ( state ==eSTChargeTargetSquare )    //Cavalry melee to manage if I'm enter in melee with a square
	{
		int ratio = FearCheck(unit.Closest(),unit);

		if ( unit.Closest().Retreating() && !lead.CanICharge( unit, stance->CavCharge ))  //retreating target I can continue?
		{
				unit.SetClosest(NULL);
				unit.SetMainTarg(NULL);
				comm.Comm( eComHalt );
		}
		else if (ratio<CAVATTACK)
		{
			unit.SetClosest(NULL);
			unit.SetMainTarg(NULL);				
			comm.Comm( eComHalt );	
		}
	}
	else if ( state == eSTMarchNoTarget )   
	{
			if (lead.IsFormType(eFTMarch))
				return false;
		
			int brigform = stance->brigform;
			comm.Comm(eComFormType);    //formation for the regiment coherent with that of brigade
			comm.Val(brigform);
	}
//-------end new cavalry--------------------------
	return comm.Comm() != eComMax;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetCommand
//*************************************************************************************************************
//*************************************************************************************************************

bool GetArtCommand( CXOff lead, int state, SXComm &comm, CXUnit unit )
{
	bool bMoved = false;

	CXVec pt;
	CXVec dir;
	int fallback;
	int redeploydst = 10;

	comm.Comm(eComMax);

	const TStance *stance;


	if ( !lead.HasStance() || lead.PlyrCtrl() )
		return false;

	stance = lead.GetStance();

	if ( state == eSTStandNoTarget )
	{
		if ( unit.Morale() <= MORAL_CANTWHEEL || unit.TCOn() )
			return false;

		if ( lead.HasTargets() )
		{
			// Maintain line			
			if ( !bMoved && unit.Morale() >= stance->maintainline && lead.GetMaintainDest(unit,pt,dir) )
			{				
			   if (CanGunRedeploy(lead,unit,pt)) //if destination is not good I avoid the change
			   {				
					comm.Comm( eComMoveDir );
					comm.Vec(true,pt);
					comm.Dir(true,dir);
			   }
			}

			if ( !bMoved && comm.Comm() == eComMax )
			{
				if (unit.Blocked() && unit.EnemyClose().Valid())  
				{
					if (unit.LimState()==eLimLimbered && unit.WheelToTarget(unit.EnemyClose())) //limbered troops can face the wrong direction, I want to be sure they face the enemy before move back
						return true;
					if (unit.LimState()==eLimLimbered)  //limbered troops can need more space after because unlimbered they take more space
						redeploydst = 20;
					comm.Comm( eComMoveDir );
					comm.Vec(true,unit.GetLocBehind(redeploydst));
					comm.Dir(true,unit.Dir());
				}
				else if (unit.LimState()==eLimLimbered && lead.GetLineDest(unit,pt))
				{
					if (CanGunRedeployLine(lead,unit,pt))
					{
						comm.Comm( eComMove );
						comm.Vec(true,pt);
					}
				}
			}

		}
	}
	else if ( state == eSTStandShooting )
	{
		if ( lead.ShouldGetAmmo( unit ) )
			comm.Comm( eComRetreat );
		else if ( unit.Morale() <= stance->retreat && unit.MorChange() < 0 )
			comm.Comm( eComRetreat );
		else if ( unit.OnDefTerr() && lead.StopOnDefTerr(unit) && lead.NewDefTerr() )
			return false;
		else if ( unit.Morale() <= stance->fback && unit.MorChange() < 0 )
			comm.Comm( eComFallback );
	}
	else if ( state == eSTStandTargetNoAmmo )
		comm.Comm( eComRetreat );
	else if ( state == eSTStandTargetOnlyMelee )
	{
	}
	else if ( state == eSTStandArtilleryFallBack )
	{
		fallback = stance->ArtyFallback;
		comm.Val(fallback);
		return true;
	}
	return comm.Comm() != eComMax;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************


