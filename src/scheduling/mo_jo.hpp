
#ifndef _mo_jo_h 
#define _mo_jo_h

#include <iostream>

#include "../basics/matrix.hpp"

//**************************************************************************

/// flag
const int MO_SOURCE=-1;
/// flag indicating that operation has no successor
const int NO_SUCCESSOR=-1;

class Lisa_MO;
class Lisa_JO;

//**************************************************************************

/// basic class representing an order
/** for just one machine or job
    @version 2.3rc1
    @author unknown
*/ 
class Lisa_OrderWithoutRepetition{
private: 
  /// contains the successor for each operation
  Lisa_Vector<int> * succv;
  /// where is the first operation ?
  int start;
  
  /// size, number of operations
  int n;
  
  /// default constructor
  /** needed to hack around non std constructs in Lisa_MO and Lisa_JO */
  Lisa_OrderWithoutRepetition();
  
  //@{
  /// friends
  /** needed to hack around non std constructs in Lisa_MO and Lisa_JO */
  friend class Lisa_MO;
  friend class Lisa_JO;
  //@}
  
public: 
  /// construct only with given size:
  Lisa_OrderWithoutRepetition(const int n);
  
  /// what is the succ of operation i ?
  int succ(int i) const;
  
  /// insert i after j
  void insert(const int i, const int j);
  
  /// write the order as rank vector
  void write_rank(Lisa_Vector<int>* rank) const;
  /// read order from a rank vector
  /** returns FALSE if given vector is not a permutation */
  int read_rank(Lisa_Vector<int>* rank);
  
  /// destructor
  ~Lisa_OrderWithoutRepetition();
}; 

//**************************************************************************

/// class representing a machine order 
/** Do not confuse Lisa_MO and Lisa_JO !
    @version 2.3rc1
    @author unknown
*/
class Lisa_MO:public Lisa_Object{
private:
  /// one machineorder for each job
  Lisa_OrderWithoutRepetition * orders;
  /// problem size
  int n,m;
public: 
  /// get size
  int get_n() const {return n;}
  /// get size
  int get_m() const {return m;}
  
  /// construct only with given size
  Lisa_MO(const int n, const int m);
  
  /// what machine is the succ of (i,j) in MO of job i?
  int succ(int i, int j) const;
  
  /// insert machine j after k in MO of job i:
  void insert(const int i, const int j, const int k);
  
  /// write machine order as rank matrix
  void write_rank(Lisa_Matrix<int>* rank) const;
  
  /// read machine order from rank matrix
  /** returns FALSE if given vectors in matrix are not permutations */
  int read_rank(Lisa_Matrix<int>* rank);
  
  void write (std::ostream& = std::cout) const;
  void read (std::istream& = std::cin);
  
  ///destructor
  ~Lisa_MO();
};

//**************************************************************************

/// class representing a job order 
/** Do not confuse Lisa_MO and Lisa_JO !
    @version 2.3rc1
    @author unknown
*/
class Lisa_JO:public Lisa_Object{
private:
  /// one job order for each machine
  Lisa_OrderWithoutRepetition * orders;
  /// problem size
  int n,m;
public:
  /// get size
  int get_n() const {return n;}
  /// get size
  int get_m() const {return m;}
  /// construct only with given size:
  Lisa_JO(const int n, const int m);
  /// what job is the succ of (i,j) in JO of machine k?
  int succ(int i, int j) const;
  /// insert job i after k in JO of machine j:
  void insert(const int i, const int j, const int k);
  /// write job order to rank matrix
  void write_rank(Lisa_Matrix<int>* rank)const;
  /// read job order from rank matrix 
  /** returns FALSE if given vectors in matrix are not all permutations */
  int read_rank(Lisa_Matrix<int>*);
  void write (std::ostream& = std::cout) const;
  void read (std::istream& = std::cin);
  /// destructor
  ~Lisa_JO();
};

//**************************************************************************

#endif

