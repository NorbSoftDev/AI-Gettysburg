//*************************************************************************************************************
//*************************************************************************************************************
// File: xunit.cpp
//*************************************************************************************************************
//*************************************************************************************************************

#include "stdafx.h"

#include "../sowmod/xunit.h"
#include "../sowmod/xlink.h"

extern SSowModFuncs &gMod;

bool SEnemy::IsValid( CXUnit unit ) 
{
	return ( men > 0 ) && ( cdist < unit.MaxRangeYds() );
}

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXVec
//*************************************************************************************************************
//*************************************************************************************************************

CXVec::CXVec()
{
	m_vec = gMod.CVecNew();
}

CXVec::CXVec( const CXVec &vec )
{
	m_vec = gMod.CVecNew();

	gMod.CVecCopy(vec.m_vec,m_vec);
}

CXVec::CXVec( const CVec *vec )
{
	m_vec = gMod.CVecNew();
	gMod.CVecCopy(vec,m_vec);
}

CXVec::~CXVec()
{
	gMod.CVecDel(m_vec);
}

bool CXVec::operator==( const CXVec &vec ) const
{
	return gMod.CVecEq(m_vec,vec.m_vec);
}

bool CXVec::operator!=( const CXVec &vec ) const
{
	return gMod.CVecNEq(m_vec,vec.m_vec);
}

void CXVec::operator=( const CXVec &vec )
{
	gMod.CVecCopy(vec.m_vec,m_vec);
}

int CXVec::DistYds( const CXVec &vec ) const
{
	return gMod.CVecDistYds(m_vec,vec.m_vec);
}

int CXVec::Dist2Yds( const CXVec &vec ) const
{
	return gMod.CVecDist2Yds(m_vec,vec.m_vec);
}

bool CXVec::empty() const
{
	return gMod.CVecempty(m_vec);
}

void CXVec::CopyTo( CVec *vec ) const
{
	gMod.CVecCopy(m_vec,vec);
}

CXVec::operator const CVec *() const
{
	return m_vec;
}

CXVec::operator CVec *()
{
	return m_vec;
}

CXVec::operator const CVec &() const
{
	return *m_vec;
}

CXVec::operator CVec &()
{
	return *m_vec;
}

//------------davide-----------------
int CXVec::GetX() const
{
	return gMod.CVecGetX(m_vec);
}
int CXVec::GetY() const
{
	return gMod.CVecGetY(m_vec);
}
//--------------------------

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXRect
//*************************************************************************************************************
//*************************************************************************************************************

CXRect::CXRect () { m_rect = gMod.CRectNew(); }
CXRect::CXRect ( const CFRect *rect ) { m_rect = gMod.CRectNew(); gMod.CRectCopy(rect,m_rect); }
CXRect::CXRect ( const CXRect &rect ) { m_rect = gMod.CRectNew(); gMod.CRectCopy(rect.m_rect,m_rect); }
CXRect::~CXRect() { gMod.CRectDel(m_rect); }

CXRect::operator const CFRect *() const { return m_rect; }
CXRect::operator const CFRect &() const { return *m_rect; }

CXRect::operator CFRect *() { return m_rect; }
CXRect::operator CFRect &() { return *m_rect; }

void CXRect::Set( int topx, int topy, int botx, int boty ) { gMod.CRectSet(m_rect,topx,topy,botx,boty);}

int CXRect::TopX(bool bSet, int ival)	{ return gMod.CRectTopX(m_rect,bSet,ival);}
int CXRect::TopY(bool bSet, int ival)	{ return gMod.CRectTopY(m_rect,bSet,ival);}
int CXRect::BotX(bool bSet, int ival)	{ return gMod.CRectBotX(m_rect,bSet,ival);}
int CXRect::BotY(bool bSet, int ival)	{ return gMod.CRectBotY(m_rect,bSet,ival);}

void CXRect::Norm() { gMod.CRectNorm(m_rect);}
CXVec CXRect::Mid() const { CXVec vec; gMod.CRectMid(m_rect,vec); return vec; }

void CXRect::operator=( const CXRect &rect )
{
	gMod.CRectCopy(rect.m_rect,m_rect);
}

//*************************************************************************************************************
//*************************************************************************************************************
// Class: SXComm
//*************************************************************************************************************
//*************************************************************************************************************

SXComm::SXComm()
{
	m_comm = gMod.SCommNew();
}

SXComm::SXComm(const SComm *ptr)
{
	m_comm = gMod.SCommNew();

	gMod.SCommCopy( ptr, m_comm );
}

SXComm::~SXComm()
{
	gMod.SCommDel(m_comm);
}

int SXComm::Comm(int ival)
{
	return gMod.SCommComm(m_comm,ival);
}

int SXComm::Val(int ival)
{
	return gMod.SCommVal(m_comm,ival);
}

int SXComm::Unit(int ival)
{
	return gMod.SCommUnit(m_comm,ival);
}

SXComm::operator const SComm *() const
{
	return m_comm;
}

SXComm::operator SComm *()
{
	return m_comm;
}

SXComm &SXComm::operator=( const SXComm &comm )	 {gMod.SCommCopy(comm,m_comm); return *this;}
CXVec SXComm::Vec(bool bset, CXVec loc)					 {return gMod.SCommVec(m_comm,bset,loc);}
CXVec SXComm::Dir(bool bset, CXVec dir)					 {return gMod.SCommDir(m_comm,bset,dir);}
int SXComm::Morale(int ival)										 {return gMod.SCommMorale(m_comm,ival);} 

void SXComm::Param( int i, const char *cmd )											 {return gMod.SCommParam(m_comm,i,cmd );}
void SXComm::SetBit(DWORD cbit)																		 {return gMod.SCommSetBit(m_comm,cbit);}
void SXComm::DelBit(DWORD cbit)																		 {return gMod.SCommDelBit(m_comm,cbit);}
bool SXComm::ChkBit(DWORD cbit) const															 {return gMod.SCommChkBit(m_comm,cbit);}
bool SXComm::BuildCommand( const char *command, int from, int to ) {return gMod.SCommBuildCommand(m_comm,command,from,to);}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXPos
//*************************************************************************************************************
//*************************************************************************************************************

CXPos::CXPos()
{
	m_pos = gMod.CPosNew();
}

CXPos::CXPos(const CPos *pos)
{
	m_pos = gMod.CPosNew();

	gMod.CPosCopy(pos,m_pos);
}

CXPos::~CXPos()
{
	gMod.CPosDel(m_pos);
}

CXVec CXPos::Loc() const { return CXVec(gMod.CPosLoc(m_pos)); }
CXVec CXPos::Dir() const { return CXVec(gMod.CPosDir(m_pos)); }

int CXPos::GetAngle() const { return gMod.CPosGetAngle(m_pos); }

int CXPos::GetAngleDiff(CXVec xvec,bool bReal) const { return gMod.CPosGetAngleDiff(m_pos, xvec, bReal  ); }

CXPos::operator const CPos *() const
{
	return m_pos;
}

CXPos::operator const CPos &() const
{
	return *m_pos;
}

void CXPos::operator=( const CXPos &pos )
{
	gMod.CPosCopy(pos.m_pos,m_pos);
}

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXUtil
//*************************************************************************************************************
//*************************************************************************************************************

int CXUtil::Ran( int index, int low, int high )
{
	return gMod.CUtilRan( index, low, high );
}

int CXUtil::GetTime()
{
	return gMod.CUtilGetTime();
}

int CXUtil::Ask(int itype)
{
	return gMod.CUtilAsk(itype);
}

int CXUtil::Vis2()																						 { return gMod.CUtilVis2();}
int CXUtil::OpenType()																				 { return gMod.CUtilOpenType();}
int CXUtil::ScenType()																				 { return gMod.CUtilScenType();}
int CXUtil::GetDifficulty()																		 { return gMod.CUtilGetDifficulty();}
int CXUtil::StratAI ()																				 { return gMod.CUtilStratAI ();}
int CXUtil::MaxCoord()																				 { return gMod.CUtilMaxCoord();}
int CXUtil::YtoU(int yds)                                      { return gMod.CUtilYtoU(yds);}
int CXUtil::UtoY(int mapunits)                                 { return gMod.CUtilUtoY(mapunits);}
int CXUtil::TriggerEvent( int evtid, int unitid, int evttype ) { return gMod.CUtilTriggerEvent( evtid, unitid, evttype );}
int CXUtil::MultSpec(int theint, int theflt, int divfltby)     { return gMod.CUtilMultSpec(theint,theflt,divfltby); }

CXVec CXUtil::FindClosestFort( CXUnit unit, int frange ) { CXVec vec; gMod.CUtilFindClosestFort(unit,frange,vec); return vec; }

bool CXUtil::HexIsFire(CXVec loc)																							  { return gMod.CUtilHexIsFire(loc);}
bool CXUtil::AreMajorsInRect( CXRect rect, int *objs, int army )                { return gMod.CUtilAreMajorsInRect(rect,objs,army);}
bool CXUtil::IsMulti()																													{ return gMod.CUtilIsMulti();}
void CXUtil::AddLog( const char *str )																					{ gMod.CUtilAddLog(str);}
void CXUtil::AddComm( SXComm *comm )																						{ gMod.CUtilAddComm(*comm);}
void CXUtil::AddTimeComm( SXComm *comm, int starttime )													{ gMod.CUtilAddTimeComm(*comm,starttime);}

CXUnit CXUtil::GetSold(int id)										 { return gMod.CUtilGetSold(id);}
SXArmyPlay CXUtil::GetAttackPlay()								 { return gMod.CUtilGetAttackPlay();}
SXArmyPlay CXUtil::GetDefendPlay()								 { return gMod.CUtilGetDefendPlay();}
SXArmyPlay CXUtil::GetMeetingPlay( CXRect rect )	 { return gMod.CUtilGetMeetingPlay(rect);}

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXUnit
//*************************************************************************************************************
//*************************************************************************************************************

CXUnit::CXUnit() { m_unit = NULL; }
CXUnit::CXUnit( CUnit *unit ) { m_unit = (CUnit *)unit; }
CXUnit::operator const CUnit *() const { return m_unit; }
CXUnit::operator CUnit *() { return m_unit; }
int CXUnit::GetQuad( CXUnit sold ) {	return gMod.CUnitGetQuad( m_unit, sold.m_unit ); }
int CXUnit::GetQuad( CXVec &loc ) {	return gMod.CUnitGetQuadVec( m_unit, loc ); }
bool CXUnit::DoesShareHex( CXUnit unit ) { return gMod.CUnitDoesShareHex( m_unit, unit ); }

void CXUnit::RouteUnit()
{
	gMod.CUnitRouteUnit(m_unit);
}


void CXUnit::FollowStop()
{
	gMod.CUnitFollowStop(m_unit);
}


bool CXUnit::CheckRouting() const
{
	return gMod.CUnitCheckRouting(m_unit);
}

void CXUnit::ModMorale(int pts)
{
	gMod.CUnitModMorale(m_unit,pts);
}
//-------davide----------------------
void CXUnit::ModFatigue(int pts)
{
	gMod.CUnitModFatigue(m_unit,pts);
}

//----------------------------------------

int CXUnit::GetUnitTypeVal(int itype) const
{
	return gMod.CUnitGetUnitTypeVal(m_unit,itype);
}

void CXUnit::SetMainTarg( CXUnit sold )
{
	gMod.CUnitSetMainTarg(m_unit,sold);
}


CXUnit CXUnit::MainTarg() const
{
	return CXUnit(gMod.CUnitMainTarg(m_unit));
}


CXUnit CXUnit::FollowTarg() const 
{
	return CXUnit(gMod.CUnitFollowTarg(m_unit));
}


CXUnit CXUnit::GetCharger() const
{
	return CXUnit(gMod.CUnitGetCharger(m_unit));
}


int CXUnit::Ammo() const
{
	return gMod.CUnitAmmo(m_unit);
}

int CXUnit::Rank      (int itype) const { return gMod.CUnitRank      (m_unit,itype); }
int CXUnit::Split     () const { return	gMod.CUnitSplit     (m_unit); }
int CXUnit::State     () const { return	gMod.CUnitState     (m_unit); }
int CXUnit::NumMen    () const { return	gMod.CUnitNumMen    (m_unit); }
int CXUnit::Morale    () const { return	gMod.CUnitMorale    (m_unit); }
int CXUnit::PrevMor   () const { return	gMod.CUnitPrevMor   (m_unit); }
int CXUnit::LimState  () const { return	gMod.CUnitLimState  (m_unit); }
int CXUnit::UnitType  () const { return	gMod.CUnitUnitType  (m_unit); }
int CXUnit::StartMen  () const { return	gMod.CUnitStartMen  (m_unit); }
int CXUnit::NumTargs  () const { return	gMod.CUnitNumTargs  (m_unit); }
int CXUnit::HaltTics  () const { return	gMod.CUnitHaltTics  (m_unit); }
int CXUnit::ShootTime () const { return	gMod.CUnitShootTime (m_unit); }
int CXUnit::MorChange () const { return	gMod.CUnitMorChange (m_unit); }
int CXUnit::MorOffBon () const { return	gMod.CUnitMorOffBon (m_unit); }
int CXUnit::NumSprites() const { return	gMod.CUnitNumSprites(m_unit); }
int CXUnit::MorUnitBon() const { return	gMod.CUnitMorUnitBon(m_unit); }

int CXUnit::GetMod  ( int type ) const { return gMod.CUnitGetMod(m_unit,type); }

int CXUnit::KillTime( int ival )
{
	return gMod.CUnitKillTime(m_unit,ival);
}

int CXUnit::FollowType( int ival )
{
	return gMod.CUnitFollowType(m_unit,ival);
}

int CXUnit::FollowTics( int ival )
{
	return gMod.CUnitFollowTics(m_unit,ival);
}

int CXUnit::FormCloseYds  () const { return gMod.CUnitFormCloseYds  (m_unit); }
int CXUnit::FormIndex     () const { return gMod.CUnitFormIndex     (m_unit); } 
int CXUnit::FormTypeIndex (int ftype) const { return gMod.CUnitFormTypeIndex(m_unit,ftype); } 

CXPos CXUnit::Pos() const { return CXPos(gMod.CUnitPos(m_unit)); }
CXVec CXUnit::Loc() const { return CXVec(gMod.CUnitLoc(m_unit)); }
CXVec CXUnit::Dir() const { return CXVec(gMod.CUnitDir(m_unit)); }
CXVec CXUnit::Dst() const { return CXVec(gMod.CUnitDst(m_unit)); }
CXPos CXUnit::DeP() const { return CXPos(gMod.CUnitDeP(m_unit)); }

CXVec CXUnit::FollowLoc() const { return CXVec(gMod.CUnitFollowLoc(m_unit)); }

bool CXUnit::FindClosestFeature(unsigned int feature,int radyds,CXVec &retloc) const 
{ 
	return gMod.CUnitFindClosestFeature(m_unit,feature,radyds,retloc); 
}

void CXUnit::SetFollowLoc(CXVec &loc) { gMod.CUnitSetFollowLoc(m_unit,loc); }

bool CXUnit::IsArt     () const { return gMod.CUnitIsArt     ( m_unit ); }
bool CXUnit::CanRun    () const { return gMod.CUnitCanRun    ( m_unit ); }
bool CXUnit::Hidden    () const { return gMod.CUnitHidden    ( m_unit ); }
bool CXUnit::CurrTarg  (int ival) { return gMod.CUnitCurrTarg  ( m_unit,ival ); }
bool CXUnit::Moving    (bool bSubs) const { return gMod.CUnitMoving    ( m_unit,bSubs ); }
bool CXUnit::Routed    () const { return gMod.CUnitRouted    ( m_unit ); }
bool CXUnit::Blocked   () const { return gMod.CUnitBlocked   ( m_unit ); }
bool CXUnit::Marching  () const { return gMod.CUnitMarching  ( m_unit ); }
bool CXUnit::CanWheel  () const { return gMod.CUnitCanWheel  ( m_unit ); }
bool CXUnit::CanFight  () const { return gMod.CUnitCanFight  ( m_unit ); }
bool CXUnit::PlyrCtrl  () const { return gMod.CUnitPlyrCtrl  ( m_unit ); }
bool CXUnit::IsSplit   () const { return gMod.CUnitIsSplit   ( m_unit ); }
bool CXUnit::Attached  () const { return gMod.CUnitAttached  ( m_unit ); }
bool CXUnit::IsFlanked () const { return gMod.CUnitIsFlanked ( m_unit ); }
bool CXUnit::IsOfficer () const { return gMod.CUnitIsOfficer ( m_unit ); }
bool CXUnit::CanTarget () const { return gMod.CUnitCanTarget ( m_unit ); }
bool CXUnit::UnderFire () const { return gMod.CUnitUnderFire ( m_unit ); }
bool CXUnit::OnDefTerr () const { return gMod.CUnitOnDefTerr ( m_unit ); }
bool CXUnit::IsHighGrnd() const { return gMod.CUnitIsHighGrnd( m_unit ); }
bool CXUnit::UnderFire1() const { return gMod.CUnitUnderFire1( m_unit ); }
bool CXUnit::Retreating() const { return gMod.CUnitRetreating( m_unit ); }
bool CXUnit::NewDefTerr() const { return gMod.CUnitNewDefTerr( m_unit ); }

bool CXUnit::SwitchCmn() { return gMod.CUnitSwitchCmn(m_unit); }

bool CXUnit::CanHaltTerrain() { return gMod.CUnitCanHalt(m_unit,NULL); }

bool CXUnit::CanHaltUnitRect( const CXVec *arect ) { return gMod.CUnitCanHalt(m_unit,*arect); }

bool CXUnit::FCommand( int command, int fval  ) { return gMod.CUnitFCommand(m_unit,command,fval); }
bool CXUnit::ICommand( int command, int val   ) { return gMod.CUnitICommand(m_unit,command,val); }
bool CXUnit::ICommand( int command, CXVec vec ) { return gMod.CUnitICommand2(m_unit,command,vec); }

bool CXUnit::CanCharge( CXUnit targ, bool bMelee ) const { return gMod.CUnitCanCharge(m_unit,targ.m_unit,bMelee); }
bool CXUnit::ValidWalk( const CXVec &vec ) const { return gMod.CUnitValidWalk( m_unit, vec ); }

bool CXUnit::ValidDest   ( const CXVec &vec ) const { return gMod.CUnitValidDest  ( m_unit, vec ); }
bool CXUnit::ValidSpot   ( const CXVec &vec ) const { return gMod.CUnitValidSpot  ( m_unit, vec ); }
bool CXUnit::IsShotTarg  ( int i            ) const { return gMod.CUnitIsShotTarg ( m_unit, i ); }
bool CXUnit::IsValidShot ( int isprite      ) const { return gMod.CUnitIsValidShot( m_unit, isprite ); }
bool CXUnit::IsFormType  (int ftype) const { return gMod.CUnitIsFormType ( m_unit, ftype ); }
bool CXUnit::GetBackwards() const { return gMod.CUnitGetBackwards( m_unit ); }

//-----davide ------new fucntions for the modders--------
int  CXUnit::DBonusVal() const { return gMod.CUnitDBonusVal( m_unit ); }
int  CXUnit::GetLevel(int ePointName) const { return gMod.CUnitGetLevel( m_unit, ePointName ); }
int  CXUnit::GetPoints(int ePointName) const { return gMod.CUnitGetPoints( m_unit, ePointName ); }
int  CXUnit::HGround() const { return gMod.CUnitHGround( m_unit ); }
int  CXUnit::UnitAIType() const { return gMod.CUnitUnitAIType( m_unit ); }
bool CXUnit::UsingRoad() const { return gMod.CUnitUsingRoad( m_unit ); }
//------------------------------------------------------------
bool CXUnit::InFort         () const           { return gMod.CUnitInFort    ( m_unit ); }
bool CXUnit::CanFormFire    () const           { return gMod.CUnitCanFormFire    ( m_unit ); }
bool CXUnit::WheelToTarget  ( int iunit )      { return gMod.CUnitWheelToTarget  ( m_unit, iunit ); }
bool CXUnit::WheelToTarget  ( CXUnit target ) { return gMod.CUnitWheelToTarget2 ( m_unit, target.m_unit ); }
bool CXUnit::UnderRecentFire() const           { return gMod.CUnitUnderRecentFire( m_unit ); }

int CXUnit::VolleyStatus() const { return gMod.CUnitVolleyStatus(m_unit); }

bool CXUnit::JustWheeled(int ival) { return gMod.CUnitJustWheeled(m_unit,ival); }
bool CXUnit::IsSprinting(int ival) { return gMod.CUnitIsSprinting(m_unit,ival); }
bool CXUnit::WantCover  (int ival) { return gMod.CUnitWantCover  (m_unit,ival); }
bool CXUnit::AttackMarch(int ival) { return gMod.CUnitAttackMarch(m_unit,ival); }

void CXUnit::TargBeg () const { gMod.CUnitTargBeg(m_unit); }
bool CXUnit::TargNext( CXUnit &sold, int &dist ) const 
{ 
	CUnit *unit = sold.m_unit;

	bool bres = gMod.CUnitTargNext(m_unit,unit,dist); 

	sold.m_unit = unit;

	return bres;
} 

void CXUnit::EnemyBeg () const { gMod.CUnitEnemyBeg(m_unit); }
bool CXUnit::EnemyNext( CXUnit &sold, int &dist ) const 
{ 
	CUnit *unit = sold.m_unit;

	bool bres = gMod.CUnitEnemyNext(m_unit,unit,dist); 

	sold.m_unit = unit;

	return bres;
} 

void CXUnit::FriendBeg () const { gMod.CUnitFriendBeg(m_unit); }
bool CXUnit::FriendNext( CXUnit &sold, int &dist ) const 
{ 
	CUnit *unit = sold.m_unit;

	bool bres = gMod.CUnitFriendNext(m_unit,unit,dist); 

	sold.m_unit = unit;

	return bres;
} 

bool CXUnit::GetValidDest( const CXPos &spot, CXVec *pt, bool bExtra ) const 
{ 
	return gMod.CUnitGetValidDest(m_unit,spot,*pt,bExtra);
}

void CXUnit::ValidatePath()
{ 
	gMod.CUnitValidatePath(m_unit);
}

CXUnit CXUnit::TargClose() const { return CXUnit(gMod.CUnitTargClose(m_unit)); }

int  CXUnit::TargCloseYds () const { return gMod.CUnitTargCloseYds(m_unit); }
void CXUnit::TargAdd      ( CXUnit targ, int dist ) { gMod.CUnitTargAdd(m_unit, targ.m_unit,dist ); }
bool CXUnit::TargFind     ( CXUnit unit, int &dist ) const { return gMod.CUnitTargFind(m_unit, unit.m_unit,dist ); }
bool CXUnit::EnemyFind    ( CXUnit unit, int &dist ) const { return gMod.CUnitEnemyFind(m_unit,unit.m_unit,dist); }
int  CXUnit::EnemyCloseYds() const { return gMod.CUnitEnemyCloseYds(m_unit); }
int  CXUnit::MaxRangeYds  () const { return	gMod.CUnitMaxRangeYds  (m_unit); }
int  CXUnit::LongRangeYds () const { return gMod.CUnitLongRangeYds (m_unit); }

CXVec CXUnit::GetLocLeft  ( int yds ) const { CXVec vec; gMod.CUnitGetLocLeft  (m_unit,yds,vec); return vec; }
CXVec CXUnit::GetLocRight ( int yds ) const { CXVec vec; gMod.CUnitGetLocRight (m_unit,yds,vec); return vec; }
CXVec CXUnit::GetLocAhead ( int yds ) const { CXVec vec; gMod.CUnitGetLocAhead (m_unit,yds,vec); return vec; }
CXVec CXUnit::GetLocBehind( int yds ) const { CXVec vec; gMod.CUnitGetLocBehind(m_unit,yds,vec); return vec; }
CXUnit CXUnit::EnemyClose () const { return CXUnit(gMod.CUnitEnemyClose(m_unit)); }
CXUnit CXUnit::FriendClose() const { return CXUnit(gMod.CUnitFriendClose(m_unit)); }

void   CXUnit::SetClosest( CXUnit unit        ) { gMod.CUnitSetClosest(m_unit, unit.m_unit ); }
void   CXUnit::SetFollow ( CXUnit unit       ) { gMod.CUnitSetFollow (m_unit, unit.m_unit ); }
void   CXUnit::SetDest   ( const CXVec &dest  ) { gMod.CUnitSetDest   (m_unit, dest ); }
void   CXUnit::SetDest   ( const CXPos &dest  ) { gMod.CUnitSetDest2  (m_unit, dest ); }
bool   CXUnit::Command   ( const SXComm *comm ) { return gMod.CUnitCommand( m_unit, *comm ); }
CXOff  CXUnit::Leader    () const               { return CXOff (gMod.CUnitLeader ( m_unit )); }
CXUnit CXUnit::Closest   () const               { return CXUnit(gMod.CUnitClosest( m_unit )); }

void CXUnit::Repath() { gMod.CUnitRepath(m_unit); }

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CheckPath
// Should only be used when in the mix of fighting
//*************************************************************************************************************
//*************************************************************************************************************

bool CXUnit::CheckPath    ()          { return gMod.CUnitCheckPath    (m_unit); }
int  CXUnit::MountState   () const    { return gMod.CUnitMountState   (m_unit); }
int  CXUnit::RunTimer     ( int val ) { return gMod.CUnitRunTimer     (m_unit, val ); }
int  CXUnit::FireMode     () const	  { return gMod.CUnitFireMode     (m_unit); }
int  CXUnit::DistArtyConsv() const		{ return gMod.CUnitDistArtyConsv(m_unit); }
int  CXUnit::DistArtyPanic() const		{ return gMod.CUnitDistArtyPanic(m_unit); }
void CXUnit::Limber       ()					{ return gMod.CUnitLimber       (m_unit); }
int  CXUnit::AmmoCmd      () const		{ return gMod.CUnitAmmoCmd      (m_unit); }
int  CXUnit::AmmoType     ( int val ) { return gMod.CUnitAmmoType     (m_unit, val ); }

void CXUnit::ChooseAmmo        ( int distyds, CXUnit closest ) { gMod.CUnitChooseAmmo( m_unit, distyds, closest ); } 

bool CXUnit::HasInfOrArtFriends( const int &wdist2, int imax ) const { return gMod.CUnitHasInfOrArtFriends( m_unit, wdist2, imax ); } 

bool CXUnit::PrevTarg      (int ival)				{ return gMod.CUnitPrevTarg      (m_unit,ival);}
int  CXUnit::GetID         ()	const					{ return gMod.CUnitGetID         (m_unit) ;}
bool CXUnit::ForceArtyMove ()	const					{ return gMod.CUnitForceArtyMove (m_unit) ;}
void CXUnit::GiveMeAmmo    ( CXUnit sold )	{ return gMod.CUnitGiveMeAmmo    (m_unit,sold );}
bool CXUnit::NeedAmmo      ( int *i )				{ return gMod.CUnitNeedAmmo      (m_unit,i );}
bool CXUnit::Standing      ()	const					{ return gMod.CUnitStanding      (m_unit);} 
int  CXUnit::FriendCloseYds()	const					{ return gMod.CUnitFriendCloseYds(m_unit);} 
bool CXUnit::FriendEmpt    ()	const					{ return gMod.CUnitFriendEmpt    (m_unit);} 
bool CXUnit::AppSameRank   ()	const					{ return gMod.CUnitAppSameRank   (m_unit);} 
bool CXUnit::InCorp        ()	const					{ return gMod.CUnitInCorp        (m_unit);} 
bool CXUnit::PathEmpty     ()	const					{ return gMod.CUnitPathEmpty     (m_unit);} 
bool CXUnit::MyGuys        ()	const					{ return gMod.CUnitMyGuys        (m_unit);} 
bool CXUnit::IsMe          ()	const					{ return gMod.CUnitIsMe          (m_unit);} 
bool CXUnit::TCOn          ()	const					{ return gMod.CUnitTCOn          (m_unit);} 
bool CXUnit::Locked        ()	const					{ return gMod.CUnitLocked        (m_unit);} 
																						
const char *CXUnit::Name() const		{ return gMod.CUnitName(m_unit);}

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXOff
//*************************************************************************************************************
//*************************************************************************************************************

CXOff::CXOff() : CXUnit()
{
}

CXOff::CXOff( CUnit *unit ) : CXUnit(unit)
{
}

bool CXOff::CanICharge        ( CXUnit unit, int ival ) const	{ return gMod.COffCanICharge( m_unit, unit, ival ); } 
bool CXOff::StopOnDefTerr     ( CXUnit unit ) const	{ return gMod.COffStopOnDefTerr( m_unit, unit ); } 
bool CXOff::ShouldGetAmmo     ( CXUnit unit ) const	{ return gMod.COffShouldGetAmmo( m_unit, unit ); } 
bool CXOff::ShouldAttack      ( bool bReg, CXUnit targ, CXUnit unit, bool bWeight ) { return gMod.COffShouldAttack( m_unit, bReg, targ, unit, bWeight ); } 
void CXOff::GetScoutLoc       ( CXUnit cav, CXVec &loc, bool bNew ) const { gMod.COffGetScoutLoc( m_unit, cav, loc, bNew ); } 
int  CXOff::Personality       () const				{ return gMod.COffPersonality( m_unit); } 
int  CXOff::ChargeArtyDistYds ()	{ return gMod.COffChargeArtyDistYds( m_unit); } 
bool CXOff::FindClosestSpec   ( int type, const CXVec &loc, CXVec &ammo ) const { return gMod.COffFindClosestSpec( m_unit, type, loc, ammo ); } 

int CXOff::TicksForOffSDK( int iwhich, int ival ) { return gMod.COffTicksForOffSDK(m_unit,iwhich,ival); } 

bool CXOff::GetLineDest    ( CXUnit unit, CXVec &pt )							 { return gMod.COffGetLineDest    (m_unit,unit,pt ); } 
bool CXOff::GetFlankDest   ( CXUnit unit, CXVec &pt, CXUnit targ ) { return gMod.COffGetFlankDest   (m_unit,unit,pt,targ ); } 
bool CXOff::GetForwardDest ( CXUnit unit, CXVec &pt, CXUnit targ ) { return gMod.COffGetForwardDest (m_unit,unit,pt,targ ); } 
bool CXOff::GetMaintainDest( CXUnit unit, CXVec &pt, CXVec &dir  ) { return gMod.COffGetMaintainDest(m_unit,unit,pt,dir  ); } 

const TStance *CXOff::GetStance() const { return gMod.COffGetStance(m_unit); }                          

const TTacOrders *CXOff::GetOrders() const 
{ 
	const TTacOrders *ord = gMod.COffGetOrders(m_unit);

	if ( !ord )
		ord = gMod.gTableTacOrders; 
	
	return ord;
}

CXUnit CXOff::CheckWheel    ()                       { return gMod.COffCheckWheel    (m_unit); }                          
CXUnit CXOff::GetSaveClosest( int &dist )					   { return gMod.COffGetSaveClosest(m_unit, dist );}
bool CXOff::HasStance () const	                     { return gMod.COffHasStance (m_unit);}
bool CXOff::IQuit     ()														 { return gMod.COffIQuit     (m_unit);}
bool CXOff::Orders    ( SXComm *comm )			         
{ 
	if ( comm )
		return gMod.COffOrders(m_unit, *comm);

	return gMod.COffOrders(m_unit, NULL);
}
bool CXOff::OffThink  ()														 { return gMod.COffOffThink  (m_unit);}
bool CXOff::InTrouble ()														 { return gMod.COffInTrouble (m_unit);}
bool CXOff::HasOrders () const                   	   { return gMod.COffHasOrders (m_unit);}
bool CXOff::HasTargets() const											 { return gMod.COffHasTargets(m_unit);}
int CXOff::CourTime   ( bool bSet, int ival )				 { return gMod.COffCourTime   (m_unit, bSet,ival );}
int CXOff::FightForm  () const				               { return gMod.COffFightForm  (m_unit);}
int CXOff::WheelTime  ( bool bSet, int ival )				 { return gMod.COffWheelTime  (m_unit, bSet,ival );}
bool CXOff::ReEval    ( bool bSet, bool bval )			 { return gMod.COffReEval    (m_unit, bSet, bval );}
bool CXOff::WantAttach( bool bSet, bool bval )			 { return gMod.COffWantAttach(m_unit,  bSet, bval );}
void CXOff::CheckEnemySee     ()										 { gMod.COffCheckEnemySee     (m_unit);}
void CXOff::CheckEnemyEng     ()										 { gMod.COffCheckEnemyEng     (m_unit);}
void CXOff::ReEvalTactics     ()										 { gMod.COffReEvalTactics     (m_unit);}
void CXOff::ObjectiveCompleted()										 { gMod.COffObjectiveCompleted(m_unit);}
bool CXOff::Fighting() const												 { return gMod.COffFighting(m_unit);}
bool CXOff::UnderCloseFire() const									 { return gMod.COffUnderCloseFire(m_unit);}
int   CXOff::TACType() const												 { return gMod.COffTACType(m_unit);}
int   CXOff::TACObj () const												 { return gMod.COffTACObj (m_unit);}
CXVec CXOff::TACLoc () const												 { return gMod.COffTACLoc (m_unit);}
const char *CXOff::TACOrders() const								 { return gMod.COffTACOrders(m_unit);}
int CXOff::ORDTACMod() const												 { return gMod.COffORDTACMod(m_unit);}
const char *CXOff::ORDCall() const									 { return gMod.COffORDCall(m_unit);}
int CXOff::NumSubs() const													 { return gMod.COffNumSubs(m_unit);}
CXUnit CXOff::Sub(int i)														 { return gMod.COffSub(m_unit, i);}
int CXOff::NumArty() const													 { return gMod.COffNumArty(m_unit);}
CXUnit CXOff::Arty(int i)														 { return gMod.COffArty(m_unit, i);}

int CXOff::NumDetach() const								 { return gMod.COffNumDetach(m_unit);}
CXUnit CXOff::Det(int i)										 { return gMod.COffDet(m_unit,i);}
int   CXOff::TACOBJRad2() const							 { return gMod.COffTACOBJRad2(m_unit);}
bool  CXOff::TACOBJDone() const							 { return gMod.COffTACOBJDone(m_unit);}
CXVec CXOff::TACOBJLoc () const							 { return gMod.COffTACOBJLoc (m_unit);}
bool CXOff::ORDTACDivPlay() const						 { return gMod.COffORDTACDivPlay(m_unit);}
int CXOff::OrdersState( int ival )			     { return gMod.COffOrdersState( m_unit,ival );}
const char *CXOff::AssignedOrders() const		 { return gMod.COffAssignedOrders(m_unit);}

CXUnit CXOff::GetClosest  ( int &dist, bool bSubsOnly )          { return gMod.COffGetClosest(m_unit,dist,bSubsOnly );}
CXUnit CXOff::SupportUnits( bool btarg, bool bForce, bool bGet ) { return gMod.COffSupportUnits(m_unit,btarg,bForce,bGet );}
bool CXOff::GetOrders( const int &dist, CXUnit targ, CXUnit unit ) { return gMod.COffGetOrders2( m_unit,dist, targ, unit );}
void CXOff::SendOrdersByCour( CXUnit to, int count, SXComm *comm, bool bCheck, const char *orders, bool bForce)
{ return gMod.COffSendOrdersByCour( m_unit,to, count, *comm, bCheck, orders, bForce);}

bool CXOff::OffScared       ()												 { return gMod.COffOffScared       (m_unit);}
bool CXOff::RunningPlay     () const									 { return gMod.COffRunningPlay     (m_unit);}
bool CXOff::OrdersAllowWheel() const									 { return gMod.COffOrdersAllowWheel(m_unit);}
bool CXOff::EngagingEnemy() const											 { return gMod.COffEngagingEnemy(m_unit);}
bool CXOff::UnLimbered() const												 { return gMod.COffUnLimbered(m_unit);}
bool CXOff::HasPlay() const														 { return gMod.COffHasPlay(m_unit);}
bool CXOff::PSTATDone(bool bSet, bool val)						 { return gMod.COffPSTATDone(m_unit,bSet,val);}
bool CXOff::PSTATLValid(int i, bool bSet, bool val)		 { return gMod.COffPSTATLValid(m_unit,i,bSet,val);}
bool CXOff::PSTATRValid(int i, bool bSet, bool val)		 { return gMod.COffPSTATRValid(m_unit,i,bSet,val);}

void CXOff::MoveToBestGround( int rad, bool bReEval )										{ gMod.COffMoveToBestGround(m_unit,rad,bReEval );}
void CXOff::Rally()																											{ gMod.COffRally(m_unit);}
void CXOff::DelPlay()																										{ gMod.COffDelPlay(m_unit);}
void CXOff::EvalLoc      ()																							{ gMod.COffEvalLoc      (m_unit);}
void CXOff::EvalQuad     ()																							{ gMod.COffEvalQuad     (m_unit);}
void CXOff::EvalObjective()																							{ gMod.COffEvalObjective(m_unit);}
void CXOff::CheckReserves()																							{ gMod.COffCheckReserves(m_unit);}
void CXOff::ExecutePlay( bool bLeft, int spot, CXOff off, int cavspot )	{ gMod.COffExecutePlay(m_unit,bLeft,spot,off,cavspot);}
void CXOff::RunPlay( CXOff best, CXUnit targ )													{ gMod.COffRunPlay(m_unit,best,targ );}
void CXOff::CompleteAdd(int i)																					{ gMod.COffCompleteAdd(m_unit,i);}
void CXOff::RunArmyPlay( CXUnit off, CXRect &rect )											{ gMod.COffRunArmyPlay(m_unit,off,rect );}

const char *CXOff::PLAYLOrdersCall(int i) const			{ return gMod.COffPLAYLOrdersCall(m_unit,i);}
CXVec	CXOff::PSTATLRct(int which) const							{ return gMod.COffPSTATLRct(m_unit,which);}
CXOff CXOff::PSTATLOff(int i)												{ return gMod.COffPSTATLOff(m_unit,i);}
CXOff CXOff::PSTATROff(int i)												{ return gMod.COffPSTATROff(m_unit,i);}
CXRect CXOff::PLAYRect(SXArmyPlay play,int i) const	{ return gMod.COffPLAYRect(m_unit,play,i);}


// army
SXArmyPlay CXOff::ArmyPlay(bool bSet, SXArmyPlay aplay ) { return gMod.COffArmyPlay(m_unit,bSet, aplay );}

int CXOff::HelpTime( bool bSet, int ival )						{ return gMod.COffHelpTime(m_unit,bSet,ival );}
int CXOff::NumComplete() const												{ return gMod.COffNumComplete(m_unit);}
int CXOff::Complete(int i) const											{ return gMod.COffComplete(m_unit,i);}
int CXOff::StartQuad() const													{ return gMod.COffStartQuad(m_unit);}
int CXOff::PLAYCorpNum(SXArmyPlay play, int i) const	{ return gMod.COffPLAYCorpNum(m_unit,play,i);}
int CXOff::PLAYFTime  (SXArmyPlay play, int i) const	{ return gMod.COffPLAYFTime  (m_unit,play,i);}
int CXOff::PLAYSize   (SXArmyPlay play) const					{ return gMod.COffPLAYSize   (m_unit,play);}
bool CXOff::GetMeetAttackQuad( CXRect &rect )					{ return gMod.COffGetMeetAttackQuad(m_unit,rect );}

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXCour
//*************************************************************************************************************
//*************************************************************************************************************

CXCour::CXCour() : CXUnit()
{
}

CXCour::CXCour( CUnit *unit ) : CXUnit(unit)
{
}

int CXCour::TargId    ( bool bSet, int ival)	{ return gMod.CCourTargId    (m_unit,bSet, ival);}
int CXCour::NumCour() const										{ return gMod.CCourNumCour(m_unit);}
int CXCour::NumComm(bool bSet, int ival)			{ return gMod.CCourNumComm   (m_unit,bSet, ival);}
int CXCour::StartTime ( bool bSet, int ival)	{ return gMod.CCourStartTime (m_unit, bSet, ival);}
int CXCour::RecalcTics( bool bSet, int ival)	{ return gMod.CCourRecalcTics(m_unit, bSet, ival);}
bool CXCour::HasComm() const									{ return gMod.CCourHasComm(m_unit);}
const char *CXCour::Orders() const						{ return gMod.CCourOrders(m_unit);}
SXComm CXCour::Comm(int i)										{ return gMod.CCourComm(m_unit,i);}

void CXCour::DelComm()																								{ return gMod.CCourDelComm(m_unit);}
void CXCour::DelOrders()																							{ return gMod.CCourDelOrders(m_unit);}
void CXCour::ShowScreen( bool bCaptured, bool bForce, bool bLogOnly )	{ return gMod.CCourShowScreen(m_unit,bCaptured,bForce,bLogOnly );}

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: STown
//*************************************************************************************************************
//*************************************************************************************************************

SXTown::SXTown() { m_town = NULL; }
SXTown::SXTown( STown *town ) { m_town = town; }
SXTown::SXTown( const SXTown &town ) { m_town = town.m_town; }

SXTown::operator STown *() { return m_town; }

CXVec SXTown::Loc() const { return CXVec(gMod.STownLoc(m_town)); }

int SXTown::Occupied() const { return gMod.STownOccupied(m_town); }
int SXTown::OccupiedTimer() const { return gMod.STownOccupiedTimer(m_town); }

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXCamp
//*************************************************************************************************************
//*************************************************************************************************************

bool CXCamp::TownNext(CXUnit me, SXTown *town, int &dist)
{
	CUnit *unit  = me;
	STown *stown = *town;

	bool bres = gMod.CCampTownNext(unit,stown,dist); 

	*town = stown;

	return bres;
}

SXTown CXCamp::TownClose(CXUnit me, int &dist)
{
	SXTown town = gMod.CCampTownClose(me,dist);

	return town;
}

SXTown CXCamp::StartTown(int which)
{
	SXTown town = gMod.CCampStartTown(which);

	return town;
}

SXTown CXCamp::TargTown()
{
	SXTown town = gMod.CCampTargTown();

	return town;
}

//----davide new campaign AI functions-----------
void CXCamp::SetForceTimer(CXUnit me, int timer)
{
	gMod.CCampSetForceTimer(me,timer);
}
int CXCamp::GetForceTimer(CXUnit me)
{
	return gMod.CCampGetForceTimer(me);
}

//-----------------------------------------------
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
















