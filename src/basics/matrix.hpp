/*
 * ************** matrix.h*******************************
 * 
 * vectors and matrices with dynamic size and fast access
 *
 * @author Thomas Tautenhahn 
 *        21.10.99
 *
 * Patch: Marc Moerig 
 *        03.01.2003
 *
 *
*/

#ifndef _lisa_matrix_h_ 
#define _lisa_matrix_h_ 

#include <iostream>

#include "../main/global.hpp"
#include "../misc/except.hpp"
#include "../misc/int2str.hpp"

using namespace std;

/** @name Vectors and Matrices
    Classes #Lisa_Vector<T># and #Lisa_Matrix<T># provide easy to use 
    vectors and matrices with dynamic size and fast access to the elements.
    Define #LISA_DEBUG# to get automatic array bounds check! \\
    
    Include file LiSA/src/basics/matrix.hpp, it uses iostream.
    Use only for basic types int, bool, long, float. Elements of vectors 
    must not allocate memory dynamically! For matrices of lists use 
    lmatrix.h \\
    
    Allways be carefull to use the right operator []! If X is of type 
    Lisa_Vector<int>*, then the first Element of X is (*X)[0] and not 
    "*X[0]" or "X[0]"!
    @author Thomas Tautenhahn
    @version 2.3pre3
    @see Lisa_VectorOfLists
    @see Lisa_MatrixOfLists 
*/ 

//@{
    
template<class T> class Lisa_Matrix;

/** Basic vector class in LiSA. */

template<class T>
class Lisa_Vector{
private:
  /// size
  unsigned int m;
  
  /// array to hold elements
  T* contents;
  
  /** default constructor to create an empty vector */
  Lisa_Vector();

  /// friends
  friend class Lisa_Matrix<T>;
public:

  /// construct vector with m elements of type T
  Lisa_Vector(const unsigned int m);
  
  /// construct vector as a copy of other
  Lisa_Vector(const Lisa_Vector<T>& other);
  
  /// destructor 
  ~Lisa_Vector();
  
  /// assign value to all elements 
  void fill(const T value);             
  
  /// access to j-th element of vector
  inline T& operator[] (const unsigned int j){ 
#ifdef LISA_DEBUG
    if (j>=m){
        G_ExceptionList.lthrow("element "+ztos(j)+" of a "+
                                ztos(m)+"-element vector requested",
                                OUT_OF_RANGE);
	  return contents[0];
	}
#endif
    return contents[j];
  }
  
  inline T operator[](const unsigned int j)const{ 
#ifdef LISA_DEBUG
    if (j>=m){
      G_ExceptionList.lthrow("element "+ztos(j)+" of a "+
				             ztos(m)+"-element vector requested",
                             OUT_OF_RANGE);
	  return contents[0];
	}
#endif
    return contents[j];
  }
  
  /// length of vector
  int get_m()const{ return m; }
  
  /// assign a vector to another  
  const Lisa_Vector<T>& operator=(const Lisa_Vector<T>& );
  
  /// compare vectors
  bool operator==(const Lisa_Vector<T>&) const;
  
  /// compare lexicographically
  bool operator<=(const Lisa_Vector<T>&) const;
  
  /// input in lisa-format
  void write(ostream& = cout) const;
  
  /// output in lisa format
  void read(istream& = cin);
  
  /// search index of maximal vector element
  unsigned int index_of_max() const;
  
  /// search index of minimal vector element
  unsigned int index_of_min() const;
  
  /// returns pointer to first element of the vector
  inline T* get_first() const { return contents; }
  
  /// returns pointer after the last element of the vector
  inline T* get_last() const { return contents+m; }  
};

//******************************************************************************

/** Basic matrix class in LiSA. A Lisa_Matrix is a vector of vectors. */
template<class T>
class Lisa_Matrix{
private: 
  /// size
  const unsigned int m;
  const unsigned int n;
  /// array holding contents 
  Lisa_Vector<T>* row;
public:
  /// construct matrix of given format n x m
  Lisa_Matrix(const int n_in, const int m_in);
  
  /// construct copy of a given matrix
  Lisa_Matrix(const Lisa_Matrix<T>& other);
  
  /// destructor
  ~Lisa_Matrix();
  
  /// assign value to all matrix elements
  void fill(const T value);
  
  /// access to i-th row vector
  inline Lisa_Vector<T>& operator[] (const unsigned i){ 
#ifdef LISA_DEBUG
    if (i>=n){
        G_ExceptionList.lthrow("row "+ztos(i)+" of a "+
				               ztos(n)+"-row matrix requested",
                               OUT_OF_RANGE);
        return row[0];
	}
#endif
    return row[i];
  } 
  
  inline const Lisa_Vector<T>& operator[](const unsigned i)const{ 
#ifdef LISA_DEBUG
    if (i>=n){
	  G_ExceptionList.lthrow("row "+ztos(i)+" of a "+
				             ztos(n)+"-row matrix requested",
                             OUT_OF_RANGE);
	  return row[0];
	}
#endif
    return row[i];
  }  
  
  /// number of columns
  int get_m()const{ return m; }
  
  /// number of rows           
  int get_n() const { return n; } 
  
  /// assign another matrix 
  const Lisa_Matrix<T>& operator=(const Lisa_Matrix<T>& other);
  
  /// compare matrices element by element
  bool operator==(const Lisa_Matrix<T>&) const;
  
  /// compare lexicographically
  bool operator<=(const Lisa_Matrix<T>&) const;
  
  /// output in LiSA format
  void write(ostream& = cout) const;
  
  /// output in LiSA format
  void read(istream& = cin);
  
  /// pointer to first row (for iterating the contents)
  inline Lisa_Vector<T> * first_row() const { return row; }
  
  /// pointer after last row (for iterating the contents)
  inline Lisa_Vector<T> * end_row() const { return row+n; }
};

// stream operations for these classes:

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_Vector<T>& v){
     v.write(strm);
     return strm;
}

template<class T>
inline istream& operator >> (istream&strm, Lisa_Vector<T>& v){
     v.read(strm);
     return strm;
}

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_Matrix<T>& m){
     m.write(strm);
     return strm;
}

template<class T>
inline istream& operator >> (istream&strm, Lisa_Matrix<T>& m){
     m.read(strm);
     return strm;
}

inline istream& operator >> (istream&strm, void* ){
     return strm;
}

#endif

//@}
















