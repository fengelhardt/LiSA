/*
 * ************** schedule.hpp *******************************
 * 
 * description:    definition of class Lisa_Schedule
 *
 * owner:          Per Willenius
 *
 * date:           17.9.1999
 *
 * version:        V 1.0  
 * 
 * history:         7. 9. 98 kleinere Anpassungen TAU
*/

/**  @name Classes for Communication of LiSA Modules
 
    Include file LiSA/src/lisa_dt/problem.h, it uses iostream and string.

    @author Per Willenius and Thomas Tautenhahn
    @version 2.3pre3
*/ 

//@{


#ifndef _schedule_h 
#define _schedule_h 

#include <string>
#include <iostream>

#include "../basics/matrix.hpp"
#include "../basics/list.hpp"
#include "../lisa/filentry.hpp"

using namespace std;

const int LENGHT_OF_INFO_VECTOR = 15;
const int LENGHT_OF_OBJECTIVE = 8;
// only regular objective
const int NUMBER_INFO = 0;
const int MAXRANK_INFO = 9;
const int NO_OF_SOURCES = 10;
const int NO_OF_SINKS = 11;
const int NO_OF_IKL =12;

//########################## Class Lisa_Schedule #####################################

/** Lisa_Schedule 
 */
class Lisa_Schedule  : public Lisa_FileEntry
{
protected:
  int m, n; // for Matrix-Handling 
public:
  /// Lisa_Schedule is valid iff valid==TRUE
  int valid;
  /// get number of maschines
  int get_m(void) const {return m;}
  /// get number of jobs
  int get_n(void) const {return n;}
  /// semiactive==TRUE iff Lisa_Schedule is defined as a semiactive Schedule
  bool semiactive;
  /// constructor
  Lisa_Schedule(int n_ein=0,int m_ein=0); 
  /// constructor
  Lisa_Schedule(Lisa_Schedule &other); 
  /// the latin rectangle
  Lisa_Matrix<int> *LR; 
  /// the latin rectangle (for preemtion)
  //Lisa_MatrixOfLists<int> *LRpmt;
  ///  MO, JO  as matrices (direct successor)
  Lisa_Matrix<int> *NMO,*NJO;  
  /// C-matrix
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
  /// construct maschine order
  int LR_to_MO(); 
  /// construct job order
  int LR_to_JO(); 
  /// construct latin rectangle
  int MO_JO_to_LR(); 
  /// return some properties of the schedule
  int get_property(int property);
  /// inititialize Lisa_Schedule with new number of jobs and maschines 
  void init(int DEFAULT_M, int DEFAULT_N);
  virtual void write (ostream& = cout) const;
  virtual void read (istream& = cin);
  /// destructor
  virtual ~Lisa_Schedule();
};



//########################## Class Schedule_Node #####################################

/** Schedule_Node 
Contain the schedule as latin rectangle as C-matrix and an
additional vector for some additional informations
 */

class ScheduleNode  : public Lisa_FileEntry
{
protected:
public:
  /// The Schedule
  Lisa_Schedule *actual_schedule;
  /// an vector to hold some informations
  Lisa_Vector<int> *schedule_info;
  /// the pointer to an entry of schedule_info;
  static int sinfo_pointer;
  ScheduleNode();
  ScheduleNode(Lisa_Schedule*);
  ScheduleNode(const ScheduleNode&);
   /// assignment
   const ScheduleNode & operator=( const ScheduleNode & other); 
  /// to order entries in List
   bool operator<=( const ScheduleNode & other) const;
  /// to order entries in List
   bool operator>=( const ScheduleNode & other) const;
  /// to order entries in List
   bool operator<( const ScheduleNode & other) const;
  /// compare only schedule_info[sinfo_pointer]
   bool operator==( const ScheduleNode & other) const;
  /// compare only schedule_info[sinfo_pointer]
   bool operator!=( const ScheduleNode & other) const;
   /// output to stream (stream operator itself is already defined)
   virtual void write(ostream & = cout) const;
  /// input from stream (stream operator itself is already defined)
   virtual void read(istream &   = cin);
   /// construct copy of (*this) and return pointer to this copy
  //ScheduleNode* copy();
   /// You have to implement a destructor if your node uses dynamic memory!
   ~ScheduleNode();
};

#endif

//@}




























