/*
 * ************** lvalues.hpp *******************************
 * 
 * description:    definition of class Lisa_Values
 *
 * owner:          Per Willenius
 *
 * date:           27.9.1999
 *
 * version:        V 1.0  
 * 
 * history:         7. 9. 98 kleinere Anpassungen TAU
*/

/**  @name Classes for Communication of LiSA Modules
 
    Include file LiSA/src/lisa_dt/problem.h, it uses iostream and string.

    @author Per Willenius and Thomas Tautenhahn
    @version 2.3pre1
*/ 

//@{


#ifndef _lvalues_h 
#define _lvalues_h 

// **************** defines ********************
const int LENGHT_OF_EXTRA_VECTOR = 5;

//  ************** System Includes  ******************
//#include "../basics/lisa_str.hpp"
#include <iostream.h>


//  ************** Lisa Includes  ******************
#include "../basics/matrix.hpp"
#include "../lisa_dt/mo.hpp"
#include "../lisa_dt/filentry.hpp"


//  ************** Class Definitions ******************


//########################## Class Lisa_Values #####################################

/// collection of all values which defines the instance of the problem

class Lisa_Values  : public Lisa_FileEntry
{ 
private:
  int n,m;   // for Matrix-Handling 
public:
  /// Lisa_Values is valid iff valid==TRUE;
  int valid;
  /// number of columns in the matrices (number of maschines)
  int get_m(void){return m;};
  /// number of rows in the matrices (number of jobs)
  int get_n(void){return n;};  
  /// Processing Time Matrix
  Lisa_Matrix<TIMETYP> *PT; 
  // Processing Time Matrix (with preemtion)
  //  Lisa_MatrixOfLists<TIMETYP> *PTl;
  /// machine order
  Lisa_MO *MO;
  /// Set of Operations 
  Lisa_Matrix<bool> *SIJ; 
  /// Release Dates and Due Dates
  Lisa_Vector<TIMETYP> *RD, *DD; 
  /// Weights for each job
  Lisa_Vector<double> * WI; 
  /// second type of Weights for each job
  Lisa_Vector<double> * WI2;
  /// additional vector of parameters
  Lisa_Vector<double> * EXTRA;
  /// construct Processing Time Matrix
  void make_PT(void);
  // construct Processing Time Matrix (with preemtion)
  //void make_PTl(void);	
  /// construct  machine order
  void make_MO(void);
  /// returns OK, if MO is valid
  int valid_MO();
  /// delete machine order
  void no_MO();
  /// delete vector of release dates
  void no_RD();
  /// delete vector of due dates
  void no_DD();
  /// delete vector of weights
  void no_WI();
  /// construct Set of Operations  
  void make_SIJ(void);
  /// construct Release Dates
  void make_RD(void);
  /// construct Due Dates
  void make_DD(void);
  /// construct Weights 
  void make_WI(void);
  /// construct Weights2
  void make_WI2(void);
   /// construct additional vector of parameters
  void make_EXTRA(void);
  /** an entry in SIJ will be set if and only if the corresponding processing time 
    is greater than zero*/
  void PT_to_SIJ();
  /// constructor
  Lisa_Values();
  /// destructor
  ~Lisa_Values();
  /// new number of maschines or jobs
  void init(int n, int m);
  void write(ostream& = cout) const;
  void read(istream& = cin);
};



#endif

//@}




























