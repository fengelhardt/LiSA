/**
 * @author  LiSA
 * @version 3.0
 */

// Include the header files for the used objects! Standard:
// LisaProblemtype requires global.hpp.
// The file global.hpp is needed by LisaProblemtype.
// The files ctrlpara.hpp, ptype.hpp and lvalues.hpp
// are used for parsing the input file.
// The object Lisa_Schedule (header: schedule.hpp) 
// is necessary for writing the result into the output file.
// The file except.hpp is used for the exception handling.  

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>

#include "../../basics/lmatrix.hpp"
#include "../../main/global.hpp"
#include "../../scheduling/matching.hpp"

// splits a bipartit graph with even degree in two subgraphs with round about
// half of the edges of the original graph
class Eulersplit {

  private: 
    
    // the adjacency matrix
    Lisa_Matrix<TIMETYP>* adjMatrix;
    
    // subgraph 1
    Lisa_Matrix<TIMETYP>* g1;

    // subgraph 2
    Lisa_Matrix<TIMETYP>* g2;

    // the vertices in I and their neighbors stored as list
    Lisa_VectorOfLists<int>* left;

    // the vertices in J and their neighbors stored as list
    Lisa_VectorOfLists<int>* right;

    // I or J
    int side;

    // g1 or g2
    int insert;

    // recursion
    int splitSubGraph();

    // search for a neighbour of vertex v
    int searchNeighbour(int v);

    // get a right neighbour of vertex v
    int getRightNeighbour(int lv);

    // get a left neighbour of vertex v
    int getLeftNeighbour(int rv);

    // insert {u,v}
    void insertEdge(int u, int v);

  public:
    
    // constructor
    Eulersplit(Lisa_Matrix<TIMETYP> *matrix);

    // destructor
    ~Eulersplit();
  
    // split the graph
    void splitGraph();

    // get subgraph 1
    Lisa_Matrix<TIMETYP>* get_G1();

    // get subgraph 2
    Lisa_Matrix<TIMETYP>* get_G2();

};
