/*
 * ************** matching.hpp ******************************************
 * 
 * a matching algorithm for LiSA
 *
 * Owner: Marc Mörig
 *
 * started 11.06.00
 * last changes 05.11.01
 *
 */

#ifndef _matching_h
#define _matching_h

#include <iostream.h>
#include "../basics/matrix.hpp"
#include "../main/global.hpp"
#include "../basics/order.hpp"
#include "../misc/except.hpp"

/** @name Matchings
    The classes Lisa_BipartMatching, Lisa_WeightedMatching 
    and Lisa_BottleneckMatching provide the 
    possibility to calculate Matchings on weighted and
    unweighted Bipartite Graphs. They will also work if |I| != |J|.
    
    The weightings for Lisa_WbipartMatching and Lisa_BottleneckMatching 
    should be from 0 to MAXTIME. Otherwise you may get undefined results.
    
    @author Marc Mörig
 */

//@{

/** This class is not meant to be used directly.
    It represents an operation, so it can be sorted in a 
    Lisa_List and later be found again in the processing 
    time matrix. Used in Lisa_BottleneckMatching.
*/

class Lisa_BottleneckNode : public Lisa_Object{
public:
  /// the machine number for that operation
  int i;
  /// the job number for that operation
  int j;
  /// the processing time for that operation
  TIMETYP value;
  
  /// construct a new Node
  Lisa_BottleneckNode(){
    i = -1;
    j = -1;
  }

  /// construct a new Node 
  Lisa_BottleneckNode(int i_in, int j_in, TIMETYP value_in){
    i = i_in;
    j = j_in;
    value = value_in;
  }
  
  /// destruct this Node
  ~Lisa_BottleneckNode(){}
  
  /// write the object values to a stream 
  void write (ostream& strm = cout) const{
    strm << "[ " << i << " " << j << " " << value << " ]";
  }
  
  /// read the object values from a stream
  void read (istream& strm = cin){
    string s = ""; strm >> s;
    if (s!="[") 
      G_ExceptionList.lthrow("[ expected in Lisa_Bottleneck.read(istream&)",SYNTAX_ERROR);
    strm >> i >> j >> value;
    s = ""; strm >> s;
    if (s!="]") 
      G_ExceptionList.lthrow("] expected in Lisa_Bottleneck.read(istream&)",SYNTAX_ERROR);
  }
  
  /// compare operator for sorting 
  bool operator< (const Lisa_BottleneckNode& other) const{
    return value < other.value;
  } 
  
};


/** Class for calculating maximum matchings on unweighted bipartite 
    graphs.
*/
class Lisa_BipartMatching : public Lisa_GenericObject{
private: 

  // some flags indidcating the state in J_marked and I_marked 
  static const int UNMARKED = -1;
  static const int STARTMARK = -2;

  // pointer to the first elements of the vectors 
  // and matrix used
  int* I_matched_PC;
  int* J_matched_PC;
  Lisa_Vector<bool>* G_row_PC;
  int* I_marked_PC;
  int* J_marked_PC;
  int* q_start_C;
  bool* i_start_C;

  // queues used in calculate
  Lisa_Vector<int>* queue;
  Lisa_Vector<bool>* in_I;  

  // the matrix size
  int m,n;
  
  // indicates wether a max matching has been calculated or not
  bool calculated;
  
  // the matrix of edges ... if the vertex i,j have an edge the value will be
  // 1 else it will be 0
  Lisa_Matrix<bool> *graph; 
    
  // these vectors contain for every vertice the index of the vertice it is 
  // matched with ... it will be UNMARKED (-1) if the vertice is not matched
  Lisa_Vector<int> *I_matched,*J_matched;
  
  // for finding new matching edges the algorithm needs to mark the vertex
  // and find a way from an unmatched vertice in I to an unmatched vertice 
  // in J 
  // the values will be the index of the preceding vertice on the way or
  // UNMARKED (-1) to show this vertice is unmarked
  // STARTMARK (-2) to show this is a starting(first) vertice
  Lisa_Vector<int>  *I_marked,*J_marked;
  
  // these vectors represent the sets of marked vertex in I and J
  Lisa_Vector<bool> *S,*T;
  
  // creates a start matching by going through all edges 
  // and match them if possible
  void inline simple_start_matching();
  void inline ordered_start_matching(); // not used in the current version !

  // the algorithm itself ... it trys to find a way from an unmatched
  // vertice in I to an unmatched vertice in J ... the edges on that way have
  // to be changingly matched and unmatched ... if such a way exists matched 
  // edges will be unmatched while unmatched edges will be matched so there is
  // one more matched edge after this ... if no such way exists the matching 
  // is a maximum one
  void calculate();
  
public:
  
  /** the Vector representing the maximum matching will continue this value
      if the vertice has no matching partner
  */
  static const int UNMATCHED = -1;
  
  /** construct a new matching object with an adjazentmatrix, 
      this constuctor will make a local copy of it
  */
  Lisa_BipartMatching(const Lisa_Matrix<bool> *in);
  /// construct a new matching object with the sizes of I and J
  Lisa_BipartMatching(const int n_in,const int m_in); 
  /// destruct the matching object
  ~Lisa_BipartMatching();

  /// this vector contains wether a vertex in I is marked or not
  const Lisa_Vector<bool>* get_S();
  /// this vector contains wether a vertex in J is marked or not
  const Lisa_Vector<bool>* get_T();
 
  /** the returned vector contains the matching partner
      for all vertice in I or UNMARKED (-1) if it is not
      matched
  */
  const Lisa_Vector<int>* get_matching_I();
  /** the returned vector contains the matching partner
      for all vertice in J or UNMARKED (-1) if it is not
      matched
  */
  const Lisa_Vector<int>* get_matching_J();

  /// allow access to the adjazentmatrix
  bool inline get_edge(const int i,const int j) const;
  
  /** allow access to the adjazentmatrix,
      the calculated matching is resetted
  */
  void inline set_edge(const int i,const int j,const bool value);
  
  /// enables or disables all edges
  void inline set_all_edges(const bool value);
  
  /// returns wether the calculated matching is perfect or not
  bool inline is_perfect();  

  /** writes the object values into a stream
      those values might be different from those returned as matching
      since this method was meant for debugging mostly
  */
  void write(ostream & = cout) const;
};

/** Basic class and interface 
    of Lisa_BottleneckMatching and Lisa_WeightedMatching
*/

class Lisa_Matching : public Lisa_GenericObject{
protected:
  int n,m,mx;
  bool calculated,inverted;
  Lisa_BipartMatching* bipart;

public:
  
  /** the Vectors returned by get_matching_I() and 
      get_matching_J() will contain this value if a 
      vertice has no matching partner
   */
  static const int UNMATCHED = Lisa_BipartMatching::UNMATCHED;
  /** get_graph() will return this value if an edge has
      been removed from the set of edges
   */
  static const TIMETYP REMOVED = -MAXOBJECTIVE;
  
  /// destructs this object 
  virtual ~Lisa_Matching(){};
  
  /** the returned vector contains the matchingpartner 
      for all vertice in I or UNMATCHED (-1) if it is not 
      matched
  */
  virtual const Lisa_Vector<int>* get_matching_I() = 0;
  /** the returned vector contains the matchingpartner 
      for all vertice in J or UNMATCHED (-1) if it is 
      not matched
  */
  virtual const Lisa_Vector<int>* get_matching_J() = 0;

  /// returns the weighting of the edge (i,j) or REMOVED
  virtual TIMETYP get_edge(const int i,const int j) const = 0;

  /** sets the weighting of the edge (i,j) but only 
      if it has not been removed already, the object takes care itself 
      if it has been inverted or not
  */
  virtual void set_edge(const int i,const int j,const TIMETYP value) = 0;
  
  /** updates all not removed edges with 
      new values from the matrix, the inputmatrix must 
      have the same size as the matrix the object was 
      initialized with, the object takes care itself
      if it has been inverted or not
  */
  virtual void set_all_edges(const Lisa_Matrix<TIMETYP>* in) = 0;
  
  /** tell the object to minimize the matchings, calling 
      this twice switchs back to maximum matchings and so on
  */ 
  virtual void invert() = 0;
  
  /// removes the edge (i,j)
  virtual void remove(const int i,const int j) = 0;
};


/** Class for maximum weighted matchings on bipartite graphs. 
    
    The weightings have to be from 0 to MAXTIME .
 */

class Lisa_WeightedMatching : public Lisa_Matching{
private:
  
  // the vertex marking, actually I_mark[i]+J_mark[j] >= graph[i][j] 
  // must be true 
  Lisa_Vector<TIMETYP> *I_mark,*J_mark;
  
  // this matrix contains the weigths for each edge
  Lisa_Matrix<TIMETYP> *graph;
   
  // the algorithm, it calculates a a maximum weighted matching by calculating 
  // a perfect matching on a corresponding unweihgted graph ... this graph is 
  // calculated with the help of the vertex marking
  void calculate();

  // calculates new vertex markings
  void inline calc_start_marks();

  // changes the vertex markings ( if no perfect matching was found on 
  // the corresponding unweigthed graph ) 
  void inline calc_new_marks(); 

public:
  
  /** construct an object with a matrix of edge weightings,
      a local copy will be made
  */
  Lisa_WeightedMatching(const Lisa_Matrix<TIMETYP> *in);
  /// construct an object with the sizes of I an J
  Lisa_WeightedMatching(const int n_in, const int m_in);
  /// destruct the object
  ~Lisa_WeightedMatching();

  const Lisa_Vector<int>* get_matching_I();
  const Lisa_Vector<int>* get_matching_J();

  /// this method provides fast access
  TIMETYP get_edge(const int i,const int j) const;

  /// this method provides fast access
  void set_edge(const int i,const int j,const TIMETYP value);
  
  /// this is rather slow if you want to set only some values
  void set_all_edges(const Lisa_Matrix<TIMETYP>* in);
  
  void remove(const int i,const int j);
  
  void invert();

  /** writes the object values into a stream
      those values might be different from those returned as matching
      since this method was meant for debugging mostly
  */  
  void write(ostream & = cout) const;
 
}; 


/** Lisa_BottleneckMatching calculates perfect matchings
    on weighted gaphs.
    
    The weightigs should always be from 0 to MAXTIME.
*/

class Lisa_BottleneckMatching : public Lisa_Matching{
private:
  // indicates wether the Lisa_List is sorted or not
  bool sorted;
  // ordered List of edgeweightings 
  Lisa_List<Lisa_BottleneckNode>* lst;
  // the matchings, these vectors contain the last matching found
  Lisa_Vector<int> *I_matched,*J_matched;
  // operations for moving the pointer on the list
  // and including/excluding the weigtings for the matching set
  void inline inc_next();
  void inline rem_curr();
  // calculates a new matching 
  void calculate();

public:

  /// create the object with edgeweightings 
  Lisa_BottleneckMatching(const Lisa_Matrix<TIMETYP>* in);
  /// delete the object
  ~Lisa_BottleneckMatching();
  
  const Lisa_Vector<int>* get_matching_I();
  const Lisa_Vector<int>* get_matching_J();
  
  /** writes the object values into a stream
      those values might be different from those returned as matching
      since this method was meant for debugging mostly
  */
  void write(ostream & = cout) const;
  
  void remove(const int i,const int j);
  
  void invert();

  /** this is a rather 
      inefficient method and will take O(m*n) time !
  */
  void set_edge(const int i,const int j,const TIMETYP value);
  
  /** fastest method to change the weightings for 
      for all not removed operations
  */
  void set_all_edges(const Lisa_Matrix<TIMETYP>* in);
 
  /** this is a rather inefficient operation and may take 
      O(m*n) time
   */
  TIMETYP get_edge(const int i,const int j) const;
};

//@}

#endif


























