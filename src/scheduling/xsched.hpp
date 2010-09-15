
#ifndef _xsched_h 
#define _xsched_h 

#include "../misc/except.hpp"
#include "../basics/matrix.hpp"

#include "schedule.hpp"
#include "pgraph.hpp"

/// extended schedule
/** Lisa_XSchedule is an extension of Lisa_Schedule. To guarantee the 
    communication with Lisa_Schedule Lisa_XSchedule contains a pointer to an 
    instance of Lisa_Schedule.

    @author Per Willenius 
    @version 3.0
  */  
class Lisa_XSchedule{
protected:
  /// the schedule
  class Lisa_Schedule *P; 
  /// size of matrices
  int n,m;
public:
  /// critical path
  Lisa_Matrix<bool> *CP;
  
  /// sequence graph
  Lisa_SGraph *PG; 

  /// complete sequence graph
  Lisa_CSGraph *CS; 

  /// default constructor
  Lisa_XSchedule();
  /// constructor
  /** create with a given schedule */
  Lisa_XSchedule(class Lisa_Schedule *Pl);

  /// destructor
  ~Lisa_XSchedule();

  /// if machine or job number changed
  void init();
  /// refresh the internal structure
  void inline updateLR(Lisa_Matrix<bool> *SIJ) {P_to_PG();PG_to_LR(SIJ); P->valid=true;} 
  
  /// makes the critical path
  void make_CP();

  /// generate sequence graph
  int P_to_PG();
  /// generate latin rectangle
  int PG_to_LR(Lisa_Matrix<bool> *SIJ); 
};

#endif

