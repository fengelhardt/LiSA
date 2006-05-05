/**
* @author  Andreas Winkler
* @version 2.3final
*/

#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <iostream>
#include <iomanip>
#include <fstream>

#include "../../main/global.hpp"
#include "../../misc/int2str.hpp"

#include "iterate.hpp"

using namespace std;

//**************************************************************************

Lisa_Iter::Lisa_Iter(){
  abort_algorithm = false;
  abort_at_bound = -MAXNUMBER;
  abort_stuck = MAXNUMBER;
  
  time_t zeit = time(0);
  seed = long( zeit );
  cout<<"\nseed = "<<zeit<<"\n";
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

Lisa_Iterator::Lisa_Iterator( int methodi, unsigned int param1 ){

  method = NOMETHOD;

  max_stuck=MAXNUMBER;
  anti_neighbour = false;
  abort_algorithm = false;
  method = methodi;
  switch ( method ){
    case  II:
      if ( !((int(param1)==ENUM) || (int(param1)==RAND)) ){
        G_ExceptionList.lthrow("wrong parameter in init( II, "+ztos(int(param1))); 
        exit( 7 );
      }
      search_type = param1;
      break;
    case  SA: 
    case  SA_anti:
    case  TA: 
    case  TS:
      G_ExceptionList.lthrow("wrong call function init( int, int, int )");
      exit( 7 );
    default:
      G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
      exit( 7 );
  }
}

//**************************************************************************

Lisa_Iterator::Lisa_Iterator( int methodi, unsigned int param1, unsigned int param2 ){

  method = NOMETHOD;

  max_stuck=MAXNUMBER;
  anti_neighbour = false;
  abort_algorithm = false;
  method = methodi;
  switch(method){
    case  II:
      G_ExceptionList.lthrow("wrong call of function init( II, int )");
      exit( 7 );
      break;
    case  SA:
      factor0 = -0.01 / log( double( param1 ) / 100.0 );
      max_stuck = param2;
      search_type = RAND;
      break;
    case  SA_anti: 
      method = SA;
      anti_neighbour = true;
      factor0 = -0.01 / log( double( param1 ) / 100.0 );
      max_stuck = param2;
      search_type = RAND;
      break;
    case TA:
      factor0 = double( param1 ) / 1000.0;
      max_stuck = param2;
      search_type = RAND;
      break;
    case  TS:
      G_ExceptionList.lthrow("wrong call function init( int, int, int )");
      exit( 7 );
      break;
    default:
      G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
      exit( 7 );
  }
}

//**************************************************************************

Lisa_Iterator::Lisa_Iterator(int methodi, unsigned int param1, 
                             unsigned int param2, unsigned int param3 ){

  method = NOMETHOD;

  max_stuck=MAXNUMBER;
  anti_neighbour = false;
  abort_algorithm = false; 
  method = methodi;
  switch ( method ){
    case  II:
      G_ExceptionList.lthrow("wrong call of function init( II, int )");
      exit( 7 );
      break;
    case TS:
      tabu_lenght=param1;
      number_neighbours = param2;
      search_type = param3;
      break;
    case SA: 
    case  SA_anti:
    case  TA:
      G_ExceptionList.lthrow("wrong call function init( int, int, int )");
      exit( 7 );
      break;
    default:
      G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
      exit( 7 );
  }
}

//**************************************************************************

void Lisa_Iterator::iterate( Lisa_Neighbourhood *NB, int objective_type, 
                             long steps ){
  if( steps < 0 ){
    G_ExceptionList.lthrow("wrong parameter in iterate( "+ztos(int(steps))+" )");
    exit( 7 );
  }
  
  if( method == NOMETHOD ){
    G_ExceptionList.lthrow("no method specified in iterate( int )");
    exit( 7 );
  }
  
  double  t=0, t_old, t_first=0, t_end=0, decr=0;
  bool    accept=0;
  long    stuck_since, total_stuck;
  TIMETYP best_value, best_step, last_break_value;
  int     test, nn, non_move, max_non_move;
  long     steps_per_output_line = 1;
  
  if ( steps >= PROGRESS_INDICATOR_STEPS )
    steps_per_output_line = long(steps/PROGRESS_INDICATOR_STEPS);
  
  // getting some memory <-- removed this -marc-
  
  NB->put_orig_to_best();
  
  if ( (objective_type==IRREG1) || (objective_type==IRREG2) ) exit(7);
  
  NB->set_objective_type( objective_type );
  NB->set_objective( objective_type,ORIG_SOLUTION );
  best_value = NB->get_objective_value( ORIG_SOLUTION );
  last_break_value = best_value;
  best_step = MAXNUMBER;
  stuck_since = total_stuck = 0;
  max_non_move = steps/10;
  non_move = max_non_move;
  
  // preparing parmeters:
  switch ( method ){
    case II:
      NB->init_tabulist( 1 );
      break;
    case SA:
      t = double ( NB->get_objective_value(ORIG_SOLUTION) ) * factor0;
      t_end = double ( NB->get_objective_value(ORIG_SOLUTION) )
                       * (-0.001 / log( exp( -3 * log( 10. )) ) );
    
      decr = 1 / exp( log(t/t_end) / steps );
      t_old = t;
      t_first = t;
      break;
    case TA:
      t = double ( NB->get_objective_value(ORIG_SOLUTION) ) * factor0;
      decr = t / steps;
      t_old = t;
      t_first = t;
      break;
    case TS:
      NB->init_tabulist( tabu_lenght );
  }

  if ( method != TS ){ 
    for (  ; steps; steps-- ){// iteration loop for SA, II and TA:
      
      switch ( method ){
        case SA:
          t = t * decr;
          break;
        case TA:
          t = t - decr;
      }
      
      test = NB->prepare_move(search_type);
      NB->set_objective( objective_type, ORIG_SOLUTION );
      
      if(!(steps%steps_per_output_line)){
        cout << "steps= " << steps << " OBJECTIVE= " 
             << NB->get_objective_value(ORIG_SOLUTION) 
             << " best= " << best_value << endl;
      }
      
      if (test==OK){
        if ( NB->do_move() == OK ){
          
          NB->set_objective( objective_type, WORK_SOLUTION );
          
          // deceide whether to accept new solution:
          switch(method){
            
            case II:
              accept = (   NB->get_objective_value(WORK_SOLUTION)
                         < NB->get_objective_value(ORIG_SOLUTION) );
            
              if(accept) stuck_since=0;
              else if( ++stuck_since > abort_stuck ){
                G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters",
                                       Lisa_ExceptionList::WARNING);
                abort_algorithm = true;
              }
              break;
            
            case SA:
              accept = (   NB->get_objective_value(WORK_SOLUTION)
                         < NB->get_objective_value(ORIG_SOLUTION) );
            
              if( !accept ){
                accept = ( lisa_random( 0, 1000000, &seed ) <
                           1000000*exp(-(NB->get_objective_value(WORK_SOLUTION) - NB->get_objective_value(ORIG_SOLUTION))/t));
              }
            
              if (++total_stuck>=abort_stuck){
                G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters.",
                                       Lisa_ExceptionList::WARNING);
                abort_algorithm = true;
              }
            
              if (++stuck_since>=max_stuck){
              
                if ( anti_neighbour == true ){
                  NB->anti_neighbour();
                  NB->set_objective( objective_type,ORIG_SOLUTION );
                }
              
                last_break_value = 2*NB->get_objective_value(ORIG_SOLUTION);
                stuck_since = 0;
                t = t_first;
              
                if ( steps > 5 ) decr = 1/exp( log(t/t_end) / steps );
              }
              break;
            
            case TA:
              accept = (   NB->get_objective_value(WORK_SOLUTION)
                         - NB->get_objective_value(ORIG_SOLUTION) < t );
            
              if (++total_stuck>=abort_stuck){
                G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters.",Lisa_ExceptionList::WARNING);
                abort_algorithm = true;
              }
            
              if(++stuck_since>=max_stuck){ 
                last_break_value = NB->get_objective_value(ORIG_SOLUTION);
                stuck_since = 0;
                t = t_first;
                //t = t_old;
                decr = t / steps;
              }
          } // switch ( method )
          
          
          if ( accept ){
            NB->accept_solution();
            
            if (( NB->get_objective_value(WORK_SOLUTION) < best_value )){
              total_stuck = 0;
              best_value = NB->get_objective_value(ORIG_SOLUTION);
              NB->put_orig_to_best();
              
              if ( best_value <= abort_at_bound ){
                G_ExceptionList.lthrow("Iteration aborted early because objective reached lower bound. You might want to set other parameters.",Lisa_ExceptionList::WARNING);
                abort_algorithm = true;
              }
            }
            
            if (( NB->get_objective_value(WORK_SOLUTION) < last_break_value )){
              t_old = t;
              stuck_since = 0;
              last_break_value = NB->get_objective_value(ORIG_SOLUTION);
            }
          }
        } // if do_move == OK 
      } // if test == OK
      
      if ( (test==NO_NGHBOURS) || (abort_algorithm) ) steps = 1;
    } // for ...
  } else {
    
    // iteration loop for tabu search
    while( steps > 0 ){
      
      nn=number_neighbours;
      best_step = MAXNUMBER;
      NB->clean_tabu_param();
      
      while ((nn>0)&&(NB->prepare_move(search_type)==OK)){
        
        NB->set_objective( objective_type, ORIG_SOLUTION );
        
        if ( NB->use_tabulist() == OK ){
          nn--;
          if ( NB->do_move() == OK ){
            
            steps--;
            
            if (!(steps%steps_per_output_line)){
              cout << "steps= " << steps << " OBJECTIVE= " 
                   << NB->get_objective_value( ORIG_SOLUTION )
                   << " best= " << best_value << endl;
            }
            
            non_move = max_non_move;
            NB->set_objective( objective_type, WORK_SOLUTION );
            
            if ( NB->get_objective_value( WORK_SOLUTION ) < best_step ){
              NB->put_work_to_best_ngh();
              NB->store_tabu_param();
              best_step=NB->get_objective_value( WORK_SOLUTION );
            }
          }else{
            
            non_move--;
            
            if ( non_move == 0 ){
              non_move = max_non_move;
              steps--;
              
              if (!(steps%steps_per_output_line)){
                cout << "steps= " << steps << " OBJECTIVE= " 
                     << NB->get_objective_value( ORIG_SOLUTION )
                     << " best= " << best_value << endl;
              }
            }
          }
        }else{
          NB->clean_tabu_param();
          NB->set_tabulist();
        }
      }
      
      non_move--;
      
      if ( non_move == 0 ){
        non_move = max_non_move;
        steps--;
        
        if (!(steps%steps_per_output_line)){
          cout << "steps= " << steps << " OBJECTIVE= " 
               << NB->get_objective_value( ORIG_SOLUTION )
               << " best= " << best_value << endl;
        }
      }
      
      NB->set_objective( objective_type, ORIG_SOLUTION );
      
      if ( best_step < MAXNUMBER ) NB->accept_best_ngh();
      
      if ( best_step < best_value ){
        best_value = NB->get_objective_value( ORIG_SOLUTION );
        NB->put_orig_to_best();
        
        if ( best_value <= abort_at_bound ){
          G_ExceptionList.lthrow("Iteration aborted early because objective reached lower bound. You might want to set other parameters.",
          Lisa_ExceptionList::WARNING);
          abort_algorithm = true;
        }
      }
      
      NB->set_tabulist();
      
      if (abort_algorithm) steps = 0;
    } // while( steps...
  } // else ...
  
}

//**************************************************************************
//**************************************************************************
//**************************************************************************

Lisa_SimulatedAnnealing::Lisa_SimulatedAnnealing(Lisa_ControlParameters* CP){

  if(CP->defined("TSTART") == Lisa_ControlParameters::DOUBLE){
    Tstart = CP->get_double("TSTART");
  }else{
    G_ExceptionList.lthrow("'TSTART' undefined, using default.",Lisa_ExceptionList::WARNING);
    Tstart = 20;
  }
  
  if(CP->defined("TEND") == Lisa_ControlParameters::DOUBLE){
    Tend = CP->get_double("TEND");
  }else{
    G_ExceptionList.lthrow("'TEND' undefined, using default.",Lisa_ExceptionList::WARNING);
    Tend = 0.9;
  }
  
  if(Tstart <= Tend) Tstart = 2*Tend; 
  cout << "TSTART= " << Tstart << endl;
  cout << "TEND= " << Tend << endl;
  
  if(CP->defined("COOLSCHEME") == Lisa_ControlParameters::STRING){
    if(CP->get_string("COOLSCHEME") == "LUNDYANDMEES") cs = LUNDYANDMEES;
    else if(CP->get_string("COOLSCHEME") == "GEOMETRIC") cs = GEOMETRIC;
    else if(CP->get_string("COOLSCHEME") == "LINEAR") cs = LINEAR;
    else{
      G_ExceptionList.lthrow("'COOLSCHEME' undefined, using default.",Lisa_ExceptionList::WARNING);
      cs = LUNDYANDMEES;
    }
  }else{
    G_ExceptionList.lthrow("'TEND' undefined, using default.",Lisa_ExceptionList::WARNING);
    cs = LUNDYANDMEES;
  }
  
  if(CP->defined("COOLPARAM") == Lisa_ControlParameters::DOUBLE){
    cp = CP->get_double("COOLPARAM");
  }else{
    G_ExceptionList.lthrow("'COOLPARAM' undefined, using default.",Lisa_ExceptionList::WARNING);
    cp = 0.0005;
  }
  
  switch(cs){
    case GEOMETRIC:
      if(cp > 1) cp = 1;
      if(cp < 0) cp = 0;
      cout << "COOLSCHEME= GEOMETRIC" << endl;
      break;
    case LUNDYANDMEES:
      if(cp < 0) cp = 0;
      cout << "COOLSCHEME= LUNDYANDMEES" << endl;
      break;
    case LINEAR:
      if(cp < 0) cp = 0;
      cout << "COOLSCHEME= LINEAR" << endl;
      break;
  }
  
  cout << "COOLPARAM= " << cp << endl;
  
  if(CP->defined("EPOCH") == Lisa_ControlParameters::LONG){
    epochlength = CP->get_long("EPOCH");
  }else{
    G_ExceptionList.lthrow("'EPOCH' undefined, using default.",Lisa_ExceptionList::WARNING);
    epochlength = 100;
  }
  cout << "EPOCH= " << epochlength << endl;
  
  if(CP->defined("NUMB_NGHB") == Lisa_ControlParameters::LONG){
    numberneighbours = CP->get_long("NUMB_NGHB");
  }else{
    G_ExceptionList.lthrow("'NUMB_NGHB' undefined, using default.",Lisa_ExceptionList::WARNING);
    numberneighbours = 1;
  }
  cout << "NUMB_NGBH= " << numberneighbours << endl;
  
}

//**************************************************************************

void Lisa_SimulatedAnnealing::iterate(Lisa_Neighbourhood *ngbh, 
                                      int objective_type,
                                      long maxsteps){
  ngbh->put_orig_to_best();
  ngbh->set_objective_type(objective_type);
  ngbh->set_objective(objective_type,ORIG_SOLUTION);
  TIMETYP best_objective = ngbh->get_objective_value( ORIG_SOLUTION );
  
  //initialize progress meter
  cout << "OBJECTIVE= " << 2*best_objective << " Not a real objective, please ignore this line." << endl << endl;
  
  long steps = 0;
  long stuck = 0;
  long steps_per_output = (long) ((double) maxsteps / (double) PROGRESS_INDICATOR_STEPS);
  double T = Tstart;
  
  while(true){
    
    //do we need to abort ?
    if(steps > maxsteps) return;
    if(stuck > abort_stuck){
      G_ExceptionList.lthrow("Iteration stuck for too long and abortet. You might want to set other parameters.",
                             Lisa_ExceptionList::WARNING);
      return; 
    }
    if(best_objective <= abort_at_bound){
      G_ExceptionList.lthrow("Iteration aborted early because objective reached given lower bound. You might want to set other parameters.",
                             Lisa_ExceptionList::WARNING);
      return;
    }
    if(abort_algorithm){
      return;
    }
    
    //do we need to restart cooling ?
    if(T < Tend) T = Tstart;
    
    
    //generate some neighbours
    for(int i=0;i<numberneighbours;){
      
      TIMETYP best_nb_objective = 0;
      
      int test = ngbh->prepare_move(RAND);
      ngbh->set_objective(objective_type,ORIG_SOLUTION);
      
      if(test == NO_NGHBOURS){
        G_ExceptionList.lthrow("Iteration aborted because Neighbourhood did return NO_NGHBOURS.",
                               Lisa_ExceptionList::WARNING);
        return;      
      }else if(test == OK){
        if(ngbh->do_move() == OK){
          
          ngbh->set_objective(objective_type,WORK_SOLUTION);
          if(i==0 || best_nb_objective > ngbh->get_objective_value(WORK_SOLUTION)){
            ngbh->put_work_to_best_ngh();
            best_nb_objective = ngbh->get_objective_value(BEST_NGH_SOLUTION);
          }
          
          i++;
        }
      }
    }
        
    
    TIMETYP improvement = ngbh->get_objective_value(ORIG_SOLUTION)
                        - ngbh->get_objective_value(BEST_NGH_SOLUTION);
        
    bool accept = (improvement > 0);
    if(!accept) accept = ( lisa_random(0,1000000,&seed ) < 1000000*exp(improvement/T));
        
    if(accept){
      ngbh->accept_best_ngh();
            
      if(ngbh->get_objective_value(ORIG_SOLUTION) < best_objective){
        stuck = 0;
        best_objective = ngbh->get_objective_value(ORIG_SOLUTION);
        ngbh->put_orig_to_best(); 
      }
    }
    
    steps++;
        
    //reduce temperature
    if(!(steps%epochlength)){
      switch(cs){
        case GEOMETRIC:
          T = cp*T;
          break;
        case LUNDYANDMEES:
          T = T/(1+cp*T);
          break;
       case LINEAR:
          T = T - cp*Tend;
          break;
      }
    }

    //write some progress
    if (!(steps%steps_per_output)){
    cout << "OBJECTIVE= " << ngbh->get_objective_value(ORIG_SOLUTION)
         << "  best= " << best_objective 
         << "  ready= " << setw(3) << (int)  (100. * steps / maxsteps) 
         << "%  temp= " << setprecision(5) << T
          << endl;
    }

    stuck++;
  }
} 
  
//**************************************************************************

