/*
 ************* travel_graph.hpp *********
 *
 * Marc Mörig
 *
 * an object describing our traveling salesman problem
 * just for input/output from/to files and to calculate us the objective
 * for a given solution
 * 
 * it may or may not contain/read/write a solution
 */

#include <time.h>
#include <iostream.h>
#include "../../basics/global.hpp"
#include "../../basics/matrix.hpp"
#include "../../basics/except.hpp"
#include "../../lisa_dt/lsaobjct.hpp"

#ifndef _travel_graph_h
#define _travel_graph_h

/** @name Traveling Salesman Problem Object 
 
    This object serves mostly for writing and reading a problem.
    It can also contain a solution and calculate the objective 
    for any given solution to that problem.
    
    A Problem with #n# citys is represented as a matrix of size #n#,#n#.
    The matrix contains at position #P(i,j) i<j# the costs for traveling from city
    #i# to city #j# and also for traveling from city #j# to #i#. The Problem is therefore
    symmetrically.
    
    #P = {{ 0 4 5 6 }#\\
    #     { 0 0 3 6 }#\\
    #     { 0 0 0 4 }#\\
    #     { 0 0 0 0 }}#\\
    
    describes a problem with 4 citys.

    A solution for a Problem with #n# citys is given as a vector
    of length #n#. That vector has to contain all numbers between (and including)
    #0# and #n-1#. 
    
    For example #A = { 0 1 2 3 }# is a valid solution for #P#. Each number 
    in that vector represents a city, therefore in #A# city #0# is visited first, then city #1# and so
    on until city #3# is visited. Then our salesman has to return to city #0# of course.
    
    So the objective for solution #A# is #P(0,1) + P(1,2) + P(2,3) + P(0,3) = 4 + 3 + 4 + 6 = 17#

*/
//@{

/** Traveling Salesman Problem Object ...\\
 
    This object serves mostly for writing and reading a problem.
    It can also contain a solution and calculate the objective 
    for any given solution to that problem.
    
    A Problem with #n# citys is represented as a matrix of size #n#,#n#.
    The matrix contains at position #P(i,j) i<j# the costs for traveling from city
    #i# to city #j# and also for traveling from city #j# to #i#. The Problem is therefore
    symmetrically.
    
    #P = {{ 0 4 5 6 }#\\
    #     { 0 0 3 6 }#\\
    #     { 0 0 0 4 }#\\
    #     { 0 0 0 0 }}#\\
    
    describes a problem with 4 citys.

    A solution for a Problem with #n# citys is given as a vector
    of length #n#. That vector has to contain all numbers between (and including)
    #0# and #n-1#. 
    
    For example #A = { 0 1 2 3 }# is a valid solution for #P#. Each number 
    in that vector represents a city, therefore in #A# city #0# is visited first, then city #1# and so
    on until city #3# is visited. Then our salesman has to return to city #0# of course.
    
    So the objective for solution #A# is #P(0,1) + P(1,2) + P(2,3) + P(0,3) = 4 + 3 + 4 + 6 = 17#
    
*/
class Travel_Graph : public Lisa_Object{
private:

  // number of vertices e.g. citys
  int vertices;

  // the graph with the weights
  Lisa_Matrix<int>* graph;
  
  // contains all citys in the order they are visited
  Lisa_Vector<int>* solution;

  // create a new graph .. eg if reading from a file
  void inline init_graph(){
    if(graph) delete graph;
    graph = new Lisa_Matrix<int>(vertices,vertices);
    graph->fill(0);
  }

  // same for solution vector
  void inline init_solution(){
    if(solution) delete solution;
    solution = new Lisa_Vector<int>(vertices);
    for(int i=0;i<vertices;i++) (*solution)[i]=i;
  }

public:
  
  /** default constructur ...\\

      Create a new and empty object.*/
  Travel_Graph();
  
  /** constructor ...\\

      Create a new problem with a given size. 
      If rnd is set to #TRUE# random weights will be created.*/
  Travel_Graph(const int vertices_in,const bool rnd=1);
  
  /** destructor ...\\
      
      Delete this object. */
  ~Travel_Graph();

  /** calculate the objective for a given solution ...\\
      
      If an argument is given the objective will be calculated 
      for the argument solution vector, otherwise it will be 
      calculated for the solution in this object. If there is also
      no solution in the object 0 will be returned.*/
  int get_objective(Lisa_Vector<int>* solution_in=0) const;
  
  /** get the number of vertices ...\\
      
      The number of vertices or citys in the problem will 
      be returned. */
  int inline get_vertices()const{return vertices;}

  /** change the solution in this object to the argument solution */
  void set_solution(Lisa_Vector<int>* solution_in);
  
  /** write the solution from the object to the argument vector ...\\
      
      If there is no solution in the object the argument vector will
      be unchangend and #FALSE# will be returned. Otherwise the argument
      vector will contain the solution from the object and #TRUE# will be 
      returned. */
  bool get_solution(Lisa_Vector<int>* solution_out) const;

  /** check whether the object contains a solution or not ...\\
      
      Returns #TRUE# if this object contains a solution #FALSE# otherwise.*/
  bool inline has_solution()const{return solution ? 1 : 0;}
  
  /** removes a solution from the object ...\\
      
      If the object contains a solution it will be deleted.*/
  void inline del_solution(){if(solution){ delete solution; solution=0;}}

  /** write the object to a stream ...\\

      This will write the number of vertices, the graph matrix 
      and if exits the solution vector plus the objective to a stream.*/
  void write(ostream& strm = cout) const;

  /** read new values from a stream ...\\ 
      
      The input has to contain at least the number of vertices. If 
      that is nonzero it also has to contain the graph matrix. 
      The solution is always optional.

      valid inputs are for example:
      
      #<TRAVELGRAPH>#\\
      #vertices= 3#\\
      #graph= {#\\
      #{ 0 65 21 }#\\
      #{ 0 0 40 }#\\
      #{ 0 0 0 }#\\
      #}#\\
      #solution= { 0 1 2 }#\\
      #objective= 126#\\
      #</TRAVELGRAPH>#\\ 
      
      #<TRAVELGRAPH>#\\
      #vertices= 3#\\
      #graph= {#\\
      #{ 0 65 21 }#\\
      #{ 0 0 40 }#\\
      #{ 0 0 0 }#\\
      #}#\\
      #solution= { 0 1 2 }#\\
      #</TRAVELGRAPH>#\\
      
      #<TRAVELGRAPH>#\\
      #vertices= 3#\\
      #graph= {#\\
      #{ 0 65 21 }#\\
      #{ 0 0 40 }#\\
      #{ 0 0 0 }#\\
      #}#\\
      #</TRAVELGRAPH>#\\
      
      #<TRAVELGRAPH>#\\
      #vertices= 0#\\
      #</TRAVELGRAPH>#\\ */
  void read(istream& strm = cin);

};

//@}

#endif
