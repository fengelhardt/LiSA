/*
 * ************** aufgabe23.hpp *******************************
 * 
 * classes Lisa_List<T> where T is a C++ standard type
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998
 *
 * 19.09.2000 added qsort support
 * 
 */

#ifndef _list_h
#define _list_h
#include<iostream.h>
#include <strstream.h>
#include "global.hpp"
#include "../lisa_dt/lsaobjct.hpp"
#include "../basics/lisa_str.hpp"

// just a little bit of hide and seek, written to annoy the developer
void error(string);

// Do not use the following type explicitely!
template<class T>
class Lisa_Node
{
public:
  Lisa_Node<T> *succ;
  Lisa_Node<T> *pred;
  T value;
  
  Lisa_Node(const T &v) { value=v; pred=NULL; succ=NULL;  }
  Lisa_Node()  { pred=NULL; succ=NULL;  }
};

/** Lisa_List<T> is a doubly connected lists with different methods of access.
    It uses internal pointers, the user has only to give and receive
    elements of the standard type. Lisa_List<T> can emulate vectors, 
    stacks and queues. \\

    Include file LiSA/source/basics/list.hpp, it will use iostream, strstream, 
    and lisa_str.\\

    WARNING! Using Lisa_List<T> with a basic type T having big memory 
    requirements may lead to undesired copying procedures. In this case, let 
    T be a pointer to the desired type.

    @author Thomas Tautenhahn
    @version 2.3pre1
    @see Lisa_GenericList
    @see Lisa_BasicList
*/ 

template<class T>
class Lisa_List: public Lisa_Object
{
private:
  int where;
  bool where_ok;

  void swap_nodes(Lisa_Node<T>* first,Lisa_Node<T>* second){

    if (first->succ==second){
    
      first->succ=second->succ;
      second->pred=first->pred;
      second->succ=first;
      first->pred=second;
      
      second->pred->succ=second;
      first->succ->pred=first;

    }else if(second->succ==first){
      
      first->pred = second->pred;
      second->succ = first->succ;
      first->succ = second;
      second->pred = first;
      
      first->pred->succ=first;
      second->succ->pred=second;
      
    }else{
      
      Lisa_Node<T>* pr= first->pred; 
      Lisa_Node<T>* su= first->succ;
      first->pred=second->pred; 
      first->succ=second->succ;
      second->pred=pr; 
      second->succ=su;
      
      first->pred->succ=first;
      first->succ->pred=first;
      second->succ->pred=second;
      second->pred->succ=second;

    }
    
  }

  void qsort(Lisa_Node<T>* l,Lisa_Node<T>* h){  
    
    if (l!=h){
      Lisa_Node<T>* lo = l;
      Lisa_Node<T>* hi = h;
      
      const T pivot = lo->value;
      
      while (hi!=lo){	

	while (hi!=lo && !(hi->value < pivot))
	  hi = hi->pred;
	
	while (hi!=lo && lo->value <  pivot)
	  lo = lo->succ;
	
	if(hi!=lo){
	  swap_nodes(lo,hi);
	  if (l==lo) l = hi; // yeah ... check this ;D
	  if (h==hi) h = lo;
	  Lisa_Node<T>* swap = hi;
	  hi = lo;
	  lo = swap; 
	  hi = hi->pred;
	  if (hi!=lo) lo = lo->succ;
	}
      }
      if (hi==h){
	qsort(l,lo->pred);
      }else if(lo==l){
	qsort(hi->succ,h);
      }else{
	if(lo->value < pivot){
	  hi = hi->succ;
	}else{
	  lo = lo->pred;
	}
        if (l!=lo) qsort(l,lo);
        if (hi!=h) qsort(hi,h);
      }
    }   
  }

  //basic pointer handling:
  void insert_node_after(Lisa_Node<T>* n, Lisa_Node<T>* p)
    {
      p->succ->pred=n;           
      n->succ=p->succ;
      p->succ=n;
      n->pred=p; 
      where_ok=FALSE;
      size++; 
    }
  void exclude_node(Lisa_Node<T>* n)
    {
      if (n==first_last)
	{
	  return;
	}
      n->succ->pred=n->pred;
      n->pred->succ=n->succ;
      n->pred=n->succ=NULL;
      size--;
      where_ok=FALSE;
    }
  // basic insert method 
  void create_node(const T & v, Lisa_Node<T> * pos)
    {
      Lisa_Node<T>* n=new Lisa_Node<T>(v);
      insert_node_after(n, pos);
    }
  // basic exclude method 
  T destroy_node(Lisa_Node<T> * pos)
    {
      T value=  pos->value;              
      exclude_node(pos);
      delete pos; 
      return value;
    }
protected:
  unsigned int size;
  Lisa_Node<T> *first_last; 
  Lisa_Node<T> *current_node;

public:    
  /// moving internal list pointer current_node to the first node
  int reset()
    {
      current_node=first_last->succ;
      where=0;
      return (where_ok=(current_node!=first_last));
    }
  /// moving list pointer one node forward
  bool next()
    {
      current_node=current_node->succ;
      where++;
      return (where_ok=(current_node!=first_last));
    }
  /// moving list pointer one node backward
  bool previous()
    {
      current_node=current_node->pred;
      where--;
      return (where_ok=(current_node!=first_last));
    }
  /// moving list pointer to i-th node (may take O(length) time!)
  void locate(unsigned int const i)
    {
      if (i>=size) 
	{ 
	  error( "list index out of range");
	  current_node=first_last->succ;
          return;
	}   
      if ((!where_ok) || ( ((int)i)< where/2)) reset();           
      if (i>(size+where)/2)
	{
	  where=size-1;
	  current_node=first_last->pred;
	}
      while ( ((int)i) <where) previous();
      while ( ((int)i) >where) next();
    }

  /// get index of current pointer position:
  int get_index()
    { 
      if (!where_ok) error("undefined list index used");
      
      return where; 
    }
 
  /// length of the list
  int length() const  { return size; }
  /// Is the list empty?
  bool empty() const  { return (!size); }

  Lisa_List()    
    {
      where_ok=FALSE; 
      size=0;      
      first_last=new Lisa_Node<T>;
      first_last->succ=first_last->pred=first_last;
    }


  ~Lisa_List()
    {
      clear();
      delete first_last;
    } 
  
  /** @name General Functions: */
  //@{
  /// assign another list
  const Lisa_List<T>& operator=( Lisa_List<T> const & other) 
    {
      if (&other == this)
      return *this;

      clear();
      Lisa_Node<T>* temp;
      temp=other.first_last->succ;
      while (temp!=other.first_last) 
      { 
	append( temp->value);
	temp=temp->succ;
      }
      
      return *this;
    }

  // by iroess
  const bool operator!=( Lisa_List<T> const & other) { return !(*this == other); }

  const bool operator==( Lisa_List<T> const & other) 
    {
      if (&other == this)
      return true;
      if (!(other.length()==length())) return false;


      Lisa_Node<T>* temp=first_last->succ;
      Lisa_Node<T>* temp_other=other.first_last->succ;

      for (int i=0;
	   (temp!=first_last && temp_other!=other.first_last); 
	   i++,temp=temp->succ,temp_other=temp_other->succ) 
      { 
	if (temp->value!=temp_other->value ) return FALSE;
      }
      
      return true;
    }

  // by iroess
  const bool operator>( Lisa_List<T> const & other) { 
      if (&other == this)
      return false;

      Lisa_Node<T>* temp=first_last->succ;
      Lisa_Node<T>* temp_other=other.first_last->succ;

      for (int i=0;
	   (temp!=first_last && temp_other!=other.first_last); 
	   i++,temp=temp->succ,temp_other=temp_other->succ) 
      { 
	if (temp->value>temp_other->value ) return TRUE;
      }
      if (length()>other.length()) return TRUE;

      return FALSE;
  }


  // by iroess
  const bool operator<( Lisa_List<T> const & other) { 
      if (&other == this)
      return false;

      Lisa_Node<T>* temp=first_last->succ;
      Lisa_Node<T>* temp_other=other.first_last->succ;

      for (int i=0;
	   (temp!=first_last && temp_other!=other.first_last); 
	   i++,temp=temp->succ,temp_other=temp_other->succ) 
      { 
	if (temp->value<temp_other->value ) return TRUE;
      }
      if (length()<other.length()) return TRUE;

      return FALSE;
  }
   
  Lisa_List(const Lisa_List<T> & other)
    { 
      first_last = new Lisa_Node<T>;
      first_last->succ = first_last->pred = first_last;
      where_ok=FALSE; 
      size=0;
      (*this)=other; 
    }
  /// delete all nodes 
  void clear()
    {      
      while (first_last->succ!=first_last) 
	destroy_node(first_last->succ);
    }       
  //@} 
  
  /** @name Serial Access (for moving the internal pointer see Lisa_BasicList):*/
  //@{
  /// value of node where the internal pointer points to
  T & get() { return  current_node->value; }
  /// append value to list
  void append(const T & value) { create_node(value, first_last->pred); }
  /// insert value after current node
  void insert(const T & value) { create_node(value, current_node); }
  /** delete current node and return its value, the internal pointer 
      moves to the next node */
  T exclude()          
    { 
      Lisa_Node<T> * temp=current_node;
      current_node=current_node->succ;
      return destroy_node(temp); 
    }
  //@}
  /** @name Stack and Queue: */
  //@{
  /// put new value into stack or queue, respectively
  void push(const T & value) { create_node(value, first_last); }
  /// get value from stack
  T pop()            { return destroy_node(first_last->succ); }
  /// take value from queue 
  T dequeue()        { return destroy_node(first_last->pred); }      
  //@}
  /** @name Direct Adressing: */     
  //@{      
  /// value of i-th element in the list (may require O(length) time!)
  T & operator[](unsigned int i)
    {
      locate(i);
      return ((Lisa_Node<T>*) current_node)->value;
    } 
  /// value of first element
  T & first() 
    { return first_last->succ->value; }
  /// value of last element 
  T & last() 
    { return first_last->pred->value; }
  //@}
  
  /** @name Searching and Sorting:*/
  //@{
  /// get minimum (and put internal pointer to this node)       
  T min()
    {
      if (!size) 
	  error( "minimium over an empty list undefined");
      Lisa_Node<T> *test;
      where_ok=FALSE;
      current_node=test=first_last->succ;
      while (test->succ!=first_last)
	{
	  test=test->succ;
	  if (test->value < current_node->value ) 
	    current_node=test;
	}
      return current_node->value;
    }; 
  /** @name Searching and Sorting:*/
  //@{
  /// get maximum (and put internal pointer to this node)       
  T max()
    {
      if (!size) 
	  error( "maximum over an empty list undefined");
      Lisa_Node<T> *test;
      where_ok=FALSE;
      current_node=test=first_last->succ;
      while (test->succ!=first_last)
	{
	  test=test->succ;
	  if (test->value > current_node->value ) 
	    current_node=test;
	}
      return current_node->value;
    }; 

  // sum the values of all the entries...
  T sum()
    {
      T result=0;
      if (!size)
	  error( "sum over an empty list undefined");
      Lisa_Node<T> *test;
      where_ok=FALSE;
      test=first_last->succ;
      while (test->succ!=first_last)
	{
	  result+=test->value;
	  test=test->succ;
	}
      return result;
    }; 


  /// move internal pointer to first occurance of value or return FALSE
  bool search_first(const T & Value)
    {
      current_node=first_last;
      where=-1;
      where_ok=true;
      return search_next(Value);
    }
  /// move pointer to next occurance
  bool search_next(const T & Value)
    {
      first_last->value=Value;
      do next();
      while (!(current_node->value == Value));
      return (current_node!=first_last);
    }
  /// sort list by nondecraesing values 
  void sort()
    {
       if (size>1) qsort(first_last->succ,first_last->pred);
    };
  //@}
  // io-stuff:        
  void write(ostream& strm) const
    {
      Lisa_Node<T> * temp;
      strm << " ( ";
      temp=first_last->succ;
      while (temp!=first_last) 
	{ 
	  strm << ((Lisa_Node<T>*)temp)->value << " "; 
	  temp=temp->succ;
	}  while (temp!=first_last);
      strm << ") "<<endl;
    };

  void read(istream& strm) 
    {
      T value;
      char* S;      // benutze char*, da C-Library kein stream(string) kennt
      
      S = new char[80];      
      istrstream *is;
      
      clear();
      strm >> S;
      if (S[0]!='(') 
	error ("( expected in tlist.read");
      strm >> S;
      
      while(S[0]!=')' && S!="") 
	{
	  is=new istrstream(S);
	  *is >> value;
	  delete is;
	  append(value);
	  strm >> S; 
	}
      if (S[0]!=')') 
        error(") expected in tlist.read");
      delete S;
    }	
};

// usual i/o - stuff:
template<class T>
inline ostream& operator << (ostream&strm, const Lisa_List<T>& l)
{
  l.write(strm);
  return strm;
};

template<class T>
inline istream& operator >> (istream&strm, Lisa_List<T>& l)
{
  l.read(strm);
  return strm;
};

#endif             








