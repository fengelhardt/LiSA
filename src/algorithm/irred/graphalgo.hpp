
#ifndef graphalgo_h
#define graphalgo_h

#include "../../basics/graph.hpp"

/// Just some helpfull operations on graphs.
/** Needed for the irreducibility test.
    @see Lisa_MatrixListGraph
    @version 2.3final
    @author Marc Moerig   
 */
class Lisa_MatrixListGraphAlgorithms{
public:

  /// Removes all ARC's and EDGES from the graph.
  static inline void clear(Lisa_MatrixListGraph* graph){
    const int vert = graph->get_vertices();
    for (int i=1;i<=vert;i++){
      graph->remove_all_con(i);
    } 
  }
   
  /// Replaces each ARC with an EDGE.
  static inline void build_semigraph(Lisa_MatrixListGraph* graph){
    const int vert = graph->get_vertices();
    int c;
    
    for (int i=1;i<=vert;i++){
      for (int j=i+1;j<=vert;j++){
        c = graph->get_connection(i,j);
        if(c!=Lisa_Graph::NONE&&c!=Lisa_Graph::EDGE){ 
          if (c==Lisa_Graph::ARC) graph->insert_arc(j,i);
          else graph->insert_arc(i,j);
        }
      }
    }
  }
 
  /// Create a comparability graph.
  /** Build the transitive hull, then create the semigraph of it.
      The targetgraph is supposed to have the same number of vertice as the 
      inputgraph, but no edges or arcs at all.*/ 
  static inline void build_compgraph(Lisa_MatrixListGraph* source,Lisa_MatrixListGraph* target){
    const int vert = source->get_vertices();
    
    Lisa_Vector<int> queue(vert+1);
    Lisa_Vector<bool> done(vert+1);
    int qs,qe,curr;
    int succ;
    
    for (int i=1;i<=vert;i++){
      queue.fill(0);  
      done.fill(0);
      qs = qe = 0;
      
      source->init_succ_pointer(i);
      succ=source->get_next_successor(i);
      while((succ<vert+1)){
        queue[qe++] = succ;
        done[succ] = 1;
        succ=source->get_next_successor(i);
      }  
      
      while(qs<qe){
        curr = queue[qs++];
        target->insert_edge(i,curr);
        source->init_succ_pointer(curr);
        succ=source->get_next_successor(curr);
        while((succ<vert+1)){
          if(!done[succ]){
            done[succ] = 1;
            queue[qe++] = succ;
          }
          succ = source->get_next_successor(curr);
        }
      } 
    }  
  }
  
  /// Sort vertices topologically.
  /** The Method in the Lisa_MatrixListGraph object performs this operation
      such that if i is the vertex his position will be stored at
      (*knot_sequence)[i-1]. 
 
      This Method however does it the other way around, so that i is 
      the position while (*knot_sequence)[i] contains the vertex. */
  static inline bool topsort(Lisa_MatrixListGraph* graph,Lisa_Vector<int>* knot_sequence){
    const int vert = graph->get_vertices();
    
    Lisa_Vector<int> sort(vert);
    const bool done = graph->topsort(&sort);
    
    for (int i=0;i<vert;i++)  (*knot_sequence)[sort[i]-1]=i+1;
    
    return done;
  }
  

  /// Compare graphs. 
  /** This method returns true,1 if any edge in the first graph 
      is also contained in the second graph, and the second graph has 
      at least one more edge. Both graph's need to contain no arc's, 
      have the same number of vertices and those are fixed. 
      This method does NOT test if the graphs are isomorph. */
  static inline bool smaller(Lisa_MatrixListGraph* first,Lisa_MatrixListGraph* second){
    const int vert = first->get_vertices();
    int fc,sc;
    bool missing=0;
    
    for (int i=1;i<=vert;i++){
      for (int j=i+1;j<=vert;j++){
        fc = first->get_connection(i,j);
        sc = second->get_connection(i,j);
        if(fc!=sc){
          if (fc == Lisa_Graph::EDGE) return 0;
          else missing = 1;
        }
      }
    }
    
    return missing;
  }
  
  /// Compare graphs.
  /** This method returns true,1 if any ARC,EDGE in the first graph 
      is also contained in the second graph, and vice versa. Both 
      graph's need to have the same number of vertices and they are 
      fixed. This method does NOT test if the graphs are isomorph. */
  static inline bool equal(Lisa_MatrixListGraph* first,Lisa_MatrixListGraph* second){
    const int vert = first->get_vertices();
    
    for (int i=1;i<=vert;i++){
      for (int j=i+1;j<=vert;j++){
        if(first->get_connection(i,j) != second->get_connection(i,j)) return 0;
      }
    }
    
    return 1;
  }
  
};

#endif

