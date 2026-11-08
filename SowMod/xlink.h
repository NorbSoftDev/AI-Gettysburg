//*************************************************************************************************************
//*************************************************************************************************************
// File: xlink.h
//*************************************************************************************************************
//*************************************************************************************************************

#pragma once

class CVec;
class CPos;
class CUnit;
class CFRect;
class SArmyPlay;

struct SComm;
struct STown;
struct TStance;
struct TTacOrders;
struct SDivStrategy;

struct SSowModFuncs
{
	// tables
	const SDivStrategy *gTableDivStrat;
	const TStance      *gTableBrigStance;
	const TTacOrders   *gTableTacOrders;

	// CVec
	CVec *(*CVecNew)();
	void (*CVecCopy)(const CVec *src, CVec *dst);
	void (*CVecDel)(CVec *vec);
	bool (*CVecEq)(const CVec *src, const CVec *dst);
	bool (*CVecNEq)(const CVec *src, const CVec *dst);
	int  (*CVecDistYds)(const CVec *src, const CVec *dst);
	int  (*CVecDist2Yds)(const CVec *src, const CVec *dst);
	bool (*CVecempty)(const CVec *src);
//-----davide------x and y return--------
	int  (*CVecGetX)(const CVec *src);
	int  (*CVecGetY)(const CVec *src);
//------------------------------------------

	// CRect
	CFRect *(*CRectNew)();
	void (*CRectCopy)(const CFRect *src, CFRect *dst);
	void (*CRectDel)(CFRect *rect);
	
	void (*CRectSet)(CFRect *rect, int topx, int topy, int botx, int boty );
	
	int (*CRectTopX)(CFRect *rect,bool bSet, int ival);
	int (*CRectTopY)(CFRect *rect,bool bSet, int ival);
	int (*CRectBotX)(CFRect *rect,bool bSet, int ival);
	int (*CRectBotY)(CFRect *rect,bool bSet, int ival);
	
	void  (*CRectNorm)(CFRect *rect);
	CVec *(*CRectMid)(const CFRect *rect,CVec *vec);

	// SComm
	SComm *(*SCommNew)();
	void (*SCommCopy)(const SComm *src, SComm *dst);
	void (*SCommDel)(SComm *vec);
	int (*SCommComm)(SComm *comm,int i);
	int (*SCommVal)(SComm *comm,int i);
	int (*SCommUnit)(SComm *comm,int i);
	CVec *(*SCommVec)(SComm *comm,bool bset, CVec *loc);
	CVec *(*SCommDir)(SComm *comm,bool bset, CVec *dir);
	int  (*SCommMorale)(SComm *comm,int ival);					
	void (*SCommParam)(SComm *comm, int i, const char *cmd );
	void (*SCommSetBit)(SComm *comm,DWORD cbit);							
	void (*SCommDelBit)(SComm *comm,DWORD cbit);							
	bool (*SCommChkBit)(const SComm *comm,DWORD cbit);	
	bool (*SCommBuildCommand)(SComm *comm, const char *command, int from, int to );

	// CPos
	CPos *(*CPosNew)();
	void (*CPosCopy)(const CPos *src, CPos *dst);
	void (*CPosDel)(CPos *pos);
	const CVec *(*CPosLoc)(const CPos *pos);
	const CVec *(*CPosDir)(const CPos *pos);
	int (*CPosGetAngle)(const CPos *pos); // angle around y axis
	int (*CPosGetAngleDiff)(const CPos *pos, const CVec *vec,bool bReal); // based on pos facing, angle to vec,breal modifies output
	
	// CUtil
	int (*CUtilRan)( int index, int low, int high );
	int (*CUtilAsk)( int itype );
	int (*CUtilGetTime)();
	int (*CUtilVis2)();																						 
	int (*CUtilYtoU)(int yds);																						 
	int (*CUtilUtoY)(int mapunits);																						 
	int (*CUtilOpenType)();																				 
	int (*CUtilScenType)();																				 
	int (*CUtilGetDifficulty)();																				 
	int (*CUtilStratAI )();																				 
	int (*CUtilMaxCoord)();																				 
	int (*CUtilTriggerEvent)( int evtid, int unitid, int evttype );
	int (*CUtilMultSpec)(int,int,int);
	bool (*CUtilHexIsFire)(CVec *loc);																							  
	bool (*CUtilAreMajorsInRect)( CFRect *rect, int *majobj, int army );
	bool (*CUtilIsMulti)();																													
	void (*CUtilAddLog)( const char *str );																					
	void (*CUtilAddComm)( SComm *comm );																						
	void (*CUtilAddTimeComm)( SComm *comm, int starttime );													
	void (*CUtilFindClosestFort)( const CUnit *unit, int frange, CVec *vec );																				 
	CUnit     *(*CUtilGetSold)(int id);										 
	SArmyPlay *(*CUtilGetAttackPlay)();								 
	SArmyPlay *(*CUtilGetDefendPlay)();								 
	SArmyPlay *(*CUtilGetMeetingPlay)( CFRect *rect );	 

	// CUnit
	int (*CUnitGetQuad)( const CUnit *me, const CUnit *un );
	int (*CUnitGetQuadVec)( const CUnit *me, const CVec *loc );
	bool (*CUnitDoesShareHex)( const CUnit *me, const CUnit *un );
	void (*CUnitRouteUnit)(CUnit *me);
	void (*CUnitFollowStop)(CUnit *me);
	void (*CUnitModMorale)(CUnit *me,int pts);
//--------davide---------------------------------------
	void (*CUnitModFatigue)(CUnit *me,int pts);
//------------------------------------------------------------
	bool (*CUnitCheckRouting)(const CUnit *me);
	int (*CUnitGetUnitTypeVal)(const CUnit *me,int itype);
	void (*CUnitSetMainTarg)(CUnit *me, const CUnit *un);
	CUnit *(*CUnitMainTarg)(const CUnit *me);
	CUnit *(*CUnitFollowTarg)(const CUnit *me);
	CUnit *(*CUnitGetCharger)(const CUnit *me);
	int (*CUnitAmmo)(const CUnit *me);
	int (*CUnitRank      )(const CUnit *me,int itype);
	int (*CUnitSplit     )(const CUnit *me);
	int (*CUnitState     )(const CUnit *me);
	int (*CUnitNumMen    )(const CUnit *me);
	int (*CUnitMorale    )(const CUnit *me);
	int (*CUnitPrevMor   )(const CUnit *me);
	int (*CUnitLimState  )(const CUnit *me);
	int (*CUnitUnitType  )(const CUnit *me);
	int (*CUnitStartMen  )(const CUnit *me);
	int (*CUnitNumTargs  )(const CUnit *me);
	int (*CUnitHaltTics  )(const CUnit *me);
	int (*CUnitShootTime )(const CUnit *me);
	int (*CUnitNumSprites)(const CUnit *me);
	int (*CUnitMorChange )(const CUnit *me);
	int (*CUnitMorOffBon)(const CUnit *me);
	int (*CUnitMorUnitBon)(const CUnit *me);
	int (*CUnitGetMod)(const CUnit *me,int itype);
	int (*CUnitKillTime)(CUnit *me,int ival);
	int (*CUnitFollowType)(CUnit *me,int ival);
	int (*CUnitFollowTics)(CUnit *me,int ival);
	int (*CUnitFormCloseYds  )(const CUnit *me);
	int (*CUnitFormIndex     )(const CUnit *me); 
	int (*CUnitFormTypeIndex)(const CUnit *me,int ftype); 
	const CPos *(*CUnitPos)(const CUnit *me);
	const CVec *(*CUnitLoc)(const CUnit *me);
	const CVec *(*CUnitDir)(const CUnit *me);
	const CVec *(*CUnitDst)(const CUnit *me);
	const CPos *(*CUnitDeP)(const CUnit *me);
	const CVec *(*CUnitFollowLoc)(const CUnit *me);
	bool (*CUnitFindClosestFeature)(const CUnit *me,unsigned int feature, int radyds, CVec &retvec);
	void (*CUnitSetFollowLoc)(CUnit *me,CVec &loc);
	bool (*CUnitIsArt     )( const CUnit *me );
	bool (*CUnitCanRun    )( const CUnit *me );
	bool (*CUnitHidden    )( const CUnit *me );
	bool (*CUnitCurrTarg  )( CUnit *me,int ival);
	bool (*CUnitMoving    )( const CUnit *me, bool bSubs );
	bool (*CUnitRouted    )( const CUnit *me );
	bool (*CUnitBlocked   )( const CUnit *me );
	bool (*CUnitMarching  )( const CUnit *me );
	bool (*CUnitCanWheel  )( const CUnit *me );
	bool (*CUnitCanFight  )( const CUnit *me );
	bool (*CUnitPlyrCtrl  )( const CUnit *me );
	bool (*CUnitIsSplit   )( const CUnit* me );
	bool (*CUnitAttached  )( const CUnit *me );
	bool (*CUnitIsFlanked )( const CUnit *me );
	bool (*CUnitIsOfficer )( const CUnit *me );
	bool (*CUnitCanTarget )( const CUnit *me );
	bool (*CUnitUnderFire )( const CUnit *me );
	bool (*CUnitOnDefTerr )( const CUnit *me );
	bool (*CUnitIsHighGrnd)( const CUnit *me );
	bool (*CUnitUnderFire1)( const CUnit *me );
	bool (*CUnitRetreating)( const CUnit *me );
	bool (*CUnitNewDefTerr)( const CUnit *me );
	bool (*CUnitSwitchCmn)( CUnit *me );
	bool (*CUnitCanHalt)( CUnit *me, const CVec *arect );
	bool (*CUnitFCommand)( CUnit *me, int command, int fval ); // sends as float
	bool (*CUnitICommand)( CUnit *me, int command, int val );
	bool (*CUnitICommand2)( CUnit *me, int command, CVec *vec );
	bool (*CUnitCanCharge)( const CUnit *me, const CUnit *targ, bool bMelee );
	bool (*CUnitValidWalk)( const CUnit *me, const CVec *vec );
	bool (*CUnitValidDest   )( const CUnit *me, const CVec *vec );
	bool (*CUnitValidSpot   )( const CUnit *me, const CVec *vec );
	bool (*CUnitIsShotTarg  )( const CUnit *me, int i );
	bool (*CUnitIsValidShot )( const CUnit *me, int isprite );
	bool (*CUnitIsFormType )( const CUnit *me, int ftype );
	bool (*CUnitGetBackwards)( const CUnit *me );
//---davide  ---- new functions for the modders------------------
	int (*CUnitDBonusVal)( const CUnit *me );
	int (*CUnitGetLevel)( const CUnit *me, int ePointName );
	int (*CUnitGetPoints)( const CUnit *me, int ePointName );
	int (*CUnitHGround)(const CUnit *me);
	int (*CUnitUnitAIType) (const CUnit *me);
	bool (*CUnitUsingRoad) (const CUnit *me);
//------------------------------------------------------
	bool (*CUnitInFort )     ( const CUnit *me );
	bool (*CUnitCanFormFire )   ( const CUnit *me );
	bool (*CUnitWheelToTarget)  ( CUnit *me, int iunit );
	bool (*CUnitWheelToTarget2)  ( CUnit *me, CUnit *target );
	bool (*CUnitUnderRecentFire)( const CUnit *me );
	bool (*CUnitJustWheeled)(CUnit *me,int ival);
	int  (*CUnitVolleyStatus)(const CUnit *me);
	bool (*CUnitIsSprinting)(CUnit *me,int ival);
	bool (*CUnitWantCover  )(CUnit *me,int ival);
	bool (*CUnitAttackMarch)(CUnit *me,int ival);
	void (*CUnitTargBeg)(const CUnit *me);
	bool (*CUnitTargNext)( const CUnit *me, CUnit *&sold, int &dist );
	void (*CUnitEnemyBeg)(const CUnit *me);
	bool (*CUnitEnemyNext)( const CUnit *me, CUnit *&sold, int &dist );
	void (*CUnitFriendBeg)(const CUnit *me);
	bool (*CUnitFriendNext)( const CUnit *me, CUnit *&sold, int &dist );
	bool (*CUnitGetValidDest)( const CUnit *me, const CPos &spot, CVec *pt, bool bExtra );
	void (*CUnitValidatePath)( CUnit *me );
	CUnit *(*CUnitTargClose)(const CUnit *me);
	void (*CUnitTargAdd)(CUnit *me, CUnit *targ, int dist );
	
	int  (*CUnitTargCloseYds )(const CUnit *me);
	bool (*CUnitTargFind     )(const CUnit *me, CUnit *unit, int &dist );
	bool (*CUnitEnemyFind    )(const CUnit *me, CUnit *unit, int &dist );
	int  (*CUnitEnemyCloseYds)(const CUnit *me);
	int  (*CUnitMaxRangeYds  )(const CUnit *me);
	int  (*CUnitLongRangeYds )(const CUnit *me);
	
	CUnit *(*CUnitEnemyClose  )(const CUnit *me);
	CUnit *(*CUnitFriendClose  )(const CUnit *me);
	void   (*CUnitGetLocLeft ) (const CUnit *me, int yds, CVec *vec );
	void   (*CUnitGetLocRight) (const CUnit *me, int yds, CVec *vec );
	void   (*CUnitGetLocAhead) (const CUnit *me, int yds, CVec *vec );
	void   (*CUnitGetLocBehind)(const CUnit *me, int yds, CVec *vec );
	void   (*CUnitSetClosest)(CUnit *me, CUnit *unit );
	void   (*CUnitSetFollow )(CUnit *me, CUnit *unit );
	void   (*CUnitSetDest   )(CUnit *me, const CVec &dest );
	void   (*CUnitSetDest2  )(CUnit *me, const CPos &dest );
	bool   (*CUnitCommand   )(CUnit *me, const SComm *comm );
	CUnit *(*CUnitLeader    )(CUnit *me);
	CUnit *(*CUnitClosest   )(CUnit *me);
	void (*CUnitRepath)(CUnit *me);
	bool (*CUnitCheckPath    )(CUnit *me);
	int  (*CUnitMountState   )(const CUnit *me);
	int  (*CUnitRunTimer     )(CUnit *me, int val );
	int  (*CUnitFireMode     )(const CUnit *me);
	int  (*CUnitDistArtyConsv)(const CUnit *me);
	int  (*CUnitDistArtyPanic)(const CUnit *me);
	void (*CUnitLimber       )(CUnit *me);
	int  (*CUnitAmmoCmd      )(const CUnit *me);
	int  (*CUnitAmmoType     )(CUnit *me, int val );
	void (*CUnitChooseAmmo        )( CUnit *me, int distyds, CUnit *closest );
	bool (*CUnitHasInfOrArtFriends)( const CUnit *me,const int &wdist2, int imax );

	bool (*CUnitPrevTarg      )(CUnit *me,int ival);
	int  (*CUnitGetID         )(const CUnit *me);		
	bool (*CUnitForceArtyMove )(const CUnit *me);		
	void (*CUnitGiveMeAmmo    )(CUnit *me, CUnit *sold );
	bool (*CUnitNeedAmmo      )(CUnit *me, int *i );
	bool (*CUnitStanding      )(const CUnit *me);		
	int  (*CUnitFriendCloseYds)(const CUnit *me);		
	bool (*CUnitFriendEmpt    )(const CUnit *me);		
	bool (*CUnitAppSameRank   )(const CUnit *me);		
	bool (*CUnitInCorp        )(const CUnit *me);		
	bool (*CUnitPathEmpty     )(const CUnit *me);		
	bool (*CUnitMyGuys        )(const CUnit *me);		
	bool (*CUnitIsMe          )(const CUnit *me);		
	bool (*CUnitTCOn          )(const CUnit *me);		
	bool (*CUnitLocked        )(const CUnit *me);		
																							
	const char *(*CUnitName)(const CUnit *me); 

	// Officers
	bool (*COffCanICharge        )( const CUnit *me, CUnit *unit, int ival );
	bool (*COffStopOnDefTerr     )( const CUnit *me, CUnit *unit );
	bool (*COffShouldGetAmmo     )( const CUnit *me, CUnit *unit );
	bool (*COffFindClosestSpec   )( const CUnit *me,int type, const CVec &loc, CVec &ammo );
	bool (*COffShouldAttack      )( CUnit *me,bool bReg, CUnit *targ, CUnit *unit, bool bWeight );
	void (*COffGetScoutLoc       )( const CUnit *me, CUnit *cav, CVec &loc, bool bNew );
	int  (*COffPersonality       )( const CUnit *me);
	int  (*COffChargeArtyDistYds )( CUnit *unit);

	const TStance *(*COffGetStance)(const CUnit *unit);
	const TTacOrders *(*COffGetOrders)(const CUnit *unit);

	int (*COffTicksForOffSDK)( CUnit *me, int iwhich, int ival );

	bool (*COffGetLineDest    )( CUnit *me,CUnit *unit, CVec &pt );
	bool (*COffGetFlankDest   )( CUnit *me,CUnit *unit, CVec &pt, CUnit *targ );
	bool (*COffGetForwardDest )( CUnit *me,CUnit *unit, CVec &pt, CUnit *targ );
	bool (*COffGetMaintainDest)( CUnit *me,CUnit *unit, CVec &pt, CVec &dir  );

	CUnit *(*COffCheckWheel    )(CUnit *me);                                  
	CUnit *(*COffGetSaveClosest)(CUnit *me,int &dist );			
	bool (*COffHasStance )(const CUnit *me);	              
	bool (*COffIQuit     )(CUnit *me);											
	bool (*COffOrders    )(CUnit *me, SComm *comm );	
	bool (*COffOffThink  )(CUnit *me);											
	bool (*COffInTrouble )(CUnit *me);											
	bool (*COffHasOrders )(const CUnit *me);
	bool (*COffHasTargets)(const CUnit *me);
	int  (*COffCourTime )(CUnit *me, bool bSet, int ival );
	int  (*COffFightForm)(const CUnit *me);
	int  (*COffWheelTime)(CUnit *me, bool bSet, int ival );
	bool (*COffReEval    )(CUnit *me, bool bSet, bool bval );
	bool (*COffWantAttach)(CUnit *me, bool bSet, bool bval );
	void (*COffCheckEnemySee)     (CUnit *me);
	void (*COffCheckEnemyEng)     (CUnit *me);
	void (*COffReEvalTactics)     (CUnit *me);
	void (*COffObjectiveCompleted)(CUnit *me);
	bool (*COffFighting)(const CUnit *me);
	bool (*COffUnderCloseFire)(const CUnit *me);
	int   (*COffTACType)(const CUnit *me);
	int   (*COffTACObj )(const CUnit *me);
	CVec *(*COffTACLoc )(const CUnit *me);
	const char *(*COffTACOrders)(const CUnit *me);
	int (*COffORDTACMod)(const CUnit *me);
	const char *(*COffORDCall)(const CUnit *me);
	int (*COffNumSubs)(const CUnit *me);
	CUnit *(*COffSub)(CUnit *me,int i);
	int (*COffNumArty)(const CUnit *me);
	CUnit *(*COffArty)(CUnit *me,int i);
	int (*COffNumDetach)(const CUnit *me);
	CUnit *(*COffDet)(CUnit *me,int i);
	int   (*COffTACOBJRad2)(const CUnit *me);
	bool  (*COffTACOBJDone)(const CUnit *me);
	CVec *(*COffTACOBJLoc )(const CUnit *me);
	bool (*COffORDTACDivPlay)(const CUnit *me);
	int (*COffOrdersState)(CUnit *me, int ival);
	const char *(*COffAssignedOrders)(const CUnit *me);
	CUnit *(*COffGetClosest  )(CUnit *me, int &dist, bool bSubsOnly );
	CUnit *(*COffSupportUnits)(CUnit *me, bool btarg, bool bForce, bool bGet );
	bool (*COffOffScared       )(CUnit *me);
	bool (*COffRunningPlay     )(const CUnit *me);
	bool (*COffOrdersAllowWheel)(const CUnit *me);
	bool (*COffGetOrders2)(CUnit *me, const int &dist, CUnit *targ, CUnit *unit );
	bool (*COffEngagingEnemy)(const CUnit *me);
	void (*COffSendOrdersByCour)(CUnit *me, CUnit *to, int count, SComm *comm, bool bCheck, const char *orders, bool bForce );
	void (*COffMoveToBestGround)(CUnit *me, int rad, bool bReEval );
	bool (*COffUnLimbered)(const CUnit *me);
	void (*COffRally)(CUnit *me);
	bool (*COffHasPlay)(const CUnit *me);
	void (*COffDelPlay)(CUnit *me);
	int (*COffHelpTime)(CUnit *me, bool bSet, int ival);
	const char *(*COffPLAYLOrdersCall)(const CUnit *me,int i);
	void (*COffEvalLoc      )(CUnit *me);
	void (*COffEvalQuad     )(CUnit *me);
	void (*COffEvalObjective)(CUnit *me);
	void (*COffCheckReserves)(CUnit *me);
	bool  (*COffPSTATDone)(CUnit *me,bool bSet, bool val );
	CVec	*(*COffPSTATLRct)(const CUnit *me,int which);
	bool  (*COffPSTATLValid)(CUnit *me,int i, bool bSet, bool val);
	bool  (*COffPSTATRValid)(CUnit *me,int i, bool bSet, bool val);
	CUnit * (*COffPSTATLOff)(CUnit *me,int i);
	CUnit * (*COffPSTATROff)(CUnit *me,int i);
	void (*COffExecutePlay)(CUnit *me, bool bLeft, int spot, CUnit * off, int cavspot );
	void (*COffRunPlay)(CUnit *me, CUnit * best, CUnit *targ );
	bool (*COffGetMeetAttackQuad)(CUnit *me, CFRect *rect );
	SArmyPlay *(*COffArmyPlay)(CUnit *me,bool bSet, SArmyPlay *aplay );
	int (*COffNumComplete)(const CUnit *me);
	int (*COffComplete)(const CUnit *me,int i);
	int (*COffStartQuad)(const CUnit *me);
	void (*COffCompleteAdd)(CUnit *me,int i);
	int (*COffPLAYCorpNum)(const CUnit *me,SArmyPlay *play, int i);
	int (*COffPLAYFTime  )(const CUnit *me,SArmyPlay *play, int i);
	int (*COffPLAYSize   )(const CUnit *me,SArmyPlay *play);
	void (*COffRunArmyPlay)(CUnit *me, CUnit *off, CFRect *rect );
	CFRect *(*COffPLAYRect)(const CUnit *me,SArmyPlay *play,int i);

	// CCour
	int (*CCourTargId    )(CUnit *me, bool bSet, int ival);
	int (*CCourStartTime )(CUnit *me, bool bSet, int ival);
	int (*CCourRecalcTics)(CUnit *me, bool bSet, int ival);
	const char *(*CCourOrders)(const CUnit *me);
	int (*CCourNumCour)(const CUnit *me);
	int (*CCourNumComm)(CUnit *me,bool bSet, int ival);
	bool (*CCourHasComm)(const CUnit *me);
	SComm *(*CCourComm)(CUnit *me,int i);
	void (*CCourDelComm)(CUnit *me);
	void (*CCourDelOrders)(CUnit *me);
	void (*CCourShowScreen)(CUnit *me, bool bCaptured, bool bForce, bool bLogOnly );

	// Town
	const CVec *(*STownLoc)(const STown *me);
	int (*STownOccupied)(const STown *me);
	int (*STownOccupiedTimer)(const STown *me);

	// Campaign
	bool  (*CCampTownNext)(const CUnit *me, STown *&town, int &dist);
	STown *(*CCampTownClose)(const CUnit *me,int &dist);
	STown *(*CCampTargTown)();
	STown *(*CCampStartTown)(int which);
//------davide-----new campaign AI function--------------------
	void  (*CCampSetForceTimer)(const CUnit *me,int timer);
	int   (*CCampGetForceTimer)(const CUnit *me);
//----------------------------------------------------------

};

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

