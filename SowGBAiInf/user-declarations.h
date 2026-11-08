//*************************************************************************************************************
//*************************************************************************************************************
// all the modders data declarations need to be placed here
//*************************************************************************************************************
//*************************************************************************************************************
#pragma once

#include "../sowmod/xunit.h"
#include "../sowmod/xunitdef.h"


struct SIndex
{
	int FightFormIndex;
	int MarchFormIndex;
};


struct SEnemyExtension
{

//  insert type targets 
	int cavalrythreat;  //danger level from the cavlary
	CXUnit cavalryclosest;
	int    cavalrycdist;
	CXUnit closesttarget;
	int closesttargetdist;
 //   CXUnit righttarget;
	//int righttargetdist;
 //   CXUnit lefttarget;
	//int lefttargetdist;
 //   CXUnit reartarget;
	//int reartargetdist;
 //   CXUnit upperrighttarget;
	//int upperrighttargetdist;
 //   CXUnit upperlefttarget;
	//int upperlefttargetdist;
//  ---------------------------

};


