/**
 * @author  Christian Schulz
 * @version 2.3final
 */
 
#include <stdlib.h>
#include "graph.hpp"

using namespace std;

//**************************************************************************

void Lisa_Graph::write(ostream & strm)const{
  
  if(strm==NULL){
    G_ExceptionList.lthrow("No valid stream in Lisa_MatrixListGraph::write().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  Lisa_Matrix<int> out(get_vertices(),get_vertices());
  get_adjacency_matrix(&out);

  strm << endl << "<GRAPH>" << endl;
  strm << "vertices= "<< get_vertices() << endl;
  strm << "adjacency_matrix= " << out << endl; 
  strm << "</GRAPH>" <<endl;

}

//**************************************************************************

void Lisa_Graph::read(istream & strm){
  
  if(strm==NULL){
    G_ExceptionList.lthrow("No valid stream in Lisa_MatrixListGraph::read().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  string S;
  int new_size;
  Lisa_Matrix<int>* adj_Matrix=0;
  for (;;){
    S=""; 
    strm >> S;
    if (S==""){ 
      G_ExceptionList.lthrow((string) "Unexpected End of File in Lisa_MatrixListGraph::read(). "
                             +" Could not find starttag <GRAPH>.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    } 
    if (S=="<GRAPH>") break;
  }
  
  S=""; 
  strm >> S; 
  if (S=="vertices=") {  
    strm >> new_size; 
    init(new_size);
    adj_Matrix=new Lisa_Matrix<int>(new_size,new_size);
  }else{
    G_ExceptionList.lthrow("'vertices=' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S=="adjacency_matrix="){  
    strm >> *adj_Matrix;
    read_adjacency_matrix(adj_Matrix);
    delete adj_Matrix;
  }else{
    G_ExceptionList.lthrow("'adjacency_matrix=' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S=""; 
  strm >> S; 
  if (S!="</GRAPH>"){
    G_ExceptionList.lthrow("'</GRAPH>' expected in <GRAPH>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;  
  } 

}

//**************************************************************************
//**************************************************************************
//**************************************************************************

int Lisa_MatrixListGraph::signum(const int start,const int end)const{
  if(start==end) return 0;
  
  if(((*matrix)[start][end].x>0)&&((*matrix)[start][end].y>0)){
    return 1;
  }else{
    return -1;
  }
}

//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const int number_of_knots){
  matrix=0;
  succ_pred_pointer=0;
  init(number_of_knots);
}

//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_MatrixListGraph *const othergraph){
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph->get_vertices());
  
  for(int i=0; i<=size; i++){
    for(int j=0; j<=size; j++){
      (*matrix)[i][j].x=(*(othergraph->matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph->matrix))[i][j].y;
    }
  }
}
  
//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_Graph *const othergraph){
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph->get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph->get_adjacency_matrix(&out);
  read_adjacency_matrix(&out);
}
  
//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_MatrixListGraph& othergraph){
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph.get_vertices());
  
  for(int i=0; i<=size; i++){
    for(int j=0; j<=size; j++){
      (*matrix)[i][j].x=(*(othergraph.matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph.matrix))[i][j].y;
    }
  }
}

//**************************************************************************

Lisa_MatrixListGraph::Lisa_MatrixListGraph(const Lisa_Graph & othergraph){
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph.get_vertices());
  
  Lisa_Matrix<int> out(size,size);
  othergraph.get_adjacency_matrix(&out);
  read_adjacency_matrix(&out);
}

//**************************************************************************

Lisa_MatrixListGraph::~Lisa_MatrixListGraph(){
  delete matrix;
  delete succ_pred_pointer;
}

//**************************************************************************

void Lisa_MatrixListGraph::init(const int n_in){  
  size = n_in;
  
  if(matrix) delete matrix;
  matrix= new Lisa_Matrix<Lisa_Pair>(size+1,size+1);
  
  if(succ_pred_pointer) delete succ_pred_pointer;
  succ_pred_pointer=new Lisa_Vector<Lisa_Pair>(size);
  
  clear();
}

//**************************************************************************

void Lisa_MatrixListGraph::clear(){
  
  for (int i=0; i<=size; i++){
    for (int j=0; j<=size; j++){
      if((i==0||j==0)||(i==j)){
        (*matrix)[i][j].x=size+1;
        (*matrix)[i][j].y=size+1;
      }else{
        (*matrix)[i][j].x=0;
        (*matrix)[i][j].y=0;
      }
    }
  }
  
  for(int i=0; i<size; i++){
    (*succ_pred_pointer)[i].x=i+1;
    (*succ_pred_pointer)[i].y=i+1;
  }
  
}

//**************************************************************************
  
void Lisa_MatrixListGraph::get_adjacency_matrix(Lisa_Matrix<int> *const adj)const{
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixListGraph::get_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  for(int i=1; i<=size; i++){
    for(int j=1; j<=size; j++){
      
      if(signum(i,j)==1) {
        (*adj)[i-1][j-1]=1;
      }else{
        (*adj)[i-1][j-1]=0;
      }
      
    }
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::read_adjacency_matrix(const Lisa_Matrix<int> *const adj){
#ifdef LISA_DEBUG
  if(adj->get_n() != size || adj->get_m() != size){
    G_ExceptionList.lthrow("Wrong matrix size in argument to Lisa_MatrixListGraph::read_adjacency_matrix().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif  
  
  clear();
  
  for(int i=0; i<size; i++){
    for(int j=0; j<size; j++){
    
      if((i!=j)&&((*adj)[i][j]==1)){ // connection exists ?  
        if((*adj)[j][i]==1){ // arc or edge ?
          insert_edge(i+1,j+1);
        }else{
          insert_arc(i+1,j+1);
        }
      }
      
    }
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::insert_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::insert_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  if(start==end) return;
  
  int con=get_connection(start, end);
  
  if(con!=EDGE){
    if(con==ARC){
      exclude_arc(start,end);
    }
    if(con==CRA){
      exclude_arc(end,start);
    }
    
    int end_of_NB_List;
    
    //Aendern der Kantenliste von start
    
    end_of_NB_List=(*matrix)[start][start].y;
    
    if( end_of_NB_List == size+1){
      (*matrix)[start][start].x=end;
    }else{
      (*matrix)[start][end_of_NB_List].x=end;
    }
    
    (*matrix)[start][start].y=end;
    (*matrix)[start][end].x=size+1;
    (*matrix)[start][end].y=end_of_NB_List;
    
    //Aendern der Kantenliste von end
    
    end_of_NB_List=(*matrix)[end][end].y;
    
    if( end_of_NB_List == size+1){
      (*matrix)[end][end].x=start;
    }else{
      (*matrix)[end][end_of_NB_List].x=start;
    }
    
    (*matrix)[end][end].y=start;
    (*matrix)[end][start].x=size+1;
    (*matrix)[end][start].y=end_of_NB_List;
    
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::insert_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::insert_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;
  
  int con=get_connection(start, end);
  
  //Test ob arc schon vorhanden
  if((con!=ARC)&&(con!=EDGE)){
    if(con==CRA){
      exclude_arc(end,start);
      insert_edge(start,end);
    }else{
      //Einfuegen des Bogens
      //Aendern der Succ-Liste von start
      
      int end_of_Succ_List=(*matrix)[start][0].y;
      if( end_of_Succ_List== size+1){
        (*matrix)[start][0].x=end;
      }else{
        (*matrix)[start][end_of_Succ_List].x=end;
      }
      
      (*matrix)[start][0].y=end;
      (*matrix)[start][end].x=size+1;
      (*matrix)[start][end].y=end_of_Succ_List;
      
      //Aendern der Pred-Liste von end
      
      int end_of_Pred_List=(*matrix)[0][end].y;
      if( end_of_Pred_List == size+1){
        (*matrix)[0][end].x=start;
      }else{
        (*matrix)[end][end_of_Pred_List].x=-start;
      }
      
      (*matrix)[0][end].y=start;
      (*matrix)[end][start].x=-(size+1);
      (*matrix)[end][start].y=-end_of_Pred_List;
      
    }
  }

}
//**************************************************************************

void Lisa_MatrixListGraph::exclude_all(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::exclude_all().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;
  
  int con=get_connection(start,end);
  
  if(con==EDGE){
    exclude_edge(start,end);
  }else if(con==ARC){
    exclude_arc(start,end);
  }else if(con==CRA){
    exclude_arc(end,start);
  }

}

//**************************************************************************

void Lisa_MatrixListGraph::exclude_edge(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::exclude_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;

  //Test ob Kante vorhanden
  int con=get_connection(start,end);
  
  if(con==EDGE){
    int succ_of_edge=(*matrix)[start][end].x;
    int pred_of_edge=(*matrix)[start][end].y;
    
    if(succ_of_edge==size+1){
      (*matrix)[start][start].y=pred_of_edge;
    }else{
      (*matrix)[start][succ_of_edge].y=pred_of_edge;
    }
    
    if(pred_of_edge==size+1){
      (*matrix)[start][start].x=succ_of_edge;
    }else{
      (*matrix)[start][pred_of_edge].x=succ_of_edge;
    }
    
    (*matrix)[start][end].x=0;
    (*matrix)[start][end].y=0;
    
    //Aendern der end-Kantenliste
    
    succ_of_edge=(*matrix)[end][start].x;
    pred_of_edge=(*matrix)[end][start].y;
    
    if(succ_of_edge==size+1){
      (*matrix)[end][end].y=pred_of_edge;
    }else{
      (*matrix)[end][succ_of_edge].y=pred_of_edge;
    }
    
    if(pred_of_edge==size+1){
      (*matrix)[end][end].x=succ_of_edge;
    }else{
      (*matrix)[end][pred_of_edge].x=succ_of_edge;
    }
    
    (*matrix)[end][start].x=0;
    (*matrix)[end][start].y=0;

  }

}

//**************************************************************************

void Lisa_MatrixListGraph::exclude_arc(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::exclude_arc().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  if(start==end) return;
  
  //Test ob Bogen vorhanden
  int con=get_connection(start,end);
  
  if(con==ARC){
    int succ_of_arc=(*matrix)[start][end].x;
    int pred_of_arc=(*matrix)[start][end].y;
    
    if(succ_of_arc==size+1){
      (*matrix)[start][0].y=pred_of_arc;
    }else{
      (*matrix)[start][succ_of_arc ].y=pred_of_arc;
    }
    
    if(pred_of_arc==size+1){
      (*matrix)[start][0].x=succ_of_arc;
    }else{
      (*matrix)[start][pred_of_arc].x=succ_of_arc;
    }
    
    (*matrix)[start][end].x=0;
    (*matrix)[start][end].y=0;
    
    //Aendern der predecessor-Liste
    
    succ_of_arc=(*matrix)[end][start].x;
    pred_of_arc=(*matrix)[end][start].y;
    
    if(succ_of_arc==-(size+1)){
      (*matrix)[0][end].y=-pred_of_arc;
    }else{
      (*matrix)[end][-succ_of_arc].y=pred_of_arc;
    }
    
    if(pred_of_arc==-(size+1)){
      (*matrix)[0][end].x=-succ_of_arc;
    }else{
      (*matrix)[end][-pred_of_arc ].x=succ_of_arc ;
    }
    
    (*matrix)[end][start].x=0;
    (*matrix)[end][start].y=0;
    
  }else if(con==EDGE){
    exclude_edge(start, end);
    insert_arc(end, start);
  }
  
}

//**************************************************************************

int Lisa_MatrixListGraph::get_connection(const int start,const int end){
#ifdef LISA_DEBUG
  if( start<=0 || start>size || end<=0 || end>size ){
    G_ExceptionList.lthrow("Vertexpair "+ztos(start)+" "+ztos(end)+
                           " out of range in Lisa_MatrixListGraph::get_connection().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return NONE;
  }
#endif

  if(start==end) return NONE;
  
  if((((*matrix)[start][end]).x==0)&&(((*matrix)[start][end]).y==0)){
    return NONE;
  }else{

    if(signum(start, end)==-1){
      return CRA;
    }else{

      if(signum(end, start)==-1){
        return ARC;
      }else{
        return EDGE;
      }
    }
  }
  
}

//**************************************************************************

void Lisa_MatrixListGraph::init_succ_pointer(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::init_succ_pointer().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  (*succ_pred_pointer)[knot-1].x=knot;
}

//**************************************************************************

void Lisa_MatrixListGraph::init_pred_pointer(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::init_pred_pointer().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif
  
  (*succ_pred_pointer)[knot-1].y=knot;
}
 
//**************************************************************************

int Lisa_MatrixListGraph::get_next_successor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::get_next_successor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif

  int old_knot=(*succ_pred_pointer)[knot-1].x;
  int next_knot=(*matrix)[knot][old_knot].x;
  
  if(next_knot==size+1){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=(*matrix)[knot][0].x;
    }else{//END OF AN EDGE LIST
      if(old_knot==(*matrix)[knot][knot].y){
        next_knot=(*matrix)[knot][0].x; 
      }
    }
  }
  
  if(next_knot==size+1){
    init_succ_pointer(knot);
  }else{
    (*succ_pred_pointer)[knot-1].x=next_knot;
  }
  
  return next_knot;
}

//**************************************************************************

int Lisa_MatrixListGraph::get_next_predeccessor(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::get_next_predecessor().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif 
 
  int old_knot=(*succ_pred_pointer)[knot-1].y;
  int next_knot=abs((*matrix)[knot][old_knot].x);
  
  if(next_knot==size+1){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=abs((*matrix)[0][knot].x);
    }else{ //END OF AN EDGE LIST
      if(old_knot==(*matrix)[knot][knot].y){
        next_knot=abs((*matrix)[0][knot].x);
      }
    }
  }
  
  if(next_knot==size+1){
    init_pred_pointer(knot);
  }else{
    (*succ_pred_pointer)[knot-1].y=next_knot;
  }
  
  return next_knot;
}

//**************************************************************************

int Lisa_MatrixListGraph::get_next_edge(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::get_next_edge().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return size+1;
  }
#endif 

  int next_knot=get_next_successor(knot);
  
  if((next_knot!=size+1)&&(get_connection(knot,next_knot)!=EDGE)){  
    next_knot=size+1;
  } 
  
  return next_knot;
}

//**************************************************************************

void Lisa_MatrixListGraph::clear(const int knot){

#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_MatrixListGraph::remove_all_con().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return;
  }
#endif

  //EDGES
  int next=(*matrix)[knot][knot].x;
  while(next<=size){
    exclude_edge(knot,next);
    next=(*matrix)[knot][knot].x;
  }

  //ARCS
  next=(*matrix)[knot][0].x;
  while(next<=size){
    exclude_arc(knot,next);
    next=(*matrix)[knot][0].x;
  } 

  //CRA
  next=(*matrix)[0][knot].x;
  while(next<=size){
    exclude_arc(next,knot);
    next=(*matrix)[0][knot].x;
  } 
  
}

//**************************************************************************

int Lisa_MatrixListGraph::number_of_succ(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_Graph::number_of_succ().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif 

  int succ_count=0,old_knot=knot,next_knot;
  
  for(;;){
    
    next_knot=(*matrix)[knot][old_knot].x;
    
    if(next_knot==size+1){
      //START OF AN EDGE LIST
      if(old_knot==knot){
        next_knot=(*matrix)[knot][0].x;
      }else{//END OF AN EDGE LIST
        if(old_knot==(*matrix)[knot][knot].y){
          next_knot=(*matrix)[knot][0].x; 
        }
      }
    }
    
    if(next_knot==size+1){
      break;
    }else{
      old_knot=next_knot;
      succ_count++;
    }
     
  }
  
  return succ_count;
}

//**************************************************************************

int Lisa_MatrixListGraph::number_of_pred(const int knot){
#ifdef LISA_DEBUG
  if( knot<=0 || knot>size ){
    G_ExceptionList.lthrow("Vertex "+ztos(knot)+
                           " out of range in Lisa_Graph::number_of_pred().",
                           Lisa_ExceptionList::OUT_OF_RANGE);
    return 0;
  }
#endif
  
  int pred_count=0,old_knot=knot,next_knot;
  
  for(;;){

    next_knot=abs((*matrix)[knot][old_knot].x);
    
    if(next_knot==size+1){
      //START OF AN EDGE LIST
      if(old_knot==knot){
        next_knot=abs((*matrix)[0][knot].x);
      }else{ //END OF AN EDGE LIST
        if(old_knot==(*matrix)[knot][knot].y){
          next_knot=abs((*matrix)[0][knot].x);
        }
      }
    }
    
    if(next_knot==size+1){
      break;
    }else{
      old_knot=next_knot;
      pred_count++;
    }
    
  }

  return pred_count;
}

//**************************************************************************

void Lisa_MatrixListGraph::show(){
  
  for (int i=0; i<=size; i++){
    for (int j=0; j<=size; j++){
      cout<<(*matrix)[i][j].x<<"=="<<(*matrix)[i][j].y<<"   ";
    }
    cout<<endl;
  }
  
} 

//**************************************************************************

bool Lisa_MatrixListGraph::valid(){
  
  int el_of_list;
  int* knot_list = new int[size];
  int count=-1;
  
  for(int knot=1; knot<=size; knot++){
    //Ueberpruefen der Nachfolgerliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[knot][0].x;
    
    if(el_of_list==0){
      delete[] knot_list;
      return false;
    }
    
    if(el_of_list==size+1){	  
      if((*matrix)[knot][0].y!=size+1){
        cout<<" matrix "<<endl;
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[knot][0].y==size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        count++;
        knot_list[count]=el_of_list;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
        
        if(get_connection(el_of_list,knot)!=CRA){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=(*matrix)[knot][el_of_list].x;
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[knot][0].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=(*matrix)[knot][el_of_list].y;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }                      
      
    }
    
    count=-1;
    
    //Ueberpruefen der Vorgaengerliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[0][knot].x;
    
    if(el_of_list==0) {
      delete[] knot_list;  
      return false;
    }
    
    if(el_of_list==(size+1)){
      if((*matrix)[0][knot].y!=(size+1)){
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[0][knot].y==(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        count++;
        knot_list[count]=el_of_list;
        
        if(get_connection(el_of_list,knot)!=ARC){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=-((*matrix)[knot][el_of_list].x);
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }	      
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[0][knot].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=-(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=-((*matrix)[knot][el_of_list].y);
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }
    }
    count=-1;
    
    //Ueberpruefen der Kantenliste
    
    //Hinrichtung
    
    el_of_list=(*matrix)[knot][knot].x;
    
    if(el_of_list==0) {
      delete[] knot_list;
      return false;
    }
    
    if(el_of_list==(size+1)){
      if((*matrix)[knot][knot].y!=(size+1)){
        delete[] knot_list;
        return false;
      }
    }else{
      if((*matrix)[knot][knot].y==(size+1)){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=(size+1)){
        count++;
        knot_list[count]=el_of_list;
        
        if(get_connection(el_of_list,knot)!=EDGE){
          delete[] knot_list;
          return false;
        }
        
        el_of_list=(*matrix)[knot][el_of_list].x;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }      
      }
      
      //Rueckrichtung
      
      el_of_list=(*matrix)[knot][knot].y;
      
      if(el_of_list==0) {
        delete[] knot_list;
        return false;
      }
      
      if((*matrix)[knot][el_of_list].x!=size+1){
        delete[] knot_list;
        return false;
      }
      
      while(el_of_list!=size+1){
        
        if(count<0){
          delete[] knot_list;
          return false;
        }
        
        if(el_of_list!=knot_list[count]){
          delete[] knot_list;
          return false;
        }
        
        knot_list[count]=0;
        
        count--;
        
        el_of_list=(*matrix)[knot][el_of_list].y;
        
        if(el_of_list==0) {
          delete[] knot_list;
          return false;
        }
      }
      
      count++;
      if(count!=0){
        delete[] knot_list;
        return false;
      }
      
    }
    count=-1;
  }
  
  delete[] knot_list;  
  return true;
}

//**************************************************************************

bool Lisa_GraphAlg::topsort(const Lisa_Graph *const g,
                             Lisa_Vector<int> *const knot_sequence){
#ifdef LISA_DEBUG
  if(knot_sequence->get_m() != g->get_vertices()){
    G_ExceptionList.lthrow("Argument vector size incorrect in Lisa_GraphAlgo::topsort().",
                           Lisa_ExceptionList::ANY_ERROR);
    return false;
  }
#endif
  
  knot_sequence->fill(0);
  
  //graph copy
  Lisa_MatrixListGraph top(g);
  //stores the number of successors of knot i in i-1
  Lisa_Vector<int> pred(top.get_vertices());
  
  int source=0,v=0,next=1;
  
  while(v<=top.get_vertices()){
    //new predeccessors
    for(v=1; v<=top.get_vertices(); v++){
      if((*knot_sequence)[v-1]==0){
        pred[v-1]=top.number_of_pred(v);
      }
    }
    v=1;
    source=0;
    //find first source
    while((v<=top.get_vertices())&&(source==0)){
      if(((*knot_sequence)[v-1]==0)&&(pred[v-1]==0)){
        source=1;
      }
      else{
        v++;
      }
    }
    
    //remove all connections of the first source found
    if(v<=top.get_vertices()){
      top.clear(v);
      (*knot_sequence)[v-1]=next;
      next++;
    }
  }
  
  return (next==top.get_vertices()+1);
}

//**************************************************************************

