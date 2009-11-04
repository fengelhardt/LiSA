
#ifndef _order_h
#define _order_h

#include "matrix.hpp"

/// a class for indirect sorting.
/** This class provides a sorted index for elements of a matrix or vector.
    It does not affect the elements sorted, so you can have several orders 
    for instance for the set of jobs. All sorting methods implemented are 
    stable, you can sort with different keys consecutively. The keys must 
    provide a method to be casted to type double. 

    For sorting lists better use the corresponding list functions. 
    Lisa_List can even sort keys which cannot be casted into float values. 

    @author Thomas Tautenhahn
    @version 3.0pre1
    @see Lisa_Vector
    @see Lisa_Matrix 
*/ 
class Lisa_Order{
private: 
  /// the vector of keys
  Lisa_Vector<double> * key;
  /// the vector of indexes
  Lisa_Vector<int> * index;
  
  /// size of matrix/vector
  int m, n;
  /// number of keys/indexes
  int anz;
  
public: 
  /// construct an index for a vector of n elements 
  Lisa_Order(int n);
  /// construct an index for a matrix of n x m elements
  Lisa_Order(int n, int m);
  /// destructor
  ~Lisa_Order();
  
  /// set all keys to value of k
  void inline init_keys(const double k){ key->fill(k); }
  
  //@{    
  /// let order read the keys as a matrix
  void read(const Lisa_Matrix<int>* keys);
  void read(const Lisa_Matrix<float>* keys);
  void read(const Lisa_Matrix<double>* keys);     
  //@}
  
  //@{
  /// give the keys on form of a vector
  void read(const Lisa_Vector<int>*);    
  void read(const Lisa_Vector<float>*);
  void inline read(const Lisa_Vector<double> *const ve){ (*key)=(*ve); }
  //@}
  
  /// give the i-th key separately
  void inline read_one_key(const int i,const double k){ (*key)[i]=k; }
  
  /// give key at position (i,j) separately
  void inline read_one_key(const int i,const int j,const double k){(*key)[m*i+j]=k;} 
  
  /// get one-dimensional index
  /** for example vector[(*order)[3]] */
  int inline operator[](const int h){ return (*index)[h]; }
  
  /// get two dimensional index, row
  /** for example matrix[order->row(3)][order->col(3)] */
  int inline row(const int h){ return (*index)[h]/m; }
  /// get two dimensional index, col
  /** for example matrix[order->row(3)][order->col(3)] */
  int inline col(const int h){ return (*index)[h]%m; }
  
  /// computing the order according to nondecreasing keys
  /** this means vector[(*order)[i]]<= vector[(*order)[i+1]] for all i, 
      algorithm is stable (elements with same key stay in previous order) so 
      you can sort with multiple keys */
  void sort();
  
  /// put elements with same key together
  /** first element stays the same, then all elements with same key, then next 
      element not listed yet from the original order and so on, algorithm is 
      stable */
  void group();
};
       
#endif

