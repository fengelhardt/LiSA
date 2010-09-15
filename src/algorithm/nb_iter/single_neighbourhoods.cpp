/**
 * @author  Andreas Winkler
 * @version 3.0
 */
 
#include <cstdlib>

#include "single_neighbourhoods.hpp"

using namespace std;


//**************************************************************************

API_Neighbourhood::API_Neighbourhood(Lisa_1Schedule *Plan,Lisa_1Problem *PPi){
  num = 1;
  seed = 247639875L;
  PP = PPi;
   
  if ( !( P[0] = new Lisa_1Schedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  *(P[0])=*Plan;

  if ( !( P[1] = new Lisa_1Schedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }

  if ( !( P[2] = new Lisa_1Schedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  
  if ( !( P[3] = new Lisa_1Schedule( PP ) )){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  
  tabulist = 0;
}

//**************************************************************************

API_Neighbourhood::~API_Neighbourhood(){
  if ( P[0] ) delete P[0];
  if ( P[1] ) delete P[1];
  if ( P[2] ) delete P[2];
  if ( P[3] ) delete P[3];
  if ( tabulist ) delete tabulist;
}

//**************************************************************************

int API_Neighbourhood::copy_schedule(int a,int b){
  *P[b]=*P[a];
  return OK;
}

//**************************************************************************

int API_Neighbourhood::accept_solution(){
  return copy_schedule( WORK_SOLUTION, ORIG_SOLUTION);
}

//**************************************************************************
  
int API_Neighbourhood::accept_best_ngh(){
  return copy_schedule( BEST_NGH_SOLUTION, ORIG_SOLUTION);
}

//**************************************************************************

int API_Neighbourhood::put_orig_to_best(){
  return copy_schedule( ORIG_SOLUTION, BEST_SOLUTION );
}

//**************************************************************************

int API_Neighbourhood::put_work_to_best_ngh(){
  return copy_schedule( WORK_SOLUTION, BEST_NGH_SOLUTION );
}

//**************************************************************************

int API_Neighbourhood::prepare_move( int typ ){
  // determ sa possible move
  // typ=ENUM : enumerativ
  // typ=RAND : randomly
  int number=1;
  int nend=20;
  int test;
  
  if ( typ == ENUM ){
    do{
	    test = OK;
	    number++;
	    pos = num++;
	    if ( num == PP->n ){
	      num = 1;
	      return NO_NGHBOURS;
	    }
	   
      // is this move setting to be tabu ? :
	    tabu_param[0][0] = P[0]->get_sequ(pos);
	    tabu_param[0][1] = P[0]->get_sequ(pos+1);
	    tabu_param[0][2] = 0;
	    tabu_param[0][3] = 0;
	   
      if ( (*PP->prec)[ P[0]->get_sequ(pos) ][ P[0]->get_sequ(pos+1)   ] == 1) test = !OK;
    
    }while( (test==!OK) && (number < nend) ); 
    
    if ( test==OK ){
      return OK;
    }else{
	   cout << "NoNghb  ";
	   return NO_NGHBOURS;
	  }
  
  }else if ( typ == RAND ){
    
    do{
	    test = OK;
      number++;
      pos = lisa_random( 1, PP->n-1, &seed );
	   
      // is this move setting to be tabu ? :
      tabu_param[0][0] = P[0]->get_sequ(pos);
      tabu_param[0][1] = P[0]->get_sequ(pos+1);
      tabu_param[0][2] = 0;
      tabu_param[0][3] = 0;
      
      if ( (*PP->prec)[ P[0]->get_sequ(pos) ][ P[0]->get_sequ(pos+1)   ] == 1) test = !OK;
	 
    }while( (test==!OK) && (number < nend) );
    
    if ( test==OK ){ 
      return OK;
    }else{
	   cout << "NoNghb  ";
	   return NO_NGHBOURS;
    }
  }
  
  printf( "\nwrong parameter in prepare_move( %i )", typ );
  return !OK;
}

//**************************************************************************

int API_Neighbourhood::do_move(){
  *P[1]=*P[0];

  // the following swap means, that in next time the re-move is set 
  // to be tabu
  int help = tabu_param[0][0];
  tabu_param[0][0] = tabu_param[0][1];
  tabu_param[0][1] = help;

  return P[1]->shift(pos, pos+1);
}

//**************************************************************************

int API_Neighbourhood::anti_neighbour(){
  return OK;
}

//**************************************************************************

void API_Neighbourhood::set_objective_type( int o ){
  objective_type = o;
}

//**************************************************************************

void API_Neighbourhood::set_objective( int z, int a){
#ifdef LISA_DEBUG
  if ((a<0) || (a>=4)){
    G_ExceptionList.lthrow("wrong schedule in set_objective_type("+ztos(a)+")");
    exit( 7 );
  }
#endif
  P[a]->SetValue(z);
}

//**************************************************************************

TIMETYP API_Neighbourhood::get_objective_value( int a){
#ifdef LISA_DEBUG
  if ((a<0) || (a>=4)){
    G_ExceptionList.lthrow("wrong schedule in get_objective_type("+ztos(a)+")");
    exit( 7 );
  }
#endif
  return P[a]->GetValue();
}

//**************************************************************************

int API_Neighbourhood::init_tabulist( unsigned int length ){
  if(tabulist) delete tabulist;
  if ( !(tabulist = new Lisa_Tabu( length )) ){
    G_ExceptionList.lthrow("out of memory",Lisa_ExceptionList::NO_MORE_MEMORY);
    exit( 7 );
  }
  return OK;
}

//**************************************************************************

int API_Neighbourhood::use_tabulist(){
#ifdef LISA_DEBUG
  if(!tabulist){
    G_ExceptionList.lthrow("No tabulist created.",Lisa_ExceptionList::UNDEFINED_OBJECT);
    exit( 7 );
  }
#endif
  return tabulist->use(tabu_param[0][0],tabu_param[0][1],
                       tabu_param[0][2],tabu_param[0][3]);
}

//**************************************************************************

int API_Neighbourhood::set_tabulist(){
#ifdef LISA_DEBUG
  if(!tabulist){
    G_ExceptionList.lthrow("No tabulist created.",Lisa_ExceptionList::UNDEFINED_OBJECT);
    exit( 7 );
  }
#endif
  tabulist->set(tabu_param[1][0],tabu_param[1][1],
		            tabu_param[1][2],tabu_param[1][3]);
  return OK;
}

//**************************************************************************

void API_Neighbourhood::store_tabu_param(){
  for (int i=0; i<=3; i++ ) tabu_param[1][i] = tabu_param[0][i];
}

//**************************************************************************

void API_Neighbourhood::clean_tabu_param(){
  for (int i=0; i<4; i++ )  tabu_param[0][i] = 0;
}

//**************************************************************************

void API_Neighbourhood::return_schedule( Lisa_1Schedule *Plan ){
  *Plan = *(P[BEST_SOLUTION]);
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

shift_Neighbourhood::shift_Neighbourhood( Lisa_1Schedule *Plan, Lisa_1Problem *PPi )
	                                      :API_Neighbourhood( Plan, PPi ){
  pos1 = 1;
  pos2 = 1;
  inc_dec = 1;
}

//**************************************************************************

int shift_Neighbourhood::prepare_move( int typ ){
  int i;
  // determs a possible move
  // typ=ENUM : enumerativ
  // typ=RAND : randomly
  int test,richtung;
  int number = 0;
  int nend = 20;

  if ( typ == ENUM ){
  
    do{
	    test = OK;
	    number++;
	    pos2 += inc_dec;
	    
      if ( (pos1==2) && (pos2==0) ){
	       pos1 = 1;
	       pos2 = 1;
	       inc_dec = 1;
	       return NO_NGHBOURS;
	    }
      
	    if ( (pos2==PP->n+1) || (pos2==0) ){
         pos1 += inc_dec;
	       if ( (pos1==PP->n) || (pos1==1) ) inc_dec *= -1;
	       pos2 = pos1 + inc_dec;
	    }
	   
     // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos1);
	   if ( pos1 < pos2 ) tabu_param[0][1] = 1;
	   else tabu_param[0][1] = -1;
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   if ( pos1 < pos2 ){
	     for ( i=pos1+1; i<=pos2; i++ )
		     if ( (*PP->prec)[ P[0]->get_sequ(pos1) ] [ P[0]->get_sequ(i) ] == 1 )
           test = !OK;
     }else{
       for ( i=pos2; i<pos1; i++ )
         if ( (*PP->prec)[ P[0]->get_sequ(i)][ P[0]->get_sequ(pos1) ] == 1 )
           test = !OK;
     }
    
    }while ( (test==!OK) && (number < nend) );
    
    if ( test==OK ){
      return OK;
    }else{
	    cout << "NoNghb  ";
	    return NO_NGHBOURS;
    }
  
  }else if ( typ == RAND ){
       
    do{
      test = OK;
	    number++;
	    pos1 = lisa_random( 1, PP->n, &seed );
	    richtung = lisa_random( 0, 1, &seed );
	    
      if ( richtung==0 ){
	      if ( pos1==1 ){
          pos1 = 2;
          pos2 = 1;
        }else{
          pos2 = lisa_random( 1, pos1-1, &seed );
        }
      }else{
        if ( pos1==PP->n ){
          pos1 = PP->n-1;
          pos2 = PP->n;
        }else{
          pos2 = lisa_random( pos1+1, PP->n, &seed );
        }
      }
	   
     // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos1);
	   if ( pos1 < pos2 ) tabu_param[0][1] = 1;
	   else tabu_param[0][1] = -1;
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   
     if ( pos1 < pos2 ){
       for ( i=pos1+1; i<=pos2; i++ )
         if ( (*PP->prec)[ P[0]->get_sequ(pos1) ][ P[0]->get_sequ(i) ] == 1 )
           test = !OK;
	   }else{
       for ( i=pos2; i<pos1; i++ )
         if ( (*PP->prec)[ P[0]->get_sequ(i) ][ P[0]->get_sequ(pos1) ] == 1 )
           test = !OK;
	   }
    
    }while ( (test==!OK) && (number < nend) );
    
    if ( test==OK ){
      return OK;
    }else{
      return NO_NGHBOURS;
    }
  }
  
  G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
  return NO_NGHBOURS;
}

//**************************************************************************

int shift_Neighbourhood::do_move(){
  *P[1]=*P[0];
  // the following means, that in next time the re-move is set 
  // to be tabu
  tabu_param[0][1] = (-1) * tabu_param[0][1];
  return P[1]->shift( pos1, pos2 );
}

//**************************************************************************
//**************************************************************************
//**************************************************************************
PI_Neighbourhood::PI_Neighbourhood( Lisa_1Schedule *Plan, Lisa_1Problem *PPi )
	                                      :API_Neighbourhood( Plan, PPi ){
  pos1 = 1;
  pos2 = 1;
}

//**************************************************************************

int PI_Neighbourhood::prepare_move( int typ ){
#ifdef LISA_DEBUG
  if( typ != ENUM && typ != RAND){
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return NO_NGHBOURS;
  }
#endif

  // determin a possible move
  for(int j=0;j<PP->n;j++){
    bool move_ok = true;
     
    //create two random swap positions
    pos1 = lisa_random( 1, PP->n, &seed );
    do{
      pos2 = lisa_random( 1, PP->n, &seed ); 
    }while(pos1==pos2);
     
    //wlog pos1 < pos2
    if(pos1 > pos2){
      const int temp = pos2;
      pos2= pos1;
      pos1 = temp;
    }
     
    //can we swap these positions without violating precedence constrains ?
    for(int i=pos1+1;i<=pos2;i++){
      if( (*PP->prec)[ P[0]->get_sequ(pos1) ][ P[0]->get_sequ(i) ] == 1 ){
        move_ok = false;
        break;
      }
    }
    if(!move_ok) continue;
     
    for(int i=pos2-1;i>=pos1;i--){
      if( (*PP->prec)[ P[0]->get_sequ(i) ][ P[0]->get_sequ(pos2) ] == 1 ){
        move_ok = false;
        break;
      }  
    }
    if(!move_ok) continue;
     
    // save parameters to check whether this move is tabu
    tabu_param[0][0] = P[0]->get_sequ(pos1);
    tabu_param[0][1] = P[0]->get_sequ(pos2);
     
    return OK;
   }
    
   // failed to create a valid neighbour n times
   return NO_NGHBOURS;
}

//**************************************************************************

int PI_Neighbourhood::do_move(){
  *P[1]=*P[0];
  P[1]->swap(pos1,pos2);
  return OK;
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

