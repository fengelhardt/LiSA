
#ifndef _list_h
#define _list_h

#include <iostream>
#include <string>

#include "../main/global.hpp"
#include "../lisa/lsaobjct.hpp"

//**************************************************************************

/// throw an error from list
void error(std::string);

//**************************************************************************

/// a list node 
/** you should not use this type explicitly
    @see Lisa_List 
    @author Thomas Tautenhahn
    @version 2.3pre3
 */
template<class T>
class Lisa_Node{
public:
  /// pointer to successor in list
  Lisa_Node<T> *succ;
  /// pointer to predecessor inlist
  Lisa_Node<T> *pred;
  /// the object we are holding
  T value;
  
  /// constructor
  /** create node with an initial value */
  Lisa_Node(const T &v) { value=v; pred=NULL; succ=NULL;  }
  /// default constructor
  Lisa_Node()  { pred=NULL; succ=NULL;  }
};

//**************************************************************************

/// Lisa_List is a doubly connected lists with different methods of access.
/** It uses internal pointers, the user has only to give and receive
    elements of the standard type. Lisa_List can emulate vectors, 
    stacks and queues. 

    Using Lisa_List with a basic type T having big memory 
    requirements may lead to undesired copying procedures. In this case, let 
    T be a pointer to the desired type.

    @author Thomas Tautenhahn
    @version 2.3pre3
*/    
template<class T>
class Lisa_List: public Lisa_Object{
private:
  /// position we're at in the list
  int where;
  /// are we at a valid node ?
  bool where_ok;

//**************************************************************************
  
  /// swaps two nodes  
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

//**************************************************************************


/// performs a qsort on the list elements
/** this requires T to have < operator */
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

//**************************************************************************

/// basic pointer handling
void insert_node_after(Lisa_Node<T>* n, Lisa_Node<T>* p){
  p->succ->pred=n;           
  n->succ=p->succ;
  p->succ=n;
  n->pred=p; 
  where_ok=FALSE;
  size++; 
}

//**************************************************************************

/// remove a node from list
void exclude_node(Lisa_Node<T>* n){
  
  if (n==first_last) return;
  
  n->succ->pred=n->pred;
  n->pred->succ=n->succ;
  n->pred=n->succ=NULL;
  size--;
  where_ok=FALSE;
}

//**************************************************************************

/// basic insertion method 
void create_node(const T & v, Lisa_Node<T> * pos)
{
  Lisa_Node<T>* n=new Lisa_Node<T>(v);
  insert_node_after(n, pos);
}

//**************************************************************************

/// basic exclude method 
T destroy_node(Lisa_Node<T> * pos)
{
  T value=  pos->value;              
  exclude_node(pos);
  delete pos; 
  return value;
}

//**************************************************************************

protected:
  /// current size of our list
  unsigned int size;
  /// first and last node of list
  /** please note that the list ia actually a ring, this node is alway in 
      the list */
  Lisa_Node<T> *first_last; 
  
  /// internal pointer to the current node
  Lisa_Node<T> *current_node;

public:

  /// move the internal list pointer to the first node
  /** returns FALSE if the list contains no elements, TRUE otherwise */
  int reset(){
      current_node=first_last->succ;
      where=0;
      return (where_ok=(current_node!=first_last));
    }

//**************************************************************************

  /// move the list pointer one node forward
  /** returns TRUE if the current node is valid, 
      FALSE if the end of the list is reached */
  bool next()
    {
      current_node=current_node->succ;
      where++;
      return (where_ok=(current_node!=first_last));
    }

//**************************************************************************

  /// move the list pointer one node back
  /** returns TRUE if the current node is valid,
      FALSE if the beginning of the list is reached */
  bool previous()
    {
      current_node=current_node->pred;
      where--;
      return (where_ok=(current_node!=first_last));
    }

//**************************************************************************

  /// move the list pointer to i-th node
  /** this operation is slightly inefficient
      because it might take linear time to find an element */
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

//**************************************************************************

  /// get index of current pointer position
  /**  this will return the number of the current node if 
      it is valid */
  int get_index()
    { 
      if (!where_ok) error("undefined list index used");
      
      return where; 
    }

//**************************************************************************
 
  /// returns the length of the list
  int length() const  { return size; }

//**************************************************************************
  
  /// returns TRUE if the list is empty FALSE otherwise
  bool empty() const  { return (!size); }

//**************************************************************************

  /// constructor
  /**  create an empty new list*/
  Lisa_List()
    {
      where_ok=FALSE; 
      size=0;      
      first_last=new Lisa_Node<T>;
      first_last->succ=first_last->pred=first_last;
    }

//**************************************************************************
   
  /// create a new list with another list
  /** a new list will be created and all elements from the other list
      will be copied into the new list */
  Lisa_List(const Lisa_List<T> & other)
    { 
      first_last = new Lisa_Node<T>;
      first_last->succ = first_last->pred = first_last;
      where_ok=FALSE; 
      size=0;
      (*this)=other; 
    }

//**************************************************************************

  /// destructor
  ~Lisa_List()
    {
      clear();
      delete first_last;
    } 

//**************************************************************************
  
  /// assign another list
  /** this will take all elements from the other list 
      and copy them into this list */
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

//**************************************************************************

  ///compare operator
  /** @author Ivo Roessling */
  bool operator!=( Lisa_List<T> const & other)const { return !(*this == other); }

//**************************************************************************

  ///compare operator
  /** @author Ivo Roessling */
  bool operator==( Lisa_List<T> const & other)const {
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

//**************************************************************************

  ///compare operator
  /** @author Ivo Roessling */
  bool operator>( Lisa_List<T> const & other) const{ 
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

//**************************************************************************

  ///compare operator
  /** @author Ivo Roessling */
  bool operator<( Lisa_List<T> const & other) const{ 
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

//**************************************************************************

  /// delete all nodes
  /** this makes the list completly empty */
  void clear()
    {
      while (first_last->succ!=first_last) 
	destroy_node(first_last->succ);
    }       

//**************************************************************************

  /// returns the value of the current node
  /** results are undefined if the currend node is not valid */
  T & get() { return  current_node->value; }

//**************************************************************************

  /// append value to list
  /** a new node will be created and inserted after the last node */
  void append(const T & value) { create_node(value, first_last->pred); }

//**************************************************************************
  
  /// insert value after current node
  /** a new node will be created and inserted after the current node */
  void insert(const T & value) { create_node(value, current_node); }

//**************************************************************************

  /// delete current node and return its value
  /** the internal pointer moves to the next node */
  T exclude()          
    { 
      Lisa_Node<T> * temp=current_node;
      current_node=current_node->succ;
      return destroy_node(temp); 
    }

//**************************************************************************

  /// put a new value into stack or queue, respectively
  void push(const T & value) { create_node(value, first_last); }

//**************************************************************************

  /// get value from stack
  T pop()            { return destroy_node(first_last->succ); }

//**************************************************************************

  /// take value from queue
  T dequeue()        { return destroy_node(first_last->pred); }      

//**************************************************************************

  /// returns the value of i-th element in the list
  /** if that node exists this operation may require linear time */
  T & operator[](unsigned int i)
    {
      locate(i);
      return ((Lisa_Node<T>*) current_node)->value;
    } 

//**************************************************************************

  /// return the value of the first element if the list is not empty 
  /** if the list is empty the return value is undefined */
  T & first() 
    { return first_last->succ->value; }

//**************************************************************************

  /// return the value of the last element if the list is not empty
  /** if the list is empty the return value is undefined */
  T & last() 
    { return first_last->pred->value; }

//**************************************************************************

  /// locates and returns the minimum element of the list
  /** if that is not empty also puts the internal pointer to this node */       
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
    }

//**************************************************************************
  
  /// locates and returns the maximum element of the list
  /** if that is not empty also puts the internal pointer to this node */       
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
    } 

//**************************************************************************

  // sum the values of all the entries...
  /*
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
    } 
  */

//**************************************************************************

  /// moves the internal pointer to the first occurance of the value
  /** returns TRUE if that value is in the list FALSE otherwise */
  bool search_first(const T & Value)
    {
      current_node=first_last;
      where=-1;
      where_ok=true;
      return search_next(Value);
    }

//**************************************************************************

  /// moves the internal pointer to next occurance of the value
  /** starting from the current position 
      returns TRUE if that value is in the rest of the list FALSE otherwise */
  bool search_next(const T & Value)
    {
      first_last->value=Value;
      do next();
      while (!(current_node->value == Value));
      return (current_node!=first_last);
    }

//**************************************************************************

/// sort list by nondecraesing values
/** this sort uses a quicksort algorithm */
void sort(){
  if (size>1) qsort(first_last->succ,first_last->pred);
}

//**************************************************************************
    
/// fills all nodes in the list with wert   
void fill(T wert) {
  for (Lisa_Node<T> * temp=first_last->succ;temp!=first_last;temp=temp->succ){
        temp->value=wert;
  }
}


//**************************************************************************

  /// write the list values to a stream
  /** if T is a pointer type it will write 
      the pointer, but not the object to the stream */
  void write(std::ostream& strm) const{

   Lisa_Node<T> * temp;
   strm << " ( ";
   temp=first_last->succ;
   
   while (temp!=first_last){ 
     strm << ((Lisa_Node<T>*)temp)->value << " "; 
     temp=temp->succ;
   }while (temp!=first_last);
   
   strm << ") ";
 }

//**************************************************************************

/// read the list values from a stream
void read(std::istream& strm){
  T value;

  clear();
    
  std::string S = "";
  strm >> S;
  if (S!="(") error ("( expected in Lisa_List::read().");
  
  std::streampos pos = strm.tellg();
  strm >> S;
  
  while(S!=")" && S != ""){
    
    strm.seekg(pos);
    strm >> value;
    append(value);
    
    pos = strm.tellg();
    strm >> S; 
  }
  
  if (S!=")") error(") expected in Lisa_List::read().");
}

//**************************************************************************

};

//**************************************************************************

/// stream operator for writing a Lisa_List
template<class T>
inline std::ostream& operator << (std::ostream& strm, const Lisa_List<T>& l)
{
  l.write(strm);
  return strm;
}

//**************************************************************************

/// stream operator for reading a Lisa_List
template<class T>
inline std::istream& operator >> (std::istream&strm, Lisa_List<T>& l)
{
  l.read(strm);
  return strm;
}

//**************************************************************************

#endif

