/*
 * ************** lmatrix.hpp *******************************
 * 
 * classes listvector & listmatrix
 *
 * @author Ivo Roessling
 * @version 2.3pre3
 *
 * 11.05.2001
 * 
 */

#ifndef _lmatrix_h
#define _lmatrix_h

#include "../main/global.hpp"
#include "matrix.hpp"
#include "list.hpp"

//**************************************************************************

// Do not use the following type explicitely!
template<class T>
class Lisa_nestedList:public Lisa_List<T>
{
public:
  void write(ostream& strm) const
    {
      Lisa_Node<T> * temp;
      strm << " ( ";
      temp=first_last->succ;
      while (temp!=first_last) 
	{ 
	  strm << ((Lisa_Node<T>*)temp)->value << " "; 
	  temp=temp->succ;
	} while (temp!=first_last);
      strm << ") ";
    }

  void fill(T wert) {
    for (Lisa_Node<T> * temp=first_last->succ;
	 temp!=first_last;
	 temp=temp->succ) {
      temp->value=wert;
    }
  }

  /*
    bool split_up(int x) {
    if (length()<=0) return false;
    if ((first_last->pred)->value < x) return false;
    (first_last->pred)->value-=x;
    append(x);
    return true;
  }
  */

};

//**************************************************************************
 
template<class T> class Lisa_MatrixOfLists;

// Do not use the following type explicitely!
template<class T>
class Lisa_VectorOfLists{ 
protected:
  // size:
  unsigned int m;
  Lisa_nestedList<T>* contents;
  
  // default constructor
  Lisa_VectorOfLists(){contents=0; m=0;}

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
  inline Lisa_nestedList<T>& operator[] (const unsigned int j)   
    { 
      #ifdef LISA_DEBUG
      if (j>=m) 
	{
	  G_ExceptionList.lthrow("element "+ztos(j)+" of a "+
				 ztos(m)+"-element vector requested",
				 OUT_OF_RANGE);
	  return contents[0];
	}
      #endif
      return contents[j];
    }
  
  inline Lisa_nestedList<T> operator[] (const unsigned int j) const   
    { 
      #ifdef LISA_DEBUG
      if (j>=m) 
	{
	  G_ExceptionList.lthrow("element "+ztos(j)+" of a "+
				 ztos(m)+"-element vector requested",
				 OUT_OF_RANGE);
	  return contents[0];
	}
      #endif
      return contents[j];
    }
    
  /// length of vector
  int get_m() const { return m; }
  
  /// assign a vector to another  
  const Lisa_VectorOfLists<T>& operator=(const Lisa_VectorOfLists<T>& );
  
  /// compare vectors
  bool operator==(const Lisa_VectorOfLists<T>&) const;
  bool operator!=(const Lisa_VectorOfLists<T>& other) const { return !(&other == this); }
  
  /// compare lexicographically
  bool operator<=(const Lisa_VectorOfLists<T>&) const;
  
  // input/output in lisa-format
  void write(ostream& = cout) const;
  void read(istream& = cin);
  
  /// search index of maximal vector element
  int index_of_max() const;
  int index_of_maxsum() const;
  int index_of_maxlast() const;
  
  /// search index of minimal vector element
  int index_of_min() const;
  int index_of_minsum() const;
  int index_of_minlast() const;
  
  Lisa_nestedList<T>* get_first() const { return contents; }
  Lisa_nestedList<T>* get_last() const { return contents+m; }
};

//**************************************************************************

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_VectorOfLists<T>& l)
{
  l.write(strm);
  return strm;
}

template<class T>
inline istream& operator >> (istream&strm, Lisa_VectorOfLists<T>& l)
{
  l.read(strm);
  return strm;
}

//**************************************************************************

// Do not use the following type explicitely!
template<class T>
class Lisa_MatrixOfLists: public Lisa_Object 
{
protected:
  unsigned int n, m;
  Lisa_VectorOfLists<T>* row;
  Lisa_Matrix<int> *A;

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
  Lisa_VectorOfLists<T>& operator[] (const unsigned i)  
    { 
      #ifdef LISA_DEBUG
      if (i>=n)
	{
	  G_ExceptionList.lthrow("row "+ztos(i)+" of a "+
				 ztos(n)+"-row listmatrix requested",
				 OUT_OF_RANGE);
	  return row[0];
	}
      #endif
      return row[i];
    }
  const Lisa_VectorOfLists<T>& operator[] (const unsigned i) const  
    { 
      #ifdef LISA_DEBUG
      if (i>=n)
	{
	  G_ExceptionList.lthrow("row "+ztos(i)+" of a "+
				 ztos(n)+"-row listmatrix requested",
				 OUT_OF_RANGE);
	  return row[0];
	}
      #endif
      return row[i];
    }
  /// number of columns
  int get_m() const { return m; }
  /// number of rows           
  int get_n() const { return n; } 
  /// assign another matrix 
  const Lisa_MatrixOfLists<T>& operator=(const Lisa_MatrixOfLists<T>& other);
  /// compare matrices element by element
  bool operator==(const Lisa_MatrixOfLists<T>&) const;
  
  /// compare lexicographically
  //iroess  bool operator<=(const Lisa_MatrixOfLists<T>&) const;
  // input/output in LiSA-format:
  void write(ostream& = cout) const;
  void read(istream& = cin);  
  ~Lisa_MatrixOfLists();
  /// pointer to first row (for iterating the contents)
  Lisa_VectorOfLists<T> * first_row() const { return row; }
  /// Pointer after last row (for iterating the contents)
  Lisa_VectorOfLists<T> * end_row() const { return row+n; }

  Lisa_Matrix<int> * get_MatrixA() const;
  void apply_MatrixA(const Lisa_Matrix<int>& MatrixA, T NullElt);
  /*
public:
  Lisa_MatrixOfLists(const Lisa_MatrixOfLists<T> & other);
  Lisa_MatrixOfLists(const int n_in, const int m_in);
  Lisa_MatrixOfLists(const Lisa_Matrix<int> other_A);
  Lisa_MatrixOfLists(Lisa_Matrix<int> *other_A);

  void read(istream & = cin) {}
  void write(ostream & = cout) const{}
  */
};

//**************************************************************************

template<class T>
inline ostream& operator << (ostream&strm, const Lisa_MatrixOfLists<T>& l)
{
  l.write(strm);
  return strm;
}

template<class T>
inline istream& operator >> (istream&strm, Lisa_MatrixOfLists<T>& l)
{
  l.read(strm);
  return strm;
}

#endif             

//**************************************************************************

