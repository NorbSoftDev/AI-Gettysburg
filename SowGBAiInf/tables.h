//*************************************************************************************************************
//*************************************************************************************************************
// File: tables.h
//*************************************************************************************************************
//*************************************************************************************************************

#ifndef __TABLES_H__
#define __TABLES_H__

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: 
//*************************************************************************************************************
//*************************************************************************************************************

enum EAreaMod
{
	eAMMin,
	eAMMaj,
	eAMLoc,
	eAMRes,
	eAMAtt,
	eAMSpot,
	eAMMax,
};

enum EGrades
{
	eGradeFallback,
	eGradeRetreat,
	eGradeBroken,
	eGradeRoute,
	eGradeCasualty,
	eGradeCaptureArty,
	eGradeCaptureInf,
	eGradeOffKill,
	eGradeCaptureWagon,
	eGradeMAX,
};

enum EFatType
{
	eFTInfPts,
	eFTInfTerr,
	eFTInfAttr,
	eFTCav1MntPts,
	eFTCav1MntTerr,
	eFTCav1MntAttr,
	eFTCav1DisPts,
	eFTCav1DisTerr,
	eFTCav1DisAttr,
	eFTCav2MntPts,
	eFTCav2MntTerr,
	eFTCav2MntAttr,
	eFTCav2DisPts,
	eFTCav2DisTerr,
	eFTCav2DisAttr,
	eFTArtUnlPts,
	eFTArtUnlTerr,
	eFTArtUnlAttr,
	eFTArtLimPts,
	eFTArtLimTerr,
	eFTArtLimAttr,
	eFTMax,
};

enum EMeleeVal
{
	eMVInf,
	eMVT1Mnt,
	eMVT1DMnt,
	eMVT2Mnt,
	eMVT2DMnt,
	eMeleeValMAX,
};

#define MAX_ELEVLVLS 19
#define MAX_ELEVROWS 10

extern const short TableGrades[eGradeMAX][2];

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CTables
//*************************************************************************************************************
//*************************************************************************************************************

class CTables
{
	private:
		short m_melee[eMeleeValMAX][eMeleeValMAX*2];
		short m_fatigue[eFTMax][eUnitStateMax];
		short m_elevation[MAX_ELEVROWS][MAX_ELEVLVLS];

	public:
		CTables () { Init(); }
		~CTables() { Free(); }

		void Init();
		void Free();

		int GetFatigue      ( int state, int unittype, int terrfat, int attrfat, const CUnit *unit ) const;
		int UnitMoraleBonus ( int men, int level ) const;
		int GetAccuracyBonus( const CFixed &myheight, const CFixed &theirheight, const CFixed &range2 ) const;

		int MeleeHit     (int i1, int i2) const { return m_melee[i1][i2]; }
		int MeleeHitIndex(int itype, bool bCav, bool btype2, bool bMounted) const;

		int Captured( int m, int e ) const;

		CFixed GetAreaMod( int type, int style ) const;

	private:
		void InitStateTables();
};
#endif

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************


