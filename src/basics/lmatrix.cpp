/*
 * ************** listmatrix.cpp *******************************
 * 
 * class listmatrix
 *
 * Owner: Ivo Roessling
 *
 * 11.05.01
 */
#include <ostream>
#include <string>

#include "pair.hpp"
#include "lmatrix.hpp"

using namespace std;

extern string G_ExtraException;

/* *******************************************************
 * 
 * begin of classdef for listvector
 *
 */


// creating a VectorOfLists with size m  (--> combination of m Lisa_nestedLists)
template<class T>
Lisa_VectorOfLists<T>::Lisa_VectorOfLists(int m_in)
     {
       m=m_in;   //setting the internal size variable
       contents= new Lisa_nestedList<T>[m];   //the contents: an array of Lisa_nestedList (=nestedList)
     }  


// copy-constructor
template<class T>
Lisa_VectorOfLists<T>::Lisa_VectorOfLists(const Lisa_VectorOfLists<T>& other){
#ifdef LISA_DEBUG
       G_ExceptionList.lthrow("(Warning) listvector given by value");
#endif
       m=other.m; 
       
       contents= new Lisa_nestedList<T>[m];
       for (unsigned int i=0;i<m;i++) 
	 contents[i] = other.contents[i];
}


template<class T>
Lisa_VectorOfLists<T>::Lisa_VectorOfLists(const Lisa_Vector<T>& nonpreemtive) {
#ifdef LISA_DEBUG
       G_ExceptionList.lthrow("(Warning) listvector given by value");
#endif
       m=nonpreemtive.get_m(); 
       
       contents= new Lisa_nestedList<T>[m];
       for (unsigned int i=0;i<m;i++) 
	 contents[i].append(nonpreemtive[i]);
}


//*************************
//iroess: noch zu implementieren:
template<class T>
void Lisa_VectorOfLists<T>::fill(T wert) 
     {
       unsigned int i;
       for(i=m; i--; ) contents[i].fill(wert);
     }
//*************************/

//*************************
//iroess: noch zu implementieren:
template<class T>
int Lisa_VectorOfLists<T>::index_of_max() const
{
  unsigned int i,j;
  T value=contents[0].max();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->max()>value) { value=go->max(); i=j; }
  return i;
}

/*
template<class T>
int Lisa_VectorOfLists<T>::index_of_maxsum() const
{
  unsigned int i,j;
  T value=contents[0].sum();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->sum()>value) { value=go->sum(); i=j; }
  return i;
}
*/

template<class T>
int Lisa_VectorOfLists<T>::index_of_maxlast() const
{
  unsigned int i,j;
  T value=contents[0].last();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->last()>value) { value=go->last(); i=j; }
  return i;
}
//*************************/


//*************************
//iroess: noch zu implementieren:
template<class T>
int Lisa_VectorOfLists<T>::index_of_min() const 
{
  unsigned int i,j;
  T value=contents[0].min();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->min()<value) { value=go->min(); i=j; }
  return i;
}

/*
template<class T>
int Lisa_VectorOfLists<T>::index_of_minsum() const 
{
  unsigned int i,j;
  T value=contents[0].sum();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->sum()<value) { value=go->sum(); i=j; }
  return i;
}
*/

template<class T>
int Lisa_VectorOfLists<T>::index_of_minlast() const 
{
  unsigned int i,j;
  T value=contents[0].last();
  Lisa_nestedList<T> * go;
  i=0;
  for(j=1,go=contents+1 ; j<m ; go++,j++) 
    if (go->last()<value) { value=go->last(); i=j; }
  return i;
}
//*************************/

// assign-operator: assigning a VectorOfLists another one
template<class T>
const Lisa_VectorOfLists<T>& Lisa_VectorOfLists<T>::operator=(const Lisa_VectorOfLists<T>& other) 
     {
       if (&other != this) {
	 m=other.m; 
	 contents= new Lisa_nestedList<T>[m];
	 for (unsigned int i=0;i<m;i++) {
	   contents[i] = other.contents[i];
	 }

       }
       return *this;
     }  


// to write object's defining data into an (output)stream
template<class T>
void Lisa_VectorOfLists<T>::write(ostream& strm) const 
     {
       unsigned int i;
       strm << "{ ";
       for (i=0 ; i<m ; i++)
          { 
	    //            strm.width(3);                        // witdh of next output
            strm<<contents[i] << " ";
          }
       strm << "}\n";
     }


// to read object's defining data out of an (input)stream
template<class T>
void Lisa_VectorOfLists<T>::read(istream& strm) 
     {
       unsigned int i;
       string S;
  
       S="";strm >> S; 
       if (S!="{") 
          G_ExceptionList.lthrow("{ expected in listvector.read",SYNTAX_ERROR);
       for (i=0;i<m;i++)
          strm >> contents[i];
       S="";strm >> S; 
       if (S!="}")
          G_ExceptionList.lthrow("} expected in listvector.read",SYNTAX_ERROR);
     }


// the std. destructor
template<class T>
Lisa_VectorOfLists<T>::~Lisa_VectorOfLists()
     {
#ifdef LISA_DEBUG
       if (!contents) G_ExceptionList.lthrow("listvector without contents");
#endif       
       delete[] contents;  // iroess: muss ich hier vorher explizit die einzelnen eintraege killen?!?
     }


//*************************
//iroess: how(!) should i implement this relation?!
// definition of equality-test operator
template<class T>
bool Lisa_VectorOfLists<T>::operator==(const Lisa_VectorOfLists<T>& other) const
     { 
       if (!(get_m()==other.get_m())) return false;
       unsigned int i;
       for (i=0; i<m; i++) 
          if (!(contents[i]== other.contents[i])) return FALSE;
       return true;
     }
//*************************/


/*************************
//iroess: how(!) should i implement this relation?!
template<class T>
bool Lisa_VectorOfLists<T>::operator<=(const Lisa_VectorOfLists<T>& other) const
     { 
       unsigned int i;
       for (i=0; i<m; i++) 
          {
            if (contents[i]> other.contents[i]) return FALSE;
            if (contents[i]< other.contents[i]) return TRUE;
          }
       return true;
     }
*************************/



/* 
 *
 * end of classdef for listvector
 *
 * *******************************************************/



/* *******************************************************
 * 
 * begin of classdef for listmatrix
 *
 */

// copy-constructor
template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(const Lisa_MatrixOfLists<T>& other)
     {
       int i;
#ifdef LISA_DEBUG
       G_ExceptionList.lthrow("(Warning) matrix given by value");
#endif
       n=other.n;
       m=other.m;
       row=new Lisa_VectorOfLists<T>[n](m);
       for ( i=n ; i-- ; )
          row[i]=other.row[i];
     }

// constructor for matrix with format (n)x(m) of list operations
template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(int n_in, int m_in)
     {
       n=n_in; m=m_in;
       row=new Lisa_VectorOfLists<T>[n](m);
     }

// copy-constructor
template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(const Lisa_Matrix<T>& nonpreemptive) {
#ifdef LISA_DEBUG
       G_ExceptionList.lthrow("(Warning) listmatrix given by value");
#endif
       m=nonpreemptive.get_m(); 
       n=nonpreemptive.get_n(); 
       
       row=new Lisa_VectorOfLists<T>[n](m);
       for (unsigned int j=0; j<n;j++)
	 for (unsigned int i=0; i<m;i++)
	   row[j][i].append(nonpreemptive[j][i]);
}



template<class T>
void Lisa_MatrixOfLists<T>::fill(const T wert) 
     {
       int i;
       for ( i=n ; i-- ; )
           row[i].fill(wert);
     }

// assign-operator: assigning a MatrixOfLists another one
template<class T>
const Lisa_MatrixOfLists<T>& Lisa_MatrixOfLists<T>::operator=(const Lisa_MatrixOfLists<T>& other) 
{
  Lisa_VectorOfLists<T> * rowptr;
  Lisa_VectorOfLists<T> * otherptr;
  Lisa_VectorOfLists<T> * endptr;

  if (&other == this)
    return *this;
  if ((n!=other.n) || (m!=other.m))
    {
      G_ExceptionList.lthrow("wrong format argument to matrix.operator=");
      return *this;
    } 

  endptr=end_row();
  for (rowptr=first_row(), otherptr=other.first_row(); rowptr!=endptr; 
       rowptr++, otherptr++)
    *rowptr=*otherptr; 
  return *this;
}      


template<class T>
void Lisa_MatrixOfLists<T>::write(ostream& strm) const 
     {
       unsigned int i;
       strm << "{\n";
       for (i=0 ; i<n ; i++) 
          strm << " " << row[i];
       strm << "}\n";
     }


template<class T>
void Lisa_MatrixOfLists<T>::read(istream& strm) 
     {
       unsigned int i;
       string S;
  
       S="";strm >> S; 
       if (S!="{")           
         G_ExceptionList.lthrow("} expected in listmatrix.read",SYNTAX_ERROR);
       for (i=0;i<n;i++)
          {
	    strm >> row[i];
          }
       S="";strm >> S; 
       if (S!="}")           
         G_ExceptionList.lthrow("} expected in listmatrix.read",SYNTAX_ERROR);
     } 


template<class T>
Lisa_MatrixOfLists<T>::~Lisa_MatrixOfLists()
     {
       if (row) delete[] row;
     }


//*************************
//iroess: noch zu implementieren:
template<class T>
bool Lisa_MatrixOfLists<T>::operator==(const Lisa_MatrixOfLists<T>& other) const
     { 
       if (!(get_m()==other.get_m() && get_n()==other.get_n())) return false;
       unsigned int i;
       for (i=0; i<n; i++) 
          if (!(row[i]== other.row[i])) return FALSE;
       return true;
     }
//*************************/

template<class T>
Lisa_Matrix<int> * Lisa_MatrixOfLists<T>::get_MatrixA() const {
  Lisa_Matrix<int> *result=new Lisa_Matrix<int>(m,n);
  for (unsigned int j=0; j<n;j++)
    for (unsigned int i=0; i<m;i++)
      (*result)[i][j]=row[j][i].length();
  return result;
}

template<class T>
void Lisa_MatrixOfLists<T>::apply_MatrixA(const Lisa_Matrix<int>& MatrixA, T NullElt ) {
  for (unsigned int j=0; j<n;j++) 
    for (unsigned int i=0; i<m;i++) {
      for (;row[j][i].length()<MatrixA[j][i];row[j][i].dequeue());
      for (;row[j][i].length()>MatrixA[j][i];row[j][i].append(NullElt));
    }

}

/*************************
//iroess: noch zu implementieren:
template<class T>
bool Lisa_MatrixOfLists<T>::operator<=(const Lisa_MatrixOfLists<T>& other) const
     { 
       unsigned int i, j;
       for (i=0; i<n; i++)
          for (j=0; j<m; j++) 
            {
              if (row[i][j]> other.row[i][j]) return FALSE;
            if (row[i][j]< other.row[i][j]) return TRUE;
          }
       return true;
     }
*************************/

/*
template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(const int n_in, const int m_in) {
    A=new Lisa_Matrix<int>(n_in,m_in);
}

template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(const Lisa_Matrix<int> other_A) {
  *A=other_A;
}

template<class T>
Lisa_MatrixOfLists<T>::Lisa_MatrixOfLists(Lisa_Matrix<int> *other_A) {
  A=other_A;
}

*/

// define classes for following types T:
template class Lisa_nestedList<Lisa_Pair>;


template class Lisa_MatrixOfLists<int>;
template class Lisa_MatrixOfLists<float>;
template class Lisa_MatrixOfLists<double>;
template class Lisa_MatrixOfLists<bool>;
template class Lisa_MatrixOfLists<Lisa_Pair>;


template class Lisa_VectorOfLists<int>;
template class Lisa_VectorOfLists<float>;
template class Lisa_VectorOfLists<double>;
template class Lisa_VectorOfLists<bool>;
template class Lisa_VectorOfLists<string>;
template class Lisa_VectorOfLists<Lisa_Pair>;

template class Lisa_MatrixOfLists<long>;
template class Lisa_VectorOfLists<long>;
