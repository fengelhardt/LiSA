
#ifndef _travel_graph_h
#define _travel_graph_h

#include <time.h>
#include <iostream>

#include "../../main/global.hpp"
#include "../../basics/matrix.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/lsaobjct.hpp"

/// Traveling Salesman Problem Object

/** This is an example for using LiSA's neighbourhood algorithms to solve
    other (non scheduling) optimization problems. It can create solutions 
    for the symmetrical traveling salesman problem. The source code files 
    are located in the directory LiSA/src/utility/travel/ .
    
    It consists of one class describing a problem (travel_graph.cpp,
    travel_graph.hpp), two classes describing two different neighbourhoods 
    (travel_neighbourhood.cpp,travel_neighbourhood.hpp) and a main routine 
    for reading an input file, running the algorithm and write the results 
    to an output file (travel.cpp). Some sample input files are located in 
    the files/ subdirectory. 
    
    After compiling the executable will be located in the same directory 
    where the source code is. It may than be called:

    - travel [input file] [output file]

    While running some progress info will be printed to the console. All 
    algorithm/neighbourhood parameters have to be set in the input file.
    An example for an input file is:

    <CONTROLPARAMETERS>
    string METHOD TA
    string NGBH RPI
    long STEPS 100000
    long NUMB_STUCKS 1000000
    double ABORT_BOUND 0
    long PROB 40
    long MAX_STUCK 3000
    </CONTROLPARAMETERS>

    <TRAVELGRAPH>
    vertices= 3
    graph= {
    { 0 65 21 }
    { 0 0 40 }
    { 0 0 0 }
    }
    </TRAVELGRAPH>
      
    The first part contains the algorithm and neighbourhood parameters, the 
    second part is the actual traveling salesman problem. For more examples of 
    the problem input see the documentation for the Travel_Graph class.
    
    Parameters that have to be set always:
    
    - string METHOD [II SA SA_anti TA TS] the algorithm you want to use
      - II: iterative improvement
      - SA: simulated annealing
      - SA_anti: simmulated annealing with anti neighbour
      - TA: threshold accepting
      - TS: tabu search
    
    - string NGHB [RPI API]
      - RPI: random pair interchange
      - API: adjacent pair interchange
    
    - long STEPS: the number of steps after which the algorithm should abort
    
    - long NUMB_STUCKS: the number of stucks after which the algorithm should abort

    - double ABORT_BOUND: a lower bound for the objective, if reached the algorithm will abort
    
    Parameters that have to be set for II:
    
    - string NGBH_TYPE [ENUM RAND]
      - RAND: random generation of neighbours
      - ENUM: enumerative generation of neighbours

    Parameters that have to be set for SA, SA_anti and TA:
    
    - long PROB: the start propability for accepting solutions with a worse objective
    
    - long MAX_STUCK: the number of stucks after which the threshold/temperature will be raised
    
    Parameters that have to be set for TS:
    
    - string NGBH_TYPE [ENUM RAND]
    
    - long TABU_LENGTH: the length of the tabu list

    - long NUMB_NGB: the number of neighbours to generate with each step
    
    Further information about the modelling of the problem and the 
    neighbourhoods can be found in the documentation for the classes 
    Travel_Graph , Travel_RPI_Neighbourhood and Travel_API_Neighbourhood.
    
    The documentation for LiSA's neighbourhood algorithms can be found in the 
    classes Lisa_Neighbourhood , Lisa_Iterator and Lisa_Tabu.
  
    This object serves mostly for writing and reading a problem.
    It can also contain a solution and calculate the objective 
    for any given solution to that problem.
    
    A Problem with n citys is represented as a matrix of size n,n.
    The matrix contains at position P(i,j) i<j the costs for traveling from 
    city i to city j and also for traveling from city j to i. The Problem is 
    therefore symmetrically.
    
    P = {{ 0 4 5 6 }
         { 0 0 3 6 }
         { 0 0 0 4 }
         { 0 0 0 0 }}
    
    describes a problem with 4 citys.

    A solution for a Problem with n citys is given as a vector of length n. 
    That vector has to contain all numbers between (and including) 0 and n-1. 
    
    For example A = { 0 1 2 3 } is a valid solution for P. Each number in that
    vector represents a city, therefore in A city 0 is visited first, then city
    1 and so on until city 3 is visited. Then our salesman has to return to 
    city 0 of course.
    
    So the objective for solution A is 
    P(0,1) + P(1,2) + P(2,3) + P(0,3) = 4 + 3 + 4 + 6 = 17
    
    @version 3.0pre1
    @author Marc Moerig   
*/
class Travel_Graph : public Lisa_Object{
private:

  /// number of vertices e.g. citys
  int vertices;

  /// the graph with the weights
  Lisa_Matrix<int>* graph;
  
  /// contains all citys in the order they are visited
  Lisa_Vector<int>* solution;

  /// create a new graph .. eg if reading from a file
  void inline init_graph(){
    if(graph) delete graph;
    graph = new Lisa_Matrix<int>(vertices,vertices);
    graph->fill(0);
  }

  /// same for solution vector
  void inline init_solution(){
    if(solution) delete solution;
    solution = new Lisa_Vector<int>(vertices);
    for(int i=0;i<vertices;i++) (*solution)[i]=i;
  }

public:
  
  /// default constructur
  /** Create a new and empty object.*/
  Travel_Graph();
  
  /// constructor
  /** Create a new problem with a given size. 
      If rnd is set to true random weights will be created.*/
  Travel_Graph(const int vertices_in,const bool rnd=1);
  
  /// destructor
  /** Delete this object. */
  ~Travel_Graph();

  /// calculate the objective for a given solution 
  /** If an argument is given the objective will be calculated 
      for the argument solution vector, otherwise it will be 
      calculated for the solution in this object. If there is also
      no solution in the object 0 will be returned.*/
  int get_objective(Lisa_Vector<int>* solution_in=0) const;
  
  /// get the number of vertices
  /** The number of vertices or citys in the problem will 
      be returned. */
  int inline get_vertices()const{return vertices;}

  /// change the solution in this object to the argument solution
  void set_solution(Lisa_Vector<int>* solution_in);
  
  /// write the solution from the object to the argument vector 
  /**  If there is no solution in the object the argument vector will
      be unchangend and false will be returned. Otherwise the argument
      vector will contain the solution from the object and true will be 
      returned. */
  bool get_solution(Lisa_Vector<int>* solution_out) const;

  /// check whether the object contains a solution or not 
  /** Returns true if this object contains a solution false otherwise.*/
  bool inline has_solution()const{return solution ? 1 : 0;}
  
  /// removes a solution from the object
  /** If the object contains a solution it will be deleted.*/
  void inline del_solution(){if(solution){ delete solution; solution=0;}}

  /// write the object to a stream
  /** This will write the number of vertices, the graph matrix 
      and if exits the solution vector plus the objective to a stream.*/
  void write(std::ostream& strm = std::cout) const;

  /// read new values from a stream 
  /** The input has to contain at least the number of vertices. If 
      that is nonzero it also has to contain the graph matrix. 
      The solution is always optional.

      valid inputs are for example:
      
      <TRAVELGRAPH>
      vertices= 3
      graph= {
      { 0 65 21 }
      { 0 0 40 }
      { 0 0 0 }
      }
      solution= { 0 1 2 }
      objective= 126
      </TRAVELGRAPH> 
      
      <TRAVELGRAPH>
      vertices= 3
      graph= {
      { 0 65 21 }
      { 0 0 40 }
      { 0 0 0 }
      }
      solution= { 0 1 2 }
      </TRAVELGRAPH>
      
      <TRAVELGRAPH>
      vertices= 3
      graph= {
      { 0 65 21 }
      { 0 0 40 }
      { 0 0 0 }
      }
      </TRAVELGRAPH>
      
      <TRAVELGRAPH>
      vertices= 0
      </TRAVELGRAPH> */
  void read(std::istream& strm = std::cin);

};

#endif

