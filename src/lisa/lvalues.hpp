
#ifndef _lvalues_h 
#define _lvalues_h 


#include <iostream>

#include "../basics/matrix.hpp"
#include "../scheduling/mo_jo.hpp"
#include "filentry.hpp"

/// collection of all values which define the instance of a problem
/** @author Per Willenius, Thomas Tautenhahn
    @version 3.0
  */
class Lisa_Values  : public Lisa_FileEntry{ 
private:
  /// size of matrices
  int n,m; 
public:
  /// Lisa_Values is valid if valid==true;
  int valid;
  /// number of columns in the matrices (number of machines)
  inline int get_m(void)const{return m;}
  /// number of rows in the matrices (number of jobs)
  inline int get_n(void)const{return n;}
  /// processing time matrix
  Lisa_Matrix<TIMETYP> *PT; 
  
  /// machine order
  Lisa_MO *MO;
  /// set of operations 
  Lisa_Matrix<bool> *SIJ; 
  /// release dates and due dates
  Lisa_Vector<TIMETYP> *RD, *DD; 
  /// weights for each job
  Lisa_Vector<double> * WI; 
  /// second type of weights for each job
  Lisa_Vector<double> * WI2;
  /// additional vector of parameters
  Lisa_Vector<double> * EXTRA;
  /// construct processing time matrix
  void make_PT(void);
  
  /// construct machine order
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
  /// construct set of operations  
  void make_SIJ(void);
  /// construct release dates
  void make_RD(void);
  /// construct due dates
  void make_DD(void);
  /// construct weights 
  void make_WI(void);
  /// construct weights2
  void make_WI2(void);
   /// construct additional vector of parameters
  void make_EXTRA(void);
  
  /// set of operations
  /** an entry in SIJ will be set if and only if the corresponding processing 
      time is greater than zero*/
  void PT_to_SIJ();
  /// constructor
  Lisa_Values();
  /// destructor
  ~Lisa_Values();
  
  /// reinit with new number of maschines or jobs
  void init(int n, int m);
  
  /// write object values to a stream
  void write(std::ostream& = std::cout) const;
  
  /// read object values from a stream
  void read(std::istream& = std::cin);
};

#endif

