//*************************************************************************************************************
//*************************************************************************************************************
// File: xtables.h
//*************************************************************************************************************
//*************************************************************************************************************

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: SDivStratSpot
//
// Rank - If rank is empty for a spot or equals 0, spot is not used
// Orders - if orders column is blank, orders switched to NONE
// Cav Orders - 0,1 are flank screen; 2,3,4,5 - are reserve spots; 6 means guard
// place cav numbers in order they should be followed
//*************************************************************************************************************
//*************************************************************************************************************

struct SDivStratSpot
{
	BYTE lspot;
	char *lsorders;

	BYTE rspot;
	char *rsorders;

	BYTE cavlspot;
	char *cavlsorders;

	BYTE cavrspot;
	char *cavrsorders;

	BYTE artlspot;
	char *artlsorders;

	BYTE artrspot;
	char *artrsorders;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: SDivStrategy
//*************************************************************************************************************
//*************************************************************************************************************

struct SDivStrategy
{
	int imax; // value

	SDivStratSpot spots[eSpotMax];
};

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: TStance
//*************************************************************************************************************
//*************************************************************************************************************

struct TStance
{
	short imax;
	short movemult; // divide by 10
	short staymult; // divide by 10
	short ratio;    // divide by 10
	short fback;
	short retreat;
	short artycharge;

	bool terrstop;

	short ammomin;
	short ammorange;
	short fortrange;
	short chargenoammo;
	short chargemelee;
	short flanking;
	short volley;
	short maintainline;
	short moveforward;
	short advtargs;

//  new square-----------
	short CavDanger;   //morale check
	short SquareMen;   //minimal number of men for the square
	short SquareToEnemy; //minimal distance from the eneny to go in and out from a square formation
//---end new square--------
//--new cavalry-------------
	short CavChargDist;  //minimal distance of charge's start
	short CavRunDist;   //minimal distance of run before the charge
	short SquareChargeFactor;  //power factor of square to inhibit the cavalry to charge the square (high is less probably the cavalry will charge the square
	short CavCharge; //morale check
	short CavPowerFactor; // how strong the cavalry feel to decide if attack a target
//--- end new cavalry---------
//--new skirmishers--------------
	short SkirmDimension;  //dimension of split skirmishers
	short SkirmAttitude;  //officer permit or not the split of company
	short SkirmSplit;  //morale check
	short SkirmNumber; //number of skirmishers for brigade
//---end new skirmishers-------
//---new fort------------------
	short MoraleBesieged;   //moral check of besieged troops
	short LeaveFort;     //leave the fort or not if not enemy near
//----new fort----------------
// --new infantry-------------
	short InfantryCharge; //distance to start the infantry charge
//---new infantry---------
//---new artillery-------
	short ArtyFallback;   //distance of artillery fallback if not supported
//---new artillery-------
	short brigform; // brigade fighting formation type, from eFormType
	short c1comm;
	short c1dist;
	short c1morl;
	short c2comm;
	short c2dist;
	short c2morl;
	short c3comm;
	short c3dist;
	short c3morl;
	short c4comm;
	short c4dist;
	short c4morl;
	short c4pcnt;
	short c5comm;
	short c5dist;
	short c5morl;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: TTacOrders
//*************************************************************************************************************
//*************************************************************************************************************

struct TTacOrders
{
	char *name;
	char *desc;

	short style;
	short mod;
	short basemove;
	short basestay;
	short basefight;

	bool divplay;
	bool regwheel;
	bool brigwheel;
	bool keepstance;
	bool keepform;
	bool keeporders;
};

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************

