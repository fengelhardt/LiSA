/**
 * @author  Marc Moerig
 * @version 3.0pre1
 */

#include "travel_graph.hpp"

using namespace std;

//******************************************************************************

Travel_Graph::Travel_Graph(){
  // oups ... nothing
  vertices = 0;
  graph = 0;
  solution = 0;
}

//******************************************************************************

Travel_Graph::Travel_Graph(const int vertices_in,const bool rnd){
  vertices=vertices_in;
  graph = 0;
  solution = 0;

  if(vertices) init_graph();

  // create random weights
  if(rnd){
    static long seed = time(0);
    for(int i=0;i<vertices;i++)
      for (int j=i+1;j<vertices;j++)
	(*graph)[i][j] = lisa_random(1,100,&seed);
  }
}

//******************************************************************************

Travel_Graph::~Travel_Graph(){
  if(graph) delete graph;
  if(solution) delete solution;
}

//******************************************************************************

int Travel_Graph::get_objective(Lisa_Vector<int>* solution_in) const{
  
  Lisa_Vector<int>* sol = solution_in ? solution_in : solution;
  
  int sum = 0;
  
  if(sol){
    
    for(int i=0;i<vertices-1;i++)
      sum += (*sol)[i] < (*sol)[i+1] ? 
	(*graph)[(*sol)[i]][(*sol)[i+1]] : (*graph)[(*sol)[i+1]][(*sol)[i]];
    
    sum += (*sol)[0] < (*sol)[vertices-1] ? 
      (*graph)[(*sol)[0]][(*sol)[vertices-1]] : (*graph)[(*sol)[vertices-1]][(*sol)[0]];
  
  }

  return sum;
}

//******************************************************************************

bool Travel_Graph::get_solution(Lisa_Vector<int>* solution_out) const{

  if(solution){ 
    for (int i=0;i<vertices;i++) (*solution_out)[i] = (*solution)[i];
    return 1;
  }else{
    return 0;
  }

}

//******************************************************************************

void Travel_Graph::set_solution(Lisa_Vector<int>* solution_in){

  if(!solution) init_solution();
  for (int i=0;i<vertices;i++) (*solution)[i] = (*solution_in)[i];

}

//******************************************************************************

void Travel_Graph::write(ostream& strm) const{
  strm << "<TRAVELGRAPH>" << endl;
  strm << "vertices= " << vertices << endl;
  if(graph) strm << "graph= " << *graph ;
  if(solution){ 
    strm << "solution= " << *solution;
    strm << "objective= " << get_objective() << endl;
  }
  strm << "</TRAVELGRAPH>" << endl;
}

//******************************************************************************

void Travel_Graph::read(istream& strm){    
  string S;
  int vertices_in;

  for(;;){
    S=""; 
    strm >> S;
      
    if(S==""){ 
      G_ExceptionList.lthrow("Travel_Graph::read() Unexpected end of file.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    }else if(S=="<TRAVELGRAPH>"){ 
      break; // so we found our object ... start reading
    }
  }
  
  for (;;){
    S=""; 
    strm >> S;
    
    if (S==""){ 
      G_ExceptionList.lthrow("Travel_Graph::read() Unexpected end of file.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    }else if (S=="vertices="){
      strm >> vertices_in; // ok .. now we got our graph size
      break;
    }
  }

  if(vertices_in){
    
    vertices = vertices_in;
    init_graph();
    if(solution){ 
      delete solution;
      solution = 0;
    }
      
    for (;;){
      S=""; 
      strm >> S;
      
      if (S==""){ 
	G_ExceptionList.lthrow("Travel_Graph::read() Unexpected end of file.",
                         Lisa_ExceptionList::END_OF_FILE);
	return;
      }else if (S=="graph="){
	strm >> *graph;
	break; // and here is our graph ... now lets check if there is a solution saved
      }
    }

     for (;;){
      S=""; 
      strm >> S;
      
      if (S==""){ 
	G_ExceptionList.lthrow("Travel_Graph::read() Unexpected end of file.",
                         Lisa_ExceptionList::END_OF_FILE);
	return;
      }else if (S=="solution="){
	init_solution();
	strm >> *solution;
	break; // ok, we got a solution also
      }else if (S=="</TRAVELGRAPH>"){
	return; // no solution .. we have finished reading
      }
    }

  }else{ // vertices = 0
    if(graph){ 
      delete graph;
      graph = 0;
    }
    if(solution){
      delete solution;
      solution = 0;
    }
    
  }
  
  for (;;){
    S=""; 
    strm >> S;
    
    if (S==""){ 
      G_ExceptionList.lthrow("Travel_Graph::read() Unexpected end of file.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    }else if (S=="</TRAVELGRAPH>"){ 
      return; // we also found our end tag
    }
  }
}

//******************************************************************************

