/*
 * ************** matching.cpp ******************************************
 * 
 * a matching algorithm for LiSA
 *
 * @author Marc Mörig
 * @version 2.3pre3
 *
 * started 11.06.00
 * last changes 05.11.01
 *  
 */

#include "matching.hpp"

Lisa_BipartMatching::Lisa_BipartMatching(const Lisa_Matrix<bool> *in){

 graph = new Lisa_Matrix<bool>(*in);

 m = (*graph).get_m(); 
 n = (*graph).get_n();

 I_matched = new Lisa_Vector<int>(n);
 J_matched = new Lisa_Vector<int>(m);
 
 I_marked = new Lisa_Vector<int>(n);
 J_marked = new Lisa_Vector<int>(m);
 
 S = new Lisa_Vector<bool>(n);
 T = new Lisa_Vector<bool>(m); 

 queue = new Lisa_Vector<int>(n+m+1);
 in_I = new Lisa_Vector<bool>(n+m+1);
 
 I_matched_PC = I_matched->get_first();
 J_matched_PC = J_matched->get_first();
 G_row_PC = graph->first_row();
 I_marked_PC = I_marked->get_first();
 J_marked_PC = J_marked->get_first();
 q_start_C = queue->get_first();
 i_start_C = in_I->get_first();

 calculated = 0;
}

//****************************************************

Lisa_BipartMatching::Lisa_BipartMatching(const int n_in,const int m_in){

 n = n_in;
 m = m_in;

 graph = new Lisa_Matrix<bool>(n,m);
 graph->fill(0);
 
 I_matched = new Lisa_Vector<int>(n);
 J_matched = new Lisa_Vector<int>(m);

 I_marked = new Lisa_Vector<int>(n);
 J_marked = new Lisa_Vector<int>(m);
 
 S = new Lisa_Vector<bool>(n);
 T = new Lisa_Vector<bool>(m);

 queue = new Lisa_Vector<int>(n+m+1);
 in_I = new Lisa_Vector<bool>(n+m+1);

 I_matched_PC = I_matched->get_first();
 J_matched_PC = J_matched->get_first();
 G_row_PC = graph->first_row();
 I_marked_PC = I_marked->get_first();
 J_marked_PC = J_marked->get_first();
 q_start_C = queue->get_first();
 i_start_C = in_I->get_first();
 
 calculated = 0;
}

//****************************************************

Lisa_BipartMatching::~Lisa_BipartMatching(){
  delete graph;
  delete I_matched;
  delete J_matched;
  delete I_marked;
  delete J_marked;
  delete S;
  delete T;
  delete queue;
  delete in_I;
}

//****************************************************
void inline Lisa_BipartMatching::ordered_start_matching(){
 
  (*I_matched).fill(UNMATCHED);
  (*J_matched).fill(UNMATCHED);
  
  Lisa_Vector<int>* I_keys = new Lisa_Vector<int>(n);
  Lisa_Vector<int>* J_keys = new Lisa_Vector<int>(m);
  Lisa_Order* I_sort = new Lisa_Order(n);
  Lisa_Order* J_sort = new Lisa_Order(m);
  int* J_P;
  int* I_P;
  
  int *const I_stop = I_keys->get_last();
  int *const J_stop = J_keys->get_last();
  
  I_keys->fill(0);
  J_keys->fill(0);
  
  Lisa_Vector<bool>* G_row_P = G_row_PC;
  bool* G_element_P;
  
  for(I_P = I_keys->get_first();I_P!=I_stop;I_P++,G_row_P++){
    G_element_P = G_row_P->get_first();
    for (J_P = J_keys->get_first();J_P!=J_stop;J_P++,G_element_P++){
      if (*G_element_P){
	(*I_P)++;
	(*J_P)++;
      }
    }
  }
  
  I_sort->read(I_keys);
  J_sort->read(J_keys);
  
  I_sort->sort();
  J_sort->sort();
  
  int si,sj;

  for (int i=0;i<n;i++){
    si = (*I_sort)[i];
    if ((*I_matched)[si]!=UNMATCHED) continue;
    for (int j=0;j<m;j++){
      sj = (*J_sort)[j];
      if ((*J_matched)[sj]!=UNMATCHED) continue;
      if ((*graph)[si][sj]){
        (*I_matched)[si] = sj;
        (*J_matched)[sj] = si;
        break;
      }
    }   
  }

  delete I_sort;
  delete J_sort;
  delete I_keys;
  delete J_keys;
}

//*********************************************************

void inline Lisa_BipartMatching::simple_start_matching(){

  (*I_matched).fill(UNMATCHED);
  (*J_matched).fill(UNMATCHED);
  
  int* I_matched_P = I_matched_PC;
  int* J_matched_P;
  
  Lisa_Vector<bool>* G_row_P = G_row_PC;
  bool* element;

  // go through all edges and mark them if possible
  for (int i=0;i<n;i++,G_row_P++,I_matched_P++){
    if ((*I_matched_P)!=UNMATCHED) continue;
    J_matched_P = J_matched_PC;
    element = G_row_P->get_first();
    for (int j=0;j<m;j++,element++,J_matched_P++){
      if ((*J_matched_P)!=UNMATCHED) continue;
      if (*element){
        (*I_matched_P) = j;
        (*J_matched_P) = i;
        break;
      }
    }   
  }
}

//****************************************************

void Lisa_BipartMatching::calculate(){
  
  simple_start_matching();

  int lastvertex;
  int i,j;
  int match;

  int* I_marked_P;
  int* J_marked_P;
  
  int* I_matched_P;
  
  bool* element;
  
  int* q_start;
  int* q_end;
  bool* i_start;
  bool* i_end;
  
  do{
    
    q_start = q_start_C;
    q_end = q_start_C;
    
    i_start = i_start_C;
    i_end = i_start_C;
    
    J_marked->fill(UNMARKED);
    I_marked->fill(UNMARKED);
    lastvertex = UNMARKED;

    // startmarkings ... go through I, mark all unmatched vertice
    // and put them into the queue
    I_matched_P = I_matched_PC;
    I_marked_P = I_marked_PC;
    for (i=0;i<n;i++,I_matched_P++,I_marked_P++)
      if ((*I_matched_P)==UNMATCHED){
        (*I_marked_P)=STARTMARK;
	(*(i_end++)) = 1;
	(*(q_end++)) = i;
      }
    
    // pick a vertex and deal with it 
    while(i_start!=i_end){
      
      if ((*(i_start++))){ // vertex from i
	i = (*(q_start++));
	match = (*I_matched)[i];
	element = (G_row_PC+i)->get_first();
	J_marked_P = J_marked_PC;
	for (j=0;j<m;j++,element++,J_marked_P++){
	  if((*element)&&match!=j&&(*J_marked_P)==UNMARKED){
	    (*J_marked_P)=i;
	    if ((*J_matched)[j]==UNMATCHED){ // found a way !
	      lastvertex = j;
	      goto stop;
	    }
	    (*(q_end++)) = j;
	    (*(i_end++)) = 0;
	  }
	}

      }else{ // vertex from J
	j = (*(q_start++));
	match = (*J_matched)[j];
	I_marked_P = I_marked_PC;
	for (i=0;i<n;i++,I_marked_P++){
	  if(match==i&&(*I_marked_P)==UNMARKED){
	    (*I_marked_P)= j;
	    (*(q_end++)) = i;
	    (*(i_end++)) = 1;
	  }
	}
      }// if else

    }// while
    
  stop:
    
    // if a way was found go back and "invert" matched and unmatched 
    // edges on the way
    if (lastvertex!=UNMARKED){
      
      j = lastvertex;
      i = (*J_marked)[j];

      (*J_matched)[j] = i;
      (*I_matched)[i] = j;
      
      while((*I_marked)[i]!=STARTMARK){
	
	j = (*I_marked)[i];
	i = (*J_marked)[j];
	(*J_matched)[j]=i;
	(*I_matched)[i]=j;
	
      }
      
    }
    
  }while(lastvertex!=UNMARKED);
  
  // cout << (*this); // debug info 
  
  calculated = 1;
}

//****************************************************

const Lisa_Vector<bool>* Lisa_BipartMatching::get_S(){
  // if needed calculate new matching 
  if (!calculated) calculate();
  // get marked vertex 
  int* I_marked_P = I_marked_PC;
  for (int i=0;i<n;i++)
    if ((*(I_marked_P++))==UNMARKED) (*S)[i]=0;
    else (*S)[i]=1; 
  return S;
}

//****************************************************

const Lisa_Vector<bool>* Lisa_BipartMatching::get_T(){
  // if needed calculate new matching 
  if (!calculated) calculate();
  // get marked vertex
  int* J_marked_P = J_marked_PC;
  for (int j=0;j<m;j++)
    if ((*(J_marked_P++))==UNMARKED) (*T)[j]=0;
    else (*T)[j]=1; 
  return T;
}

//****************************************************

const Lisa_Vector<int>* Lisa_BipartMatching::get_matching_I(){
  if (!calculated) calculate();
  return I_matched;
}

//****************************************************

const Lisa_Vector<int>* Lisa_BipartMatching::get_matching_J(){
  if (!calculated) calculate();
  return J_matched;
}

//****************************************************

bool inline Lisa_BipartMatching::get_edge(const int i,const int j) const{
  return (*graph)[i][j];
}

//****************************************************

void inline Lisa_BipartMatching::set_edge(const int i,const int j,const bool value){
  // reset flag
  calculated = 0;
  (*graph)[i][j] = value;
}

//****************************************************

void inline Lisa_BipartMatching::set_all_edges(const bool value){
  calculated = 0;
  graph->fill(value);
}

//****************************************************

bool Lisa_BipartMatching::is_perfect(){
  // if all vertex in the smaller set are matched 
  // the matching is perfect
  
  if (!calculated) calculate();
  
  if (n<m){
    for (int i=0;i<n;){
      if ((*I_matched)[i++]==UNMATCHED){
	return 0;
      }
    }
  }else{
    for (int j=0;j<m;){
      if ((*J_matched)[j++]==UNMATCHED){
	return 0;
      }
    }
  }
  return 1;
}

//****************************************************

void Lisa_BipartMatching::write(ostream & strm) const {
  strm << "<BIPARTMATCHING>"<<endl;
  
  strm << "  ";
  for (int i=0;i<n;i++){
    strm.width(3);
    strm << (*I_marked)[i];
    if ((*I_matched)[i]!=UNMATCHED) strm << "*";
    else strm << " ";
  }
  strm << "|" << endl;
  
  strm << "  ";
  for (int i=0;i<n;i++) strm << "----" ; 
  strm << "+----" << endl;
  
  for (int j=0;j<m;j++){
    strm << "  ";
    for (int i=0;i<n;i++){
      strm.width(3);
      strm << (*graph)[i][j];
      if ((*I_matched)[i]==j&&(*J_matched)[j]==i) strm << "*";
      else if ((*I_matched)[i]!=j&&(*J_matched)[j]!=i) strm << " ";
      else strm << "#";
    }
    strm << "|";
    strm.width(3);
    strm << (*J_marked)[j]; 
    if ((*J_matched)[j]!=UNMATCHED) strm << "*";
    else strm << " ";
    strm << endl;
  }
  strm << "</BIPARTMATCHING>"<<endl;
}

//****************************************************
//****************************************************

const TIMETYP Lisa_Matching::REMOVED = -MAXOBJECTIVE;

Lisa_WeightedMatching::Lisa_WeightedMatching(const Lisa_Matrix<TIMETYP> *in){
  
  n = (*in).get_n();
  m = (*in).get_m();
  
  mx = (m<n?n:m);
  
  graph = new Lisa_Matrix<TIMETYP>(mx,mx);
  graph->fill(0);
  
  for (int i=0;i<n;i++)
    for (int j=0;j<m;j++)
      (*graph)[i][j] = (*in)[i][j];
  
  I_mark = new Lisa_Vector<TIMETYP>(mx);
  J_mark = new Lisa_Vector<TIMETYP>(mx); 
  
  bipart = new Lisa_BipartMatching(mx,mx);
  
  calculated = 0;
  inverted = 0;
}

//****************************************************

Lisa_WeightedMatching::Lisa_WeightedMatching(const int n_in, const int m_in){
  
  n = n_in;
  m = m_in;
  
  mx = (m<n?n:m);
  
  graph = new Lisa_Matrix<TIMETYP>(mx,mx);
  graph->fill(0);
  
  I_mark = new Lisa_Vector<TIMETYP>(mx);
  J_mark = new Lisa_Vector<TIMETYP>(mx); 
  
  bipart = new Lisa_BipartMatching(mx,mx);
  
  calculated = 0;
  inverted = 0;
} 

//****************************************************

Lisa_WeightedMatching::~Lisa_WeightedMatching(){
  delete I_mark;
  delete J_mark;
  delete graph;
  delete bipart;
}

//****************************************************

const Lisa_Vector<int>* Lisa_WeightedMatching::get_matching_J(){
  if (!calculated) calculate();
  return (*bipart).get_matching_J();
}

//****************************************************

const Lisa_Vector<int>* Lisa_WeightedMatching::get_matching_I(){
  if (!calculated) calculate();
  return (*bipart).get_matching_I();
}

//****************************************************

void inline Lisa_WeightedMatching::calculate(){
  
  calc_start_marks();
  
  do{
    
    // cout<<(*this); // debug info output
    
    // create a new unweighted graph with the help of the markings
    for (int i=0;i<mx;i++)
      for (int j=0;j<mx;j++)
	if ((*I_mark)[i]+(*J_mark)[j]==(*graph)[i][j])
	  (*bipart).set_edge(i,j,1);
        else (*bipart).set_edge(i,j,0);
    
    calc_new_marks();
    
  }while(!(*bipart).is_perfect());
  
  calculated = 1;
}

//****************************************************

void inline Lisa_WeightedMatching::calc_start_marks(){
  TIMETYP ma,ms;
  
  // set all I_marks on the lowest possible value 
  for (int i=0;i<mx;i++){
    ma = 0;
    
    for (int j=0;j<mx;j++)
      if (ma<(*graph)[i][j]) 
	ma = (*graph)[i][j];
    
    (*I_mark)[i] = ma; 
  }
  
  // set all J_marks on the lowest possible value 
  ms = (*I_mark)[0];
  
  for (int i=0;i<mx;i++)
    if (ms>(*I_mark)[i]) 
      ms = (*I_mark)[i];
  
  for (int j=0;j<mx;j++){
    ma = -ms;
    
    for (int i=0;i<mx;i++)
      if (ma<(*graph)[i][j]-(*I_mark)[i]) 
	ma = (*graph)[i][j]-(*I_mark)[i]; 
    
    (*J_mark)[j] = ma;
  }
}

//****************************************************

void inline Lisa_WeightedMatching::calc_new_marks(){
  const Lisa_Vector<bool> *Sv = (*bipart).get_S();
  const Lisa_Vector<bool> *Tv = (*bipart).get_T();
  TIMETYP d=0,t; 
  
  // for all edges which have a vertice in S but not in T 
  // get the smallest d>0
  for (int i=0;i<mx;i++)
    if ((*Sv)[i])
      for (int j=0;j<mx;j++)
	if (!(*Tv)[j]){
	  t = (*I_mark)[i]+(*J_mark)[j]-(*graph)[i][j];
	  if (d==0) d = t;
	  else if (t>0&&t<d) d = t;
	}
  
  // change the markings by d
  for (int i=0;i<mx;i++)
    if ((*Sv)[i])
      (*I_mark)[i]-=d;
  
  for (int j=0;j<mx;j++)
    if ((*Tv)[j])
      (*J_mark)[j]+=d;

}

//****************************************************

TIMETYP Lisa_WeightedMatching::get_edge(const int i,const int j) const{
  TIMETYP out = (*graph)[i][j];
  if (out!=REMOVED && inverted) out = MAXTIME - out;
  return out;
}

//****************************************************

void Lisa_WeightedMatching::set_edge(const int i,const int j,const TIMETYP value){
  calculated = 0;
  if ((*graph)[i][j] != REMOVED){
    if (inverted)  (*graph)[i][j] = MAXTIME - value;
    else  (*graph)[i][j] = value;
  }
}

//****************************************************

void Lisa_WeightedMatching::set_all_edges(const Lisa_Matrix<TIMETYP>* in){
  calculated = 0;
  
  Lisa_Vector<TIMETYP>* row = graph->first_row();
  TIMETYP* element;

  for (int i=0;i<n;i++){
    element = (*(row++)).get_first();
    for (int j=0;j<m;j++,element++){
      if ((*element) != REMOVED){
	if (inverted) (*element) = MAXTIME - (*in)[i][j];
	else (*element) = (*in)[i][j];
      }
    }  
  }

}
//****************************************************

void Lisa_WeightedMatching::remove(const int i,const int j){
  calculated = 0;
  (*graph)[i][j] = REMOVED;
}

//****************************************************

void Lisa_WeightedMatching::invert(){
  calculated = 0;
  
  inverted = !inverted;
  
  Lisa_Vector<TIMETYP>* row = graph->first_row();
  TIMETYP* element;

  for (int i=0;i<n;i++){
    element = (*(row++)).get_first();
    for (int j=0;j<m;j++,element++)
      if (*element != REMOVED)
	*element = MAXTIME - (*element);
  }

}

//****************************************************

void Lisa_WeightedMatching::write(ostream & strm) const {
  
  int i,j;
  
  strm << "<WBIPARTMATCHING>" << endl;
  
  strm << (*bipart);
  
  strm << "  ";
  for (i=0;i<n;i++){
    strm.width(10);
    strm << (*I_mark)[i] << " ";
  }
  strm << " | ";
  for (;i<mx;i++){
    strm.width(10);
    strm << (*I_mark)[i] << " ";
  }
  strm << "|" << endl;
  
  strm << "  ";
  for (i=0;i<n;i++) strm << "-----------";
  strm << "-+-";
  for (;i<mx;i++) strm << "-----------";
  strm << "+----------" <<endl;
  
  for (j=0;j<m;j++){
    strm << "  ";
    for (i=0;i<n;i++){
      strm.width(10);
      strm << (*graph)[i][j];
      if ((*graph)[i][j]==(*J_mark)[j]+(*I_mark)[i]) strm << "*";
      else strm << " ";
    }
    strm << " | ";
    for (;i<mx;i++){
      strm.width(10);
      strm << (*graph)[i][j];
      if ((*graph)[i][j]==(*J_mark)[j]+(*I_mark)[i]) strm << "*";
      else strm << " ";
    }
    strm << "|";
    strm.width(7);
    strm << (*J_mark)[j];
    strm << endl;
  }
  
  strm << "  ";
  for (i=0;i<n;i++) strm << "-----------";
  strm << "-+-";
  for (;i<mx;i++) strm << "-----------";
  strm << "+-----------" << endl;
  
  for (;j<mx;j++){
    strm << "  ";
    for (i=0;i<n;i++){
      strm.width(10);
      strm << (*graph)[i][j];
      if ((*graph)[i][j]==(*J_mark)[j]+(*I_mark)[i]) strm << "*";
      else strm << " ";
    }
    strm << " | ";
    for (;i<mx;i++){
      strm.width(10);
      strm << (*graph)[i][j];
      if ((*graph)[i][j]==(*J_mark)[j]+(*I_mark)[i]) strm << "*";
      else strm << " ";
    }
    strm << "|";
    strm.width(10);
    strm << (*J_mark)[j];
    strm << endl;
  }
  strm << "</WBIPARTMATCHING>" << endl;
}

//****************************************************
//****************************************************

Lisa_BottleneckMatching::Lisa_BottleneckMatching(const Lisa_Matrix<TIMETYP>* in){
  
  m = in->get_m();
  n = in->get_n();
  
  mx = m<n ? n : m;
  
  lst = new Lisa_List<Lisa_BottleneckNode>();
  bipart = new Lisa_BipartMatching(mx,mx);

  J_matched = new Lisa_Vector<int>(mx);
  I_matched = new Lisa_Vector<int>(mx);
  
  Lisa_BottleneckNode* current;
  int i,j;
  
  for (i=0;i<n;i++){
    for(j=0;j<m;j++){
      current = new Lisa_BottleneckNode(i,j,(*in)[i][j]);
      lst->append(*current);
      delete current;
    }
    for(;j<mx;j++){
      current = new Lisa_BottleneckNode(i,j,0);
      lst->append(*current);
      delete current;
    }
  }
  for (;i<mx;i++){
    for(j=0;j<mx;j++){
      current = new Lisa_BottleneckNode(i,j,0);
      lst->append(*current);
      delete current;
    }
  }
  
  invert();

  calculated = 0;
  inverted = 1;
  sorted = 0;
}

//****************************************************  

Lisa_BottleneckMatching::~Lisa_BottleneckMatching(){
  delete lst;
  delete I_matched;
  delete J_matched;
  delete bipart;
}

//****************************************************

void Lisa_BottleneckMatching::calculate(){
  
  if(lst->empty()){
    I_matched->fill(UNMATCHED);
    J_matched->fill(UNMATCHED);
    return;
  }
  
  if (!sorted) lst->sort();
  sorted = 1;
  
  lst->reset();
  bipart->set_all_edges(0);

  int loB = 0;
  int hiB = lst->length()-1;
  int curB = hiB;
 
  Lisa_BottleneckNode& current = lst->get();
  bipart->set_edge(current.i,current.j,1);
  
  do{
    
    while(lst->get_index() < curB) inc_next();
    while(lst->get_index() > curB) rem_curr();
          
    TIMETYP threshold = lst->get().value;
    while(true){
      if (!lst->next()){ 
	lst->previous();
	break;
      }
      Lisa_BottleneckNode& current = lst->get();
      if (current.value != threshold){
	lst->previous();
	break;
      }
      bipart->set_edge(current.i,current.j,1);
      curB++;
    }
      //cout << "|==|*************>> " << loB << " " << hiB << " " << curB << " " << oldB << endl;
    
    if (bipart->is_perfect()){
      (*I_matched) = *(bipart->get_matching_I());
      (*J_matched) = *(bipart->get_matching_J());
      while(true){
	Lisa_BottleneckNode& current = lst->get();
	if (current.value == threshold && curB >= loB){
	  bipart->set_edge(current.i,current.j,0);
	  lst->previous();
	  curB--;
	}else{
	  lst->next();
	  Lisa_BottleneckNode& current = lst->get();
	  bipart->set_edge(current.i,current.j,1);
	  curB++;
	  break;
	}
      }
      hiB = curB;
    }else{
      loB = curB;
    }
    
    curB = (loB+hiB)/2;
    
    //cout << *this ;
    
  }while(loB<hiB && curB >= (mx-1) && curB!=lst->get_index());
  
  calculated = 1;
}

//****************************************************

void inline Lisa_BottleneckMatching::inc_next(){
  lst->next();
  Lisa_BottleneckNode& current = lst->get();
  bipart->set_edge(current.i,current.j,1);
}


//****************************************************

void inline Lisa_BottleneckMatching::rem_curr(){
  Lisa_BottleneckNode& current = lst->get();
  bipart->set_edge(current.i,current.j,0);
  lst->previous();
}

//****************************************************

const Lisa_Vector<int>* Lisa_BottleneckMatching::get_matching_I(){
  if (!calculated) calculate();
  return I_matched;
}

//****************************************************

const Lisa_Vector<int>* Lisa_BottleneckMatching::get_matching_J(){
  if (!calculated) calculate();
  return J_matched;
}

//****************************************************

void Lisa_BottleneckMatching::invert(){
  calculated = 0;
  sorted = 0;
  inverted = !inverted;
  lst->reset();
  do{
    Lisa_BottleneckNode& current = lst->get();
    if (current.i<n && current.j<m)
      current.value = MAXTIME-current.value;
  }while(lst->next());
}

//****************************************************

void Lisa_BottleneckMatching::remove(const int i,const int j){
  if(lst->empty()) return;
  calculated = 0;
  lst->reset();
  do{
    Lisa_BottleneckNode& current = lst->get();
    if (current.i == i && current.j == j){
      lst->exclude();
      break;
    }
  }while(lst->next());
}

//****************************************************

void Lisa_BottleneckMatching::set_edge(const int i,const int j,const TIMETYP value){
  sorted = 0;
  calculated = 0;
  lst->reset();
  do{
    Lisa_BottleneckNode& current = lst->get();
    if(current.i==i && current.j==j){
      if (inverted) current.value = MAXTIME - value;
      else current.value = value;
      break;
    }
  }while(lst->next());
}

//****************************************************

void Lisa_BottleneckMatching::set_all_edges(const Lisa_Matrix<TIMETYP>* in){
  if (lst->empty()) return;
  sorted = 0;
  calculated = 0;
  lst->reset();
  do{
    Lisa_BottleneckNode& current = lst->get();
    if(current.i<n&&current.j<m){
      if (inverted) current.value = MAXTIME - (*in)[current.i][current.j];
      else current.value = (*in)[current.i][current.j];
    }
  }while(lst->next());
}

//****************************************************

TIMETYP Lisa_BottleneckMatching::get_edge(const int i,const int j)const{
  lst->reset();
  do{
    Lisa_BottleneckNode& current = lst->get();
    if(current.i==i && current.j==j)
      return current.value;
  }while(lst->next());
  return REMOVED;
}

//****************************************************

void Lisa_BottleneckMatching::write(ostream & strm) const{
  strm << "<BOTTLENECKMATCHING>" << endl;
  strm << (*lst);
  strm << (*bipart);
  strm << "</BOTTLENECKMATCHING>" << endl;
}













