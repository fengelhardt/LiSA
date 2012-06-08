
#ifndef _schedule_h 
#define _schedule_h 

#include <string>
#include <iostream>

#include "../basics/matrix.hpp"
#include "../basics/list.hpp"
#include "../lisa/filentry.hpp"

/// length of Lisa_ScheduleNode::schedule_info
const int LENGHT_OF_INFO_VECTOR = 15;

//@{
/// flag, needs documentation 
const int LENGHT_OF_OBJECTIVE = 8;
const int NUMBER_INFO = 0;
const int MAXRANK_INFO = 9;
const int NO_OF_SOURCES = 10;
const int NO_OF_SINKS = 11;
const int NO_OF_IKL =12; 
//@}

/// schedule class for communication with algorithms
/** @author Per Willenius, Thomas Tautenhahn
    @version 3.0pre1
 */
class Lisa_Schedule  : public Lisa_FileEntry{
protected:
  /// size of matrices
  int m, n;
public:
  /// Lisa_Schedule is valid if valid==true
  int valid;
  
  /// get number of maschines
  int inline get_m() const {return m;}
  
  /// get number of jobs
  int inline get_n() const {return n;}
  
  /// semiactive==true if Lisa_Schedule is defined as a semiactive Schedule
  bool semiactive;
  
  /// constructor
  /** create a schedule for a given size */
  Lisa_Schedule(int n_ein=0,int m_ein=0);
  
  /// constructor
  /** create a schedule as copy of other schedule */
  Lisa_Schedule(Lisa_Schedule &other); 
  /// the latin rectangle
  Lisa_Matrix<int> *LR;
  
  ///  MO, JO  as matrices (direct successor)
  Lisa_Matrix<int> *NMO,*NJO;
  
  /// CIJ matrix
  Lisa_Matrix<TIMETYP> *CIJ;
  
  /// construct latin rectangle
  void make_LR();
  
  /// test, if latin rectangle is valid
  int valid_LR(Lisa_Matrix<bool> *SIJ);
  
  /// construct maschine order
  void make_NMO();
  /// construct job oreder
  void make_NJO();
  /// construct set of operations
  void make_CIJ();
  // construct maschine order
  //int LR_to_MO(); 
  // construct job order
  //int LR_to_JO(); 
  /// construct latin rectangle from machine and job order
  /** Returns true if the combination is feasible, false otherwise. In the case
      the contents of LR are undefined. */
  static bool MO_JO_to_LR(Lisa_Matrix<int> *const LR,
                         const Lisa_Matrix<bool> *const SIJ,
                         const Lisa_Matrix<int> *const MO,
                         const Lisa_Matrix<int> *const JO); 
  
  /// return some properties of the schedule
  int get_property(int property);
  
  /// inititialize Lisa_Schedule with new number of jobs and maschines 
  void init(int DEFAULT_M, int DEFAULT_N);
  
  void write(std::ostream& = std::cout) const;
  void read(std::istream& = std::cin);
 
  /// destructor
  ~Lisa_Schedule();
};

//**************************************************************************

/// a wrapper for Lisa_Schedule
/** Contains the schedule as latin rectangle as CIJ matrix and an additional 
    vector for some additional informations
    
    Used to store Schedules in a Lisa_List
    
    @author Per Willenius, Thomas Tautenhahn
    @version 3.0pre1
 */
class Lisa_ScheduleNode  : public Lisa_FileEntry{
public:
  /// the Schedule
  Lisa_Schedule *actual_schedule;
  
  /// a vector to hold some additional information
  Lisa_Vector<int> *schedule_info;
  
  /// index to an entry of schedule_info
  static int sinfo_pointer;
  
  /// default constructor
  Lisa_ScheduleNode();
  
  /// constructor
  /** create object from a given schedule */
  Lisa_ScheduleNode(Lisa_Schedule* schedule);
  
  /// copy constructor
  Lisa_ScheduleNode(const Lisa_ScheduleNode&);
  
   /// assignment operator
   const Lisa_ScheduleNode & operator=( const Lisa_ScheduleNode & other); 
   
   /// compare operator
   /** to order entries in Lisa_List, compare only 
       schedule_info[sinfo_pointer] */
   bool operator<=( const Lisa_ScheduleNode & other) const;
   /// compare operator
   /** to order entries in Lisa_List, compare only 
       schedule_info[sinfo_pointer] */
   bool operator>=( const Lisa_ScheduleNode & other) const;
   /// compare operator
   /** to order entries in Lisa_List, compare only 
       schedule_info[sinfo_pointer] */
   bool operator<( const Lisa_ScheduleNode & other) const;
   /// compare operator
   /** to order entries in Lisa_List, compare only 
       schedule_info[sinfo_pointer] */
   bool operator==( const Lisa_ScheduleNode & other) const;
   /// compare operator
   /** to order entries in Lisa_List, compare only 
       schedule_info[sinfo_pointer] */
   bool operator!=( const Lisa_ScheduleNode & other) const;
   
   /// output to stream
   void write(std::ostream & = std::cout) const;
   /// input from stream
   void read(std::istream &  = std::cin);

   /// destructor
   ~Lisa_ScheduleNode();
};

#endif

