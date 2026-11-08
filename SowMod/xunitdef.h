//*************************************************************************************************************
//*************************************************************************************************************
// File: xunitdef.h
//*************************************************************************************************************
//*************************************************************************************************************

#pragma once

#define TICSPERSEC 60
#define TICSPERMIN (TICSPERSEC * 60)

#define UTYPE_MelOrRet 0x00000001
// max val             0xffffffff

#define MAX_TICKSOFFSDK 10 // ten ints for the ai use, meant for ticks, saved with each officer

#define MORAL_ROUTED    0
#define MORAL_BROKEN    1
#define MORAL_CANTWHEEL	3

#define DISTARTYHOLD 200

// Bit Note - if you change this structure, you must also change the bitstream functions
// flags for dbits
// for m_dbits
#define eCBAll       0x001
#define eCBChngDest  0x002 
#define eCBForce     0x004 
#define eCBFromPlyr  0x008 
#define eCBMap       0x010 
#define eCBCour      0x020
#define eCBFightForm 0x040
#define eCBSubForm   0x080
#define eCBAddQueue  0x100
#define eCBDelQueue  0x200

enum EUtilAsk
{
	eUAIsCampaign,
};

// example of play quad
//
// -------1---Enemy---1-------
// ---------------------------
// 10-3-9-2--8-Off-8--2-9-3-10
// ---4------5-----5------4---
// 12-6-11---7-----7---11-6-12

enum ESpots
{
	eSpotNone,		 // empty
	eSpotTop,			 // spot 1
	eSpotMid,			 // spot 2
	eSpotFarMid,	 // spot 3
	eSpotBot,			 // spot 4
	eSpotBotMid,	 // spot 5
	eOldSpotMax,
	eSpotBack = eOldSpotMax,		 // spot 6
	eSpotBackMid,	 // spot 7
	eSpotCALine, // spot 8 CA = Cav/Art
	eSpotCAMid, // spot 9 CA = Cav/Art
	eSpotCAFarMid, // spot 10 CA = Cav/Art
	eSpotCABackMid, // spot 11 CA = Cav/Art
	eSpotCABack, // spot 12 CA = Cav/Art
	eSpotMax,
	eSpotGuard = eSpotMax, // for cav
	eSpotHigh, // for arty
};

// Campaign Status
enum ESBCCampAI
{
	eSBCInit,
	eSBCEnemyGone, // previous visible enemy to this unit, no longer visible
	eSBCEnemySeen, // previous unseen enemy to this unit, now visible
	eSBCArrive,    // force has arrived at their town destination
	eSBCWonBatt,   
	eSBCLostBatt,  
	eSBCTiedBatt,  
	eSBCThink,
};

enum ESBCCampType
{
	// objective types
	eSBCOneTown,
	eSBCMajTown,
	eSBCDestroy,
	eSBCBegTown,
};

// ground flag

#define SKD_NONE 0x00
#define SKD_ROAD 0x01
#define SKD_DEAD 0x02
#define SKD_FORT 0x04
#define SKD_UFRT 0x08 // unoccupied fort (or one that's held by our side and not full)
#define SKD_DBON 0x10
#define SKD_PATH 0x20	// path around dead areas

//*************************************************************************************************************
//*************************************************************************************************************
// Enum: EWeapRanges
//*************************************************************************************************************
//*************************************************************************************************************

enum EWeapRanges
{
	eWeapMinRange,
	eWeapOptimal,
	eWeapTypical,
	eWeapLong,
	eWeapMaxRange,
	eWeapMax,
};

//*************************************************************************************************************
//*************************************************************************************************************
// Enum: EEventType
//*************************************************************************************************************
//*************************************************************************************************************

enum EEventType
{
	eEvtTime,
	eEvtGiveUp,
	eEvtContinue,
	eEvtSeeTarg,	 // should work for any officer
	eEvtCourier,	 // a courier just delivered us a message
	eEvtObjDone,   // an objective has been reached
	eEvtArrived,   // unit has arrived and all his men are set
	eEvtDeath,     // the character dies
	eEvtInTrouble, 
	eEvtFighting,  
	eEvtGrade,     // when someone reaches a certain grade
	eEvtObjArmy1,  // when army 1 wins the obj
	eEvtObjArmy2,  // when army 2 wins the obj
	eEvtFailCheck,
	eEvtIArrived,  // unit has arrived
	eEvtRan,       // random event
	eEvtDist,      // distance to enemy
	eEvtMax,
};

enum ETypeOpen
{
	eTypeNone,
	eTypeAtt1, // must be value 1
	eTypeAtt2, // must be value 2
	eTypeMeet,
	eTypeHunt,
};

enum EScenario
{
	eScenNorm,
	eScenSave,
	eScenSand,
	eScenSandSave,
	eScenReplay,
	eScenCamp,
//  davide multi save
	eScenMultiSave,
//---------------------
};

enum ETactType
{
	eTacReserve,
	eTacVP,
	eTacQuad,
	eTacQuadVP, // we're assigned a quad, but we're going for a vp now
	eTacQuadLoc,
	eTacLoc,
	eTacHold,
	eTacMax,
};

enum EOrderState
{
	eOrdersMove,
	eOrdersStay,
	eOrdersFighting,
	eOrderStateMax,
};

enum ECommands
{
	// Units
	eComRun,	//0
	eComFire,
	eComRetreat,
	eComWLeft,
	eComWRight,
	eComHalt,		 //5
	eComForm,
	eComAdvance,
	eComFallback,
	eComUseRoad,
	eComCharge,			//10
	eComRoute,
	eComAboutFace,
	eComRally,
	eComCasualty,
	eComOrders,			//15
	eComFightFormSet,
	eComAttach,
	eComCourier,
	eComTactics,
	eComStop,	// not used			 //20
	eComDoneQuad,
	eComDoneVP,
	eComDoneLoc,
	eComHelp,
	eComTComm,					//25
	eComSetAmmo,
	eComHighGround,
	eComArtyFHold,
	eComArtyFTroops,
//---davide-----------------------
	eComArtyFCav,
//---------------------------------
	eComArtyFArty,		 //30
	eComArtyFAI,
	eComLimber,
	eComShowUnit,
	eComHideUnit,
	eComPlyrDest,
	eComSwitchCmn,
	eComResupply,
	eComDetach,
	eComForceMove,
	eComMove,
	eComMoveDir,
	eComMovePath,
	eComMoveRoad,
	eComGoTo,
	eComRetreatTo,
	eComObjActivate,
	eComAddLog,
	eComDelEvt,
	eComTCommOn,
	eComTCommOff,
	eComKillOff,
	eComWeather,
	eComEndDir,
	eComOrderDest,
	eComCover,
	eComFollow,
	eComScout,
	eComScreen,
	eComGuard,
	eComRaid,
	eComClearPath,
	eComAddPath,
	eComRFlank,
	eComLFlank,
	eComRObliq,
	eComLObliq,
	eComAltStart,
	eComSplitBrig,

	//Misc
	eComLoadLevel,
	eComCloseLevel,
	eComLoadSpec,
	eComLoadGame,
	eComSaveGame,
	eComQuickLoad,
	eComLoadScreen,
	eComPopScreen,
	eComCloseScreen,
	eComExit,
	eComSetVars,
	eComBegRandom,
	eComBLANKSTUFF1,
	eComLoadMainGame,
	eComLoadScen,
	eComDelSave,
	eComHighScore,
	eComGatherHigh,
	eComPlayMP3,
	eComMyCour,
	eComSetLeads,
	eComOptions,
	eComEndScen,
	eComRanEvt,
	eComStopMP3,
	eComFollowNone,
	eComGetAway,
	eComResign,
	eComLoadOptions,
	eComDefOptions,
	eComCloseNoMusic,
	eComGatherCarry,
	eComWalk,
	eComForcePath,
	eComSetVar,
	eComLobbyFree,
	eComLobbyLogon,
	eComNOTUSED,				 // free spot
	eComNOTUSED2,
	eComLobbySendRoom,
	eComLobbyEnterRoom,
	eComLobbyLeaveRoom,
	eComLobbyCreateRoom,
	eComLobbyLaunch,
	eComMultiLoad,
	eComMultiLaunch,
	eComMultiFree,
	eComMultiSendChat,
	eComMultiSetOff,
	eComMultiSetScen,
	eComMultiHost,
	eComMultiJoin,
	eComGroup,
	eComLock,
	eComUnlock,
	eComTarget,
	eComSunrise,
	eComSunset,
	eComNight,

	// new stuff
	eComJoinBrig,
	eComGoToSel,
	eComAltForm,
	eComMoveModUp,
	eComMoveModDn,
	eComSaveMods,
	eComMPSandBox,
	eComTCAllOn,
	eComTCAllOff,
	eComTargetSel,
	eComGuardSel,
	eComJoinBrigSel,

	// new split commands
	eComOnlyLimber, 
	eComOnlyUnlimber, 
	eComOnlyMount, 
	eComOnlyDismount, 
	eComOnlyLayDown, 
	eComOnlyRiseUp, 

	// keyboard commands
	eComMoveForward,
	eComMoveBackward,
	eComMoveUp,
	eComMoveDown,
	eComRotateRight,
	eComRotateLeft,
	eComRankUp,
	eComRankDown,
	eComRankNext,
	eComRankPrev,
	eComToggleTrees,
	eComPause,
	eComDumpUnused,
	eComZoom,
	eComHUD,
	eComDumpLocs,
	eComDumpDead,
	eComSpeedUp,
	eComSpeedDown,
	eComCamFollow,
	eComChat,
	eComGoToUnit,
	eComPopCourMsg,
	eComClearCourMsg,
	eComSendCourMsg,
	eComMultiCourMsg, // Multiplayer only, means that a cour msg should be on the stack and should be run
	eComMoveSpec,
	eComWheelSpec,
	eComWheelDir,
	eComMoveLeft,
	eComMoveRight,
	eComStopSfx,
	eComMoveFwd,
	eComMoveFormFwd,
	eComMarchMusic,
	eComFillMPServer,
	eComMapPort,
	eComAttackMarch,
	eComJustRun,
	eComAddPoints,
	eComDeactivate,
	eComLobbyEnterPass,

	eComDumpStats,
	eComDumpScen,
	eComSetFOV,
	eComDumpOOBScen,
	eComUNUSED,
	eComLobbyRefresh,
	// ECF Patch
	eComImmortal,
	eComMortal,
	eComTCOfficers,
	eComCourDoc,
	eComQuickSel,
	eComAddCourTxt,
	eComSurrender,

	eComTacOrders,
	eComTacReserve,
	eComTacVP,
	eComTacQuad,
	eComTacQuadVP,
	eComTacQuadLoc,
	eComTacLoc,
	eComTacHold,
	eComTeamChat,
	eComMultiKick,
	eComToggleFlags,

	eComLoadReplay,
	eComDelReplay,
	eComSaveReplay,
	eComReplayPlay,
	eComReplayFrame,

	eComUnitsOnly,
	eComWheelDest,
	eComSetCourVars,
	eComSplit,
	eComUnSplit,
	eComState,

	eComLoadLayout,
	eComVolleyOn,
	eComVolleyOff,
	eComPopLayout,
	eComMsgBox,
	eComWLeftFwd,
	eComWLeftBck,
	eComWRightFwd,
	eComWRightBck,

	eComBegCamp,
	eComLoadCamp,
	eComSaveCamp,

	eComFormType,
	eComNewSel,

	eComFlushCQ,
	eComAddCQ,
	eComSendCQ,
	eComDelCQ,
	eComFlushVars,
	
	eComSBCDetach,
	eComSBCAttach,
	eComSBCPause,
	eComSBCPlay,
	eComSBCFastPlay,
	eComSBCSave,
	eComSBCBattle,
	eComSBCRetreat,
	eComSBCSetDest,
	eComSBCFight,

	eComLoadMenu,
	eComLoadBar,
	eComCloseBar,
	eComLoadCour,

	eComDelCamp,
	eComWinBatt,
	eComShowMenu,
	eComParseCmd,
	eComLoadBarCour,
	eComSBCTrain,

//----davide---------------------
	eComPlayAtLoc,
//-----------------------------

	eComBench,

	eComMax,
};


enum UTYPE_SHRT
{
	UVAL_InfWeight,
	UVAL_CavWeight,
	UVAL_ArtWeight,
	UVAL_AutoRtePct,
	UVAL_ReacquireTics,
	UVAL_MarchTargYds,
	UVAL_CanCharge,
	UVAL_CanChargeRet,
	UVAL_MeleeIndex,
	UVAL_MeleeMntIndex,
	UVAL_FatIndex,
	UVAL_FatMntLimIndex,
	UVAL_MntStopDefTerr,
	UVAL_BaseVolleyYds,
	eUTSMAX,
};

enum EArtyAmmo
{
	eAACan,
	eAAShell,
	eAAShrap,
	eAASolid,
	eAAMAX,
};

enum EMountState
{
	eMountUnmounted,
	eMountMounted,
};

enum EUnitType
{
	eUnitAny,  // any - 0
	eUnitInf,  // infantry - 1
	eUnitCav,  // cavarlery - 2
	eUnitArt,  // artillery - 3
	eUnitAmmo, // ammo wagon - 4
	eUnitCour, // courier - 5
	eUnitTypeMax,
};

enum ELimberState
{
	eLimLimbered,
	eLimLimbering,
	eLimUnLimbered,
	eLimUnLimbering,
};

enum EAskOfficer
{
	eSTStandNoTarget,
	eSTStandNoTargetFire,
	eSTStandShooting,
	eSTStandTargetOnlyMelee,
	eSTStandTargetNoAmmo,
	eSTStandCavalryDanger,    //new square
	eSTStandCavalryDangerEnd,    //new square
	eSTMarchCavalryDanger,    //new square
	eSTStandTarget,      //new cavalry  //new infantry
	eSTAttackTarget,    //new cavalry	
	eSTStandNoWheel,   //new infantry
	eSTMeleeTargetSquare,    //new cavalry	
	eSTChargeTargetSquare,    //new cavalry
	eSTMarchTargetOnlyMelee, //new cavalry // new infantry
	eSTMarchNoTarget,    // new cavalry //new infantry
	eSTStandFortTarget,  //new fort
	eSTStandFortMorale,  //new fort
	eSTStandFortNoTarget,  //new fort
	eSTStandEnterFort,  //new fort
	eSTStandSkirmishTarget,  //new skirmishers
	eSTStandSkirmishNoTarget,  //new skirmishers
	eSTStandArtilleryFallBack,  //new artillery
};

enum EFireMode
{
	eFAI,
	eFTroops,
	eFArty,
	eFConserve,
//------------davide---------
	eFCav,
//--------------------------
};

enum EUnitState
{
	eUnitMarch,
	eUnitStand,
	eUnitLoad,
	eUnitRam,
	eUnitShoot,
	eUnitRun,
	eUnitCharge,
	eUnitMelee,
	eUnitTakeCover,
	eStateMax,
	eUnitFallback = eStateMax, // All before this have sprites
	eUnitRetreat,
	eUnitAdvance,
	eUnitRouted,
	eUnitStateMax,
};

enum EGame
{
	eGameFallbackPct,
	eGameRetreatPct,
	eGameAccuracy,
	eGameReload,
	eGameMelee,
	eGameMoraleRadius,
	eGameMoraleBonus,
	eGameMoraleSec,
	eGameMoralePts,
	eGameWheelDist,
	eGameStanceMod,
	eGameRallyDist,
	eGameRunForAmmo,
	eGameRunForRally,
	eGameAmmoFromDead,
	eGameMaxAmmoGiven,
	eGameUnitBonusRadius,
	eGameOffChooseVP,
	eGameRegMaxMoraleMod,
	eGameFireTime,
	eGameFirstAid,
	eGameLimber,
	eGameCoverStand,
	eGameStabTime,
	eGameFatigue,
	eGameCasMorMod,
	eGameCasMorRadMod,
	eGameFatRun,
	eGameArtyConsv,
	eGameArtyPanic,
	eGameMarchUnderFire,
	eGameVolleyYds,
	eGameChargeRngYds,
	eGameMax,
};

enum EQuad
{
	eQuadNorth,
	eQuadEast,
	eQuadSouth,
	eQuadWest,
	eQuadMax,
};

enum ERank
{
	eRankSide,
	eRankArmy,
	eRankCorp,
	eRankDiv,
	eRankBrig,
	eRankReg,
	eRankMAX,
};

enum EFollow
{
	eFollowNone,
	eFollowGuard,
	eFollowScout,
	eFollowScreen,
	eFollowRaid,
	eFollowMAX,
};

enum EPts
{
	ePtsWeather,
	ePtsInitiative,
	ePtsLeadership,
	ePtsLoyalty,
	ePtsAbility,
	ePtsStyle,
	ePtsLevel,
	ePtsFatigue,
	ePtsMorale,
	ePtsMax,
//----davide addition to new call--the skills go from 9 to
	ePtsCloseOrder = ePtsMax,
	ePtsOpenOrder,
	ePtsEdgedWeapon,
	ePtsFireArms,
	ePtsMarksmanship,
	ePtsHorsemanship,
	ePtsSurgeon,
	ePtsCalisthenics,
//-----------------------------------------
};

enum EDifficulty
{
	eDiffGreen,
	eDiffTrain,
	eDiffNorm,
	eDiffComm,
	eDiffLead,
	eDiffHist,
	eDiffGrog,
	eDiffCust,
	eDiffMAX,
};

enum EFormType
{
	eFTFight,
	eFTMarch,
	eFTSquare,
	eFTAssault,
	eFTSkirmish,
	eFTLine,
	eFTAltSkirmish,
	eFTColumnHalf,
	eFTColumnFull,
	eFTSpecial,
	eFTMIN,
};

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

