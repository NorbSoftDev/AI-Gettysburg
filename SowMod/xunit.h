//*************************************************************************************************************
//*************************************************************************************************************
// File: xunit.h
//*************************************************************************************************************
//*************************************************************************************************************

#pragma once

#include <vector>

class CVec;
class CPos;
class CUnit;
class CFRect;
class CXUnit;
class SArmyPlay;

struct SComm;
struct STown;
struct TStance;
struct TTacOrders;

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXVec
//*************************************************************************************************************
//*************************************************************************************************************

class CXVec
{
	public:
		CXVec ();
		CXVec ( const CVec  *vec );
		CXVec ( const CXVec &vec );
		~CXVec();

		bool operator==( const CXVec &vec ) const;
		bool operator!=( const CXVec &vec ) const;

		operator const CVec *() const;
		operator const CVec &() const;

		operator CVec *();
		operator CVec &();

		bool empty() const;

		int DistYds ( const CXVec &vec ) const;
		int Dist2Yds( const CXVec &vec ) const;

		void CopyTo( CVec *vec ) const;

		void operator =(const CXVec &vec);

//--------davide--------------------------------
		int GetX() const;
		int GetY() const;
//----------------------------------------------

	private:
		CVec *m_vec;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXRect
//*************************************************************************************************************
//*************************************************************************************************************

class CXRect
{
	public:
		CXRect ();
		CXRect ( const CFRect *rect );
		CXRect ( const CXRect &rect );
		~CXRect();

		operator const CFRect *() const;
		operator const CFRect &() const;

		operator CFRect *();
		operator CFRect &();

		void Set( int topx, int topy, int botx, int boty );

		int TopX(bool bSet = false, int ival = -1);
		int TopY(bool bSet = false, int ival = -1);
		int BotX(bool bSet = false, int ival = -1);
		int BotY(bool bSet = false, int ival = -1);

		void Norm();

		CXVec Mid() const;

		void operator = (const CXRect &rect);

	protected:
		CFRect *m_rect;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: SXComm
//*************************************************************************************************************
//*************************************************************************************************************

class SXComm
{
	public:
		SXComm();
		SXComm(const SComm *);
		~SXComm();

		SXComm &operator=( const SXComm &comm );

		int Comm(int ival = -1);
		int Val (int ival = -1);
		int Unit(int ival = -1);

		CXVec Vec(bool bset = false, CXVec loc = CXVec() );
		CXVec Dir(bool bset = false, CXVec loc = CXVec() );

		int Morale(int ival = -1);

		operator SComm *();
		operator const SComm *() const;

		void Param( int i, const char *cmd );

		void SetBit(DWORD cbit);
		void DelBit(DWORD cbit);

		bool ChkBit(DWORD cbit) const;

		bool BuildCommand( const char *command, int from = -1, int to = -1 );

	private:
		SComm *m_comm;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXPos
//*************************************************************************************************************
//*************************************************************************************************************

class CXPos
{
	public:
		CXPos();
		CXPos(const CPos *pos);
		~CXPos();

		CXVec Loc() const;
		CXVec Dir() const;

		int GetAngle() const;
		int GetAngleDiff(CXVec xvec, bool bReal = false ) const;
		operator const CPos *() const;
		operator const CPos &() const;

		operator CPos *();
		operator CPos &();

		void operator = (const CXPos &pos);

	private:
		CPos *m_pos;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: SXArmyPlay
//*************************************************************************************************************
//*************************************************************************************************************

class SXArmyPlay
{
	public:
		SXArmyPlay() { m_play = NULL; }
		SXArmyPlay( void *play ) { m_play = (SArmyPlay *)play; }

		operator const SArmyPlay *() const { return m_play; }
		operator SArmyPlay *() { return m_play; }

		bool operator==( const SXArmyPlay &play ) const { return m_play == play.m_play; }
		bool operator==( const SXArmyPlay *play ) const { return play == NULL ? m_play == NULL : m_play == play->m_play; }

		bool operator!=( const SXArmyPlay &play ) const { return m_play != play.m_play; }
		bool operator!=( const SXArmyPlay *play ) const { return play == NULL ? m_play != NULL : m_play != play->m_play; }

		SXArmyPlay &operator=( const SXArmyPlay &play ) { m_play = play.m_play; return *this; }

	protected:
		SArmyPlay *m_play;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXUtil
//*************************************************************************************************************
//*************************************************************************************************************

class CXUtil
{
	public:
		static int Ran    ( int index, int low, int high );
		static int GetTime();

		static int Vis2();

		static int Ask( int itype );

		static bool HexIsFire(CXVec loc);

		static void AddLog( const char *str );

		static bool IsMulti();

		static CXUnit GetSold(int id);

		static int MultSpec( int theint, int theflt, int divfltby );

		static int YtoU(int yds);
		static int UtoY(int mapunits);

		static int OpenType();
		static int ScenType();
		static int StratAI ();

		static int MaxCoord();

		static int GetDifficulty();
		//	else if ( CGlob::gOpt[eOVDiff].val != eDiffGreen )

		static CXVec FindClosestFort( CXUnit unit, int frange );

		static void AddComm( SXComm *comm );

		static void AddTimeComm( SXComm *comm, int starttime );

		static SXArmyPlay GetAttackPlay(); 
		static SXArmyPlay GetDefendPlay();
		static SXArmyPlay GetMeetingPlay( CXRect rect );

		static int TriggerEvent( int evtid, int unitid, int evttype = -1 );

		static bool AreMajorsInRect( CXRect rect, int *majobj, int army );
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXUnit
//*************************************************************************************************************
//*************************************************************************************************************

class CXOff;

class CXUnit
{
	public:
		CXUnit();
		CXUnit( CUnit *unit );

		operator const CUnit *() const;
		operator CUnit *();

		bool operator==( const CXUnit &unit ) const { return m_unit == unit.m_unit; }
		bool operator==( const CXUnit *unit ) const { return unit == NULL ? m_unit == NULL : m_unit == unit->m_unit; }

		bool operator!=( const CXUnit &unit ) const { return m_unit != unit.m_unit; }
		bool operator!=( const CXUnit *unit ) const { return unit == NULL ? m_unit != NULL : m_unit != unit->m_unit; }

		bool operator!() const { return !Valid(); }

		CXUnit &operator=( const CXUnit &unit ) { m_unit = unit.m_unit; return *this; }

		bool Valid() const { return m_unit != NULL; }

		int GetID() const;
		int GetQuad( CXUnit sold );
		int GetQuad( CXVec &loc );

		void ModMorale   ( int pts );
//--------davide----------------------------------------
		void ModFatigue (int pts );
//--------------------------------------------------------
		bool DoesShareHex( CXUnit unit );
		void RouteUnit();
		void FollowStop();
		bool CheckRouting() const;

		int GetUnitTypeVal(int itype) const;

		void SetMainTarg( CXUnit sold );

		CXUnit MainTarg  () const;
		CXUnit FollowTarg() const;
		CXUnit GetCharger() const;

		const char *Name() const;

		bool ForceArtyMove() const;

		int Ammo      () const;
		int Rank      (int itype = -1) const;
		int Split     () const;             //-1 if the unit is not a splitted unit, otherwise the id of parent unit
		int State     () const;
		int NumMen    () const;
		int GetMod    ( int type ) const;
		int Morale    () const;
		int PrevMor   () const;
		int LimState  () const;
		int UnitType  () const;
		int StartMen  () const;
		int KillTime  ( int val = -1 ); 
		int NumTargs  () const; 
		int HaltTics  () const;
		int MorChange () const;
		int MorOffBon () const;
		int ShootTime () const;
		int FollowType( int val = -1 );
		int NumSprites() const;
		int MorUnitBon() const;
		int FollowTics( int val = -1 );

		int FormCloseYds () const;
		int FormIndex    () const;
		int FormTypeIndex(int ftype) const;

		CXPos Pos() const;
		CXVec Loc() const;
		CXVec Dir() const;
		CXVec Dst() const;
		CXPos DeP() const;

		CXVec FollowLoc() const;
		bool  FindClosestFeature( unsigned int feature, int radyds, CXVec &vec ) const;   //SKD_DBONUS (30+ bonus) and SKD_FORT for fortifications
		void  SetFollowLoc(CXVec &loc);

		void GiveMeAmmo( CXUnit sold );

		bool CanHaltTerrain ();
		bool CanHaltUnitRect( const CXVec *arect );

		bool IsMe        () const;
		bool TCOn        () const;
		bool IsArt       () const;
		bool InCorp      () const;
		bool InFort      () const;
		bool CanRun      () const;
		bool MyGuys      () const;
		bool Hidden      () const;
		bool Moving      (bool bSubs = false) const;
		bool Routed      () const;
		bool Locked      () const;
		bool Blocked     () const;
		bool NeedAmmo    ( int * = NULL );
		bool IsSplit     () const;
		bool Standing    () const;
		bool Marching    () const;
		bool CanWheel    () const;
		bool CanFight    () const;
		bool ICommand    ( int command, int val = 0 );
		bool FCommand    ( int command, int fval = 0 );
		bool ICommand    ( int command, CXVec vec );
		bool Attached    () const;
		bool PlyrCtrl    () const;
		bool CurrTarg    (int ival = -1);
		bool PrevTarg    (int ival = -1);
		bool PathEmpty   () const;
		bool IsFlanked   () const;
		bool IsOfficer   () const;
		bool CanCharge   ( CXUnit targ = NULL, bool bMelee = false ) const;
		bool CanTarget   () const;
		bool UnderFire   () const;  //if the unit has a dead in the last 3 minutes
		bool ValidWalk   ( const CXVec &vec ) const;
		bool SwitchCmn   ();
		bool ValidDest   ( const CXVec &vec ) const;
		bool ValidSpot   ( const CXVec &vec ) const;
		bool OnDefTerr   () const;
		bool IsHighGrnd  () const;
		bool UnderFire1  () const;	//if the unit has a dead in the last minute
		bool Retreating  () const;
		bool NewDefTerr  () const;
		bool IsShotTarg  ( int isprite ) const;
		bool IsValidShot ( int isprite ) const;
		bool IsFormType  ( int ftype ) const;
		bool GetBackwards() const;
//----davide new functions for the modders---
		int  DBonusVal() const;
		int  GetLevel(int ePointName) const;
		int  GetPoints(int ePointName) const;
		int  HGround() const;
		int  UnitAIType() const;
		bool UsingRoad() const;
//--------------------------------------------

		bool CanFormFire    () const;
		bool WheelToTarget  ( int iunit );
		bool WheelToTarget  ( CXUnit target );
		bool UnderRecentFire() const;  //if the unit has a dead in the last frame

		bool JustWheeled ( int ival = -1 );
		int  VolleyStatus() const;
		bool IsSprinting ( int ival = -1 );
		bool WantCover   ( int ival = -1 );
		bool AttackMarch ( int ival = -1 );

		void TargBeg () const;
		bool TargNext( CXUnit &sold, int &dist ) const;

		void EnemyBeg () const;
		bool EnemyNext( CXUnit &sold, int &dist ) const;

		bool FriendEmpt() const;
		void FriendBeg () const;
		bool FriendNext( CXUnit &sold, int &dist ) const;

		void ValidatePath();
		bool GetValidDest( const CXPos &spot, CXVec *pt, bool bExtra ) const;

		bool AppSameRank() const;

		bool   TargFind    ( CXUnit unit, int &dist ) const; 
		CXUnit TargClose   () const;
		int    TargCloseYds() const;
		void   TargAdd     ( CXUnit targ, int dist );

		bool EnemyFind( CXUnit unit, int &dist ) const;

		CXUnit EnemyClose   () const;
		CXUnit FriendClose  () const;
		int    EnemyCloseYds() const;

		int    FriendCloseYds() const;

		int MaxRangeYds () const;
		int LongRangeYds() const;

		CXVec GetLocLeft  ( int yds ) const;
		CXVec GetLocRight ( int yds ) const;
		CXVec GetLocAhead ( int yds ) const;
		CXVec GetLocBehind( int yds ) const;

		void SetClosest( CXUnit unit );
		void SetFollow ( CXUnit unit );

		CXOff  Leader () const;
		CXUnit Closest() const;

		void SetDest( const CXVec &dest );
		void SetDest( const CXPos &dest );

		bool Command( const SXComm *comm );

		void Repath();
		bool CheckPath();

		bool HasInfOrArtFriends( const int &wdist2, int imax ) const;

	public: // cav only
		int RunTimer  ( int val = -1 );
		int MountState() const;

	public: // arty only
		int FireMode     () const;
		int DistArtyConsv() const;
		int DistArtyPanic() const;

		void Limber();

		int AmmoCmd () const;
		int AmmoType( int ival = -1 );

		void ChooseAmmo( int distyds, CXUnit closest ); 
	
	protected:
		CUnit *m_unit;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXOff
//*************************************************************************************************************
//*************************************************************************************************************

class CXOff : public CXUnit
{
	public:
		CXOff();
		CXOff( CUnit *unit );

		CXOff &operator=( CUnit *unit ) { m_unit = unit; return *this; }

		bool CanICharge( CXUnit unit, int ival = -1 ) const;

		bool StopOnDefTerr( CXUnit unit ) const;
		bool ShouldGetAmmo( CXUnit unit ) const;

		int Personality() const;

		bool FindClosestSpec( int type, const CXVec &loc, CXVec &ammo ) const; 

		bool ShouldAttack ( bool bReg, CXUnit targ, CXUnit unit, bool bWeight = true );

		int ChargeArtyDistYds();

		const TStance *GetStance() const;
		const TTacOrders *GetOrders() const;

		int TicksForOffSDK( int iwhich, int ival = -1 );

		void GetScoutLoc    ( CXUnit cav, CXVec &loc, bool bNew ) const;
		bool GetLineDest    ( CXUnit unit, CXVec &pt );
		bool GetFlankDest   ( CXUnit unit, CXVec &pt, CXUnit targ );
		bool GetForwardDest ( CXUnit unit, CXVec &pt, CXUnit targ );
		bool GetMaintainDest( CXUnit unit, CXVec &pt, CXVec &dir  );

		CXUnit CheckWheel    ();
		CXUnit GetSaveClosest( int &dist );

		bool HasStance() const;

		bool IQuit     ();
		bool Orders    ( SXComm *comm = NULL );
		bool OffThink  ();
		bool InTrouble ();
		bool HasOrders () const;
		bool HasTargets() const;

		int CourTime ( bool bSet = false, int ival = -1 );
		int FightForm() const; // returns base fight form, or if there is a stance, the fight form from the stance
		int WheelTime( bool bSet = false, int ival = -1 );

		bool ReEval    ( bool bSet = false, bool bval = false );
		bool WantAttach( bool bSet = false, bool bval = false );

		void CheckEnemySee     ();
		void CheckEnemyEng     ();
		void ReEvalTactics     ();
		void ObjectiveCompleted();

		bool Fighting() const;
		bool UnderCloseFire() const;

		int   TACType() const;
		int   TACObj () const;
		CXVec TACLoc () const;

		const char *TACOrders() const;

		int ORDTACMod() const;
		const char *ORDCall() const;

		int NumSubs() const;
		CXUnit Sub(int i);

		int NumArty() const;
		CXUnit Arty(int i);

		int NumDetach() const;
		CXUnit Det(int i);

		int   TACOBJRad2() const;
		bool  TACOBJDone() const;
		CXVec TACOBJLoc () const;
		
		bool ORDTACDivPlay() const;

		int OrdersState( int ival = -1 );

		const char *AssignedOrders() const;

		CXUnit GetClosest  ( int &dist, bool bSubsOnly = false );
		CXUnit SupportUnits( bool btarg, bool bForce = false, bool bGet = false );

		bool OffScared       ();
		bool RunningPlay     () const;
		bool OrdersAllowWheel() const;

		bool GetOrders( const int &dist, CXUnit targ, CXUnit unit = CXUnit() );

		bool EngagingEnemy() const;

		void SendOrdersByCour( CXUnit to, int count, SXComm *comm, bool bCheck, const char *orders = NULL, bool bForce = false );

		// arty only
		void MoveToBestGround( int rad, bool bReEval );
		bool UnLimbered() const;

		// div
		void Rally();

		bool HasPlay() const;
		void DelPlay();

		int HelpTime( bool bSet = false, int ival = -1 );

		const char *PLAYLOrdersCall(int i) const;

		void EvalLoc      ();
		void EvalQuad     ();
		void EvalObjective();
		void CheckReserves();

		bool  PSTATDone(bool bSet = false, bool val = false );
		CXVec	PSTATLRct(int which) const;
		bool  PSTATLValid(int i, bool bSet=false, bool val = false);
		bool  PSTATRValid(int i, bool bSet=false, bool val = false);
		CXOff PSTATLOff(int i);
		CXOff PSTATROff(int i);

		void ExecutePlay( bool bLeft, int spot, CXOff off, int cavspot = -1 );

		void RunPlay( CXOff best, CXUnit targ );

		bool GetMeetAttackQuad( CXRect &rect );

		// army
		SXArmyPlay ArmyPlay(bool bSet = false, SXArmyPlay aplay = SXArmyPlay() );

		int NumComplete() const;
		int Complete(int i) const;
		int StartQuad() const;

		void CompleteAdd(int i);

		int PLAYCorpNum(SXArmyPlay play, int i) const;
		int PLAYFTime  (SXArmyPlay play, int i) const;
		int PLAYSize   (SXArmyPlay play) const;

		void RunArmyPlay( CXUnit off, CXRect &rect );

		CXRect PLAYRect(SXArmyPlay play,int i) const;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Class: CXCour
//*************************************************************************************************************
//*************************************************************************************************************

class CXCour : public CXUnit
{
	public:
		CXCour();
		CXCour( CUnit *unit );

		CXCour &operator=( CUnit *unit ) { m_unit = unit; return *this; }

		int TargId    ( bool bSet = false, int ival = -1 );
		int StartTime ( bool bSet = false, int ival = -1 ); // m_starttime );
		int RecalcTics( bool bSet = false, int ival = -1 ); // m_recalcticks

		const char *Orders() const;

		int NumCour() const;
		int NumComm(bool bSet = false, int ival = -1);

		bool HasComm() const; // m_comm != NULL

		SXComm Comm(int i);

		void DelComm(); //delete [] m_comm;	m_comm      = NULL;
		void DelOrders(); //	m_orders[0] = 0;

		void ShowScreen( bool bCaptured = false, bool bForce = false, bool bLogOnly = false );
};

//*************************************************************************************************************
//*************************************************************************************************************
// Struct: SEnemy
//*************************************************************************************************************
//*************************************************************************************************************

struct SEnemy
{
	CXUnit closest;

	int men;
	int dist;
	int cdist;
	int threat;

	bool IsValid( CXUnit unit );
};

//*************************************************************************************************************
//*************************************************************************************************************
// Function: SXTown
//*************************************************************************************************************
//*************************************************************************************************************

struct SXTown
{
	public:
		SXTown();
		SXTown( STown *town );
		SXTown( const SXTown &town );

		CXVec Loc() const;

		operator STown *();

		int Occupied() const; // -1 - none,0-plyr,1-ai

		int OccupiedTimer() const; // return minutes, just cause it makes sense I think

	private:
		STown *m_town;
};

//*************************************************************************************************************
//*************************************************************************************************************
// Function: CXCamp
//*************************************************************************************************************
//*************************************************************************************************************

class CXCamp
{
	public:
		static bool  TownNext(CXUnit me, SXTown *sold, int &dist);
		static SXTown TownClose(CXUnit me, int &dist);

		static SXTown StartTown(int bai); // 0-plyr,1-ai
		static SXTown TargTown ();

//----davide---new campaign AI functions---------------------------
		static void SetForceTimer(CXUnit me, int timer);
		static int  GetForceTimer(CXUnit me);
//-------------------------------------------------------------
};

//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************
//*************************************************************************************************************





