//*************************************************************************************************************
//*************************************************************************************************************
// File: sowaiinf.h
//*************************************************************************************************************
//*************************************************************************************************************

#pragma once

#include "../sowmod/xlink.h"

class CSowAiInfApp
{
	public:
		CSowAiInfApp();

		SSowModFuncs m_sow;
};
extern CSowAiInfApp theApp;

#ifdef __cplusplus
extern "C" {
#endif

#ifdef USING_SOWIMPORT
#define SOWDLLCALL __declspec(dllimport)
#else
#define SOWDLLCALL __declspec(dllexport)
#endif

SOWDLLCALL int SowInit     ( void * ); // SSowModFuncs
SOWDLLCALL int SowInfAIFunc( int, void * );
SOWDLLCALL int SowArtAIFunc( int, void * );
SOWDLLCALL int SowCavAIFunc( int, void * );
SOWDLLCALL int SowMntInfAIFunc( int, void * );

SOWDLLCALL int SowUnitBrigThink ( int, void * ); 
SOWDLLCALL int SowArtyOffThink  ( int, void * ); 
SOWDLLCALL int SowCavOffThink   ( int, void * ); 
SOWDLLCALL int SowSoldAmmoThink ( int, void * ); 
SOWDLLCALL int SowUnitDivThink  ( int, void * ); 
SOWDLLCALL int SowUnitCorpThink ( int, void * ); 
SOWDLLCALL int SowUnitArmyThink ( int, void * ); 
SOWDLLCALL int SowUnitSideThink ( int, void * ); 
SOWDLLCALL int SowSoldCourThink ( int, void * );

#ifdef __cplusplus
}
#endif

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

