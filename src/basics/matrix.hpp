
#ifndef _lisa_matrix_h_ 
#define _lisa_matrix_h_ 

#include <iostream>

#include "../main/global.hpp"
#include "../basics/list.hpp"
#include "../misc/except.hpp"
#include "../misc/int2str.hpp"

//**************************************************************************

template<class T> class Lisa_Matrix;

//**************************************************************************

/// basic vector class in LiSA.
/** Class Lisa_Vector provides an easy to use vector with dynamic size and 
    fast access to the elements.
    
    Define LISA_DEBUG to get an automatic array bounds check! 
    
    Elements of vectors must not allocate memory dynamically! For vectors of 
    lists use Lisa_VectorOfLists.
    
    @author Thomas Tautenhahn
    @version 2.3final
    @see Lisa_Matrix
*/
template<class T>
class Lisa_Vector:public Lisa_Object{
private:
  /// size
  unsigned int m;
  
  /// array to hold elements
  T* contents;
  
  /// default constructor to create an empty vector
  Lisa_Vector();

  /// friend needed to hack around non std constructs in Lisa_Matrix
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
                                Lisa_ExceptionList::OUT_OF_RANGE);
	  return contents[0];
	}
#endif
    return contents[j];
  }
  
  /// access to j-th element of vector  
  inline T operator[](const unsigned int j)const{ 
#ifdef LISA_DEBUG
    if (j>=m){
      G_ExceptionList.lthrow("element "+ztos(j)+" of a "+
				             ztos(m)+"-element vector requested",
                             Lisa_ExceptionList::OUT_OF_RANGE);
	  return contents[0];
	}
#endif
    return contents[j];
  }
  
  /// length of vector
  inline int get_m()const{ return m; }
  
  /// assign a vector to another  
  const Lisa_Vector<T>& operator=(const Lisa_Vector<T>& );
  
  /// compare vectors
  bool operator==(const Lisa_Vector<T>&) const;
  
  /// compare lexicographically
  bool operator<=(const Lisa_Vector<T>&) const;
  
  /// input in lisa-format
  void write(std::ostream& = std::cout) const;
  
  /// output in lisa format
  void read(std::istream& = std::cin);
  
  /// search index of maximal vector element
  unsigned int index_of_max() const;
  
  /// search index of minimal vector element
  unsigned int index_of_min() const;
  
  /// returns pointer to first element of the vector
  inline T* get_first() const { return contents; }
  
  /// returns pointer after the last element of the vector
  inline T* get_last() const { return contents+m; }  
};

//**************************************************************************

/// basic matrix class in LiSA
/** A Lisa_Matrix is a vector of vectors, it provides an easy to use matrix 
    with dynamic size and fast access to the elements.
    
    Define LISA_DEBUG to get an automatic array bounds check! 
    
    Elements of matrices must not allocate memory dynamically! For matrices of 
    lists use Lisa_MatrixOfLists.
    
    @author Thomas Tautenhahn
    @version 2.3final
    @see Lisa_Vector
*/
template<class T>
class Lisa_Matrix:public Lisa_Object{
private: 
  /// size
  const unsigned int m,n;
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
                               Lisa_ExceptionList::OUT_OF_RANGE);
        return row[0];
	}
#endif
    return row[i];
  } 
  
  /// access to i-th row vector  
  inline const Lisa_Vector<T>& operator[](const unsigned i)const{ 
#ifdef LISA_DEBUG
    if (i>=n){
	  G_ExceptionList.lthrow("row "+ztos(i)+" of a "+
				             ztos(n)+"-row matrix requested",
                             Lisa_ExceptionList::OUT_OF_RANGE);
	  return row[0];
	}
#endif
    return row[i];
  }  
  
  /// number of columns
  inline int get_m()const{ return m; }
  
  /// number of rows           
  inline int get_n() const { return n; } 
  
  /// assign another matrix 
  const Lisa_Matrix<T>& operator=(const Lisa_Matrix<T>& other);
  
  /// compare matrices element by element
  bool operator==(const Lisa_Matrix<T>&) const;
  
  /// compare lexicographically
  bool operator<=(const Lisa_Matrix<T>&) const;
  
  /// output in LiSA format
  void write(std::ostream& = std::cout) const;
  
  /// output in LiSA format
  void read(std::istream& = std::cin);
  
  /// pointer to first row (for iterating the contents)
  inline Lisa_Vector<T> * first_row() const { return row; }
  
  /// pointer after last row (for iterating the contents)
  inline Lisa_Vector<T> * end_row() const { return row+n; }
};

//**************************************************************************

/// dummy stream read operator
/** why is this here ? */
inline std::istream& operator >> (std::istream& strm, void* ){
     return strm;
}

//**************************************************************************

#endif

