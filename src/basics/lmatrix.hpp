
#ifndef _lmatrix_h
#define _lmatrix_h

#include "../main/global.hpp"
#include "matrix.hpp"
#include "list.hpp"

//**************************************************************************
 
template<class T> class Lisa_MatrixOfLists;

//**************************************************************************

/// vector of lists
/**
 * @see Lisa_MatrixOfLists
 * @author Ivo Roessling
 * @version 2.3rc1
 */
template<class T>
class Lisa_VectorOfLists : public Lisa_Object{ 
protected:
  /// length of vector
  unsigned int m;
  
  /// array modelling vector
  Lisa_List<T>* contents;
  
  ///default constructor
  Lisa_VectorOfLists(){contents=0; m=0;}

  /// need a friend to hack around non std constructs in Lisa_MatrixOfLists
  friend class Lisa_MatrixOfLists<T>;  
public:
 
  /// construct listvector with m elements of type T
  Lisa_VectorOfLists(const int m);
  
  /// construct listvector as a copy of other
  Lisa_VectorOfLists(const Lisa_VectorOfLists<T>& other);
  
  /// construct listvector out of a nonpremptive one
  Lisa_VectorOfLists(const Lisa_Vector<T>& nonpreemtive);
  
  /// destructor 
  ~Lisa_VectorOfLists();
  
  /// assign value to all elements 
  void fill(const T value);             
  
  /// access to j-th element of vector 
  inline Lisa_List<T>& operator[] (const unsigned int j){ 
    #ifdef LISA_DEBUG
    if (j>=m){
      G_ExceptionList.lthrow("element "+ztos(j)+" of a "+ztos(m)+
                             "-element vector requested",OUT_OF_RANGE);
      return contents[0];
    }
    #endif
    return contents[j];
  }
    
  /// access to j-th element of vector   
  inline Lisa_List<T> operator[] (const unsigned int j)const{ 
    #ifdef LISA_DEBUG
    if (j>=m){
      G_ExceptionList.lthrow("element "+ztos(j)+" of a "+ztos(m)+
                             "-element vector requested",OUT_OF_RANGE);
      return contents[0];
    }
    #endif
    return contents[j];
  }
    
  /// get length of vector
  int inline get_m() const { return m; }
  
  /// assign a vector to another  
  const Lisa_VectorOfLists<T>& operator=(const Lisa_VectorOfLists<T>& );
  
  //@{
  /// compare vectors
  bool operator==(const Lisa_VectorOfLists<T>&) const;
  bool inline operator!=(const Lisa_VectorOfLists<T>& other) const { return !(&other == this); }
  //@}
  
  /// compare lexicographically
  bool operator<=(const Lisa_VectorOfLists<T>&) const;
  
  /// write vector values to stream
  void write(std::ostream& = std::cout) const;
  /// read vector values from stream
  void read(std::istream& = std::cin);
  
  //@{
  /// search index of maximal vector element
  int index_of_max() const;
  int index_of_maxsum() const;
  int index_of_maxlast() const;
  //@}
  
  //@{
  /// search index of minimal vector element
  int index_of_min() const;
  int index_of_minsum() const;
  int index_of_minlast() const;
  //@}
  
  /// get pointer to first element in vector
  inline Lisa_List<T>* get_first() const { return contents; }
  /// get pointer to last element in vector
  inline Lisa_List<T>* get_last() const { return contents+m; }
};

//**************************************************************************

/// matrix of lists
/**
 * @see Lisa_VectorOfLists
 * @author Ivo Roessling
 * @version 2.3rc1
 */
template<class T>
class Lisa_MatrixOfLists: public Lisa_Object{
protected:
  /// matrix size
  unsigned int n, m;
  /// array holding vectors
  Lisa_VectorOfLists<T>* row;
  //Lisa_Matrix<int> *A;

public:
  /// construct list matrix of given format n x m
  Lisa_MatrixOfLists(const int n_in, const int m_in);
  /// construct copy of a given pmt matrix
  Lisa_MatrixOfLists(const Lisa_MatrixOfLists<T>& other);
  /// construct listmatrix out of a nonpremptive one
  Lisa_MatrixOfLists(const Lisa_Matrix<T>& nonpreemptive);
  /// assign value to all matrix elements
  void fill(const T value);
  /// access to i-th row vector
  inline Lisa_VectorOfLists<T>& operator[] (const unsigned i){ 
    #ifdef LISA_DEBUG
    if (i>=n){
      G_ExceptionList.lthrow("row "+ztos(i)+" of a "+ztos(n)+
                             "-row listmatrix requested",OUT_OF_RANGE);
      return row[0];
    }
    #endif
    return row[i];
  }
  
  /// access to i-th row vector    
  inline const Lisa_VectorOfLists<T>& operator[] (const unsigned i) const{ 
    #ifdef LISA_DEBUG
    if (i>=n){
      G_ExceptionList.lthrow("row "+ztos(i)+" of a "+ztos(n)+
                             "-row listmatrix requested",OUT_OF_RANGE);
      return row[0];
    }
    #endif
    return row[i];
  }
    
  /// number of columns
  inline int get_m() const { return m; }
  /// number of rows           
  inline int get_n() const { return n; } 
  /// assign another matrix 
  const Lisa_MatrixOfLists<T>& operator=(const Lisa_MatrixOfLists<T>& other);
  /// compare matrices element by element
  bool operator==(const Lisa_MatrixOfLists<T>&) const;
  
  // compare lexicographically
  // iroess  bool operator<=(const Lisa_MatrixOfLists<T>&) const;
  
  /// write object data to a stream
  void write(std::ostream& = std::cout) const;
  
  /// read object data from a stream
  void read(std::istream& = std::cin);
 
  /// destructor  
  ~Lisa_MatrixOfLists();
  
  /// pointer to first row (for iterating the contents)
  inline Lisa_VectorOfLists<T> * first_row() const { return row; }

  /// Pointer after last row (for iterating the contents)
  inline Lisa_VectorOfLists<T> * end_row() const { return row+n; }

  /// get A matrix
  Lisa_Matrix<int> * get_MatrixA() const;
  
  /// apply A matrix
  void apply_MatrixA(const Lisa_Matrix<int>& MatrixA, T NullElt);

};

//**************************************************************************

#endif             

//**************************************************************************

