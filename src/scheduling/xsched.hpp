/*
 * ****************** xsched.hpp *******************************
 * 
 * description:        Class Lisa_XSchedule
 *
 * owner:              Per Willenius
 *
 * date:               28.9.1999
 *
 * version:            V 1.0
*/

/**  @name Extended Schedule
 
    Include file LiSA/src/lisa_dt/xsched.hpp, 
    it uses:
    lisa_dt/problem.h basics/matrix.h  basics/pgraph.h

    @author Per Willenius 
    @version 2.3pre2
*/ 

//@{

#ifndef _xsched_h 
#define _xsched_h 

#include "../misc/except.hpp"
#include "../basics/matrix.hpp"

#include "schedule.hpp"
#include "pgraph.hpp"

/** Lisa_XSchedule is an extension of Lisa_Schedule.
    To guarantee the communication with Lisa_Schedule
    Lisa_XSchedule contain a pointer to an instance of Lisa_Schedule
    */  
class Lisa_XSchedule
{
protected:
  /// the Schedule
  class Lisa_Schedule *P; 
  /// Groesse der Matritzen
  int n,m;
public:
  /// critical path
  class Lisa_Matrix<bool> *CP;
  /// sequence graph
  class Lisa_SGraph *PG; 
  /// Comparability Graph
  class Cgraph *CG; 
  /// transitive closure
  class Cgraph *TH; 
  /// reverse transitive closure
  class Cgraph *TH_V;
  /// complete sequence graph
  class CSgraph *CS; 
  /// Implicationclasses
  class Lisa_ImplicationClass *IKL; 
  /// constructor
  Lisa_XSchedule();
  /// constructor
  Lisa_XSchedule(class Lisa_Schedule *Pl);
  /// if maschine or job number changed
  void init();
  /// refresh the internal structure
  void updateLR(Lisa_Matrix<bool> *SIJ) {  
    P_to_PG();PG_to_LR(SIJ); P->valid=TRUE;}  
  /// mekes the critical path
  void make_CP();
  /// test, if sequence is irreducible
  int irreducibel(){return 1;}
  /// generate Sequence Graph
  int P_to_PG();
  /// generate Latin Rectangle
  int PG_to_LR(Lisa_Matrix<bool> *SIJ); 
  /// destructor
  ~Lisa_XSchedule();
};

#endif

//@}













