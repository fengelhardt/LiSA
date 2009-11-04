
#ifndef _matching_h
#define _matching_h

#include <iostream>

#include "../basics/matrix.hpp"
#include "../main/global.hpp"
#include "../basics/order.hpp"
#include "../misc/except.hpp"

//**************************************************************************

/// node representing an operation for Lisa_BottleneckMatching
/** This class is not meant to be used directly.
    It represents an operation, so it can be sorted in a 
    Lisa_List and later be found again in the processing 
    time matrix.
    
   @author Marc Moerig
   @version 3.0pre1
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
  /** Lisa_List can only store types with a default constructor */
  inline Lisa_BottleneckNode(){
    i = j = -1;
  }

  /// construct a new Node 
  inline Lisa_BottleneckNode(int i_in, int j_in, TIMETYP value_in){
    i = i_in;
    j = j_in;
    value = value_in;
  }
  
  /// destruct this Node
  inline ~Lisa_BottleneckNode(){}
  
  /// write the object values to a stream 
  void inline write(std::ostream& strm = std::cout) const{
    strm << "[ " << i << " " << j << " " << value << " ]";
  }
  
  /// read the object values from a stream
  void read(std::istream& strm = std::cin){
    std::string s = ""; strm >> s;
    if (s!="[") 
      G_ExceptionList.lthrow("[ expected in Lisa_Bottleneck::read()",Lisa_ExceptionList::SYNTAX_ERROR);
    strm >> i >> j >> value;
    s = ""; strm >> s;
    if (s!="]") 
      G_ExceptionList.lthrow("] expected in Lisa_Bottleneck::4read()",Lisa_ExceptionList::SYNTAX_ERROR);
  }
  
  /// compare operator for sorting in Lisa_List
  bool inline operator< (const Lisa_BottleneckNode& other) const{
    return value < other.value;
  } 
  
};

//**************************************************************************

/// Class for calculating maximum matchings on unweighted bipartite graphs.
/** It also works if |I| != |J|.
   @author Marc Moerig
   @version 3.0pre1
*/
class Lisa_BipartMatching : public Lisa_GenericObject{
private: 
  
  //@{
  /// some flags indidcating the state in J_marked and I_marked 
  static const int UNMARKED = -1;
  static const int STARTMARK = -2;
  //@}
  
  //@{
  /// pointer to the first elements of the vectors and matrices used
  int* I_matched_PC;
  int* J_matched_PC;
  Lisa_Vector<bool>* G_row_PC;
  int* I_marked_PC;
  int* J_marked_PC;
  int* q_start_C;
  bool* i_start_C;
  //@}

  //@{
  /// queues used in calculate()
  Lisa_Vector<int>* queue;
  Lisa_Vector<bool>* in_I;  
  //@}

  /// the matrix size
  int m,n;
  
  /// indicates wether a max matching has been calculated or not
  bool calculated;
  
  /// the matrix of edges
  /** if the vertex i,j have an edge the value will be 1 else it will be 0 */
  Lisa_Matrix<bool> *graph; 
    
  /// these vectors contain for every vertice the index of the vertice it is matched with
  /** it will be UNMARKED (-1) if the vertice is not matched */
  Lisa_Vector<int> *I_matched,*J_matched;
  
  /// vertex markings
  /** For finding new matching edges the algorithm needs to mark the vertex
      and find a way from an unmatched vertice in I to an unmatched vertice 
      in J. The values will be the index of the preceding vertice on the way, 
      UNMARKED (-1) to show this vertice is unmarked or STARTMARK (-2) to show 
      this is a starting(first) vertice */
  Lisa_Vector<int>  *I_marked,*J_marked;
  
  /// which vertex are already marked ?
  Lisa_Vector<bool> *S,*T;
  
  /// creates a start matching 
  /** by going through all edges and match them if still possible */
  void inline simple_start_matching();
  
  /// creates a start matching
  /** Creates a better start matching than simple_start_matching, but at higher
      computation costs. This method is not used in the current version, since 
      benchmarks showed that the overall performance was slower */
  void inline ordered_start_matching();

  /// the algorithm itself 
  /** It trys to find a way from an unmatched vertice in I to an unmatched 
      vertice in J. The edges on that way have to be changingly matched and 
      unmatched. If such a way exists matched  edges will be unmatched while 
      unmatched edges will be matched so there is one more matched edge after 
      this. If no such way exists the matching is a maximum one */
  void calculate();
  
public:
  
  /// unmatched flag
  /** the Vector representing the maximum matching will continue this value
      if the vertice has no matching partner */
  static const int UNMATCHED = -1;
  
  /// construct a new matching object
  /** with an adjazentmatrix, this constuctor will make a local copy of it */
  Lisa_BipartMatching(const Lisa_Matrix<bool> *in);
  /// construct a new matching object with the sizes of I and J
  Lisa_BipartMatching(const int n_in,const int m_in); 
  /// destruct the matching object
  ~Lisa_BipartMatching();

  /// the vector returned contains wether a vertex in I is marked or not
  const Lisa_Vector<bool>* get_S();
  /// the vector returned contains wether a vertex in J is marked or not
  const Lisa_Vector<bool>* get_T();
 
  /// the returned vector contains the matching partner for all vertice in I 
  /** or UNMARKED (-1) if it is not matched */
  const Lisa_Vector<int>* get_matching_I();
  /// the returned vector contains the matching partner for all vertice in J 
  /** or UNMARKED (-1) if it is not matched */
  const Lisa_Vector<int>* get_matching_J();

  /// allow access to the adjazentmatrix
  bool inline get_edge(const int i,const int j) const;
  
  /// allow access to the adjazentmatrix,
  /**  the calculated matching is resetted */
  void inline set_edge(const int i,const int j,const bool value);
  
  /// enables or disables all edges
  void inline set_all_edges(const bool value);
  
  /// returns wether the calculated matching is perfect or not
  /** a matching is perfect if all vertice in the smaller set are matched */
  bool inline is_perfect();  

  /// writes the object values into a stream
  /**  those values might be different from those returned as matching
      since this method was meant for debugging mostly */
  void write(std::ostream & = std::cout) const;
};

//**************************************************************************

/// weightet matching base class and interface
/** This interface provides a unified way to use Lisa_WeightedMatching and
    Lisa_BottleneckMatching. 
   @author Marc Moerig
   @version 3.0pre1
*/
class Lisa_Matching : public Lisa_GenericObject{
protected:
  /// matrix size 
  int n,m;
  /// maximum of m and n
  int mx;
  /// has a matching been calculated ?
  bool calculated;
  /// are we using inverted values ?
  /** input values can be inverted to calculate max or min matchings */
  bool inverted;
  /// used to calculate 'candidates' for our matching
  Lisa_BipartMatching* bipart;

public:
  
  /// matching flag
  /** the Vectors returned by get_matching_I() and get_matching_J() will 
      contain this value if a vertice has no matching partner */
  static const int UNMATCHED = Lisa_BipartMatching::UNMATCHED;
  
  /// edge weight flag
  /** get_graph() will return this value if an edge has
      been removed from the set of edges */
  static const TIMETYP REMOVED;
  
  /// destructs this object 
  virtual ~Lisa_Matching(){}
  
  /// get matchingpartners for vertice in I
  /** the returned vector contains the matchingpartner for all vertice in I or
      UNMATCHED (-1) if it is not matched */
  virtual const Lisa_Vector<int>* get_matching_I() = 0;
  
  /// get matchingpartners for vertice in J
  /** the returned vector contains the matchingpartner for all vertice in J or 
      UNMATCHED (-1) if it is not matched */
  virtual const Lisa_Vector<int>* get_matching_J() = 0;

  /// returns the weighting of the edge (i,j) or REMOVED
  virtual TIMETYP get_edge(const int i,const int j) const = 0;

  /// sets the weighting of the edge (i,j) 
  /** but only if this edge has not been removed already. the object takes care 
      itself if it has been inverted or not */
  virtual void set_edge(const int i,const int j,const TIMETYP value) = 0;
  
  /// updates all not removed edges
  /** with new values from the matrix, the inputmatrix must have the same size 
      as the matrix the object was initialized with, the object takes care 
      itself if it has been inverted or not */
  virtual void set_all_edges(const Lisa_Matrix<TIMETYP>* in) = 0;
  
  /// tell the object to minimize the matchings 
  /** calling this twice switches back to maximum matchings and so on */ 
  virtual void invert() = 0;
  
  /// removes the edge (i,j)
  virtual void remove(const int i,const int j) = 0;
};

//**************************************************************************

/// class for calculating maximum weighted matchings on bipartite graphs
/** The sum of the weightings of all edges in the matching is maximized
    (minimized).
    
    The weightings have to be from 0 to MAXTIME. This will also work if
    |I| != |J|.
    @author Marc Moerig
    @version 3.0pre1
 */
class Lisa_WeightedMatching : public Lisa_Matching{
private:
  
  /// the vertex marking
  /** actually I_mark[i]+J_mark[j] >= graph[i][j] must be true */ 
  Lisa_Vector<TIMETYP> *I_mark,*J_mark;
  
  /// this matrix contains the weigths for each edge
  Lisa_Matrix<TIMETYP> *graph;
   
  /// start the algorithm
  /** It calculates a a maximum weighted matching by calculating a perfect 
      matching on a corresponding unweighted graph. This graph is calculated 
      with the help of the vertex marking */
  void calculate();

  /// calculates new vertex markings
  void inline calc_start_marks();

  /// changes the vertex markings
  /** if no perfect matching was found on the corresponding unweigthed graph */ 
  void inline calc_new_marks(); 

public:
  
  /// constructor
  /** construct an object with a matrix of edge weightings,
      a local copy will be made */
  Lisa_WeightedMatching(const Lisa_Matrix<TIMETYP> *in);
  /// construct an object with the sizes of I an J
  Lisa_WeightedMatching(const int n_in, const int m_in);
  /// destruct the object
  ~Lisa_WeightedMatching();

  const Lisa_Vector<int>* get_matching_I();
  const Lisa_Vector<int>* get_matching_J();

  /// this method provides faster access to the edges
  TIMETYP get_edge(const int i,const int j) const;

  /// this method provides faster acess to the edges
  void set_edge(const int i,const int j,const TIMETYP value);
  
  /// change the weightings of all edges according to weightmatrix
  /** this is rather slow if you want to set only some values */
  void set_all_edges(const Lisa_Matrix<TIMETYP>* in);
  
  void remove(const int i,const int j);
  
  void invert();

  /// writes the object values into a stream
  /** those values might be different from those returned as matching
      since this method was meant for debugging mostly */  
  void write(std::ostream & = std::cout) const;
 
}; 

//**************************************************************************

/// class for calculating perfect matchings on weighted gaphs.   
/** The weighting of the biggest (smallest) edge in the matching is minimized 
    (maximized).
    
    The weightigs should always be from 0 to MAXTIME. This will also work if
    |I| != |J|.
    @author Marc Moerig
    @version 3.0pre1
*/
class Lisa_BottleneckMatching : public Lisa_Matching{
private:
  /// flag indicating whether lst is sorted or not
  bool sorted;
  
  /// ordered Lisa_List of edgeweightings 
  Lisa_List<Lisa_BottleneckNode>* lst;

  /// the matchings, these vectors contain the last matching found
  Lisa_Vector<int> *I_matched,*J_matched;

  //@{
  /// operations for moving the pointer on the list
  /** and including/excluding the weigtings for the matching set */
  void inline inc_next();
  void inline rem_curr();
  //@}
  
  /// calculates a new matching 
  void calculate();

public:

  /// create the object with edgeweightings 
  Lisa_BottleneckMatching(const Lisa_Matrix<TIMETYP>* in);
  /// delete the object
  ~Lisa_BottleneckMatching();
  
  const Lisa_Vector<int>* get_matching_I();
  const Lisa_Vector<int>* get_matching_J();
  
  /// writes the object values into a stream
  /** those values might be different from those returned as matching
      since this method was meant for debugging mostly */
  void write(std::ostream & = std::cout) const;
  
  void remove(const int i,const int j);
  
  void invert();

  /// change weightting of edge (i,j)
  /** this is a rather inefficient method and will take O(m*n) time ! */
  void set_edge(const int i,const int j,const TIMETYP value);
  
  /// change weightings of all edges according to weightmatrix
  /** fastest method to change the weightings for for all not removed 
      operations */
  void set_all_edges(const Lisa_Matrix<TIMETYP>* in);
 
  /// get the weighting of edge (i,j)
  /** this is a rather inefficient operation and may take O(m*n) time */
  TIMETYP get_edge(const int i,const int j) const;
};

//**************************************************************************

#endif

