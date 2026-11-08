//*************************************************************************************************************
//*************************************************************************************************************
// File: tables.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"
#include "war3d.h"

#include "tables.h"
#include "level.h"
#include "cav.h"
#include "arty.h"

#include "../shared/util.h"
#include "../shared/warstr.h"
#include "../shared/w3dfind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Tables
//*************************************************************************************************************
//*************************************************************************************************************

//	eDMAILvl,  
//	eDMMorLvl, 
//	eDMRegStr, 
//	eDMCmdHgt,   
//	eDMCmdRad,
//	eDMCourLvl,
//	eDMMapLvl,
//	eDMMax,

//	eDiffNone,
//	eDiffGreen,
//	eDiffTrain,
//	eDiffOff,
//	eDiffComm,
//	eDiffLead,
//	eDiffHist,
//	eDiffGrog,
//	eDiffCust,
	
const short TableDiffLvls[eDiffMAX][eDMMax] =
{
	{0},
	{-3,-3, 80,  0,  0,       -1,0}, //	eDiffGreen,
	{-2,-2, 90,  0,  0,       -1,2}, //	eDiffTrain,
	{ 0, 0,100,  0,  0,       -1,2}, //	eDiffOff,
	{ 1, 1,110,  0,  0,eRankArmy,3}, //	eDiffComm,
	{ 2, 2,120,  0,  0,eRankCorp,3}, //	eDiffLead,
	{ 3, 3,100, 10, 10,eRankDiv ,4}, //	eDiffHist,
	{ 4, 4,130, 10, 10,eRankBrig,4}, //	eDiffGrog,
};

// View
// map
// radius
// height

const short TableRealLvls[eRankMAX][eRLMax] =
{
	{  0,  eMLsite,    0,  0 }, // eRankSide,
	{  0,  eMLsite,    0,  0 }, // eRankArmy,
	{  0,  eMLsite, 1500, 67 }, // eRankCorp,
	{  1,  eML1k  ,  800, 33 }, // eRankDiv,
	{  1,  eML700 ,  500, 17 }, // eRankBrig,
};											 

// Only grade points added to attacking units and subtracted from effected units (Morale is never taken away)
const short TableGrades[eGradeMAX][2] = 
{
	// Grade, Morale
	{ 10,  25 }, // Fallback
	{ 15,  50 }, // Retreat
	{ 20, 200 }, // Broken
	{ 25, 425 }, // Rout
	{  1,   0 }, // Casualty
	{ 30,  75 }, // Capture Arty
	{ 35, 450 }, // Capture Inf
	{ 10,  40 }, // Kill Commander
	{ 20,  30 }, // Capture Wagon
};

// Unit Morale Bonus - Ranges found in unitattributes.csv file
// UnitMoraleBonus,0,1,2,3,4,5,6,7,8,9
// Up To # Men ,Untrained,Conscript,Irregulars,Militia,Volunteer,Seasoned,Regular,Veteran,Elite,Elite Veteran
#define MAX_EXPLVLS 10
#define MAX_MORLVLS 10
#define MAX_EXPROWS 4
#define MAX_STYLVLS 7

static const short TableUnitMorBonus[MAX_EXPROWS][MAX_EXPLVLS+1] = 
{
	{ 450, 20, 30,  40,  50,  60,  70,  80,  90, 100, 110 },
	{ 600, 40, 50,  60,  70,  80,  90, 100, 110, 120, 130 },
	{ 750, 60, 70,  80,  90, 100, 110, 120, 130, 140, 150 },
	{ 900, 80, 90, 100, 110, 120, 130, 140, 150, 160, 170 },
};

//	"Capture,,,(% out of 100),,exact # morale down, exact # levels across,,,,",
//	",Untrained,Conscript,Irregulars,Militia,Volunteer,Seasoned,Regular,Veteran,Elite,Elite Veteran",
const short TableCapture[MAX_MORLVLS][MAX_EXPLVLS] = 
{
	{ 100, 100, 75, 75, 75, 50, 50, 50, 50, 50 },
	{ 100, 100, 75, 75, 75, 50, 50, 50, 50, 50 },
	{ 100, 100, 75, 75, 75, 50, 50, 50, 50, 50 },
	{ 100, 100, 75, 75, 75, 50, 50, 50, 50, 50 },
	{  75,  75, 50, 50, 50, 30, 30, 30, 30, 30 },
	{  75,  75, 50, 50, 50, 30, 30, 30, 30, 30 },
	{  50,  50, 30, 30, 30, 15, 15, 15, 15, 15 },
	{  50,  50, 30, 30, 30, 15, 15, 15, 15, 15 },
	{  25,  25, 20, 20, 20,  0,  0,  0,  0,  0 },
	{  25,  25, 20, 20, 20,  0,  0,  0,  0,  0 },
};

//	",These are random ranges which are then multiplied by the value to give an area a rank,,,,,,,,",
//	",This is used for corps and army commanders to determine which areas of the map they want to attack,,,,,,,,",
//	",the num of reserves is the result of the random nums same as the vpattack,,,,,,,,",
// divide all results by 100
const short TableAreaMod[MAX_STYLVLS][eAMMax*2] =
{
//     Minor,    Major,Area Loc,Reserves,VPAttack, rand obj
	{ 100, 110, 250, 300, 90, 110, 80, 301,  10, 101, 100, 1000 },
	{ 100, 120, 250, 400, 90, 110, 70, 250, 100, 101, 100, 2000 },
	{ 100, 130, 250, 500, 90, 110, 50, 210, 200, 210, 100, 3000 },
	{ 100, 140, 250, 600, 90, 110, 10, 201, 200, 220, 100, 6000 },
	{ 100, 150, 250, 700, 90, 110,  1,  10, 200, 330, 100, 7000 },
	{ 100, 160, 250, 800, 90, 110,  2,  20, 200, 330, 100, 8000 },
	{ 100, 170, 250, 900, 90, 110,  3,  30, 200, 430, 100, 9000 },
};

enum eTables
{
	eTFatigue,
	eTElevation,
	eTMelee,
	eTMax,
};

static const TCHAR *gTableNames[eTMax] =
{
	_T("TABLEFATIGUE"),
	_T("TABLEELEVATION"),
	_T("TABLEMELEE"),
};

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Init
//*************************************************************************************************************
//*************************************************************************************************************

void CTables::Init()
{
	theApp.UpdateSplash( "Loading Tables" );

	InitStateTables();
}	

//*************************************************************************************************************
//*************************************************************************************************************
// Function: InitStateTables
//*************************************************************************************************************
//*************************************************************************************************************

void CTables::InitStateTables()
{
	int i;
	int sel   = -1;
	int count = 0;

	CWString  str;
	CWString  temp;
	CWMemFile file;

	CWStringArray find;

	memset( m_fatigue, 0, sizeof m_fatigue );

	CW3DFind lfind( DIR_DATA, _T("statetables.csv") );

	while ( lfind.GetLastFile( file ) )
	{
		// header
		CUtil::ReadString( file, str, true );

		while ( file.GetPosition() < file.GetLength() )
		{
			CUtil::ReadString( file, str, true );

			if ( str.IsEmpty() || str[0] == ',' )
			{
				sel   = -1;
				count = 0;
			 
			 	continue;
			}

			if ( sel == eTFatigue )
			{
				CUtil::ParseGet( str, ',', temp );

				// table must be in the same order as eFTMax
				for ( i = 0; i < eUnitStateMax; ++i )
					CUtil::ParseGet( str, ',', m_fatigue[count][i] );

				count++;
			}
			else if ( sel == eTElevation )
			{
				for ( i = 0; i < MAX_ELEVLVLS && count < MAX_ELEVROWS; ++i )
					CUtil::ParseGet( str, ',', m_elevation[count][i] );

				count++;
			}
			else if ( sel == eTMelee )
			{
				CUtil::ParseGet( str, ',', temp );

				// table must be in the same order as eFTMax
				for ( i = 0; i < eMeleeValMAX*2; ++i )
					CUtil::ParseGet( str, ',', m_melee[count][i] );

				count++;
			}
			else
			{
				CUtil::ParseGet( str, ',', temp );
				
				for ( i = 0; i < eTMax; i++ )
				{
					if ( !temp.CompareNoCase( gTableNames[i] ) )
					{
						sel = i;
						break;
					}
				}
			}
		}
	}
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Free
//*************************************************************************************************************
//*************************************************************************************************************

void CTables::Free()
{
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: UnitMoraleBonus
//*************************************************************************************************************
//*************************************************************************************************************

int CTables::UnitMoraleBonus( int men, int level ) const
{
	UINT i;

	for ( i = 0; i < MAX_EXPROWS; ++i )
	{
		if ( TableUnitMorBonus[i][0] > men )
			return TableUnitMorBonus[i][ level + 1 ];		
	}
	return TableUnitMorBonus[MAX_EXPROWS-1][ level + 1 ];		
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetAccuracyBonus
//*************************************************************************************************************
//*************************************************************************************************************

int CTables::GetAccuracyBonus( const CFixed &myheight, const CFixed &theirheight, const CFixed &range2 ) const
{
	int i;
	int j;
	
	CFixed f;
	CFixed diff = abs( myheight - theirheight );	

	if ( diff < 10*CVec::m_UnitsPerYard ) // need to change this to a table lookup
		return 0;

	for ( i = 1; i < MAX_ELEVROWS; ++i )
	{
		f = m_elevation[i][0] * CVec::m_UnitsPerYard;
	
		if ( f > diff )
			break;
	}	

	if ( i >= MAX_ELEVROWS )
		i = MAX_ELEVROWS - 1;

	for ( j = 1; j < MAX_ELEVLVLS; j += 2 )
	{
		f  = m_elevation[0][j] * CVec::m_UnitsPerYard;
		f *= f;

		if ( range2 < f )
			break;
	}

	if ( j >= MAX_ELEVLVLS )
		j = MAX_ELEVLVLS - 2;

	if ( myheight > theirheight )
		return m_elevation[i][j];
		
	return m_elevation[i][j+1];
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetAreaMod
//*************************************************************************************************************
//*************************************************************************************************************

CFixed CTables::GetAreaMod( int type, int style ) const
{
	int mod1;
	int mod2;

	CFixed res;

	mod1 = TableAreaMod[ style ][ type * 2     ];
	mod2 = TableAreaMod[ style ][ type * 2 + 1 ];

	res = CUtil::Ran( 73, mod1, mod2 );

	res /= 100;

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: GetFatigue
//*************************************************************************************************************
//*************************************************************************************************************

int CTables::GetFatigue( int state, int unittype, int terrfat, int attrfat, const CUnit *unit ) const
{
	int val;
	int base;

	switch ( unittype )
	{
		case eUnitArt: 
		{
			const CArty *art = (const CArty *)unit;

			if ( art->LimState() == eLimLimbered )
				base = eFTArtLimPts; 
			else
				base = eFTArtUnlPts; 

			break;
		}
		case eUnitCav: 
		{
			const CCav *cav = (const CCav *)unit;

			// cav type 2
			if ( cav->OOBMod(eOMCav2) )
			{
				if ( cav->Mounted() )
					base = eFTCav2MntPts; 
				else
					base = eFTCav2DisPts; 
			}
			// cav type 1
			else
			{
				if ( cav->Mounted() )
					base = eFTCav1MntPts; 
				else
					base = eFTCav1DisPts; 
			}
			break;
		}
		default:       
			base = eFTInfPts; 
			break;
	}
	val = m_fatigue[base][state] + m_fatigue[base+1][state] * terrfat + m_fatigue[base+2][state] * attrfat;

	return val;
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: Captured
//*************************************************************************************************************
//*************************************************************************************************************

int CTables::Captured( int m, int e ) const
{
	return TableCapture[m][e];
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: MeleeHitIndex
//*************************************************************************************************************
//*************************************************************************************************************

int CTables::MeleeHitIndex(int itype, bool bCav, bool btype2, bool bMounted) const
{
	int res = 0;

	// eMVInf,
	// eMVT1Mnt,
	// eMVT1DMnt,
	// eMVT2Mnt,
	// eMVT2DMnt,
	
	if ( !bCav )
		res = eMVInf;
	else if ( btype2 && bMounted )
		res = eMVT2Mnt;
	else if ( btype2 && !bMounted )
		res = eMVT2DMnt;
	else if ( bMounted )
		res = eMVT1Mnt;
	else
		res = eMVT2DMnt;

	if ( itype == 1 )
		res *= 2;
	else if ( itype == 2 )
		res = res * 2 + 1;

	return res;
}

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************



