
#ifndef _global_h 
#define _global_h 

/// debug parameter ... comment for the final version
#define LISA_DEBUG

/// maximal memory
#define MAX_MEMORY 10000

/// maximal length of string
#define MAX_STRING 25

/// default value for machine number
const int DEFAULT_M=4;

/// default value for job number
const int DEFAULT_N=6;

/// logic
//#define NOT_DEFINED -1 
///logic
#define TRUE 1
///logic
#define OK 1
///logic
#define FALSE 0

/// type of all processing times heads, tails, r_i, d_i ...
#define TIMETYP double

/// alpha entries
#define NUMBER_ALPHA 4
/// beta entries
#define NUMBER_BETA 13
/// gamma entries
#define NUMBER_GAMMA 1
/// misc entries
#define NUMBER_MISC  2

/// maximal number of tupel index
#define TUPEL_INDEX NUMBER_ALPHA+NUMBER_BETA+NUMBER_GAMMA+NUMBER_MISC

//@{
/// name of tupelentries
const int M_ENV=0;
const int M_NUMBER=1;
const int M_MPT=2;
const int M_MPM=3;
const int PMTN=4;
const int PRECEDENCE=5;
const int RI=6;
const int DI=7;
const int PIJ=8;
const int BATCH=9;
const int BOUNDED_BATCH=10;
const int JOB_NR=11;
const int NO_WAIT=12;
const int SIZE=13;
const int TIME_LAGS=14;
const int TRANSPORTATION_DELAYS=15;
const int SERVER_FLAGS=16;
const int OBJECTIVE=17;
const int M_NO=18;
const int N_NO=19;
//@}


/// number of possible values
const int NUMBER[TUPEL_INDEX]={
  25, // M_ENV
  3,  // M_NUMBER
  2,  // M_MPT
  2,  // M_MPM
  2,  // PMTN
  7,  // PRECEDENCE
  2,  // RI
  2,  // DI
  3,  // PIJ
  3,  // BATCH
  2,  // BOUNDED_BATCH
  3,  // JOB_NR
  2,  // NO_WAIT
  2,  // SIZE
  4,  // TIME_LAGS
  11,  // TRANSPORTATION_DELAYS
  4,  // SERVER_FLAGS
  11,  // OBJECTIVE
  1,  // M_NO
  1   // NO_NO
};

/// maximum number in NUMBER[]
#define MAX_NUMBER 25

/// for all entries
const int EMPTY=FALSE; 
/// for all entries
const int VALUE=1;

//**************************************************************************

// alpha

//@{
/// Problem[M_ENV]
const int ONE=1;
const int O=2;
const int F=3;
const int J=4;
const int X=5;
const int G=6;
const int P=7;
const int Q=8;
const int R=9;
const int MPT=10;
const int OMPT=11;
const int FMPT=12;
const int JMPT=13;
const int XMPT=14;
const int GMPT=15;
const int OMPM=16;
const int FMPM=17;
const int JMPM=18;
const int XMPM=19;
const int GMPM=20;
const int PMPM=21;
const int QMPM=22;
const int F_SR=23;
const int P_CS=24;
//@}

//@{
/// Problem[M_NUMBER]
const int M_ARB=EMPTY;
const int M_VAL=VALUE;
const int M_FIX=2;
//@}

//**************************************************************************

// beta

/// is set ?
const int SET=TRUE;

// Problem[PMTN]: only TRUE and FALSE

//@{
/// Problem[PRECEDENCE]
const int INTREE=1;     // intree 
const int OUTTREE=2;    // outtree
const int TREE=3;     //  tree
const int SP_GRAPH=4;
const int CHAINS=5;
const int PREC=6;  
//@}

// Problem[RI]: only TRUE and FALSE

// Problem[DI]: only TRUE and FALSE

//@{
/// Problem[PIJ]
const int PIJ_1=1;
const int PIJ_P=2;
//@}

//@{
/// Problem[BATCH]
const int S_BATCH=1;
const int P_BATCH=2;
//@}

// Problem[BOUNDED_BATCH]: only TRUE and FLASE

//@{
/// Problem[JOB_NR]
const int J_ARB=EMPTY;
const int J_VAL=VALUE;
const int J_FIX=2;
//@}

// Problem[NO_WAIT]:  only TRUE and FALSE

// Problem[SIZE]: only TRUE and FALSE

//@{
/// Problem[TIME_LAGS]
const int UNIT_TL=1;
const int CONST_TL=2;
const int GENERAL_TL=3;
//@}

//@{
/// Problem[TRANSPORTATION_DELAYS]
const int TIK_T=1;
const int TIKL_T=2;
const int TI_IN=3;
const int TKL_TLK=4;
const int TIKL_TILK=5;
const int TI=6;
const int TK=7;
const int TKL=8;
const int TIK=9;
const int TIKL=10;
//@}

//@{
/// Problem[SERVER_FLAGS]
const int SI=1;
const int SI_1=2;
const int SI_S=3;
//@}

//**************************************************************************

// Gamma

//@{
/// Problem[OBJECTIVE]
const int CMAX=1;
const int LMAX=2;
const int SUM_CI=3;
const int SUM_WICI=4;
const int SUM_UI=5;
const int SUM_WIUI=6;
const int SUM_TI=7;
const int SUM_WITI=8;
//@}

/// sum w_i | C_i - d_i |
const int IRREG1=9;

/// sum w_i T_i + sum wi_early (d_i-C_i)^+ + w_late L_max + w_early max (d_i-C_i)^+ 
/** as used by Neumann/Schwindt */
const int IRREG2=10;     


/// correspondings strings
const char name_of_tupelindex[TUPEL_INDEX][MAX_STRING] = {
"M_ENV",
"M_NUMBER",
"M_MPT",
"M_MPM",
"PMTN",
"PRECEDENCE",
"RI",
"DI",
"PIJ",
"BATCH",
"BOUNDED_BATCH",
"JOB_NR",
"NO_WAIT",
"SIZE",
"TIME_LAGS",
"TRANSPORTATION_DELAYS",
"SERVER_FLAGS",
"OBJECTIVE",
"M_NO",
"N_NO"
};

/// yet more corresponding strings
const char name_of_tupelentry[TUPEL_INDEX][MAX_NUMBER][MAX_STRING] = {
  // M_ENV:
{  "", "ONE", "O", "F", "J", "X", "G", "P", "Q", "R", "MPT", "OMPT", "FMPT", "JMPT", "XMPT", "GMPT", "OMPM", "FMPM", "JMPM", "XMPM", "GMPM", "PMPM", "QMPM", "F_SR", "P_CS" } ,  
  // M_NUMBER:
{  "M_ARB", "M_VAL", "M_FIX", "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  ""}, 
  // M_MPT:
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // M_MPM:
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" },  
  // PMTN:
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // PRECEDENCE
{  "EMPTY", "INTREE", "OUTTREE",  "TREE",  "SP_GRAPH",  "CHAINS",  "PREC",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // RI
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // DI
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // PIJ 
{  "EMPTY", "PIJ_1", "PIJ_P", "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // BATCH
{  "EMPTY", "S_BATCH", "P_BATCH", "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // BOUNDED_BATCH 
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // JOB_NR 
{ "J_ARB", "J_VAL", "J_FIX", "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // NO_WAIT
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // SIZE
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // TIME_LAGS 
{  "EMPTY",  "UNIT_TL",  "CONST_TL",  "GENERAL_TL",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // TRANSPORTATION_DELAYS
{  "EMPTY",  "TIK_T",  "TIKL_T",  "TI_IN",  "TKL_TLK",  "TIKL_TILK",  "TI",  "TK",  "TKL",  "TIK",  "TIKL",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // SERVER_FLAGS
{  "EMPTY",  "SI",  "SI_1",  "SI_S",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // OBJECTIVE
{  "", "CMAX", "LMAX", "SUM_CI",  "SUM_WICI",  "SUM_UI",  "SUM_WIUI",  "SUM_TI",  "SUM_WITI",  "IRREG1",  "IRREG2",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // M_NO
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }, 
  // NO_NO
{  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "",  "" }
};

/// highest possible time
const TIMETYP MAXTIME= (TIMETYP) 100000L;    
/// highest possible objective
const TIMETYP MAXOBJECTIVE= (TIMETYP) 10000000L;
/// highest possible number
const long MAXNUMBER= 1000000L; 

/// Length of the progress indicator for external algorithms. 
/** The algorithm should output a line starting with "OBJECTIVE=" exactly so
    many times in equal time intervals to fill one indicator window. */
#define PROGRESS_INDICATOR_STEPS 400


/// random generator (Taillard)
/** @param lb lower bound
    @param ub upper bound
    @param seed random seed
    @return a random value between lb and ub, determined by seed 
    @author Per Willenius
    @version 2.3final
 */
int lisa_random( int lb, int ub, long *seed );

/// maximum of two values
template<class T>
inline T  MAX(T  a, T  b) { return a > b ? a : b; }

/// minimum of two values
template<class T>
inline T  MIN(T  a, T  b) { return a < b ? a : b; }

#endif

