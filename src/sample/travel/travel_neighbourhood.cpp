/**
 * @author  Marc Moerig
 * @version 2.3final
 */

#include "travel_neighbourhood.hpp"

//**************************************************************************

Travel_RPI_Neighbourhood::Travel_RPI_Neighbourhood(Travel_Graph* prob_in):length(prob_in->get_vertices()){
  
  /// keep a handle to our input
  prob = prob_in;
  
  /// create some solution vectors
  sol[ORIG_SOLUTION] = new Lisa_Vector<int>(length);
  sol[WORK_SOLUTION] = new Lisa_Vector<int>(length);
  sol[BEST_SOLUTION] = new Lisa_Vector<int>(length);
  sol[BEST_NGH_SOLUTION] = new Lisa_Vector<int>(length);
  sol[POSS_SOLUTION] = new Lisa_Vector<int>(length);

  /// create two tabu vectors
  /// we only really work with tabu_param[0]
  /// the other is just a copy needed by the algorithm handled by store_tabu_param()
  tabu_para[0] = new Lisa_Vector<int>(length-1);
  tabu_para[0]->fill(0);
  tabu_para[1] = new Lisa_Vector<int>(length-1);
  tabu_para[1]->fill(0);
  
  /// no tabulist yet
  /// its not needed by some algorithms and will be created 
  /// with a call to init_tabulist() if needed
  tabulist = 0;

  if(prob->has_solution()){  // we already have a starting solution .. so read it
    prob->get_solution(sol[ORIG_SOLUTION]);
  }else{ // create a simple start solution .. going from 0 -> 1 -> ... -> n -> 0
    for(int i=0;i<length;i++){
      (*sol[ORIG_SOLUTION])[i] = i;
    }
  }
  
  /// for the random number generator
  seed = time(0);
    
}

//**************************************************************************

Travel_RPI_Neighbourhood::~Travel_RPI_Neighbourhood(){
  delete sol[ORIG_SOLUTION];
  delete sol[WORK_SOLUTION];
  if(sol[BEST_SOLUTION]) delete sol[BEST_SOLUTION];
  if(sol[BEST_NGH_SOLUTION]) delete sol[BEST_NGH_SOLUTION];
  delete sol[POSS_SOLUTION];

  delete tabu_para[0];
  delete tabu_para[1];

  if(tabulist) delete tabulist;
}

//**************************************************************************

void inline Travel_RPI_Neighbourhood::put_to_tabu_vector(Lisa_Vector<int>* solution){
  /// since 0 1 2 3 4 , 3 4 0 1 2 and even 2 1 0 4 3 are the same(!) solution 
  /// we have to make sure that all solution that are the same get the same 
  /// tabu representation

  int zeropos=0;
  
  /// find the 0 in our vector
  for(int i=0;i<length;i++){
    if((*solution)[i]==0){
      zeropos = i;
      break;
    }
  }
  
  /// do i have to go right or left ?
  const int fwd = (*solution)[(zeropos+length-1)%length] < (*solution)[(zeropos+1)%length] ? -1 : 1;
  
  /// read solution to tabu vector
  for(int i=0;i<length-1;i++){
    zeropos = (zeropos+length+fwd)%length;
    (*tabu_para[0])[i] = (*solution)[zeropos];
  }
  
}

//**************************************************************************

int Travel_RPI_Neighbourhood::prepare_move(const int type){
  static int pos1,pos2;
  /// we will switch citys located at two random position


  // we work with the original or last accepted solution 
  *sol[POSS_SOLUTION] = *sol[ORIG_SOLUTION]; 
  
  
  // find two positions at which to swap citys
  // make sure they are not the same
  do{
    pos1 = lisa_random(0,length-1,&seed);
    pos2 = lisa_random(0,length-1,&seed);
  }while(pos1==pos2);
  
  // do swap
  const int swp = (*sol[POSS_SOLUTION])[pos1];
  (*sol[POSS_SOLUTION])[pos1] = (*sol[POSS_SOLUTION])[pos2];
  (*sol[POSS_SOLUTION])[pos2] = swp;
  
  // put possible new solution to tabu vector 0
  // so the algorithm can check whether this new possible solution 
  // is in the tabulist or not
  put_to_tabu_vector(sol[POSS_SOLUTION]);
  
  return OK;
}

//**************************************************************************

int Travel_RPI_Neighbourhood::do_move(){
  
  // put old solution to tabu vector 0 
  // so it can be made tabu by the algorithm
  put_to_tabu_vector(sol[ORIG_SOLUTION]);
  
  // make our possible new solution the work solution
  *sol[WORK_SOLUTION] = *sol[POSS_SOLUTION] ;

  return OK;
}

//**************************************************************************

int Travel_API_Neighbourhood::prepare_move(const int type){
  /// we will switch a random city with its successor
  /// or enumerated any city with its successor

  static int pos1,pos2;

  // we work with the original or last accepted solution 
  *sol[POSS_SOLUTION] = *sol[ORIG_SOLUTION]; 
  
  if(type == RAND){    
    pos1 = lisa_random(0,length-1,&seed); 
  }else if(type == ENUM){
    
    if(enum_pos==length){ // ok, we have all neighbours
      enum_pos = 0;
      return NO_NGHBOURS;
    }

    pos1 = enum_pos++;
   
  }else{
    return !OK;
  }

  pos2 = (pos1+1)%length;
  
  // do a swap
  const int swp = (*sol[POSS_SOLUTION])[pos1];
  (*sol[POSS_SOLUTION])[pos1] = (*sol[POSS_SOLUTION])[pos2];
  (*sol[POSS_SOLUTION])[pos2] = swp;
  
  // put possible new solution to tabu vector 0
  // so the algorithm can check whether this new possible solution 
  // is in the tabulist or not
  put_to_tabu_vector(sol[POSS_SOLUTION]);
  
  return OK; 
}

//**************************************************************************

