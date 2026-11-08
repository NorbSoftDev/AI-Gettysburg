//*************************************************************************************************************
//*************************************************************************************************************
// File: xtables.inl
//*************************************************************************************************************
//*************************************************************************************************************

//----WARNING TO THE TABLES MODIFICATION---------------------------------------------------------------------------
//   YOU CAN ADD NEW ROWS AND NEW COLUMNS BUT ADD ALWAYS NEW COLUMNS VALUE AT THE END OF TABLE NEVER IN THE MIDDLE
//   TO ADD THEM IN THE MIDDLE CAN GENERATE ANOMALIES
//-------------------------------------------------------------------------------------------------------
//-----STRATEGY TABLE  WARNING----------------------
//  CAVALRY AND ARTILLERY CANNOT USE THE SPOTS OF INFANTRY---
//----------------------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
// Table: gDivStrat
//*************************************************************************************************************
//*************************************************************************************************************

const SDivStrategy gDivStrat[] = 
{
	{
		// Linear Warfare Defence #1
		100,
		{
			{ eSpotMid    ,"DEFEND",  eSpotMid    ,  "DEFEND",eSpotCALine  ,"DEFEND",eSpotCALine  ,"DEFEND",eSpotCAMid,"DEFEND" ,eSpotCAMid,"DEFEND"  },
			{ eSpotFarMid ,"DEFEND",  eSpotFarMid ,  "DEFEND",eSpotCAMid   ,"DEFEND",eSpotCAMid   ,"DEFEND",eSpotHigh ,"HOLD",   eSpotHigh ,"HOLD" },
			{ eSpotBotMid ,"HOLD"  ,  eSpotBotMid   ,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBot    ,"HOLD"  ,  eSpotBot      ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,  eSpotBackMid  ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD"  ,eSpotHigh ,"HOLD"   },
		}
	},{
		// British High Ground Defense
		200,
		{
			{ eSpotMid    ,"HOLD",  eSpotMid    ,"HOLD"  ,eSpotCABackMid,"DEFEND",eSpotCABackMid,"DEFEND",eSpotCAMid   ,"DEFEND",eSpotCAMid   ,"DEFEND"  },
			{ eSpotFarMid ,"HOLD",  eSpotFarMid ,"HOLD"  ,eSpotCABack   ,"DEFEND",eSpotCABack   ,"DEFEND",eSpotCALine  ,"DEFEND", eSpotCALine ,"DEFEND" },
			{ eSpotBotMid ,"HOLD",  eSpotBotMid ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotCAFarMid,"DEFEND",eSpotCAFarMid,"DEFEND" },
			{ eSpotBot    ,"DEFEND",eSpotBot    ,"DEFEND",eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotHigh    ,"HOLD"  , eSpotHigh   ,"HOLD" },
			{ eSpotBackMid,"DEFEND",eSpotBackMid,"DEFEND",eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotHigh    ,"HOLD"  ,eSpotHigh    ,"HOLD"   },
		}
	},{
		// Linear Warfare Defence #2
		300,
		{
			{ eSpotMid    ,"DEFEND",  eSpotMid      ,"DEFEND",eSpotCALine  ,"DEFEND",eSpotCALine  ,"DEFEND",eSpotCAMid ,"DEFEND",eSpotCAMid,"DEFEND"  },
			{ eSpotFarMid ,"DEFEND",  eSpotFarMid   ,"DEFEND",eSpotCAMid   ,"DEFEND",eSpotCAMid   ,"DEFEND",eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBotMid ,"HOLD"  ,  eSpotBotMid   ,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBot    ,"HOLD"  ,  eSpotBot      ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,  eSpotBackMid  ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD"  ,eSpotHigh ,"HOLD"   },
		}
	},{
		// British High Ground Defense advanced flanks
		400,
		{
			{ eSpotMid    ,"HOLD",  eSpotMid    ,"HOLD"  ,eSpotCABackMid,"DEFEND",eSpotCABackMid,"DEFEND",eSpotCAMid	,"DEFEND" ,eSpotCAMid   ,"DEFEND"  },
			{ eSpotFarMid ,"DEFEND",eSpotFarMid ,"DEFEND",eSpotCABack   ,"DEFEND",eSpotCABack   ,"DEFEND",eSpotCALine	,"DEFEND" ,eSpotCALine  ,"DEFEND" },
			{ eSpotBotMid ,"HOLD",  eSpotBotMid ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotCAFarMid ,"DEFEND" ,eSpotCAFarMid,"DEFEND" },
			{ eSpotBot    ,"DEFEND",eSpotBot    ,"DEFEND",eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotHigh     ,"HOLD"   ,eSpotHigh    ,"HOLD" },
			{ eSpotBackMid,"DEFEND",eSpotBackMid,"DEFEND",eSpotGuard    ,"HOLD"  ,eSpotGuard    ,"HOLD"  ,eSpotHigh     ,"HOLD"   ,eSpotHigh	,"HOLD"   },
		}
	},{
		// Combined arms defense #1
		500,
		{
			{ eSpotMid,    "DEFEND"  ,eSpotMid,    "PROBE",   eSpotCALine,   "PROBE",   eSpotCALine,    "DEFEND"  ,eSpotCALine   ,"DEFEND" , eSpotCALine,"PROBE"  },
			{ eSpotFarMid, "PROBE",   eSpotFarMid, "DEFEND"  ,eSpotCAMid,    "DEFEND"   ,eSpotCAMid,    "PROBE"   ,eSpotCAMid    ,"PROBE",	 eSpotCAMid ,"DEFEND" },
			{ eSpotBotMid, "DEFEND"  ,eSpotBotMid, "DEFEND"  ,eSpotCAFarMid ,"DEFEND"   ,eSpotCAFarMid ,"DEFEND"  ,eSpotCAFarMid ,"DEFEND",  eSpotHigh  ,"DEFEND"},
			{ eSpotBot   , "DEFEND"  ,eSpotBot   , "DEFEND"  ,eSpotCABackMid,"DEFEND"   ,eSpotCABackMid,"DEFEND"  ,eSpotCABackMid,"DEFEND",  eSpotBack  ,"DEFEND" },
			{ eSpotBackMid,"DEFEND"  ,eSpotBackMid,"DEFEND"  ,eSpotCABack,   "DEFEND"   ,eSpotCABack,   "DEFEND"  ,eSpotCABack   ,"DEFEND"  ,eSpotFarMid,"DEFEND"   },
		}
	},{
		// Combined arms defense #2   
		600,
		{
			{ eSpotMid,    "PROBE"  ,eSpotMid,    "DEFEND",   eSpotCALine,   "DEFEND",   eSpotCALine,   "PROBE" ,  eSpotCALine   ,"PROBE" ,  eSpotCALine,"DEFEND"  },
			{ eSpotFarMid, "DEFEND",   eSpotFarMid, "PROBE"  ,eSpotCAMid,    "PROBE"    ,eSpotCAMid,    "DEFEND"  ,eSpotCAMid    ,"DEFEND",  eSpotCAMid ,"PROBE" },
			{ eSpotBotMid, "DEFEND"  ,eSpotBotMid, "DEFEND"  ,eSpotCAFarMid ,"DEFEND"   ,eSpotCAFarMid ,"DEFEND"  ,eSpotCAFarMid ,"DEFEND",  eSpotHigh  ,"DEFEND"},
			{ eSpotBot   , "DEFEND"  ,eSpotBot   , "DEFEND"  ,eSpotCABackMid,"DEFEND"   ,eSpotCABackMid,"DEFEND"  ,eSpotCABackMid,"DEFEND",  eSpotBack  ,"DEFEND" },
			{ eSpotBackMid,"DEFEND"  ,eSpotBackMid,"DEFEND"  ,eSpotCABack,   "DEFEND"   ,eSpotCABack,   "DEFEND"  ,eSpotCABack   ,"DEFEND"  ,eSpotFarMid,"DEFEND"   },
		}
	},{
		//  Refuse the Right flank Defence #1
		700,
		{
			{ eSpotMid,     "DEFEND",eSpotBot,    "DEFEND"  ,eSpotCAMid   ,"DEFEND",eSpotCABack   ,"HOLD",eSpotCAMid,	"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotFarMid  ,"DEFEND",eSpotBack   ,"HOLD"    ,eSpotCAFarMid,"DEFEND",eSpotCABackMid,"HOLD",eSpotCAFarMid,"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBotMid,  "HOLD",  eSpotBotMid, "HOLD",   eSpotCABack   ,"HOLD",  eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBot,  	"HOLD",  eSpotBackMid,"HOLD"    ,eSpotGuard   ,"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid, "HOLD",  eSpotMid   , "DEFEND"  ,eSpotGuard   ,"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD"   },
		}
	},{
		//  Refuse the Left flank Defence #1
		800,
		{
			{ eSpotBot,    "DEFEND"  ,eSpotMid,     "DEFEND",eSpotCABack   ,"HOLD",eSpotCAMid    ,"DEFEND",eSpotHigh,"HOLD",eSpotCAMid,		"DEFEND"  },
			{ eSpotBack   ,"HOLD"  ,  eSpotFarMid  ,"DEFEND",eSpotCABackMid,"HOLD",eSpotCAFarMid ,"DEFEND",eSpotHigh,"HOLD",eSpotCAFarMid,	"DEFEND" },
			{ eSpotBotMid, "HOLD",    eSpotBotMid,  "HOLD",	 eSpotGuard    ,"HOLD",eSpotCABack   ,"HOLD",  eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotBackMid,"HOLD"  ,  eSpotBot,	    "HOLD",	 eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",  eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotMid   , "DEFEND"  ,eSpotBackMid, "HOLD",  eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",  eSpotHigh,"HOLD",eSpotHigh,		"HOLD"   },
		}
	},{
		// Refuse the Right flank Defence #2
		900,
		{
			{ eSpotMid,     "DEFEND",	eSpotBot,     "HOLD"  ,eSpotCAMid    ,"DEFEND", eSpotCABack   ,"HOLD",eSpotCAMid,	"DEFEND" ,eSpotHigh,"HOLD"  },
			{ eSpotFarMid  ,"DEFEND",	eSpotBack   , "HOLD"  ,eSpotCAFarMid ,"DEFEND", eSpotCABackMid,"HOLD",eSpotCAFarMid,"DEFEND", eSpotHigh,"HOLD" },
			{ eSpotBotMid,	"HOLD",		eSpotBotMid,  "HOLD",  eSpotCABack   ,"HOLD",   eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",   eSpotHigh,"HOLD" },
			{ eSpotBot,		"HOLD",		eSpotBackMid, "HOLD"  ,eSpotGuard    ,"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",	  eSpotHigh,"HOLD" },
			{ eSpotBackMid, "HOLD",		eSpotMid   ,  "HOLD"  ,eSpotGuard    ,"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",	  eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		//  Refuse the Left flank Defence #2																										
		1000,																																					
		{																																							
			{ eSpotBot,    "HOLD"  ,eSpotMid,     "DEFEND",	eSpotCABack   ,"HOLD",eSpotCAMid    ,"DEFEND",	eSpotHigh,"HOLD",eSpotCAMid,	"DEFEND"  },
			{ eSpotBack   ,"HOLD"  ,eSpotFarMid  ,"DEFEND",	eSpotCABackMid,"HOLD",eSpotCAFarMid ,"DEFEND",	eSpotHigh,"HOLD",eSpotCAFarMid,	"DEFEND" },
			{ eSpotBotMid, "HOLD" , eSpotBotMid,  "HOLD",	eSpotGuard    ,"HOLD",eSpotCABack   ,"HOLD",	eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotBackMid,"HOLD"  ,eSpotBot,	  "HOLD",	eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",	eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotMid   , "HOLD"  ,eSpotBackMid, "HOLD",	eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",	eSpotHigh,"HOLD",eSpotHigh,		"HOLD"   },
		}																																							
	},{																																							
		// Rear Guard Action #1																													
		1100,																																					
		{																																							
			{ eSpotBackMid,"HOLD",eSpotBackMid,"HOLD",eSpotCAFarMid, "DEFEND",eSpotCAFarMid, "DEFEND",eSpotCABackMid,"HOLD" ,eSpotCABackMid,"HOLD" },
			{ eSpotBack   ,"HOLD",eSpotBack   ,"HOLD",eSpotCAMid,    "DEFEND",eSpotCAMid,    "DEFEND",eSpotCABack   ,"HOLD" ,eSpotCABack   ,"HOLD" },
			{ eSpotBot    ,"HOLD",eSpotBot    ,"HOLD",eSpotCALine,   "DEFEND",eSpotCALine,   "DEFEND",eSpotHigh     ,"HOLD" ,eSpotHigh     ,"HOLD" },
			{ eSpotFarMid ,"HOLD",eSpotFarMid ,"HOLD",eSpotCABack,   "HOLD"  ,eSpotCABack,   "HOLD"  ,eSpotHigh     ,"HOLD" ,eSpotHigh     ,"HOLD" },
			{ eSpotBotMid ,"HOLD",eSpotBotMid ,"HOLD",eSpotCABackMid,"HOLD"  ,eSpotCABackMid,"HOLD"  ,eSpotHigh     ,"HOLD" ,eSpotHigh     ,"HOLD" },
		}																																							
	},{																																							
		// Rear Guard Action #2																												
		1200,																																					
		{																																							
			{ eSpotBackMid,"HOLD",	eSpotBackMid,"HOLD",	eSpotCAFarMid, "DEFEND",eSpotCAFarMid, "DEFEND",eSpotCABackMid, "HOLD" ,	eSpotCABackMid,"HOLD" },
			{ eSpotBack   ,"DEFEND",eSpotBack   ,"DEFEND",	eSpotCAMid,    "DEFEND",eSpotCAMid,    "DEFEND",eSpotCABack,	"DEFEND",	eSpotCABack   ,"DEFEND" },
			{ eSpotBot    ,"HOLD",	eSpotBot    ,"HOLD",	eSpotCALine,   "DEFEND",eSpotCALine,   "DEFEND",eSpotHigh     ,	"HOLD" ,	eSpotHigh     ,"HOLD" },
			{ eSpotFarMid ,"HOLD",	eSpotFarMid ,"HOLD",	eSpotCABack,   "HOLD"  ,eSpotCABack,   "HOLD"  ,eSpotHigh     ,	"HOLD" ,	eSpotHigh     ,"HOLD" },
			{ eSpotBotMid ,"HOLD",	eSpotBotMid ,"HOLD",	eSpotCABackMid,"HOLD"  ,eSpotCABackMid,"HOLD"  ,eSpotHigh     ,	"HOLD" ,	eSpotHigh     ,"HOLD" },
		}																																							
	},{																																							
		// Defensive Rotation left around central Pivot																															
		1300,																																					
		{																																							
			{ eSpotTop,		"HOLD",	eSpotBot,		"DEFEND"  ,	eSpotCALine  ,	"HOLD", eSpotCABack   ,"HOLD",eSpotCAMid,	"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotMid  ,	"HOLD",	eSpotBack   ,	"HOLD"  ,	eSpotCAMid,		"HOLD", eSpotCABackMid,"HOLD",eSpotCAFarMid,"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBot,		"HOLD", eSpotBotMid,	"HOLD",		eSpotCAFarMid  ,"HOLD", eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBack,	"HOLD", eSpotBackMid,	"HOLD"  ,	eSpotGuard    ,	"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid, "HOLD", eSpotMid   ,	"DEFEND"  ,	eSpotGuard    ,	"HOLD",	eSpotGuard    ,"HOLD",eSpotHigh,	"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Defensive Rotation right around central Pivot																															
		1400,																																					
		{																																							
			{ eSpotBot,		"DEFEND"  ,	 eSpotTop,	    "HOLD", eSpotCABack   ,"HOLD", eSpotCALine,		"HOLD", eSpotCAMid,		"DEFEND",eSpotHigh,		"HOLD"  },
			{ eSpotBack,	"HOLD"  ,	 eSpotMid  ,	"HOLD", eSpotCABackMid,"HOLD", eSpotCAMid,		"HOLD", eSpotCAFarMid,	"DEFEND",eSpotHigh,		"HOLD" },
			{ eSpotBotMid,	"HOLD",		 eSpotBot,		"HOLD", eSpotGuard    ,"HOLD", eSpotCAFarMid,	"HOLD", eSpotHigh,		"HOLD",  eSpotHigh,		"HOLD" },
			{ eSpotBackMid, "HOLD"  ,	 eSpotBack,		"HOLD", eSpotGuard    ,"HOLD", eSpotGuard    ,	"HOLD", eSpotHigh,		"HOLD",  eSpotHigh,		"HOLD" },
			{ eSpotMid   ,	"DEFEND"  ,	 eSpotBackMid,  "HOLD", eSpotGuard    ,"HOLD", eSpotGuard    ,	"HOLD", eSpotHigh,		"HOLD",  eSpotHigh,		"HOLD"   },
		}																																							
	},{																																							
		// Prussian Brigade Defence																															
		1500,																																					
		{																																							
			{ eSpotBotMid ,	 "DEFEND" ,	eSpotBotMid ,	"DEFEND",	eSpotCAFarMid ,	"HOLD" ,	eSpotCAFarMid,	"HOLD",	eSpotCAFarMid,	"DEFEND" ,	eSpotCAFarMid,	"DEFEND" },
			{ eSpotBackMid,	 "HOLD" ,	eSpotBackMid,	"HOLD",		eSpotCABack ,	"HOLD" ,	eSpotCABack,	"HOLD" ,eSpotCAMid ,	"DEFEND",	eSpotCAMid ,	"DEFEND" },
			{ eSpotFarMid,	 "HOLD",	eSpotFarMid,	"HOLD",		eSpotGuard,		"HOLD" ,	eSpotGuard,		"HOLD" ,eSpotHigh ,		"HOLD",		eSpotHigh ,		"HOLD" },
			{ eSpotBot ,	 "DEFEND",	eSpotBot ,		"DEFEND",	eSpotGuard ,	"HOLD",		eSpotGuard ,	"HOLD",	eSpotHigh ,		"HOLD",		eSpotHigh ,		"HOLD" },
			{ eSpotBack ,	 "HOLD" ,	eSpotBack ,		"HOLD",		eSpotGuard ,	"HOLD" ,	eSpotGuard ,	"HOLD",	eSpotHigh,		"HOLD" ,	eSpotHigh,		"HOLD" },
		}																																							
	},{																																							
		// Combined arms Defense #3																														
		1600,																																					
		{																																							
			{ eSpotMid,    "PROBE"  ,	eSpotMid,    "DEFEND",		eSpotCALine,   "DEFEND",   eSpotCALine,	  "PROBE" ,  eSpotCALine   ,"PROBE" ,   eSpotCALine,"DEFEND"  },
			{ eSpotFarMid, "DEFEND",    eSpotFarMid, "PROBE"  ,		eSpotCAMid,    "PROBE"   , eSpotCAMid,    "DEFEND" , eSpotCAMid    ,"DEFEND",	eSpotCAMid ,"PROBE" },
			{ eSpotBotMid, "PROBE"  ,	eSpotBotMid, "PROBE"  ,		eSpotCAFarMid ,"DEFEND"   ,eSpotCAFarMid ,"DEFEND"  ,eSpotCAFarMid ,"DEFEND",   eSpotHigh  ,"DEFEND"},
			{ eSpotBot   , "DEFEND"  ,	eSpotBot   , "DEFEND"  ,	eSpotCABackMid,"DEFEND"   ,eSpotCABackMid,"DEFEND"  ,eSpotCABackMid,"DEFEND",   eSpotBack  ,"DEFEND" },
			{ eSpotBackMid,"DEFEND"  ,	eSpotBackMid,"DEFEND"  ,	eSpotCABack,   "DEFEND"   ,eSpotCABack,   "DEFEND"  ,eSpotCABack   ,"DEFEND"  , eSpotFarMid,"DEFEND"   },
		}																																							
	},{																																							
		// Defence at V shape																												
		1700,																																					
		{																																							
			{ eSpotBackMid,"HOLD",	    eSpotBackMid,"HOLD"  ,eSpotCAFarMid ,	"DEFEND",	eSpotCAFarMid ,	"DEFEND",eSpotCAFarMid,	"DEFEND" ,	eSpotCAFarMid,	"DEFEND"  },
			{ eSpotFarMid, "DEFEND",	eSpotFarMid, "DEFEND",eSpotCABack ,		"DEFEND",	eSpotCABack ,	"DEFEND",eSpotHigh ,	"DEFEND",	eSpotHigh ,		"DEFEND" },
			{ eSpotBot   , "DEFEND",	eSpotBot   , "DEFEND",eSpotGuard ,		"DEFEND",	eSpotGuard ,	"DEFEND",eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotBack,   "DEFEND",	eSpotBack,   "DEFEND",eSpotGuard ,		"DEFEND"  , eSpotGuard ,	"DEFEND",eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotMid   , "HOLD"     , eSpotMid   , "HOLD"  ,eSpotGuard ,		"DEFEND"   ,eSpotGuard ,	"DEFEND",eSpotHigh,		"DEFEND"  ,	eSpotHigh,		"DEFEND"   },
		}																																							
	},{																																							
		//  Refuse the flanks Defence																											
		1800,																																					
		{																																							
			{ eSpotBot,    "DEFEND"  ,	eSpotBot,    "DEFEND",	eSpotCABack   ,"HOLD",	eSpotCABack   ,"HOLD",	eSpotHigh,"DEFEND",eSpotHigh,"DEFEND"  },
			{ eSpotBack   ,"HOLD"  ,	eSpotBack   ,"HOLD",	eSpotCABackMid,"HOLD",	eSpotCABackMid,"HOLD",	eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotBotMid, "HOLD",		eSpotBotMid, "HOLD",	eSpotCALine   ,"DEFEND",eSpotCALine   ,"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotBackMid,"HOLD"  ,	eSpotBackMid,"HOLD",	eSpotGuard    ,"HOLD",	eSpotGuard    ,"HOLD",	eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotMid   , "DEFEND"  ,	eSpotMid   , "DEFEND",	eSpotGuard    ,"HOLD",	eSpotGuard    ,"HOLD",	eSpotHigh,"DEFEND",eSpotHigh,"DEFEND"   },
		}																																							
	},{																																							
		// Linear Warfare Hold #1																														
		1900,																																					
		{																																							
			{ eSpotMid    ,"HOLD",		 eSpotMid    ,  "HOLD",	eSpotCALine  ,"HOLD",	eSpotCALine  ,"HOLD",	eSpotCAMid,	"HOLD" ,	eSpotCAMid,	"HOLD"  },
			{ eSpotFarMid ,"HOLD",		 eSpotFarMid ,  "HOLD",	eSpotCAMid   ,"HOLD",	eSpotCAMid   ,"HOLD",	eSpotHigh ,	"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBotMid ,"HOLD"  ,	 eSpotBotMid   ,"HOLD" ,eSpotCAFarMid,"HOLD"  ,	eSpotCAFarMid,"HOLD"  ,	eSpotHigh  ,"HOLD",		eSpotHigh  ,"HOLD" },
			{ eSpotBot    ,"HOLD"  ,	 eSpotBot      ,"HOLD" ,eSpotGuard   ,"HOLD"  ,	eSpotGuard   ,"HOLD"  ,	eSpotHigh  ,"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBackMid,"HOLD"  ,	 eSpotBackMid  ,"HOLD" ,eSpotGuard   ,"HOLD"  ,	eSpotGuard   ,"HOLD"  ,	eSpotHigh  ,"HOLD"  ,	eSpotHigh,	"HOLD"   },
		}																																							
	},{																																							
		// Refuse the flanks Hold																																
		2000,																																					
		{																																							
			{ eSpotBot,    "HOLD"  ,eSpotBot,    "HOLD",eSpotCABack   ,"HOLD",eSpotCABack   ,"HOLD",eSpotHigh,"HOLD",eSpotHigh,"HOLD"  },
			{ eSpotBack   ,"HOLD"  ,eSpotBack   ,"HOLD",eSpotCABackMid,"HOLD",eSpotCABackMid,"HOLD",eSpotHigh,"HOLD",eSpotHigh,"HOLD" },
			{ eSpotBotMid, "HOLD",  eSpotBotMid, "HOLD",eSpotCALine   ,"HOLD",eSpotCALine   ,"HOLD",eSpotHigh,"HOLD",eSpotHigh,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,eSpotBackMid,"HOLD",eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",eSpotHigh,"HOLD",eSpotHigh,"HOLD" },
			{ eSpotMid   , "HOLD"  ,eSpotMid   , "HOLD",eSpotGuard    ,"HOLD",eSpotGuard    ,"HOLD",eSpotHigh,"HOLD",eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Linear Warfare Hold #2																															
		2100,																																					
		{																																							
			{ eSpotMid    ,"HOLD",	 eSpotMid    ,  "HOLD",	eSpotCALine  ,"HOLD",	eSpotCALine  ,"HOLD",	eSpotCAMid,	"HOLD" ,	eSpotCAMid,	"HOLD"  },
			{ eSpotFarMid ,"HOLD",	 eSpotFarMid ,  "HOLD",	eSpotCAMid   ,"HOLD",	eSpotCAMid   ,"HOLD",	eSpotHigh ,	"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBotMid ,"HOLD"  , eSpotBotMid   ,"HOLD" ,eSpotCAFarMid,"HOLD"  ,	eSpotCAFarMid,"HOLD"  ,	eSpotHigh  ,"HOLD",		eSpotHigh  ,"HOLD" },
			{ eSpotBot    ,"HOLD"  , eSpotBot      ,"HOLD" ,eSpotGuard   ,"HOLD"  ,	eSpotGuard   ,"HOLD"  ,	eSpotHigh  ,"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBackMid,"HOLD"  , eSpotBackMid  ,"HOLD" ,eSpotGuard   ,"HOLD"  ,	eSpotGuard   ,"HOLD"  ,	eSpotHigh  ,"HOLD"  ,	eSpotHigh,	"HOLD"   },
		}																																							
	},{																																							
		// Prussian Brigade Hold line																															
		2200,																																					
		{																																							
			{ eSpotBotMid ,	"HOLD" ,eSpotBotMid ,	"HOLD",eSpotCAFarMid ,	"HOLD" ,eSpotCAFarMid,	"HOLD",	eSpotCAFarMid,	"HOLD" ,eSpotCAFarMid,	"HOLD"  },
			{ eSpotBackMid,	"HOLD" ,eSpotBackMid,	"HOLD",eSpotCABack   ,	"HOLD" ,eSpotCABack,	"HOLD" ,eSpotCAMid ,	"HOLD",	eSpotCAMid ,	"HOLD" },
			{ eSpotFarMid,	"HOLD", eSpotFarMid,	"HOLD",eSpotGuard,		"HOLD" ,eSpotGuard,		"HOLD" ,eSpotHigh  ,	"HOLD",	eSpotHigh  ,	"HOLD" },
			{ eSpotBot   ,	"HOLD", eSpotBot   ,	"HOLD",eSpotGuard ,		"HOLD",	eSpotGuard ,	"HOLD",	eSpotHigh  ,	"HOLD",	eSpotHigh  ,	"HOLD" },
			{ eSpotBack  ,	"HOLD" ,eSpotBack  ,	"HOLD",eSpotGuard ,		"HOLD" ,eSpotGuard ,	"HOLD",	eSpotHigh,		"HOLD" ,eSpotHigh,		"HOLD"   },
		}																																							
	},{																																							
		//  Refuse the Left flank Hold																															
		2300,																																					
		{																																							
			{ eSpotBot,    "HOLD"  ,eSpotMid,		"HOLD",eSpotCABack   ,	"HOLD",eSpotCAMid   ,	"HOLD",eSpotHigh,"HOLD",eSpotCAMid,		"HOLD"  },
			{ eSpotBack   ,"HOLD"  ,eSpotFarMid  ,	"HOLD",eSpotCABackMid,	"HOLD",eSpotCAFarMid,	"HOLD",eSpotHigh,"HOLD",eSpotCAFarMid,	"HOLD" },
			{ eSpotBotMid, "HOLD",  eSpotBotMid,	"HOLD",eSpotGuard   ,	"HOLD",eSpotCABack   ,	"HOLD",eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotBackMid,"HOLD"  ,eSpotBot,		"HOLD",eSpotGuard    ,	"HOLD",eSpotGuard    ,	"HOLD",eSpotHigh,"HOLD",eSpotHigh,		"HOLD" },
			{ eSpotMid   , "HOLD"  ,eSpotBackMid,	"HOLD",eSpotGuard    ,	"HOLD",eSpotGuard    ,	"HOLD",eSpotHigh,"HOLD",eSpotHigh,		"HOLD"   },
		}																																							
	},{																																							
		//  Refuse the Right flank Hold																																	
		2400,																																					
		{																																							
			{ eSpotMid,		"HOLD",	eSpotBot,		"HOLD"  ,eSpotCAMid   ,	"HOLD", eSpotCABack   ,	"HOLD",eSpotCAMid,	 "HOLD",eSpotHigh,"HOLD"  },
			{ eSpotFarMid ,	"HOLD",	eSpotBack   ,	"HOLD"  ,eSpotCAFarMid,	"HOLD", eSpotCABackMid,	"HOLD",eSpotCAFarMid,"HOLD",eSpotHigh,"HOLD" },
			{ eSpotBotMid,	"HOLD", eSpotBotMid,	"HOLD",  eSpotCABack   ,"HOLD", eSpotGuard	 ,	"HOLD",eSpotHigh,	 "HOLD",eSpotHigh,"HOLD" },
			{ eSpotBot,		"HOLD", eSpotBackMid,	"HOLD"  ,eSpotGuard    ,"HOLD",	eSpotGuard    ,	"HOLD",eSpotHigh,	 "HOLD",eSpotHigh,"HOLD" },
			{ eSpotBackMid, "HOLD", eSpotMid   ,	"HOLD"  ,eSpotGuard    ,"HOLD",	eSpotGuard    ,	"HOLD",eSpotHigh,	 "HOLD",eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Gran battery preparation fire																																
		2500,																																					
		{																																							
			{ eSpotMid   ,	"HOLD" ,eSpotMid   ,"HOLD" ,eSpotCALine ,	"HOLD",eSpotCALine ,	"HOLD"  ,eSpotCALine,	"BLITZ",	eSpotCALine,	"BLITZ"  },
			{ eSpotFarMid,	"HOLD" ,eSpotFarMid,"HOLD" ,eSpotCAMid ,	"HOLD",eSpotCAMid ,		"HOLD"  ,eSpotCAMid ,	"BLITZ",	eSpotCAMid ,	"BLITZ" },
			{ eSpotBotMid,	"HOLD" ,eSpotBotMid,"HOLD" ,eSpotCAFarMid,	"HOLD",eSpotCAFarMid,	"HOLD"  ,eSpotCAFarMid ,"BLITZ",	eSpotCAFarMid,	"BLITZ" },
			{ eSpotBot   ,	"HOLD" ,eSpotBot   ,"HOLD" ,eSpotGuard ,	"HOLD",eSpotGuard ,		"HOLD"	,eSpotCABack  ,	"BLITZ",	eSpotCABack,	"BLITZ" },
			{ eSpotBackMid,	"HOLD",eSpotBackMid,"HOLD" ,eSpotGuard ,	"HOLD",eSpotGuard ,		"HOLD"  ,eSpotCABackMid,"BLITZ",	eSpotCABackMid,	"BLITZ"   },
		}																																							
	},{																																							
		// Combined arms deployment hold #1																																
		2600,																																					
		{																																							
			{ eSpotMid,    "HOLD"  ,eSpotMid,    "DEFEND",eSpotCALine,   "DEFEND",eSpotCALine,   "HOLD",  eSpotCALine   ,"HOLD" , eSpotCALine,"DEFEND"  },
			{ eSpotFarMid, "DEFEND",eSpotFarMid, "HOLD"  ,eSpotCAMid,    "HOLD"  ,eSpotCAMid,    "DEFEND",eSpotCAMid    ,"DEFEND",eSpotCAMid ,"HOLD" },
			{ eSpotBotMid, "HOLD"  ,eSpotBotMid, "HOLD"  ,eSpotCAFarMid ,"HOLD"  ,eSpotCAFarMid ,"HOLD"  ,eSpotCAFarMid ,"HOLD",  eSpotHigh  ,"HOLD" },
			{ eSpotBot   , "HOLD"  ,eSpotBot   , "HOLD"  ,eSpotCABackMid,"HOLD"  ,eSpotCABackMid,"HOLD"  ,eSpotCABackMid,"HOLD",  eSpotBack  ,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,eSpotBackMid,"HOLD"  ,eSpotCABack,   "HOLD"  ,eSpotCABack,   "HOLD"  ,eSpotCABack   ,"HOLD"  ,eSpotFarMid,"HOLD"   },
		}																																							
	},{																																							
		// Combined arms deployment hold #2																																	
		2700,																																					
		{																																							
			{ eSpotMid,    "DEFEND",eSpotMid,	 "HOLD",	eSpotCALine,	"HOLD",		eSpotCALine,    "DEFEND",	eSpotCALine,	"DEFEND" ,  eSpotCALine,"HOLD"  },
			{ eSpotFarMid, "HOLD",	eSpotFarMid, "DEFEND" ,	eSpotCAMid,		"DEFEND",	eSpotCAMid,		"HOLD",		eSpotCAMid  ,	"HOLD",		eSpotCAMid ,"DEFEND" },
			{ eSpotBotMid, "HOLD"  ,eSpotBotMid, "HOLD"  ,	eSpotCAFarMid ,	"HOLD"  ,	eSpotCAFarMid ,	"HOLD"  ,	eSpotCAFarMid ,	"HOLD",		eSpotHigh  ,"HOLD" },
			{ eSpotBot   , "HOLD"  ,eSpotBot   , "HOLD"  ,	eSpotCABackMid,	"HOLD"  ,	eSpotCABackMid,	"HOLD"  ,	eSpotCABackMid,	"HOLD",		eSpotBack  ,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,eSpotBackMid,"HOLD"  ,	eSpotCABack,	"HOLD"  ,	eSpotCABack,	"HOLD"  ,	eSpotCABack   ,	"HOLD"  ,	eSpotFarMid,"HOLD"   },
		}																																							
	},{																																							
		// Large right Surround Movement probe																																
		2800,																																					
		{																																							
			{ eSpotMid,		"HOLD",		eSpotFarMid,	"ATTACK",	eSpotCAMid ,	"HOLD",     eSpotCALine,	"PROBE",	eSpotHigh,	"HOLD" ,eSpotTop,	"ATTACK"  },
			{ eSpotBotMid,	"DEFEND",	eSpotBot   ,	"PROBE",	eSpotCABackMid ,"HOLD",		eSpotCAFarMid ,	"ATTACK",	eSpotHigh,	"HOLD",	eSpotCAMid ,"ATTACK"  },
			{ eSpotBackMid,	"DEFEND",	eSpotBack,		"PROBE" ,	eSpotCAFarMid ,	"HOLD",		eSpotCABack ,	"ATTACK",	eSpotHigh,	"HOLD" ,eSpotHigh,	"DEFEND"  },
			{ eSpotFarMid,	"HOLD",		eSpotBotMid,	"PROBE" ,	eSpotCABack,	"HOLD",		eSpotGuard ,	"HOLD",		eSpotHigh,	"HOLD" ,eSpotHigh  ,"DEFEND" },
			{ eSpotBot   ,	"HOLD",		eSpotMid,		"HOLD",	    eSpotGuard ,	"HOLD",		eSpotGuard ,	"HOLD",		eSpotHigh,	"HOLD" ,eSpotHigh,	"DEFEND"},
		}																																							
	},{																																							
		//  Prussian Brigade skimishers action in hold																															
		2900,																																					
		{																																							
			{ eSpotBotMid ,"HOLD" ,	eSpotBotMid ,	"HOLD",	eSpotCAFarMid ,	"HOLD" ,	eSpotCAFarMid,	"HOLD",		eSpotCAFarMid,	"HOLD" ,	eSpotCAFarMid,	"HOLD"  },
			{ eSpotBackMid,"PROBE" ,eSpotBackMid,	"PROBE",eSpotCABack,	"DEFEND" ,	eSpotCABack,	"DEFEND" ,	eSpotCAMid ,	"HOLD",		eSpotCAMid ,	"HOLD" },
			{ eSpotFarMid, "PROBE", eSpotFarMid,	"PROBE",eSpotGuard   ,	"DEFEND" ,	eSpotGuard,		"DEFEND" ,	eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotBot   , "HOLD",	eSpotBot   ,	"HOLD",	eSpotGuard ,	"DEFEND",	eSpotGuard ,	"DEFEND",	eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotBack  , "PROBE" ,eSpotBack  ,	"PROBE",eSpotGuard ,	"DEFEND" ,	eSpotGuard ,	"DEFEND",	eSpotHigh,		"DEFEND"  ,	eSpotHigh,		"DEFEND"   },
		}																																							
	},{																																							
		// Large Left Surround Movement probe																																
		3000,																																					
		{																																							
			{ eSpotFarMid,"ATTACK",	eSpotMid,		"HOLD",		eSpotCALine,	"PROBE",	eSpotCAMid ,	"HOLD",eSpotTop,	"ATTACK" ,	eSpotHigh,"HOLD"   },
			{ eSpotBot   ,"PROBE",  eSpotBotMid,	"DEFEND",	eSpotCAFarMid ,	"ATTACK",	eSpotCABackMid ,"HOLD",eSpotCAMid ,	"ATTACK",	eSpotHigh,"HOLD"  },
			{ eSpotBack,  "PROBE" , eSpotBackMid,	"DEFEND",	eSpotCABack ,	"ATTACK",	eSpotCAFarMid ,	"HOLD",eSpotHigh,	"DEFEND",	eSpotHigh,"HOLD"  },
			{ eSpotBotMid,"PROBE" , eSpotFarMid,	"HOLD",		eSpotGuard ,	"HOLD",		eSpotCABack ,	"HOLD",eSpotHigh  ,	"DEFEND",	eSpotHigh,"HOLD"  },
			{ eSpotMid,   "HOLD",   eSpotBot   ,	"HOLD",		eSpotGuard ,	"HOLD",		eSpotGuard ,	"HOLD",eSpotHigh,	"DEFEND" ,	eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// French division probe																														
		3100,																																					
		{																																							
			{ eSpotBotMid ,"DEFEND",eSpotBotMid ,"DEFEND",eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE"  },
			{ eSpotFarMid ,"PROBE" ,eSpotFarMid ,"PROBE" ,eSpotCAFarMid,"PROBE" ,eSpotCAFarMid,"PROBE" ,eSpotCAFarMid,"PROBE" ,eSpotCAFarMid,"PROBE"  },
			{ eSpotBot    ,"PROBE" ,eSpotBot    ,"PROBE" ,eSpotCABack  ,"PROBE" ,eSpotCABack  ,"PROBE" ,eSpotHigh    ,"DEFEND",eSpotHigh    ,"DEFEND" },
			{ eSpotBackMid,"DEFEND",eSpotBackMid,"DEFEND",eSpotTop     ,"PROBE" ,eSpotTop     ,"PROBE" ,eSpotHigh    ,"DEFEND",eSpotHigh    ,"DEFEND" },
			{ eSpotBack   ,"PROBE" ,eSpotBack   ,"PROBE" ,eSpotGuard   ,"DEFEND",eSpotGuard   ,"DEFEND",eSpotHigh    ,"DEFEND",eSpotHigh    ,"DEFEND" },
		}																																							
	},{																																							
		// Linear Warfare Probe																																
		3200,																																					
		{																																							
			{ eSpotMid    ,"PROBE",  eSpotMid    ,  "PROBE",eSpotCALine  ,"PROBE" ,eSpotCALine  ,"PROBE" ,eSpotCAMid ,"PROBE" ,eSpotCAMid,"PROBE"  },
			{ eSpotFarMid ,"PROBE",  eSpotFarMid ,  "PROBE",eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE" ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBotMid ,"HOLD"  ,  eSpotBotMid  ,"HOLD" ,eSpotCAFarMid,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBot    ,"HOLD"  ,  eSpotBot     ,"HOLD" ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD",  eSpotHigh ,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,  eSpotBackMid ,"HOLD" ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD"  ,eSpotHigh, "HOLD"   },
		}																																							
	},{																																							
		// Flanking attack	#1																											
		3300,																																					
		{																																							
			{ eSpotFarMid ,"ATTACK",eSpotFarMid ,"ATTACK",eSpotCALine  ,"HOLD",eSpotCALine  ,"HOLD",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK"  },
			{ eSpotBotMid ,"PROBE" ,eSpotBotMid ,"PROBE" ,eSpotCAMid   ,"HOLD",eSpotCAMid   ,"HOLD",eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotBot    ,"PROBE" ,eSpotBot    ,"PROBE" ,eSpotCAFarMid,"HOLD",eSpotCAFarMid,"HOLD",eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotMid    ,"PROBE" ,eSpotMid    ,"PROBE" ,eSpotGuard   ,"HOLD",eSpotGuard   ,"HOLD",eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotBackMid,"HOLD"  ,eSpotBackMid,"HOLD"  ,eSpotGuard   ,"HOLD",eSpotGuard   ,"HOLD",eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
		}																																							
	},{																																							
		// Rotation probe left around the pivot	#1																														
		3400,																																					
		{																																							
			{ eSpotTop,		"HOLD",	eSpotBot,    "DEFEND"  ,eSpotCALine,	"PROBE", eSpotCABack   ,"HOLD",eSpotCAMid,		"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotMid  ,	"HOLD",	eSpotBack   ,"HOLD"  ,	eSpotCAMid,		"PROBE", eSpotCABackMid,"HOLD",eSpotCAFarMid,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBot,		"PROBE",eSpotBotMid, "HOLD",	eSpotCAFarMid  ,"PROBE", eSpotGuard    ,"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBack,	"PROBE",eSpotBackMid,"HOLD"  ,	eSpotGuard    ,	"HOLD",	 eSpotGuard    ,"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid, "PROBE",eSpotMid   , "DEFEND"  ,eSpotGuard    ,	"HOLD",  eSpotGuard    ,"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		//Rotation probe left around the pivot	#2																															
		3500,																																					
		{																																							
			{ eSpotTop,		"HOLD",		eSpotBot,    "DEFEND"  ,eSpotCALine,	"PROBE", eSpotCABack   ,"HOLD",eSpotCAMid,		"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotMid  ,	"HOLD",		eSpotBack   ,"HOLD"  ,	eSpotCAMid,		"PROBE", eSpotCABackMid,"HOLD",eSpotCAFarMid,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBot,		"PROBE",	eSpotBotMid, "HOLD",	eSpotCAFarMid  ,"PROBE", eSpotGuard   ,	"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBack,	"PROBE",	eSpotBackMid,"HOLD"  ,	eSpotGuard   ,	"HOLD",	 eSpotGuard    ,"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid, "PROBE",	eSpotMid   , "DEFEND"  ,eSpotGuard   ,	"HOLD",	 eSpotGuard    ,"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Rotation probe right around the pivot #1																																		
		3600,																																					
		{																																							
			{ eSpotBot,    "DEFEND"  ,	eSpotTop,		"HOLD",	eSpotCABack   ,"HOLD", eSpotCALine,		"PROBE",	 eSpotCAMid,	"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotBack   ,"HOLD"  ,	eSpotMid  ,		"HOLD", eSpotCABackMid,"HOLD", eSpotCAMid,		"PROBE",	 eSpotCAFarMid,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBotMid, "HOLD",		eSpotBot,		"PROBE",eSpotGuard    ,"HOLD", eSpotCAFarMid   ,"PROBE",	 eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid,"HOLD"  ,	eSpotBack,		"PROBE",eSpotGuard    ,"HOLD", eSpotGuard  ,	"HOLD",		 eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotMid   , "DEFEND"  ,	eSpotBackMid,	"PROBE",eSpotGuard    ,"HOLD", eSpotGuard    ,	"HOLD",		 eSpotHigh,		"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Rotation probe right around the pivot #2																															
		3700,																																					
		{																																							
			{ eSpotBot,    "DEFEND" ,eSpotTop,    "HOLD",	eSpotCABack   ,"HOLD", eSpotCALine,		"PROBE", eSpotCAMid,	"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotBack   ,"HOLD"  , eSpotMid  ,  "HOLD",	eSpotCABackMid,"HOLD", eSpotCAMid,		"PROBE", eSpotCAFarMid, "DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBotMid, "HOLD",	 eSpotBot,	  "PROBE",  eSpotGuard   , "HOLD", eSpotCAFarMid   ,"PROBE", eSpotHigh,     "HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid,"HOLD"  , eSpotBack,	  "PROBE",  eSpotGuard    ,"HOLD", eSpotGuard    ,	"HOLD",	 eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotMid   , "DEFEND" ,eSpotBackMid,"PROBE",  eSpotGuard    ,"HOLD", eSpotGuard    ,	"HOLD",  eSpotHigh,		"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Prussian Brigade skimishers action in attack																															
		3800,																																					
		{																																							
			{ eSpotBotMid ,"ATTACK" ,	eSpotBotMid ,	"ATTACK",	eSpotCAFarMid ,	"PROBE" ,	eSpotCAFarMid,	"PROBE",	eSpotCAFarMid,	"PROBE" ,	eSpotCAFarMid,	"PROBE"  },
			{ eSpotBackMid,"PROBE" ,	eSpotBackMid,	"PROBE",	eSpotCABack   ,	"DEFEND" ,	eSpotCABack,	"DEFEND" ,	eSpotCAMid ,	"DEFEND",	eSpotCAMid ,	"DEFEND" },
			{ eSpotFarMid, "PROBE",		eSpotFarMid,	"PROBE",	eSpotGuard   ,	"DEFEND" ,	eSpotGuard,		"DEFEND" ,	eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotBot   , "ATTACK",	eSpotBot   ,	"ATTACK",	eSpotGuard ,	"DEFEND",	eSpotGuard ,	"DEFEND",	eSpotHigh  ,	"DEFEND",	eSpotHigh  ,	"DEFEND" },
			{ eSpotBack  , "PROBE" ,	eSpotBack  ,	"PROBE",	eSpotGuard ,	"DEFEND" ,	eSpotGuard ,	"DEFEND",	eSpotHigh,		"DEFEND"  ,	eSpotHigh,		"DEFEND"   },
		}																																							
	},{																																							
		// Flanking attack #2																												
		3900,																																					
		{																																							
			{ eSpotFarMid ,"ATTACK",eSpotFarMid ,"ATTACK",eSpotCALine  ,"ATTACK",	eSpotCALine,	"ATTACK",	eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK"  },
			{ eSpotBotMid ,"PROBE" ,eSpotBotMid ,"PROBE" ,eSpotCAMid   ,"PROBE",	eSpotCAMid   ,	"PROBE",	eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotBot    ,"PROBE" ,eSpotBot    ,"PROBE" ,eSpotCAFarMid,"PROBE",	eSpotCAFarMid,	"PROBE",	eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotMid    ,"PROBE" ,eSpotMid    ,"PROBE" ,eSpotGuard   ,"HOLD",		eSpotGuard   ,	"HOLD",		eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
			{ eSpotBackMid,"HOLD"  ,eSpotBackMid,"HOLD"  ,eSpotGuard   ,"HOLD",		eSpotGuard   ,	"HOLD",		eSpotHigh    ,"PROBE" ,eSpotHigh    ,"PROBE" },
		}																																							
	},{																																							
		// French Division Attack #1																															
		4000,																																					
		{																																							
			{ eSpotBotMid ,"ATTACK",eSpotBotMid ,"ATTACK",eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"ATTACK",eSpotCAMid   ,"ATTACK"  },
			{ eSpotFarMid ,"ATTACK",eSpotFarMid ,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK"  },
			{ eSpotBot    ,"PROBE" ,eSpotBot    ,"PROBE" ,eSpotCABack  ,"PROBE" ,eSpotCABack  ,"PROBE" ,eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
			{ eSpotBackMid,"ATTACK",eSpotBackMid,"ATTACK",eSpotTop     ,"ATTACK",eSpotTop     ,"ATTACK",eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
			{ eSpotBack   ,"ATTACK",eSpotBack   ,"ATTACK",eSpotGuard   ,"DEFEND",eSpotGuard   ,"DEFEND",eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
		}																																							
	},{																																							
		// French Division Attack #2																													
		4100,																																					
		{																																							
			{ eSpotBotMid ,"ATTACK",eSpotBotMid ,"ATTACK",eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"PROBE" ,eSpotCAMid   ,"ATTACK",eSpotCAMid   ,"ATTACK"  },
			{ eSpotFarMid ,"ATTACK",eSpotFarMid ,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK"  },
			{ eSpotBot    ,"PROBE" ,eSpotBot    ,"PROBE" ,eSpotCABack  ,"ATTACK",eSpotCABack  ,"ATTACK",eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
			{ eSpotBackMid,"ATTACK",eSpotBackMid,"ATTACK",eSpotTop     ,"ATTACK",eSpotTop     ,"ATTACK",eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
			{ eSpotBack   ,"PROBE" ,eSpotBack   ,"PROBE" ,eSpotGuard   ,"DEFEND",eSpotGuard   ,"DEFEND",eSpotHigh    ,"ATTACK",eSpotHigh    ,"ATTACK" },
		}																																							
	},{																																							
		// Combined arms probe #1																															
		4200,																																					
		{																																							
			{ eSpotMid,    "PROBE"  ,eSpotMid,    "ATTACK", eSpotCALine,   "ATTACK",  eSpotCALine,   "PROBE",  eSpotCALine   ,"PROBE" , eSpotCALine,"ATTACK"  },
			{ eSpotFarMid, "ATTACK", eSpotFarMid, "PROBE"  ,eSpotCAMid,    "PROBE"   ,eSpotCAMid,    "ATTACK", eSpotCAMid    ,"ATTACK", eSpotCAMid ,"PROBE" },
			{ eSpotBotMid, "PROBE"  ,eSpotBotMid, "PROBE"  ,eSpotCAFarMid ,"PROBE"   ,eSpotCAFarMid ,"PROBE"  ,eSpotCAFarMid ,"PROBE",  eSpotHigh  ,"PROBE"},
			{ eSpotBot   , "PROBE"  ,eSpotBot   , "PROBE"  ,eSpotCABackMid,"PROBE"   ,eSpotCABackMid,"PROBE"  ,eSpotCABackMid,"PROBE",  eSpotBack  ,"PROBE" },
			{ eSpotBackMid,"PROBE"  ,eSpotBackMid,"PROBE"  ,eSpotCABack,   "PROBE"   ,eSpotCABack,   "PROBE"  ,eSpotCABack   ,"PROBE"  ,eSpotFarMid,"PROBE"   },
		}																																							
	},{																																							
		// Combined arms probe #2																															
		4300,																																					
		{																																							
			{ eSpotMid,    "ATTACK"  ,eSpotMid,   "PROBE",  eSpotCALine,   "PROBE",   eSpotCALine,	 "ATTACK",  eSpotCALine   ,"ATTACK" ,	eSpotCALine,"ATTACK"  },
			{ eSpotFarMid, "PROBE", eSpotFarMid, "ATTACK"  ,eSpotCAMid,    "ATTACK"  ,eSpotCAMid,	 "PROBE",	eSpotCAMid    ,"PROBE",		eSpotCAMid ,"PROBE" },
			{ eSpotBotMid, "PROBE"  ,eSpotBotMid, "PROBE"  ,eSpotCAFarMid ,"PROBE"   ,eSpotCAFarMid ,"PROBE"  ,	eSpotCAFarMid ,"PROBE",		eSpotHigh  ,"PROBE"},
			{ eSpotBot   , "PROBE"  ,eSpotBot   , "PROBE"  ,eSpotCABackMid,"PROBE"   ,eSpotCABackMid,"PROBE"  ,	eSpotCABackMid,"PROBE",		eSpotBack  ,"PROBE" },
			{ eSpotBackMid,"PROBE"  ,eSpotBackMid,"PROBE"  ,eSpotCABack,   "PROBE"   ,eSpotCABack,	 "PROBE"  ,	eSpotCABack   ,"PROBE"  ,	eSpotFarMid,"PROBE"   },
		}																																							
	},{																																							
		// French Revolutionary Attack																															
		4400,																																					
		{																																							
			{ eSpotBotMid, "PROBE" ,eSpotBotMid, "PROBE" ,eSpotCABack ,		"DEFEND",eSpotCABack ,	"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotFarMid, "ATTACK",eSpotFarMid, "ATTACK",eSpotCABackMid ,	"DEFEND",eSpotCABackMid,"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotBot   , "PROBE" ,eSpotBot   , "PROBE" ,eSpotGuard ,		"DEFEND",eSpotGuard ,	"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotBackMid,"PROBE" ,eSpotBackMid,"PROBE" ,eSpotGuard ,		"DEFEND",eSpotGuard ,	"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
			{ eSpotBack ,  "PROBE" ,eSpotBack ,  "PROBE" ,eSpotGuard ,		"DEFEND",eSpotGuard ,	"DEFEND",eSpotHigh,"DEFEND",eSpotHigh,"DEFEND" },
		}																																							
	},{																																							
		// Gran battery attack support																													
		4500,																																					
		{																																							
			{ eSpotMid   ,	"ATTACK" ,	eSpotMid   ,"ATTACK" ,	eSpotCALine ,	"ATTACK" ,	eSpotCALine ,	"ATTACK"  ,	eSpotCALine,	"ATTACK" ,	eSpotCALine,	"ATTACK"  },
			{ eSpotFarMid,	"ATTACK" ,	eSpotFarMid,"ATTACK" ,	eSpotCAMid ,	"ATTACK"  ,	eSpotCAMid ,	"ATTACK"  ,	eSpotCAMid ,	"ATTACK",	eSpotCAMid ,	"ATTACK" },
			{ eSpotBotMid,	"ATTACK" ,	eSpotBotMid,"ATTACK" ,	eSpotCAFarMid,	"ATTACK" ,	eSpotCAFarMid,	"ATTACK"  ,	eSpotCAFarMid  ,"ATTACK",	eSpotCAFarMid  ,"ATTACK" },
			{ eSpotBot   ,	"PROBE" ,	eSpotBot   ,"PROBE" ,	eSpotGuard ,	"PROBE",	eSpotGuard ,	"PROBE",	eSpotCABack  ,	"ATTACK",	eSpotCABack  ,	"ATTACK" },
			{ eSpotBackMid,	"PROBE",	eSpotBackMid,"PROBE",	eSpotGuard ,	"PROBE",	eSpotGuard ,	"PROBE"  ,	eSpotCABackMid,	"ATTACK"  ,	eSpotCABackMid,	"ATTACK"   },
		}																																							
	},{																																							
		// Attack on both the flanks																									
		4600,																																					
		{																																							
			{ eSpotTop   ,"ATTACK",	eSpotTop   ,"ATTACK",eSpotCALine,   "ATTACK" ,eSpotCALine,     "ATTACK",eSpotTop,		"ATTACK",eSpotTop,	 "ATTACK"  },
			{ eSpotMid,	  "ATTACK" ,eSpotMid   ,"ATTACK",eSpotCAMid,    "ATTACK", eSpotCAMid,    "ATTACK",eSpotCALine ,	"ATTACK",eSpotCALine,"ATTACK" },
			{ eSpotFarMid,"ATTACK" ,eSpotFarMid,"ATTACK",eSpotCAFarMid ,"DEFEND", eSpotCAFarMid ,"DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,"DEFEND" },
			{ eSpotBot   ,"DEFEND" ,eSpotBot   ,"DEFEND",eSpotGuard   , "DEFEND" ,eSpotGuard   , "DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,"DEFEND" },
			{ eSpotBotMid,"DEFEND",	eSpotBotMid,"DEFEND",eSpotGuard ,   "DEFEND", eSpotGuard ,   "DEFEND",eSpotHigh,	"DEFEND",eSpotHigh,	 "DEFEND"   },
		}																																							
	},{																																							
		// Attack on the right flank																												
		4700,																																					
		{																																							
			{ eSpotMid   ,	"DEFEND",	eSpotTop,	"ATTACK", eSpotCAMid ,   "DEFEND",  eSpotCALine,		"ATTACK",	eSpotHigh,"HOLD" , eSpotTop,	"ATTACK"  },
			{ eSpotFarMid  ,"DEFEND",	eSpotMid,	"ATTACK" ,eSpotCAFarMid, "DEFEND",  eSpotCAMid,		"ATTACK"  ,	eSpotHigh,"HOLD",  eSpotCALine,	"ATTACK" },
			{ eSpotBotMid,	"DEFEND",   eSpotFarMid,"ATTACK" ,eSpotGuard,    "HOLD",    eSpotCAFarMid ,	"DEFEND"  ,	eSpotHigh,"HOLD",  eSpotHigh,	"DEFEND" },
			{ eSpotBot   ,	"HOLD",     eSpotBot   ,"DEFEND" ,eSpotGuard   , "HOLD",    eSpotCABackMid ,"DEFEND",	eSpotHigh,"HOLD",  eSpotHigh,	"DEFEND" },
			{ eSpotBackMid, "HOLD",		eSpotBotMid,"DEFEND", eSpotGuard ,   "HOLD",    eSpotCABackMid, "DEFEND",	eSpotHigh,"HOLD",  eSpotHigh,	"DEFEND"     },
		}																																							
	},{																																							
		// Attack on the left flank																													
		4800,																																					
		{																																							
			{ eSpotTop   ,"ATTACK",	eSpotMid   ,	"DEFEND",	eSpotCALine,	"ATTACK" ,eSpotCAMid ,   "DEFEND",	eSpotTop,		"ATTACK" ,	eSpotHigh,"HOLD"  },
			{ eSpotMid,	  "ATTACK" ,eSpotFarMid   ,	"DEFEND",	eSpotCAMid,		"ATTACK", eSpotCAFarMid, "DEFEND",	eSpotCALine ,	"ATTACK",	eSpotHigh,"HOLD" },
			{ eSpotFarMid,"ATTACK" ,eSpotBotMid,	"DEFEND",	eSpotCAFarMid ,	"DEFEND", eSpotGuard,    "HOLD",	eSpotHigh  ,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBot   ,"DEFEND" ,eSpotBot   ,	"HOLD",		eSpotCABackMid, "DEFEND", eSpotGuard   , "HOLD",	eSpotHigh  ,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBotMid,"DEFEND",	eSpotBackMid,	"HOLD",		eSpotCABackMid, "DEFEND", eSpotGuard ,   "HOLD",	eSpotHigh,		"DEFEND"  ,	eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Linear Warfare Deep Attack																															
		4900,																																					
		{																																							
			{ eSpotBotMid   ,"ATTACK" ,eSpotBotMid   ,"ATTACK" ,eSpotCAMid ,   "ATTACK",eSpotCAMid ,	"ATTACK",	eSpotCALine,	"ATTACK",	eSpotCALine,"ATTACK"  },
			{ eSpotBackMid  ,"ATTACK" ,eSpotBackMid  ,"ATTACK" ,eSpotCABackMid,"ATTACK",eSpotCABackMid,	"ATTACK" ,	eSpotCAMid ,	"ATTACK",	eSpotCAMid ,"ATTACK" },
			{ eSpotFarMid ,  "ATTACK" ,eSpotFarMid ,  "ATTACK" ,eSpotGuard   , "PROBE",eSpotGuard   ,	"PROBE",	eSpotHigh  ,	"PROBE",	eSpotHigh  ,"PROBE" },
			{ eSpotBot    ,  "ATTACK" ,eSpotBot    ,  "ATTACK" ,eSpotGuard ,   "PROBE",eSpotGuard,		"PROBE",	eSpotHigh  ,	"PROBE",	eSpotHigh  ,"PROBE" },
			{ eSpotBack   ,  "ATTACK" ,eSpotBack   ,  "ATTACK" ,eSpotGuard ,   "PROBE" ,eSpotGuard,		"PROBE",	eSpotHigh,		"PROBE",	eSpotHigh,	"PROBE"   },
		}																																							
	},{																																							
		// Linear Warfare Attack																													
		5000,																																					
		{
			{ eSpotMid    ,"ATTACK",  eSpotMid    ,  "ATTACK",eSpotCALine  ,"ATTACK",eSpotCALine  ,"ATTACK",eSpotCAMid,	"ATTACK" ,	eSpotCAMid,	"ATTACK"  },
			{ eSpotFarMid ,"ATTACK",  eSpotFarMid ,  "ATTACK",eSpotCAMid   ,"ATTACK",eSpotCAMid   ,"ATTACK",eSpotHigh ,	"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBotMid ,"HOLD"  ,  eSpotBotMid   ,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotCAFarMid,"HOLD"  ,eSpotHigh  ,"HOLD",		eSpotHigh  ,"HOLD" },
			{ eSpotBot    ,"HOLD"  ,  eSpotBot      ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD",		eSpotHigh ,	"HOLD" },
			{ eSpotBackMid,"HOLD"  ,  eSpotBackMid  ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotGuard   ,"HOLD"  ,eSpotHigh  ,"HOLD"  ,	eSpotHigh,	"HOLD"   },
		}																																							
	},{																																							
		// Combined arms attack #1																									
		5100,																																					
		{																																							
			{ eSpotMid,    "ATTACK"  ,eSpotMid,    "BLITZ",   eSpotCALine,   "BLITZ",	 eSpotCALine,	 "ATTACK",  eSpotCALine   ,"ATTACK" ,	eSpotCALine,"BLITZ"  },
			{ eSpotFarMid, "BLITZ",   eSpotFarMid, "ATTACK"  ,eSpotCAMid,    "ATTACK"   ,eSpotCAMid,	 "BLITZ",	eSpotCAMid    ,"BLITZ",		eSpotCAMid ,"ATTACK" },
			{ eSpotBotMid, "ATTACK"  ,eSpotBotMid, "ATTACK"  ,eSpotCAFarMid ,"ATTACK"   ,eSpotCAFarMid , "ATTACK"  ,eSpotCAFarMid ,"ATTACK",	eSpotHigh  ,"ATTACK"},
			{ eSpotBot   , "ATTACK"  ,eSpotBot   , "ATTACK"  ,eSpotCABackMid,"ATTACK"   ,eSpotCABackMid, "ATTACK"  ,eSpotCABackMid,"ATTACK",	eSpotBack  ,"ATTACK" },
			{ eSpotBackMid,"ATTACK"  ,eSpotBackMid,"ATTACK"  ,eSpotCABack,   "ATTACK"   ,eSpotCABack,    "ATTACK"  ,eSpotCABack   ,"ATTACK"  ,	eSpotFarMid,"ATTACK"   },
		}																																							
	},{																																							
		// Rotation attack around the right																												
		5200,																																					
		{																																							
			{ eSpotBot,    "DEFEND" ,eSpotTop,		"HOLD",	   eSpotCABack   ,	"HOLD", eSpotCALine,	"ATTACK", eSpotCAMid,	"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotBack   ,"HOLD"  , eSpotMid  ,	"HOLD",	   eSpotCABackMid,	"HOLD", eSpotCAMid,		"ATTACK", eSpotCAFarMid,"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBotMid, "HOLD",	 eSpotBot,		"ATTACK",  eSpotGuard   ,	"HOLD", eSpotCAFarMid  ,"ATTACK", eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid,"HOLD"  , eSpotBack,		"ATTACK",  eSpotGuard    ,	"HOLD", eSpotGuard    ,	"HOLD",	  eSpotHigh,	"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotMid   , "DEFEND" ,eSpotBackMid,	"ATTACK",  eSpotGuard    ,	"HOLD", eSpotGuard    ,	"HOLD",	  eSpotHigh,	"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Rotation attack around the left																													
		5300,																																					
		{																																							
			{ eSpotTop,		"HOLD",		eSpotBot,    "DEFEND"  ,eSpotCALine   ,	"ATTACK",	eSpotCABack   ,	"HOLD",eSpotCAMid,		"DEFEND" ,	eSpotHigh,"HOLD"  },
			{ eSpotMid  ,	"HOLD",		eSpotBack   ,"HOLD"  ,	eSpotCAMid,		"ATTACK",	eSpotCABackMid,	"HOLD",eSpotCAFarMid,	"DEFEND",	eSpotHigh,"HOLD" },
			{ eSpotBot,		"ATTACK",	eSpotBotMid, "HOLD",	eSpotCAFarMid,	"ATTACK",	eSpotGuard   ,	"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBack,	"ATTACK",   eSpotBackMid,"HOLD"  ,  eSpotGuard    ,	"HOLD",		eSpotGuard    ,	"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD" },
			{ eSpotBackMid, "ATTACK",	eSpotMid   , "DEFEND"  ,eSpotGuard    ,	"HOLD",		eSpotGuard    ,	"HOLD",eSpotHigh,		"HOLD",		eSpotHigh,"HOLD"   },
		}																																							
	},{																																							
		// Prussian Brigade attack formation																															
		5400,																																					
		{																																							
			{ eSpotBotMid ,"ATTACK" ,eSpotBotMid ,"ATTACK",eSpotCAFarMid ,	"ATTACK" ,	eSpotCAFarMid,	"ATTACK",eSpotCAFarMid,	"ATTACK",eSpotCAFarMid,	"ATTACK"  },
			{ eSpotBackMid,"DEFEND" ,eSpotBackMid,"DEFEND",eSpotCABackMid  ,"DEFEND" ,	eSpotCABackMid,	"DEFEND",eSpotCAMid ,	"DEFEND",eSpotCAMid ,	"DEFEND" },
			{ eSpotFarMid, "DEFEND", eSpotFarMid, "DEFEND",eSpotGuard   ,	"DEFEND" ,	eSpotGuard,		"DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,	"DEFEND" },
			{ eSpotBot   , "ATTACK", eSpotBot   , "ATTACK",eSpotGuard ,		"DEFEND",	eSpotGuard ,	"DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,	"DEFEND" },
			{ eSpotBack  , "DEFEND" ,eSpotBack  , "DEFEND",eSpotGuard ,		"DEFEND" ,	eSpotGuard ,	"DEFEND",eSpotHigh,		"DEFEND",eSpotHigh,		"DEFEND"   },
		}																																							
	},{																																							
		// Russian Columns Deep Attack																												
		5500,																																					
		{																																							
			{ eSpotBotMid   ,"BLITZ" ,eSpotBotMid   ,"BLITZ" ,eSpotCAMid ,   "BLITZ",eSpotCAMid ,    "BLITZ", eSpotCALine,	"BLITZ" ,	eSpotCALine,"BLITZ"  },
			{ eSpotBackMid  ,"BLITZ" ,eSpotBackMid  ,"BLITZ" ,eSpotCABackMid,"BLITZ",eSpotCABackMid, "BLITZ" ,eSpotCAMid ,	"BLITZ",	eSpotCAMid ,"BLITZ" },
			{ eSpotFarMid ,  "BLITZ" ,eSpotFarMid ,  "BLITZ" ,eSpotGuard   , "ATTACK",eSpotGuard   , "ATTACK",eSpotHigh  ,	"ATTACK",	eSpotHigh  ,"ATTACK" },
			{ eSpotBot    ,  "BLITZ" ,eSpotBot    ,  "BLITZ" ,eSpotGuard ,   "ATTACK",eSpotGuard,    "ATTACK",eSpotHigh  ,	"ATTACK",	eSpotHigh  ,"ATTACK" },
			{ eSpotBack   ,  "BLITZ" ,eSpotBack   ,  "BLITZ" ,eSpotGuard ,   "ATTACK" ,eSpotGuard,   "ATTACK",eSpotHigh,	"ATTACK",	eSpotHigh,	"ATTACK"  },
		}																																							
	},{																																							
		// Encirclement attack																										
		5600,																																					
		{																																							
			{ eSpotTop   ,"BLITZ",eSpotTop   ,	"BLITZ",	eSpotCALine,   "BLITZ" , eSpotCALine,   "BLITZ", eSpotTop,		"BLITZ" ,eSpotTop,		"BLITZ"  },
			{ eSpotMid,	  "ATTACK" ,eSpotMid   ,"ATTACK",	eSpotCAMid,    "ATTACK", eSpotCAMid,    "ATTACK",eSpotCALine ,	"ATTACK",eSpotCALine ,	"ATTACK" },
			{ eSpotFarMid,"BLITZ" ,eSpotFarMid,	"BLITZ",	eSpotCAFarMid ,"DEFEND", eSpotCAFarMid ,"DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,	"DEFEND" },
			{ eSpotBot   ,"DEFEND" ,eSpotBot   ,"DEFEND",	eSpotGuard   , "DEFEND" ,eSpotGuard   , "DEFEND",eSpotHigh  ,	"DEFEND",eSpotHigh  ,	"DEFEND" },
			{ eSpotBotMid,"DEFEND",eSpotBotMid, "DEFEND",	eSpotGuard ,   "DEFEND", eSpotGuard ,   "DEFEND",eSpotHigh,		"DEFEND",eSpotHigh,		"DEFEND"   },
		}																																							
	},{																																							
		// Encirclement on the right 																												
		5700,																																					
		{																																							
			{ eSpotMid ,	 "DEFEND",	eSpotTop ,	"BLITZ",	eSpotCAMid ,	"DEFEND",	eSpotCALine,	"BLITZ",	eSpotHigh,	"HOLD", eSpotTop,	"BLITZ" },
			{ eSpotFarMid ,	 "DEFEND",	eSpotMid,	"ATTACK" ,	eSpotCAFarMid,	"DEFEND",	eSpotCAMid,		"ATTACK" ,	eSpotHigh ,	"HOLD",	eSpotCALine,"ATTACK" },
			{ eSpotBotMid,	 "DEFEND",	eSpotFarMid,"BLITZ" ,	eSpotGuard,		"HOLD",		eSpotCAFarMid , "BLITZ" ,	eSpotHigh ,	"HOLD", eSpotHigh , "DEFEND" },
			{ eSpotBot ,	 "HOLD",	eSpotBot ,	"ATTACK" ,	eSpotGuard ,	"HOLD",		eSpotCABackMid ,"ATTACK",	eSpotHigh ,	"HOLD", eSpotHigh , "DEFEND" },
			{ eSpotBackMid,	 "HOLD",	eSpotBotMid,"DEFEND",	eSpotGuard ,	"HOLD",		eSpotCABackMid, "DEFEND",	eSpotHigh,	"HOLD", eSpotHigh,  "DEFEND" },
		}																																							
	},{																																							
		// Encirclement on the left 																												
		5800,																																					
		{																																							
			{ eSpotTop   ,"BLITZ",	eSpotMid   ,	"DEFEND",	eSpotCALine,	"BLITZ",	eSpotCAMid ,   "DEFEND",	eSpotTop,		"BLITZ" ,eSpotHigh,"HOLD"  },
			{ eSpotMid,	  "ATTACK" ,eSpotFarMid,	"DEFEND",	eSpotCAMid,		"ATTACK",	eSpotCAFarMid, "DEFEND",	eSpotCALine ,	"ATTACK",eSpotHigh,"HOLD" },
			{ eSpotFarMid,"BLITZ" , eSpotBotMid,	"DEFEND",	eSpotCAFarMid , "BLITZ",	eSpotGuard,    "HOLD",		eSpotHigh  ,	"DEFEND",eSpotHigh,"HOLD" },
			{ eSpotBot   ,"ATTACK" ,eSpotBot   ,	"HOLD",		eSpotCABackMid ,"ATTACK" ,	eSpotGuard   , "HOLD",		eSpotHigh  ,	"DEFEND",eSpotHigh,"HOLD" },
            { eSpotBotMid,"DEFEND", eSpotBackMid,	"HOLD",		eSpotCABackMid ,"DEFEND",	eSpotGuard ,   "HOLD",		eSpotHigh,		"DEFEND",eSpotHigh,"HOLD" },
		}																																							
	},{																																							
		// combined arms attack #2																														
		5900,																																					
		{																																							
			{ eSpotMid,    "BLITZ"  ,	eSpotMid,    "ATTACK",  eSpotCALine,   "ATTACK",   eSpotCALine,	  "BLITZ",  eSpotCALine   ,"BLITZ" ,	eSpotCALine,"ATTACK"  },
			{ eSpotFarMid, "ATTACK",	eSpotFarMid, "BLITZ"  ,	eSpotCAMid,    "BLITZ"   , eSpotCAMid,	  "ATTACK",	eSpotCAMid    ,"ATTACK",	eSpotCAMid ,"BLITZ" },
			{ eSpotBotMid, "ATTACK"  ,	eSpotBotMid, "ATTACK"  ,eSpotCAFarMid ,"ATTACK"   ,eSpotCAFarMid ,"ATTACK" ,eSpotCAFarMid ,"ATTACK",	eSpotHigh  ,"ATTACK"},
			{ eSpotBot   , "ATTACK"  ,	eSpotBot   , "ATTACK"  ,eSpotCABackMid,"ATTACK"   ,eSpotCABackMid,"ATTACK" ,eSpotCABackMid,"ATTACK",	eSpotBack  ,"ATTACK" },
			{ eSpotBackMid,"ATTACK"  ,	eSpotBackMid,"ATTACK"  ,eSpotCABack,   "ATTACK"   ,eSpotCABack,   "ATTACK" ,eSpotCABack   ,"ATTACK"  ,  eSpotFarMid,"ATTACK"   },
		}																																							
	},{																																							
		// Mixed Order Attack																												
		6000,																																					
		{																																							
			{ eSpotMid   ,"DEFEND",eSpotMid   ,"DEFEND",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK",eSpotCAFarMid,"ATTACK"  ,eSpotCAFarMid,"ATTACK" },
			{ eSpotFarMid,"BLITZ" ,eSpotFarMid,"BLITZ" ,eSpotCABack,"ATTACK"  ,eSpotCABack,"ATTACK"  ,eSpotCABack  ,"ATTACK"  ,eSpotCABack  ,"ATTACK" },
			{ eSpotBot   ,"BLITZ" ,eSpotBot   ,"BLITZ" ,eSpotGuard ,"ATTACK"  ,eSpotGuard ,"ATTACK"  ,eSpotHigh    ,"ATTACK"  ,eSpotHigh    ,"ATTACK" },
			{ eSpotBotMid,"DEFEND",eSpotBotMid,"DEFEND",eSpotGuard ,"ATTACK"  ,eSpotGuard ,"ATTACK"  ,eSpotHigh    ,"ATTACK"  ,eSpotHigh    ,"ATTACK" },
			{ eSpotBack  ,"BLITZ" ,eSpotBack  ,"BLITZ" ,eSpotGuard ,"ATTACK"  ,eSpotGuard ,"ATTACK"  ,eSpotHigh    ,"ATTACK"  ,eSpotHigh    ,"ATTACK" },
		}
	},{
		-1000, // marks end of table
	}
};

//*************************************************************************************************************
//*************************************************************************************************************
// Table: TableStance
//*************************************************************************************************************
//*************************************************************************************************************

// 1 Max Val
// 2 Move Range   (value multiplied by max range of weapon)
// 3 Arrive Range (value multiplied by max range of weapon)
// 4 WEIGHT  Ratio (us to them)
// 5 Fallback (at morale level:)
// 6 Retreat (at morale level:)
// 7 Distance to charge arty (yards)
// 8 STOP (Def Terrain)
// 9 Min Ammo (per man) until unit retreats
// 10 Min ammo wagon dist (yards)
// 11 Range to search for forts (yards)
// 12 Charge/ NO AMMO (Morale)
// 13 Charge (Morale)
// 14 Flank (Morale)
// 15 Volley (Morale)
// 16 Reform Line (Morale)
// 17 move forward
// 18 Advance (Max Targs)
	//19 CavDanger;   //morale check
	//20 SquareMen;   //minimal number of men for the square
	//21 SquareToEnemy; //minimal distance from the eneny to go in and out from a square formation
	//22 CavChargDist;  //minimal distance of charge's start
	//23 CavRunDist;   //minimal distance of run before the charge
	//24 SquareChargeFactor;  //power factor of square to inhibit the cavalry to charge the square (high is less probably the cavalry will charge the square
	//25 CavCharge; //morale check
	//26 CavPowerFactor; // how strong the cavalry feel to decide if attack a target
	//27 SkirmDimension;  //dimension of split skirmishers
	//28 SkirmAttitude;  //officer permit or not the split of company
	//29 SkirmSplit;  //morale check
	//30 SkirmNumber;  //number of skirmishes for brigade
	//31 MoraleBesieged;   //moral check of besieged troops
	//32 LeaveFort;     //leave the fort or not if not enemy near
	//33 InfantryCharge; //distance to start the infantry charge
	//34 ArtyFallback;   //distance of artillery fallback if not supported
	//35 brigform; // brigade fighting formation type, from eFormType
// 36-end command,distance,morale
// 21 Chances (%)

//	eWeapMinRange,
//	eWeapOptimal,
//	eWeapTypical,
//	eWeapLong,
//	eWeapMaxRange,

const TStance TableStance[] = 
{
//	HOLD
//       1,  2,  3,  4, 5, 6,  7, 8, 9,  10, 11, 12, 13, 14,15, 16, 17,18, 19,  20,  21, 22,     23,   24, 25, 26,  27,28, 29,30, 31, 32, 33,  34,       35, 36, 
	{   50, 10, 10,  3, 3, 2, 20, 1, 1, 500, 99, 11, 11, 11, 9, 11, 11, 1,  4, 201,  70, 70,	176,	5,	9,	2,	10, 0,	6, 0,	4,	0, 10, 350, eFTFight, -1 },
	{  100, 10, 10,  4, 3, 2, 21, 1, 1, 500, 90, 10, 10, 10, 9, 10, 11, 1,  4, 201,  69, 71,	177,	5,	9,	2,	10, 0,	6, 0,	4,  0, 15, 340, eFTFight, -1 },
	{  150, 10, 10,  5, 0, 2, 22, 1, 0, 500, 80, 10,  9, 10, 9,  9,  9, 1,  4, 201,  68, 72,	178,	5,	9,	3,	10, 0,	6, 0,	4,	0, 20, 330, eFTFight, -1 },
	{  200, 10, 10,  6, 0, 1, 23, 1, 0, 500, 70,  9,  8, 10, 9,  8,  8, 1,  4, 201,  67, 73,	179,	5,	9,	3,	10, 0,	6, 0,	4,	0, 20, 320, eFTFight, -1 },
	{  250, 10, 10,  7, 0, 1, 25, 1, 0, 500, 60,  8,  8, 10, 9,  8,  8, 1,  4, 201,  66, 74,	180,	5,	9,	3,	10, 0,	6, 0,	4,	0, 20, 310, eFTFight, -1 },
//	DEFEND											 
	{  300, 10, 11,  5, 7, 6, 25, 1, 2, 500, 90, 10,  8,  6,  8,  8,  8, 1,  5, 201,  65, 76,	181,	4,	8,	4,	6, 1,	7, 1,	5,	0, 20, 300, eFTFight,   eComMove, eWeapMaxRange, 0, -1 },
	{  350, 10, 13,  9, 6, 5, 26, 1, 1, 500, 80, 10,  8,  6,  8,  8,  8, 1,  5, 201,  64, 77,	182,	4,	8,	4,	6, 1,	7, 1,	5,	0, 20, 295, eFTFight,   eComMove, eWeapMaxRange, 0, -1 },
	{  400, 11, 15, 10, 5, 4, 27, 1, 0, 500, 70, 10,  7,  6,  7,  7,  7, 1,  5,	201,  63, 78,	183,	4,	8,	5,	6, 1,	7, 2,	5,	0, 20, 290, eFTFight,   eComMove, eWeapMaxRange, 0, -1 },
	{  450, 11, 18, 11, 4, 3, 28, 1, 0, 500, 60, 10,  7,  6,  6,  6,  6, 2,  5, 201,  62, 79,	184,	4,	8,	5,	6, 1,	7, 2,	5,	0, 20, 285, eFTFight,   eComMove, eWeapMaxRange, 0, -1 },
	{  500, 12, 20, 12, 3, 2, 29, 1, 0, 500, 50, 10,  6,  6,  6,  6,  6, 3,  5, 201,  61, 80,	185,	4,	8,	5,	6, 1,	7, 2,	5,	0, 20, 280, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove, eWeapLong, 6, -1 },
//	SKIRMISH																	 
	{  550, 20, 22, 10, 0, 6, 35, 1, 2, 500, 14, 10,  9,  6,  8,  8,  8, 1,  6,	201,  60, 82,	186,	3,	7,	6,	6, 1,	8, 2,	6,	0, 20, 280, eFTFight,   eComMove, eWeapMaxRange, 7, eComMove, eWeapLong, 6, -1 },
	{  600, 21, 26, 15, 6, 5, 36, 1, 1, 500, 13, 10,  9,  6,  8,  8,  8, 1,  6,	201,  59, 83,	187,	3,	7,	6,	6, 1,	8, 2,	6,	0, 20, 281, eFTFight,   eComMove, eWeapMaxRange, 7, eComMove, eWeapLong, 6, -1 },
	{  650, 21, 30, 20, 5, 4, 37, 1, 0, 500, 12, 10,  9,  6,  7,  7,  7, 2,  6,	201,  58, 84,	188,	3,	7,	7,	6, 1,	8, 3,	6,	0, 21, 282, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove, eWeapLong, 6, -1 },
	{  700, 22, 33, 25, 4, 3, 38, 1, 0, 500, 11, 10,  8,  6,  6,  6,  6, 3,  6,	201,  57, 85,	189,	3,	7,	7,	6, 1,	8, 3,	6,	0, 21, 283, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove, eWeapLong, 5, -1 },
	{  750, 22, 35, 30, 3, 2, 39, 1, 0, 500, 10, 10,  8,  6,  6,  6,  6, 4,  6,	201,  56, 86,	190,	3,	7,	7,	6, 1,	8, 3,	6,	0, 21, 284, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove, eWeapLong, 5, eComAdvance, eWeapTypical, 6, -1 },
//	PROBE																						 
	{  800, 23, 37, 10, 0, 5, 40, 1, 2, 500, 14, 10,  9,  6,  8,  8,  8, 1,  7,	201,  55, 89,	191,	2,	6,	9,	10, 1,	7, 3,	5,	0, 22, 200, eFTFight,   eComMove, eWeapMaxRange, 0, -1 },
	{  850, 23, 40, 15, 5, 4, 40, 1, 1, 500, 13, 10,  9,  6,  7,  7,  7, 1,  7,	201,  54, 90,	192,	2,	6,	9,	10, 1,	7, 3,	5,	0, 22, 199, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove   , eWeapLong, 7, -1 },
	{  900, 23, 41, 20, 4, 3, 45, 1, 0, 500, 12, 10,  8,  6,  7,  7,  7, 3,  7,	201,  53, 91,	193,	2,	6,	9,	10, 1,	7, 4,	5,	0, 22, 198, eFTFight,   eComMove, eWeapMaxRange, 0, eComMove   , eWeapLong, 6, eComAdvance, eWeapTypical, 7, -1 },
	{  950, 23, 43, 25, 3, 2, 45, 1, 0, 500, 11, 10,  8,  6,  6,  6,  6, 4,  7,	201,  52, 92,	194,	2,	6,	9,	10, 1,	7, 4,	5,	0, 24, 197, eFTFight,   eComMove, eWeapMaxRange, 0, eComAdvance, eWeapLong, 5, eComAdvance, eWeapTypical, 7, -1 },
	{ 1000, 23, 45, 30, 0, 3, 45, 1, 0, 500, 10, 10,  8,  6,  6,  6,  6, 5,  7,	201,  51, 93,	195,	2,	6,	9,	10, 1,	7, 4,	5,	0, 24, 196, eFTFight,   eComMove, eWeapMaxRange, 0, eComAdvance, eWeapLong, 5, eComAdvance, eWeapTypical, 5, -1 },
//	ATTACK																					 
	{ 1050, 26, 46, 15, 6, 5, 50, 1, 2, 500, 60, 10,  8,  6,  7,  7,  7, 1,  8,	201,  50, 95,	196,	1,	5,	10,	10, 1,	6, 5,	4,	1, 25, 185, eFTFight,   eComMove, eWeapMaxRange, 0, eComAdvance, eWeapLong, 6, -1 },
	{ 1100, 26, 47, 25, 5, 4, 55, 1, 1, 500, 50, 10,  8,  6,  6,  6,  6, 1,  8,	201,  49, 96,	197,	1,	5,	10,	10, 1,	6, 5,	4,	1, 30, 184, eFTFight,   eComMove, eWeapMaxRange, 0, eComAdvance, eWeapLong, 6, eComAdvance, eWeapOptimal, 6, -1 },
	{ 1150, 26, 48, 30, 4, 3, 50, 1, 0, 500, 40, 10,  7,  6,  6,  6,  6, 3,  8,	201,  48, 97,	198,	1,	5,	11,	10, 1,	6, 6,	4,	1, 30, 183, eFTFight,   eComMove, eWeapMaxRange, 0, eComAdvance, eWeapLong, 5, eComAdvance, eWeapTypical, 5, eComCharge, eWeapOptimal, 6, 25, eComAdvance, eWeapOptimal, 6 },
	{ 1200, 26, 50, 35, 3, 2, 55, 1, 0, 500, 30, 10,  7,  6,  6,  6,  6, 4,  8,	201,  47, 98,	199,	1,	5,	12,	10, 1,	6, 6,	4,	1, 35, 182, eFTFight, eComMove, eWeapMaxRange, 0, eComMove   , eWeapLong, 5, eComAdvance, eWeapTypical, 5, eComCharge, eWeapOptimal, 6, 25, eComAdvance, eWeapOptimal, 6 },
	{ 1250, 26, 53, 40, 2, 1, 59, 1, 0, 500, 20,  9,  6,  6,  6,  6,  6, 5,  8,	201,  46, 100,  200,	1,	5,	13,	10, 1,	6, 6,	4,	1, 38, 181, eFTFight, eComMove, eWeapMaxRange, 0, eComMove   , eWeapLong, 4, eComAdvance, eWeapTypical, 4, eComCharge, eWeapOptimal, 4, 25, eComAdvance, eWeapOptimal, 6 },
	{ 0 },
};



//*************************************************************************************************************
//*************************************************************************************************************
// Table: TableTacOrders
//*************************************************************************************************************
//*************************************************************************************************************

// Order,
// Description,
// Leader Style Pts,
// Division Mod,
// MOVE Stance ID(max value col.),
// ARRIVE Stance ID(max value col.),
// FIGHT Stance ID(max value col.),
// Div Play,
// Reg Wheel,
// Brig Wheel,
// Keep Stance,
// Keep Form,
// Keep Orders - only used for stay, DO NOT MOVE!!!

// #define FORART 60 // subtracted from style for below table lookups
// #define FORCAV 40 // added to style

// divmod < 0 means we don't want to move to high ground if we are arty

const TTacOrders TableTacOrders[] = 
{
	{ "Stay"  ,"IDS_ORDERSTAY"  ,  0,-40,  20,  50, 150,0,0,0,1,1,1 },
	{ "Hold"  ,"IDS_ORDERHOLD"  , 20,-20,  20, 150, 150,0,1,0,0,1,0 },
	{ "Defend","IDS_ORDERDEFEND", 40,  0,  50, 400, 400,1,1,1,0,0,0 },
	{ "Probe" ,"IDS_ORDERPROBE" , 60, 20, 120, 900, 900,1,1,1,0,0,0 },
	{ "Attack","IDS_ORDERATTACK", 80, 40, 120,1150,1150,1,1,1,0,0,0 },
	{ "Blitz" ,"IDS_ORDERBLITZ" ,100, 60,1250,1250,1250,1,1,1,1,0,0 },
	{ "" ,"" ,0,0,0,0,0,0 },
};



//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
