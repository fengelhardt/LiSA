/*
 * ************** matrix.h*******************************
 * 
 * vectors and matrices with dynamic size and fast access
 *
 * Owner: Thomas Tautenhahn
 *
 * 21.10.99
 *
*/

#ifndef _matrix_h 
#define _matrix_h 

#include <iostream.h>
#include "../misc/except.hpp"
#include "../misc/int2str.hpp"

// remove the following line in the final program:
#define DEBUG


/** @name Vectors and Matrices
    Classes #Lisa_Vector<T># and #Lisa_Matrix<T># provide easy to use 
    vectors and matrices with dynamic size and fast access to the elements.
    Define #DEBUG# to get automatic array bounds check! \\
    
    Include file LiSA/src/basics/matrix.hpp, it uses iostream.
    Use only for basic types int, bool, long, float. Elements of vectors 
    must not allocate memory dynamically! For matrices of lists use 
    lmatrix.h \\
    
    Allways be carefull to use the right operator []! If X is of type 
    Lisa_Vector<int>*, then the first Element of X is (*X)[0] and not 
    "*X[0]" or "X[0]"!
    @author Thomas Tautenhahn
    @version 2.3pre2
    @see Lisa_VectorOfLists
    @see Lisa_MatrixOfLists 
*/ 

//@{

/** Basic vector class in LiSA. */

template<class T>
class Lisa_Vector 
{ 
public:
  /// construct vector with m elements of type T
  Lisa_Vector(const int m);
  /// construct vector as a copy of other
  Lisa_Vector(const Lisa_Vector<T>& other);
  /// assign value to all elements 
  void fill(const T value);             
  /// access to j-th element of vector
  T& operator[] (const unsigned int j)   
    { 
      #ifdef DEBUG
      if (j>=m) 
	{
	  G_ExceptionList.lthrow("element "+ztos((int)j)+" of a "+
				 ztos((int)m)+"-element vector requested",
				 OUT_OF_RANGE);
	  return contents[0];
	}
      #endif
      return contents[j];
    }; 
  T operator[] (const unsigned int j) const   
    { 
      #ifdef DEBUG
      if (j>=m) 
	{
	  G_ExceptionList.lthrow("element "+ztos((int)j)+" of a "+
				 ztos((int)m)+"-element vector requested",
				 OUT_OF_RANGE);
	  return contents[0];
	}
      #endif
      return contents[j];
    }; 
  /// length of vector
  int get_m() const { return m; }
  /// assign a vector to another  
  const Lisa_Vector<T>& operator=(const Lisa_Vector<T>& );
  /// compare vectors
  bool operator==(const Lisa_Vector<T>&) const;
  /// compare lexicographically
  bool operator<=(const Lisa_Vector<T>&) const;
  // input/output in lisa-format
  void write(ostream& = cout) const;
  void read(istream& = cin);
  /// search index of maximal vector element
  int Lisa_Vector<T>::index_of_max() const;
  /// search index of minimal vector element
  int Lisa_Vector<T>::index_of_min() const;
  ~Lisa_Vector();
  T* get_first() const { return contents; }
  T* get_last() const { return contents+m; }
private:
  // size:
  unsigned int m;
  T* contents;   
};

/** Basic matrix class in LiSA. A Lisa_Matrix is a vector of vectors. */
template<class T>
class Lisa_Matrix 
{ 
public:
  /// construct matrix of given format n x m
  Lisa_Matrix(const int n_in, const int m_in);
  /// construct copy of a given matrix
  Lisa_Matrix(const Lisa_Matrix<T>& other);
  /// assign value to all matrix elements
  void fill(const T value);
  /// access to i-th row vector
  Lisa_Vector<T>& operator[] (const unsigned i)  
    { 
      #ifdef DEBUG
      if (i>=n)
	{
	  G_ExceptionList.lthrow("row "+ztos((int)i)+" of a "+
				 ztos((int)n)+"-row matrix requested",
				 OUT_OF_RANGE);
	  return row[0];
	}
      #endif
      return row[i];
    }; 
  const Lisa_Vector<T>& operator[] (const unsigned i) const  
    { 
      #ifdef DEBUG
      if (i>=n)
	{
	  G_ExceptionList.lthrow("row "+ztos((int)i)+" of a "+
				 ztos((int)n)+"-row matrix requested",
				 OUT_OF_RANGE);
	  return row[0];
	}
      #endif
      return row[i];
    };  
  /// number of columns
  int get_m() const { return m; }
  /// number of rows           
  int get_n() const { return n; } 
  /// assign another matrix 
  const Lisa_Matrix<T>& operator=(const Lisa_Matrix<T>& other);
  /// compare matrices element by element
  bool operator==(const Lisa_Matrix<T>&) const;
  /// compare lexicographically
  bool operator<=(const Lisa_Matrix<T>&) const;
  // input/output in LiSA-format:
  void write(ostream& = cout) const;
  void read(istream& = cin);  
  ~Lisa_Matrix();
  /// pointer to first row (for iterating the contents)
  Lisa_Vector<T> * first_row() const { return row; }
  /// Pointer after last row (for iterating the contents)
  Lisa_Vector<T> * end_row() const { return row+n; }
private: 
  unsigned int n, m;
  Lisa_Vector<T>* row;
};

// stream operations for these classes:

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_Vector<T>& v)
   {
     v.write(strm);
     return strm;
   }

template<class T>
inline istream& operator >> (istream&strm, Lisa_Vector<T>& v)
   {
     v.read(strm);
     return strm;
   }

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_Matrix<T>& m)
   {
     m.write(strm);
     return strm;
   }

template<class T>
inline istream& operator >> (istream&strm, Lisa_Matrix<T>& m)
   {
     m.read(strm);
     return strm;
   }

inline istream& operator >> (istream&strm, void* )
   {
     return strm;
   }
#endif

//@}
















