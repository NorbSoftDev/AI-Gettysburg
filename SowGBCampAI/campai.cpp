//*************************************************************************************************************
//*************************************************************************************************************
// File: campai.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "sowgbcampai.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"

#include "SOWWLMOD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//eComSBCDetach
//eComSBCAttach
//eComSBCSetDest - only use this command to move

#define OCCUPATIONTIME 3600  //time necessary in order to conquer enemy base
#define RETREATDELAY 5184000 //time for retreat from a battle

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
// Function: OverAllCommander
//*************************************************************************************************************
//*************************************************************************************************************
CXOff OverAllCommander( CXOff base ) 
{
	//----leader of higher detachment
	if (!base.Leader())
		return base;
	
	return OverAllCommander( base.Leader() );

}


//*************************************************************************************************************
//*************************************************************************************************************
// Function: TrueLeader
//*************************************************************************************************************
//*************************************************************************************************************
CXOff TrueLeader( CXOff base ) 
{
	//----leader of higher detachment
	if (!base.Leader())
		return base;
	
	if (base.Leader().Attached())
		return TrueLeader( base.Leader() );

	return base.Leader();
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsCavalryDiv
//*************************************************************************************************************
//*************************************************************************************************************
bool IsCavalryDiv( CXOff base )
{
	int i;
	CXOff suboff;
	int cav = 0;

  //check if the corps is formed by cavalry officers

	if (base.Rank()<eRankDiv)
		return false;

	for (i=0;i<base.NumSubs();i++)
	{
		suboff = base.Sub(i);
		if (suboff.UnitType()==eUnitCav)
			cav += 1;
	}
    
	if (cav==base.NumSubs())
		return true;

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: IsArtyDiv
//*************************************************************************************************************
//*************************************************************************************************************
bool IsArtyDiv( CXOff base )
{
	int i = 0;
	CXOff suboff;
	int art = 0;

  //check if the corps is formed by arty officers

	if (base.Rank()<eRankDiv)
		return false;

	if (base.NumSubs()!=0)
		return false;

	if (base.NumArty()>0)
		return true;

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: OnlyCavalry
//*************************************************************************************************************
//*************************************************************************************************************
bool OnlyCavalry( CXOff base )
{
	int i = 0;
	int j = 0;
	CXOff suboff;

	//  I check if the unit contains only cavalry and not infantry

	if (base.Rank()<eRankDiv)
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			j += OnlyCavalry( suboff );
		}
		if (j==base.NumSubs())  // the total of brigades are of cavalry
			return true;
		else
			return false;
	}
	else
	{
		return IsCavalryDiv(base);
	}

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: CavalryBrigadesNumbers
//*************************************************************************************************************
//*************************************************************************************************************
int CavalryBrigadesNumbers( CXOff base )
{
	int i = 0;
	int j = 0;
	CXOff suboff;

	//  I check if the unit contains only cavalry and not infantry

	if (base.Rank()<eRankDiv)
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			j += CavalryBrigadesNumbers( suboff );
		}
		return j;
	}
	else
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			if (suboff.UnitType()==eUnitCav)
				j += 1;
		}
		return j;
	}

	return 0;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: OnlyArtillery
//*************************************************************************************************************
//*************************************************************************************************************
bool OnlyArtillery( CXOff base )
{
	int i = 0;
	int j = 0;
	CXOff suboff;

	//  I check if the unit contains only artillery

	if (base.Rank()<eRankDiv)
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			j += OnlyArtillery( suboff );
		}
		if (j==base.NumSubs())  // the total of brigades are of artillery
			return true;
		else
			return false;
	}
	else
	{
		return IsArtyDiv(base);
	}

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: ArtilleryBrigadesNumbers
//*************************************************************************************************************
//*************************************************************************************************************
int ArtilleryBrigadesNumbers( CXOff base )
{
	int i = 0;
	int j = 0;
	CXOff suboff;

	//  I check if the unit contains only cavalry and not infantry

	if (base.Rank()<eRankDiv)
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			j += ArtilleryBrigadesNumbers( suboff );
		}
		return j;
	}
	else
	{
		j += base.NumArty();
		return j;
	}

	return 0;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: MassiveSameFriendDestination
//*************************************************************************************************************
//*************************************************************************************************************
bool MassiveSameFriendDestination( CXOff base, CXVec location, CXOff friendOff ) 
{
	int i;
	CXOff suboff;

//---massive check to know if I overlap with a friend

	if (!location)
		return false;

	if (!friendOff)
		return false;

	if (location.DistYds(friendOff.DeP().Loc())<100 || location.DistYds(friendOff.Loc())<100)
		return true;

	if (friendOff.NumDetach()>0)
	{
		for (i=0;i<friendOff.NumDetach();i++)
		{
			suboff = friendOff.Det(i);
			if (base.GetID()==suboff.GetID())
				continue;

			if (MassiveSameFriendDestination(base,location,suboff))
				return true;
		}
	}

	i= 0;

	if (friendOff.NumSubs()>0)
	{
		for (i=0;i<friendOff.NumSubs();i++)
		{
			suboff = friendOff.Sub(i);
			if (base.GetID()==suboff.GetID())
				continue;
			if (MassiveSameFriendDestination(base,location,suboff))
				return true;
		}
	}


	return false;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: MassiveSameFriendDestinationRankLimited
//*************************************************************************************************************
//*************************************************************************************************************
bool MassiveSameFriendLocationRankLimited( CXOff base, CXVec location, CXOff friendOff ) 
{
	int i;
	CXOff suboff;

//---massive check to know if I overlap with a friend

	if (!location)
		return false;

	if (!friendOff)
		return false;

	if (friendOff==base)
		return false;

	if ( friendOff.Rank()<=base.Rank() && friendOff.Standing() && location.DistYds(friendOff.Loc())<100)
		return true;

	if (friendOff.NumDetach()>0)
	{
		for (i=0;i<friendOff.NumDetach();i++)
		{
			suboff = friendOff.Det(i);
			if (base.GetID()==suboff.GetID())
				continue;

			if (MassiveSameFriendLocationRankLimited(base,location,suboff))
				return true;
		}
	}

	i= 0;

	if (friendOff.NumSubs()>0)
	{
		for (i=0;i<friendOff.NumSubs();i++)
		{
			suboff = friendOff.Sub(i);
			if (base.GetID()==suboff.GetID())
				continue;
			if (MassiveSameFriendLocationRankLimited(base,location,suboff))
				return true;
		}
	}


	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: OverlapWithLeader
//*************************************************************************************************************
//*************************************************************************************************************
bool OverlapWithLeader( CXOff base ) 
{
	CXOff Leader = TrueLeader(base);

	if (Leader.GetID()==base.GetID())
		return false;

	if (base.Loc().DistYds(Leader.Loc())<100)
		return true;

	return false;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: MergeWithSubordinate
//*************************************************************************************************************
//*************************************************************************************************************
bool MergeWithSubordinate( CXOff base ) 
{
	int i;
	CXOff suboff;
	SXComm comm;

	if (base.NumDetach()>0)
	{
		for (i=0;i<base.NumDetach();i++)
		{
			suboff = base.Det(i);
			if (base.Loc().DistYds(suboff.Loc())<100)
			{
				comm.Val(suboff.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
			}
		}
	}

	return false;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetTownXQuad
//*************************************************************************************************************
//*************************************************************************************************************
SXTown GetTownXQuad( CXOff base, int counter, int quad ) 
{
	int i = 0;
	int safe = 0;
	SXTown town;
	int dist;
	SXComm comm;
	int xcounter;

	if (counter <= 0)
		xcounter = 1;
	else
		xcounter = counter;

	//i search town in a specific quad and a specifici town distance (in town counter) from me
	while (CXCamp::TownNext(base,&town,dist))
	{
		if (base.GetQuad(town.Loc())==quad || quad==4)
		{
			if (i==xcounter)
			{
				comm.Val (base.GetID());
				comm.Vec (true, town.Loc());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				break;
			}
			i += 1;
			safe = 0;
		}
		safe += 1;
		if  (safe==10)
			return NULL;
	}

	return town;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: DirectionQuadrant
//*************************************************************************************************************
//*************************************************************************************************************
SXTown DirectionQuadrant( CXOff base, int counter = 0 )
{
	CXOff Leader;
	int quad = 0;
	int dist;
	CXOff suboff;
	SXTown town;
	int i = 0;
	
	
	//get town to move around the leader location
	Leader = OverAllCommander(base);	//i need the full commander to check all the unit locations

	while (CXCamp::TownNext(base,&town,dist))
	{
		
		if (i>=counter)   //more space for some corps	
		{

			if (!MassiveSameFriendDestination(base,town.Loc(),Leader))
				break;
		}

		if (i>20)
			break;

		i += 1;

	}

	return town;



}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: DirectionQuadrantTownOccupied
//*************************************************************************************************************
//*************************************************************************************************************

SXTown DirectionQuadrantTownOccupied( CXOff base, int maxdist, int occupside = 0, int quad=4 )
{
	//nearest city enemy occupied
	int dist;
	SXTown town;
	int i = 0;
	int enemyquad = OppositeQuad(quad); // enemy quad, we DONT want to go here
	bool bQuadCheck = true;
	bool bOccCheck = true;
	SXTown lasttown;

	while (CXCamp::TownNext(base,&town,dist))
	{
		if (lasttown == town)
		{
			town = SXTown(NULL);

			if (!bOccCheck)
				bQuadCheck = false;

			bOccCheck = false;
			continue;
		}
		lasttown = town;

		if (dist>maxdist)
			return NULL;

		// here we are searching for a retreat location
		if (bQuadCheck && quad != 4 && base.GetQuad(town.Loc()) != quad)
			continue;
		else if (!bQuadCheck && quad != 4 && base.GetQuad(town.Loc()) == enemyquad)
			continue;
		else if (!bOccCheck && town.Occupied() == -1)
			return town;
		else if (town.Occupied()==occupside)
			return town;

		if (i>200)
			break;

		i += 1;
	}
	return town;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: DirectionQuadrantNoCheck
//*************************************************************************************************************
//*************************************************************************************************************
SXTown DirectionQuadrantNoCheck( CXOff base, int counter = 0 )
{
	CXOff Leader;
	int quad = 0;
	int dist;
	CXOff suboff;
	SXTown town;
	int i = 0;


	while (CXCamp::TownNext(base,&town,dist))
	{
		
		if (i>=counter)   //more space for some corps	
		{
			break;
		}

		if (i>20)
			break;

		i += 1;

	}

	return town;



}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: FollowMarchingLeader
//*************************************************************************************************************
//*************************************************************************************************************
bool FollowMarchingLeader( CXOff base )
{
	CXOff Leader;
	CXVec Location;
	SXComm comm;

	//get town to move around the leader location
	Leader = TrueLeader(base);	

	if (Leader.GetID()==base.GetID())
		return false;

	if (Leader.Marching())  //if Leader in march I follow him
	{
		Location = Leader.DeP().Loc();  //I go where my leader go;
		if (Location)
		{
			comm.Val (base.GetID());
			comm.Vec (true, Location);
			comm.Comm(eComSBCSetDest);
			CXUtil::AddComm( &comm );
			return true;
		}
	}

	return false;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function:  RecallAllMySub
//*************************************************************************************************************
//*************************************************************************************************************
void RecallAllMySub( CXOff base, CXVec location ) 
{
	int i;
	CXOff suboff;
	SXComm comm;


//call to me all my deatched parts
	if (!base)
		return;

	if (!location)
		return;

	if (base.NumDetach()>0)
	{
		for (i=0;i<base.NumDetach();i++)
		{
			suboff = base.Det(i);	
			comm.Vec(true, location);
			comm.Val (suboff.GetID());
			comm.Comm(eComSBCSetDest);
			CXUtil::AddComm( &comm );
		}
	}

	return;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function:  MassiveUnderUnitMovOrder
//*************************************************************************************************************
//*************************************************************************************************************
bool MassiveUnderUnitMovOrder( CXOff base ) 
{
	int i;
	CXOff suboff;
	int counter = 0;
	SXTown town;
	int dist;
	SXComm comm;

//---massive check to know if I overlap with a friend

	if (!base)
		return false;


	if (base.NumDetach()>0)
	{
		for (i=0;i<base.NumDetach();i++)
		{
			suboff = base.Det(i);	

			if (suboff.Rank()<=eRankCorp)  //counter for gain a little more space between bigger units
				counter = 2;

			town = DirectionQuadrant(suboff, counter);  //town of destinatio

			if (!town)   //no town I pick the nearest
				town = CXCamp::TownClose(suboff,dist);

			if (town)
			{
				comm.Vec(true, town.Loc());
				comm.Val (suboff.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
		}
	}
	else
		return false;

	i= 0;

	if (base.NumSubs()>0)
	{
		for (i=0;i<base.NumSubs();i++)
		{
			suboff = base.Sub(i);
			MassiveUnderUnitMovOrder(suboff);
		}
	}


	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: AttackerSubDeployment
//*************************************************************************************************************
//*************************************************************************************************************
bool AttackerSubDeployment( CXOff base )
{
	SXTown town;
	SXComm comm;
	int counter = CXUtil::Ran(98,0,3);
	int dist;

	//corps march around to find the enemy
	town = DirectionQuadrant(base, (base.Rank()+counter));

	if (!town)   //no town I pick the nearest
		town = CXCamp::TownClose(base,dist);

	comm.Vec(true, town.Loc());
	comm.Val (base.GetID());
	comm.Comm(eComSBCSetDest);
	CXUtil::AddComm( &comm );	


	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: DefenderSubDeployment
//*************************************************************************************************************
//*************************************************************************************************************
bool DefenderSubDeployment( CXOff base )
{
	SXTown town;
	CXOff Leader;
	int myquad = 0;
	SXComm comm;
	int counter = 0;
	int dist;




//--defender avoid overlap 
	if (base.Standing() && MassiveSameFriendLocationRankLimited(base,base.Loc(),OverAllCommander(base)))   
	{
			counter = CXUtil::Ran(98,1,10);  //randomize number of cycles
			town = DirectionQuadrant(base, counter);  //town of destination

			if (!town)   //no town I pick the nearest
				town = CXCamp::TownClose(base,dist);
			if (town)
			{
				comm.Vec(true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
			return true;
	}



	return false;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: SplitUp
//*************************************************************************************************************
//*************************************************************************************************************
bool SplitUp( CXOff base, int number, bool defender = true ) 
{
	int i;
	SXComm comm;
	CXOff suboff;
	int guardcorp=0;
	int n = 1;
	std::string str;
	bool bypass;

//split up during the match
// number identify how many unit I want split up

	if (base.NumSubs()==0 && base.NumArty()>0)
	{
		//not split if the unit is only of artillery
		return false;
	}

	if (base.NumDetach()>=number)
		return false;


	if (base.Rank()>eRankCorp)  //no split up of under division\rank
		return false;



	if (base.NumSubs()>n)
	{
		
		for (i=0;i<base.NumSubs();i++)  //always the first corps remain with the main army
		{		
			suboff = base.Sub(i);
			guardcorp=suboff.GetID();  //first troops  remain as central mass
			break;
		}
		for (i=0;i<base.NumSubs();i++)  //but if there is a dupe I use this 
		{		
			suboff = base.Sub(i);
			str = suboff.Name();
			if (str.find("DUPE") != std::string::npos)
			{
				guardcorp=suboff.GetID();  
				break;
			}
		}
		
		for (i=0;i<base.NumSubs();i++)  
		{
			suboff = base.Sub(i);			
			if (suboff.NumSubs()==0 && suboff.NumArty()>0)  //no split artillery corps from the parent.
				continue;
			str = suboff.Name();
			if (str.find("DUPE") != std::string::npos)  //I don't detach dupes
				continue;
			if (guardcorp!=suboff.GetID() && suboff.Attached() )  // check the corps is not that under direct general orders
			{
				bypass = IsCavalryDiv(suboff) && defender; 
				comm.Val(suboff.GetID());
				comm.Comm(eComSBCDetach);
				CXUtil::AddComm(&comm);
				if (i==number && !bypass )
					return true;
			}
		}
	}

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: InitialSplitUp
//*************************************************************************************************************
//*************************************************************************************************************
void InitialSplitUp( CXOff base ) 
{
	int i;
	SXComm comm;
	CXOff suboff;
	int guardcorp=0;
	std::string str;

//-----split up can be different according to the context of battle; the attacker will try to use big formations at corps level
//  to hit in few determined points with a little advance guard for column (usually cavalry brigade);
//	the defender will try to cover more gaps possible with brigade advanced
//	garrisons for regroup all the force when teh attacker show himself in a decisive point


	if (base.NumSubs()==0 && base.NumArty()>0)
	{
		//not split if the unit is only of artillery
		return;
	}


	if (base.Rank()>eRankArmy)  //no initial split up of corps 
		return;



	if (base.NumSubs()>1)  //there're units
	{
		
		for (i=0;i<base.NumSubs();i++)  //always the first corps remain with the HQ army 
		{		
			suboff = base.Sub(i);
			guardcorp=suboff.GetID();  //first troops of oob  remain as central mass
			break;
		}
		for (i=0;i<base.NumSubs();i++)  //but if there is a dupe I use this 
		{		
			suboff = base.Sub(i);
			str = suboff.Name();

			if (str.find("DUPE") != std::string::npos)
			{
				guardcorp=suboff.GetID();  
				break;
			}
		}
		
		for (i=0;i<base.NumSubs();i++)  
		{
			suboff = base.Sub(i);			
			if (suboff.NumSubs()==0 && suboff.NumArty()>0)  //no split artillery corps from the parent.
				continue;
			str = suboff.Name();
			if (guardcorp!=suboff.GetID() && suboff.Attached() && str.find("DUPE") == std::string::npos)  // check the corps is not that under direct general orders
			{
				comm.Val(suboff.GetID());
				comm.Comm(eComSBCDetach);
				CXUtil::AddComm(&comm);
				continue;
			}
			InitialSplitUp(suboff);  //recursive call
		}
	}

	return;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: CorpPicket
//*************************************************************************************************************
//*************************************************************************************************************
bool CorpPicket( CXOff base )
{
	//I detach a picket unit at division\brigade level

	if (SplitUp(base,3))
		return true;

	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: CorpAdvanceGuard
//*************************************************************************************************************
//*************************************************************************************************************
bool CorpAdvanceGuard( CXOff base )
{
	//I detach a mobile unit

	if (SplitUp(base,1,false))
		return true;	

	return false;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: EnemyNear
//*************************************************************************************************************
//*************************************************************************************************************
bool EnemyNear( CXOff base ) 
{
	CXOff sold;
	int dist;

	base.EnemyBeg();
	while (base.EnemyNext(sold,dist))
	{
		return true;
	}

	return false;
}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetEnemyNear
//*************************************************************************************************************
//*************************************************************************************************************
CXOff GetEnemyNear( CXOff base ) 
{
	CXOff sold;
	int dist;
	CXOff closest;
	int mindist;

	base.EnemyBeg();
	while (base.EnemyNext(sold,dist))
	{
		if (closest==NULL || dist<mindist)
		{
			closest = sold;
			mindist = dist;			
		}
	}

	if (closest)
		return closest;

	return NULL;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: DivisionNumbers
//*************************************************************************************************************
//*************************************************************************************************************
int DivisionNumbers( CXOff base ) 
{
	int i;
	CXOff suboff;
	int y = 0;

	if (base.Rank()==eRankDiv)
		return 1;

	//I make the count to decide the attack decision
	for (i=0;i<base.NumSubs();i++)  
	{
		suboff = base.Sub(i);			
		if (suboff.Rank()<eRankDiv)
		{
			y += DivisionNumbers(suboff);
		}
		else if (suboff.Rank()==eRankDiv)
		{
			y += 1;
		}
		else
		{
			y += 0;
		}

	}


	return y;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: AttackEnemy
//*************************************************************************************************************
//*************************************************************************************************************
bool AttackEnemy( CXOff base, bool attacker ) 
{

	CXOff closest;
	base.EnemyBeg();
	int mydivisions;
	int enemydivisions;
	int rankdifference = 0;


	closest = base.Closest();

	if (closest)
	{
		mydivisions = DivisionNumbers(base);
		enemydivisions = DivisionNumbers(closest);

		mydivisions += CavalryBrigadesNumbers(base);  //brigades cavalry add value to the strenght
		enemydivisions += CavalryBrigadesNumbers(closest);
		mydivisions += ArtilleryBrigadesNumbers(base); //artillery cavalry add value to the strenght
		enemydivisions += ArtilleryBrigadesNumbers(closest);

		if (OnlyCavalry(base))  //mono arm is not strong as multi arm
			mydivisions /= 2;
		if (OnlyCavalry(closest))
			enemydivisions /= 2;

		if (base.Rank()==closest.Rank()) //same rank
		{
			if (mydivisions>=enemydivisions)
				return true;
			if (attacker && mydivisions>=(enemydivisions - 1))
				return true;
		}
		if (base.Rank()<closest.Rank()) //superior rank
		{
			rankdifference = closest.Rank()-base.Rank();
			if (mydivisions>=enemydivisions)
				return true;
			if (attacker && mydivisions>=(enemydivisions - rankdifference))
				return true;
		}
		if (base.Rank()>closest.Rank()) //inferior rank
		{
			if (mydivisions>enemydivisions)
				return true;
			if (attacker && mydivisions==enemydivisions)
				return true;
		}

	}


	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: AttackFarEnemy
//*************************************************************************************************************
//*************************************************************************************************************
bool AttackFarEnemy( CXOff base, CXOff enemy ) 
{

	int mydivisions;
	int enemydivisions;

// attack enemy without contact
	if (enemy)
	{
		mydivisions = DivisionNumbers(base);
		enemydivisions = DivisionNumbers(enemy);
		
		if (mydivisions>enemydivisions)
			return true;

	}


	return false;

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: ChaseEnemy
//*************************************************************************************************************
//*************************************************************************************************************
CXOff ChaseEnemy( CXOff base, CXOff friendoff, int range ) 
{
	int i;
	CXOff suboff;
	CXOff result;
	CXOff enemy;

//first I check myself
	enemy = GetEnemyNear(base);
	if (AttackFarEnemy(base,enemy))
		return enemy;

//evaluate if I must chase a enemy seen by a friend	

	if (base.Loc().DistYds(friendoff.Loc())<=range && EnemyNear(friendoff))  //friend near and near to a enemy
	{
		if (AttackFarEnemy(base,GetEnemyNear(friendoff)))	
			return friendoff;
	}

	if (friendoff.NumDetach()==0)
		return NULL;


	for (i=0;i<friendoff.NumDetach();i++)
	{
		suboff = friendoff.Det(i);
		result = ChaseEnemy(base,suboff,range);
		if (result)
			return result;
	}

	return NULL;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: InitialDirection
//*************************************************************************************************************
//*************************************************************************************************************
SXTown InitialDirection( CXOff base, int camptype, bool attacker ) 
{

	int dist;

	//determine the initial movements on the base of campaign type
	switch (camptype)
	{
	case eSBCOneTown:   //occupy bruxelles
		return CXCamp::TargTown();
		break;
	case eSBCMajTown:               //occupy the biggest part of town
		return CXCamp::TownClose(base,dist);
		break;
	case eSBCDestroy:    //destroy the enemy
		return CXCamp::TownClose(base,dist);
		break;
	case eSBCBegTown:   //occupy supply base
		if (attacker)
			return CXCamp::StartTown(0);  //town of player
		else
			return CXCamp::TownClose(base,dist);  //around my town
		break;
	}

	return CXCamp::TownClose(base,dist);

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: TownMovements
//*************************************************************************************************************
//*************************************************************************************************************
SXTown TownMovements( CXOff base, int camptype, bool attacker, int counter = 0 ) 
{

	int dist;

	//determine the initial movements on the base of campaign type
	switch (camptype)
	{
		case eSBCOneTown:   //occupy bruxelles
			return CXCamp::TargTown();
			break;
		case eSBCMajTown:               //occupy the biggest part of town
			return DirectionQuadrant(base,counter);
			break;
		case eSBCDestroy:    //destroy the enemy
			return DirectionQuadrant(base,counter);
			break;
		case eSBCBegTown:   //occupy supply base
			if (attacker)
				return CXCamp::StartTown(0);  //town of player
			else
				return CXCamp::TownClose(base,dist);  //around my town
			break;
	}

	return CXCamp::TownClose(base,dist);

}
//*************************************************************************************************************
//*************************************************************************************************************
// Function: EnemyTownClose
//*************************************************************************************************************
//*************************************************************************************************************
bool EnemyTownClose( CXOff base ) 
{
	int dist;
	SXTown town;

//near town occupied?
	town = CXCamp::TownClose(base,dist);


	if (town.Occupied()==0)
		return true;

	return false;

}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Think
//*************************************************************************************************************
//*************************************************************************************************************

int CCampThink( int state, CXOff base, int camptype ) 
{
	bool attacker = false;


		//attacker reaction
	if ( ( (base.Personality()+TrueLeader(base).Personality())/2)>4)  //attacker\defender -> my style + leader style \ 2
		attacker = true;

	if (camptype == eSBCOneTown)  //occupy 1 town attacker movement
		attacker = true;

	if (camptype == eSBCDestroy)  //destroy attacker movement
		attacker = true;

	if (camptype == eSBCMajTown )  //occupy major town defender movement
		attacker = false;

	if (camptype == eSBCBegTown && OverAllCommander(base).GetID()==base.GetID()) //for this battle type the main army will be always the defender of supply zone
		attacker = false;

	if (camptype == eSBCBegTown && CXCamp::StartTown(1).Occupied()==0 && CXCamp::StartTown(0).Occupied()==1) //if my supply zone is occupied and I have already taken that of enemy i move back to my supply
	{
		if (CXCamp::StartTown(1).OccupiedTimer()>CXCamp::StartTown(0).OccupiedTimer()) //I don't move if I can win just waiting
			attacker = false;
	}


//*************************************************************
	if ( state == eSBCInit )   //init called at the start and at every split
	{

		SXComm comm;
		SXTown town;
		SXTown townInitial;
		CXOff Leader;
		int counter = CXUtil::Ran(98,0,7);  //randomic initial deployment

//the initial movements are all in the same direction
		CXCamp::SetForceTimer(base,0);  //timer initialization
//		base.TicksForOffSDK(3,0);

		//Initial split up  for corps level
		InitialSplitUp(base);

		
		townInitial = InitialDirection(base,camptype,attacker);  //set a direction quadrant using the first town

// the first generic movements have the meaning to generate different path of approaching and put distance between the base and me

		if (OverAllCommander(base).GetID()==base.GetID())  //initial order of movemenet from the big commander  the other follow
		{
			if (camptype != eSBCOneTown)
				town = GetTownXQuad(base,(2-base.Rank()),base.GetQuad(townInitial.Loc()));  //the generic direction on the base of quadrant
			else
				town = CXCamp::TargTown();  //target destination
		}
		else
		{
			if (camptype != eSBCMajTown && camptype != eSBCDestroy)    
				town = GetTownXQuad(base,counter,base.GetQuad(townInitial.Loc()));  //the generic direction on the base of quadrant
			else
				town = TownMovements(base,camptype,attacker,counter);  //target destination
		}


		if (town)
		{
			comm.Vec (true, town.Loc());
			comm.Val (base.GetID());
			comm.Comm(eComSBCSetDest);
			CXUtil::AddComm( &comm );
		}
	}
//*************************************************************

	if ( state == eSBCArrive )  //event when arrive to a town
	{
		SXComm comm;	
		CXOff nearfriend;


		if (EnemyNear(base))  //enemy near I don't make this
		{
			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
			}			
			return true;
		}

//		if (base.TicksForOffSDK(3)>1 && base.TicksForOffSDK(3)<=16)  //retreat phase
		if (CXCamp::GetForceTimer(base)>0)
			return true;

		if (camptype == eSBCOneTown)  //occupy 1 town
		{
			

			SXTown town = CXCamp::TargTown();


			if (town!=NULL)   //I move to the enemy supply base if not already occupied not by this unit
			{

				if (OverlapWithLeader(base))  //reattach phase 
				{
					comm.Val(base.GetID());
					comm.Comm(eComSBCAttach);
					CXUtil::AddComm(&comm);
					return true;
				}

				if (base.Loc().DistYds(town.Loc())<5)  //already on the objecive town
					return true;

				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

		}

		if (camptype == eSBCMajTown )  //occupy the biggest part of map towns
		{
			if (DefenderSubDeployment( base ))  //move on the occupation town
				return true;

			if (CorpPicket(base))  //arrived to a town I detach subunits to take other towns
				return true;
		}

		if (camptype == eSBCDestroy)  // hunt enemy 
		{

			nearfriend = ChaseEnemy(base,OverAllCommander(base),15000);  //help friend 15000 yards from me

			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			int dist;   //attack nearest city occupied by enemy
			SXTown town;
			town = CXCamp::TownClose(base,dist);
			if (EnemyTownClose(base))
			{					
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			town = DirectionQuadrantTownOccupied(base,15000);  // I search the first occupied town
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}


			CorpAdvanceGuard(base);  //detach a subunit
//			AttackerSubDeployment( base );
			return true;	
		}

		if (camptype == eSBCBegTown)  //take enemy supply base, protect mine
		{
	
			int dist;
			SXTown town = CXCamp::StartTown(0);
			SXTown Mytown = CXCamp::StartTown(1);
			//Defenders needs to remain near the supply base the attackers needs to attack enemy base
			if (attacker)  //things to do as attacker 
			{	

				if (town!=NULL && base.Loc().DistYds(town.Loc())>5 )   //I move to the enemy supply base if not already occupied not by this unit
				{
					CorpAdvanceGuard(base);
					comm.Vec (true, town.Loc());
					comm.Val (base.GetID());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm( &comm );
					return true;
				}
				else
				{
					MergeWithSubordinate(base);  //merge with subordinate in range before the battle
					if (OverlapWithLeader(base))  //reattach phase 
					{
						comm.Val(base.GetID());
						comm.Comm(eComSBCAttach);
						CXUtil::AddComm(&comm);
						return true;
					}
				}
			}
		
			if (!attacker)  //things to do as defender
			{					

				CorpPicket(base); //arrived to a town I detach a picket unit near
				if (Mytown!=NULL && Mytown.Occupied()!=1 )   //I move mytown if not ocupied by me
				{
					comm.Vec (true, Mytown.Loc());
					comm.Val (base.GetID());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm( &comm );
					return true;
				}
				else if (Mytown!=NULL && Mytown.Occupied()==1 && base.Loc().DistYds(Mytown.Loc())>5)  //I move in patrol if my town already occupied by me
				{
					town = DirectionQuadrant(base, 2);  //town of destination
					if (!town)   //no town I pick the nearest
						town = CXCamp::TownClose(base,dist);
					if (town)
					{
						comm.Vec(true, town.Loc());
						comm.Val (base.GetID());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm( &comm );
					}
				}


			}



		}

	}
//*************************************************************
	if ( state == eSBCEnemySeen )  //enemy just seen
	{
		MergeWithSubordinate(base);  //merge with subordinate in range before the battle

		// if you receive this message, you MUST call either eSBCBattle, or eSBCRetreat
		// the opposing force will be in the enemy list of this officer

		if (AttackEnemy(base,attacker))
		{
			return eComSBCBattle;
		}
		else
		{
//			base.TicksForOffSDK(3,16);  //set for retreat order
			CXCamp::SetForceTimer(base,CXUtil::GetTime());
			return eComSBCRetreat;
		}
	}
//*************************************************************
	if ( state == eSBCEnemyGone )  //enemy disappeared from my radar
	{

		SXComm comm;
		CXOff nearfriend;

		if (!base.Standing())
			return true;

//		if (base.TicksForOffSDK(3)>1 && base.TicksForOffSDK(3)<=16)  //retreat phase
		if (CXCamp::GetForceTimer(base)>0)
			return true;

		if (camptype == eSBCDestroy)  // hunt enemy 
		{
			SXTown town;

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}

			CXOff leader = TrueLeader(base);
			if (leader)							//call my leader
			{
				//Leader goes to my location
				comm.Vec (true, base.Loc());
				comm.Val (leader.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

			nearfriend = ChaseEnemy(base,OverAllCommander(base),15000);  //help friend 15000 yards from me

			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			town = DirectionQuadrantTownOccupied(base,15000);  // I search the first occupied town
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}


			AttackerSubDeployment( base );
			return true;	
		}

		if (camptype == eSBCOneTown  )  
		{
			SXTown town = CXCamp::TargTown();
			//Defenders needs to remain near the supply base the attackers needs to attack enemy base	

			if (town!=NULL)   //I move to the enemy supply base if not already occupied not by this unit
			{

				MergeWithSubordinate(base);  //merge with subordinate in range before the battle
				if (OverlapWithLeader(base))  //reattach phase 
				{
					comm.Val(base.GetID());
					comm.Comm(eComSBCAttach);
					CXUtil::AddComm(&comm);
					return true;
				}

				if (base.Loc().DistYds(town.Loc())<5)  //already on the objecive town
					return true;

				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

		}

		if (camptype == eSBCBegTown)  
		{
			
			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
			}
			return true;

		}

		if (camptype == eSBCMajTown)
		{
			int dist;
			CXOff leader = OverAllCommander(base);

			if (leader.GetID()!=base.GetID())							//call my overall commander
			{
				//I call the overall commander to my location
				comm.Vec (true, base.Loc());
				comm.Val (leader.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

			SXTown town = CXCamp::TownClose(base,dist);
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

		}


	}
//*************************************************************
	if ( state == eSBCWonBatt )  //battle won
	{
		SXComm comm;
		SXTown town;
		CXOff nearfriend;

//		base.TicksForOffSDK(3,0);  //reset for retreat order
		CXCamp::SetForceTimer(base,0);

		if (camptype == eSBCDestroy)  // hunt enemy 
		{
			
			CXOff leader = TrueLeader(base);
			if (leader)							//call my leader
			{
				//I go to my leader location
				comm.Vec (true, base.Loc());
				comm.Val (leader.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

			nearfriend = ChaseEnemy(base,OverAllCommander(base),15000);  //help friend 15000 yards from me
			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			town = DirectionQuadrantTownOccupied(base,15000);  // I search the first occupied town
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			if (!base.Standing())
				return true;

			int dist;
			town = CXCamp::TownClose(base,dist);
			if (EnemyTownClose(base))
			{					
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

			return true;

		}

		if (camptype == eSBCOneTown  )  
		{
			SXTown town = CXCamp::TargTown();


			if (town!=NULL)   //I move to the enemy supply base if not already occupied not by this unit
			{
				MergeWithSubordinate(base);  //merge with subordinate in range before the battle
				if (OverlapWithLeader(base))  //reattach phase 
				{
					comm.Val(base.GetID());
					comm.Comm(eComSBCAttach);
					CXUtil::AddComm(&comm);
					return true;
				}

				if (base.Loc().DistYds(town.Loc())<5)  //already on the objecive town
					return true;

				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

		}

		if (camptype == eSBCBegTown  )  
		{

			nearfriend = ChaseEnemy(base,OverAllCommander(base),10000);  //help friend 10000 yards from me

			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			return true;

		}

		if (camptype == eSBCMajTown)
		{

			nearfriend = ChaseEnemy(base,OverAllCommander(base),10000);  //help friend 10000 yards from me

			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			if (!base.Standing())
				return true;

			int dist;
			SXTown town = CXCamp::TownClose(base,dist);
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
		}


	}
//*************************************************************
	if ( state == eSBCLostBatt )  //battle lost
	{
		SXComm comm;

		if (camptype == eSBCDestroy)  
		{

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}

			if (!base.Standing())
				return true;
			
			CXOff leader = TrueLeader(base);
			if (leader && leader.GetID()!=base.GetID())
			{
				//I go to my leader location
				comm.Vec (true, TrueLeader(base).Dst());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
			return true;
		}

		if (camptype == eSBCMajTown)   
		{

			if (!base.Standing())
				return true;

			CXOff leader = TrueLeader(base);
			if (leader && leader.GetID()!=base.GetID())
			{
				//I go to my leader location
				comm.Vec (true, TrueLeader(base).Dst());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
			return true;
		}

		if (camptype == eSBCBegTown  )  
		{

			if (!base.Standing())
				return true;

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}

			SXTown town = CXCamp::StartTown(1);  //I return to my city after have lost the battle
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
			return true;

		}

		if (camptype == eSBCOneTown)   
		{

			SXTown town = CXCamp::TargTown();
			if (town)
			{

				MergeWithSubordinate(base);  //merge with subordinate in range before the battle
				if (OverlapWithLeader(base))  //reattach phase 
				{
					comm.Val(base.GetID());
					comm.Comm(eComSBCAttach);
					CXUtil::AddComm(&comm);
				}

			}
			return true;
		}


	}
//*************************************************************
	if ( state == eSBCTiedBatt )  //battle tied
	{
		SXComm comm;

//		base.TicksForOffSDK(3,0);  //reset for retreat order
		CXCamp::SetForceTimer(base,0);

		if (camptype == eSBCDestroy)  // hunt enemy 
		{

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}


			RecallAllMySub(base,base.Loc());  //call all my detached parts
			CXOff leader = TrueLeader(base);
			if (leader)							//call my leader
			{
				//I call my leader
				comm.Vec (true, base.Loc());
				comm.Val (leader.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}
		}

		if (camptype == eSBCMajTown)  // hunt enemy 
		{

			CXOff leader = OverAllCommander(base);
			if (leader.GetID()!=base.GetID())	//call overallcommander
			{
				//I go to my leader location
				comm.Vec (true, base.Loc());
				comm.Val (leader.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
			}

		}

		if (camptype == eSBCBegTown)  
		{

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle

			RecallAllMySub(base,base.Loc());  //call all my detached parts
			return true;
		}


		if (camptype == eSBCOneTown)   
		{

			SXTown town = CXCamp::TargTown();
			if (town)
			{
				MergeWithSubordinate(base);  //merge with subordinate in range before the battle
				if (OverlapWithLeader(base))  //reattach phase 
				{
					comm.Val(base.GetID());
					comm.Comm(eComSBCAttach);
					CXUtil::AddComm(&comm);
				}

			}
			return true;
		}

	}

//*************************************************************
	if ( state == eSBCThink )  //30 minutes heartbeat
	{

		SXComm comm;
		SXTown town;
		CXOff nearfriend;
		int randomaction = CXUtil::Ran(98,1,3);
		int counter;
		CXOff offenemy;

		CXOff leader = OverAllCommander(base);




		if (EnemyNear(base))  //enemy near I don't make this
		{

			MergeWithSubordinate(base);  //merge with subordinate in range before the battle
			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}	

		}

//		if ( base.TicksForOffSDK(3)>1 && base.TicksForOffSDK(3)<=16) //waiting phase
		if (CXCamp::GetForceTimer(base)>0 && ( CXUtil::GetTime() - CXCamp::GetForceTimer(base) ) < RETREATDELAY)
		{
			int dist = 500;
			SXTown townclose = CXCamp::TownClose(base,dist);
			CXOff enemynear = GetEnemyNear(base);
//			base.TicksForOffSDK(3,(base.TicksForOffSDK(3)-1));
			offenemy = GetEnemyNear(base);
			if (offenemy && base.Standing() && DivisionNumbers(offenemy)>DivisionNumbers(base) && base.GetID()!=TrueLeader(base).GetID() &&
				OverlapWithLeader(base))
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
			}
//----i march to the nearest occupied town in the quadrant opposite to that of nearest enemy----------------
			else if (townclose && enemynear)
			{

				int quadtomove = 4;
				quadtomove = OppositeQuad(base.GetQuad(enemynear));
				SXTown desttown = DirectionQuadrantTownOccupied(base,90000,1,quadtomove);	

				if (desttown)
				{
					if (desttown.Loc()!=base.Dst())
					{
						comm.Val(base.GetID());
						comm.Vec(true,desttown.Loc());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm(&comm);
					}
				}
				else if (base.GetQuad(CXCamp::StartTown(1).Loc())==quadtomove)  //no destination I return to the starting point
				{
					comm.Val(base.GetID());
					comm.Vec(true,CXCamp::StartTown(1).Loc());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm(&comm);
				}
			}
//--------------------------------------------------------------------------------
			return true;
		}

		CXCamp::SetForceTimer(base,0);  //reset retreat timer

		if (camptype == eSBCOneTown)
		{

			if (OverlapWithLeader(base))  //reattach phase 
			{
				comm.Val(base.GetID());
				comm.Comm(eComSBCAttach);
				CXUtil::AddComm(&comm);
				return true;
			}

			if (!base.Standing())
				return true;
			//to check if I occupy already bruxelles or not

			SXTown town = CXCamp::TargTown();
			//Defenders needs to remain near the supply base the attackers needs to attack enemy base	

			if (town!=NULL)   //I move to the enemy supply base if not already occupied not by this unit
			{

				if (base.Loc().DistYds(town.Loc())<5)  //already on the objecive town
					return true;

				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

		}



		if (camptype == eSBCMajTown )  //occupy the biggest part of town
		{


			if (!base.Standing())
				return true;

			if (leader.GetID()!=base.GetID() )    //normal unit
			{

				if (EnemyNear(base))  //enemy near I don't make this
				   return true;

				int dist;
				town = CXCamp::TownClose(base,dist);
				if (EnemyTownClose(base))
				{					
					comm.Vec (true, town.Loc());
					comm.Val (base.GetID());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm( &comm );
				}				
			}
			else if (leader.GetID()==base.GetID() )  //if the leader of army continue to move to support units or search enemy
			{
				if (randomaction==1 || EnemyNear(base))
				{
					nearfriend = ChaseEnemy(base,OverAllCommander(base),15000);  //help friend 15000 yards from me

					if (nearfriend)  //I chase enemy less strong than me
					{
						comm.Vec (true, nearfriend.Loc());
						comm.Val (base.GetID());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm( &comm );
						return true;
					}

					int dist;   //attack nearest city occupied by enemy
					town = CXCamp::TownClose(base,dist);
					if (EnemyTownClose(base))
					{					
						comm.Vec (true, town.Loc());
						comm.Val (base.GetID());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm( &comm );
					}

				}
				else if (randomaction==2)
				{
					//nothing
				}
				else if (randomaction==3)
				{
					//patrol
					counter = CXUtil::Ran(98,1,5);  //randomize number of cycles
					town = DirectionQuadrantNoCheck(base,counter);  // I search the first free town
					if (town)
					{
						comm.Vec (true, town.Loc());
						comm.Val (base.GetID());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm( &comm );
					}
				}


			}
			return true;
		}


		if (camptype == eSBCDestroy)  // hunt enemy 
		{

			SXTown town;


			nearfriend = ChaseEnemy(base,OverAllCommander(base),15000);  //help friend 15000 yards from me

			if (nearfriend)  //I chase enemy less strong than me
			{
				comm.Vec (true, nearfriend.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}


			if (EnemyNear(base))  //enemy near I don't make this
			   return true;

			int dist;   //attack nearest city occupied by enemy
			town = CXCamp::TownClose(base,dist);
			if (EnemyTownClose(base))
			{					
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}

			if (!base.Standing())
				return true;			
			
			town = DirectionQuadrantTownOccupied(base,15000);  // I search the first occupied town
			if (town)
			{
				comm.Vec (true, town.Loc());
				comm.Val (base.GetID());
				comm.Comm(eComSBCSetDest);
				CXUtil::AddComm( &comm );
				return true;
			}	

			if (base.Standing())		
				CorpAdvanceGuard(base);

			AttackerSubDeployment( base );
			return true;	

		}

		if (camptype == eSBCBegTown)
		{

			if (!base.Standing())
				return true;

			SXTown town = CXCamp::StartTown(0);
			SXTown Mytown = CXCamp::StartTown(1);
			//Defenders needs to remain near the supply base the attackers needs to attack enemy base
			if (attacker )  //things to do as attacker 
			{	
				if (town!=NULL && base.Loc().DistYds(town.Loc())>5)   //I move to the enemy supply base if not already occupied not by this unit
				{
					comm.Vec (true, town.Loc());
					comm.Val (base.GetID());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm( &comm );
				}
				else
				{
					MergeWithSubordinate(base);  //merge with subordinate in range before the battle
					if (OverlapWithLeader(base))  //reattach phase 
					{
						comm.Val(base.GetID());
						comm.Comm(eComSBCAttach);
						CXUtil::AddComm(&comm);
						return true;
					}
				}
			}
		
			if (!attacker )  //things to do as defender
			{					
				int dist;
				
				if (Mytown!=NULL && Mytown.Occupied()!=1 )   //I move to the enemy supply base if not already occupied not by this unit
				{
					comm.Vec (true, Mytown.Loc());
					comm.Val (base.GetID());
					comm.Comm(eComSBCSetDest);
					CXUtil::AddComm( &comm );
				}
				else if (randomaction==3 && Mytown.Occupied()==1 && base.Loc().DistYds(Mytown.Loc())>5)  //I move in patrol
				{
					town = DirectionQuadrant(base, 2);  //town of destination
					if (!town)   //no town I pick the nearest
						town = CXCamp::TownClose(base,dist);
					if (town)
					{
						comm.Vec(true, town.Loc());
						comm.Val (base.GetID());
						comm.Comm(eComSBCSetDest);
						CXUtil::AddComm( &comm );
					}
				}
			}

			return true;

		}

	}  //eSBCThink

	return true;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Callers
//*************************************************************************************************************
//*************************************************************************************************************

int SowAIFunc( int state, void *punit, int val )
{
	int res;

	CXOff unit((CUnit *)punit);

	res = CCampThink( state, unit, val );		

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

