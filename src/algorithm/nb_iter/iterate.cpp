/**
 * @author  Andreas Winkler
 * @version 2.3final
 */

#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <signal.h>

#include <iostream>         
#include <fstream>

#include "../../main/global.hpp"
#include "../../misc/int2str.hpp"

#include "iterate.hpp"

using namespace std;

//**************************************************************************

bool abort_algorithm;

//**************************************************************************

void set_abort(int i) 
 {  
   abort_algorithm=TRUE;
   cout << "\nSignal " << i << " received, writing results \n";
 }

//**************************************************************************

// WARNING! The following two functions have to be replaced for Windows!
void Lisa_Iterator::run_start()
 {
   signal(SIGINT, &set_abort);
 }

//**************************************************************************

void Lisa_Iterator::run_stop()
 {
   signal(SIGINT, SIG_DFL);
 }

//**************************************************************************

Lisa_Iterator::Lisa_Iterator()
  {
    time_t zeit;
    //seed = 12345671L;
    zeit = time(0);
    seed = long( zeit );
    cout<<"\nseed = "<<zeit<<"\n";
    method = NOMETHOD;
    abort_algorithm = FALSE;
    abort_at_bound = -MAXNUMBER;
    abort_stuck = MAXNUMBER;
    max_stuck=MAXNUMBER;
    anti_neighbour = FALSE;
  }

//**************************************************************************

void Lisa_Iterator::init( int methodi, unsigned int param1 )
  {
    method = methodi;
    switch ( method )
      {
        case  II: if ( !((int(param1)==ENUM) || (int(param1)==RAND)) )
		    {
		      G_ExceptionList.lthrow("wrong parameter in init( II, "+ztos(int(param1))); 
		      exit( 7 );
		    }
	          search_type = param1;
		  break;
	case  SA: 
        case  SA_anti:
	case  TA: 
        case  TS: G_ExceptionList.lthrow("wrong call function init( int, int, int )");
		  exit( 7 );
		  break;
        default:  G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
                  exit( 7 );
      }
  }

//**************************************************************************

void Lisa_Iterator::init( int methodi, unsigned int param1, unsigned int param2 )
  {
    method = methodi;
    switch ( method )
      {
        case  II: G_ExceptionList.lthrow("wrong call of function init( II, int )");
		  exit( 7 );
		  break;
        case  SA: factor0 = -0.01 / log( double( param1 ) / 100.0 );
		  max_stuck = param2;
		  search_type = RAND;
		  break;
        case  SA_anti: 
	          method = SA;
		  anti_neighbour = TRUE;
	          factor0 = -0.01 / log( double( param1 ) / 100.0 );
		  max_stuck = param2;
		  search_type = RAND;
		  break;
	case  TA: factor0 = double( param1 ) / 1000.0;
		  max_stuck = param2;
		  search_type = RAND;
		  break;
        case  TS: G_ExceptionList.lthrow("wrong call function init( int, int, int )");
		  exit( 7 );
		  break;
        default:  G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
                  exit( 7 );
      }
  }

//**************************************************************************

void Lisa_Iterator::init( int methodi, unsigned int param1, 
			  unsigned int param2, unsigned int param3 )
  {
    method = methodi;
    switch ( method )
      {
	case  II: G_ExceptionList.lthrow("wrong call of function init( II, int )");
		  exit( 7 );
		  break;
        case  TS: tabu_lenght=param1;
                  number_neighbours = param2;
		  search_type = param3;
		  break;
	case  SA: 
        case  SA_anti:
 	case  TA: G_ExceptionList.lthrow("wrong call function init( int, int, int )");
		  exit( 7 );
		  break;
        default:  G_ExceptionList.lthrow("wrong method specified in init("+ztos(method)+",int,int)");
                  exit( 7 );
      }
  }

//**************************************************************************

void Lisa_Iterator::set_abort_at_stuck( int abort )
  {
    abort_stuck = abort;
  }

//**************************************************************************

void Lisa_Iterator::set_abort_at_bound( TIMETYP abort )
  {
    abort_at_bound = abort;
  }

//**************************************************************************

void Lisa_Iterator::iterate( Lisa_Neighbourhood *NB, int objective_type, 
			     long steps )
  {
  
    if ( steps < 0 )
      {
	G_ExceptionList.lthrow("wrong parameter in iterate( "+ztos(int(steps))+" )");
	exit( 7 );
      }
    if ( method == NOMETHOD )
      {
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



    if ( (objective_type==IRREG1) || (objective_type==IRREG2) )
      exit(7);

    NB->set_objective_type( objective_type );
    NB->set_objective( objective_type,ORIG_SOLUTION );
    best_value = NB->get_objective_value( ORIG_SOLUTION );
    last_break_value = best_value;
    best_step = MAXNUMBER;
    stuck_since = total_stuck = 0;
    max_non_move = steps/10;
    non_move = max_non_move;

    // preparing parmeters:
    switch ( method )
      {
        case II: NB->init_tabulist( 1 );
	         break;
	case SA: t = double ( NB->get_objective_value(ORIG_SOLUTION) )
		      * factor0;
	         t_end = double ( NB->get_objective_value(ORIG_SOLUTION) )
		   * (-0.001 / log( exp( -3 * log( 10. )) ) );
		 decr = 1 / exp( log(t/t_end) / steps );
		 t_old = t;
		 t_first = t;
		 break;
	case TA: t = double ( NB->get_objective_value(ORIG_SOLUTION) ) 
		      * factor0;
		 decr = t / steps;
		 t_old = t;
		 t_first = t;
		 break;
        case TS: NB->init_tabulist( tabu_lenght );
      }
    run_start();

    if ( method != TS )
      // iteration loop for SA, II and TA:
      for (  ; steps; steps-- )
	{
	  switch ( method )
	    {
	     case SA: t = t * decr;
	              break;
	     case TA: t = t - decr;
	    }
 	  test = NB->prepare_move(search_type);
	  NB->set_objective( objective_type, ORIG_SOLUTION );
	  if (!(steps%steps_per_output_line))
	    cout << "steps= " << steps << " OBJECTIVE= " 
	  	 << NB->get_objective_value(ORIG_SOLUTION) 
	  	 << " best= " << best_value << endl;
	  if (test==OK)
	   {
	    if ( NB->do_move() == OK )
	     {
	      NB->set_objective( objective_type, WORK_SOLUTION );
	      // deceide whether to accept new solution:
              switch ( method )
		{
		 case II: accept = ( NB->get_objective_value(WORK_SOLUTION)
			           < NB->get_objective_value(ORIG_SOLUTION) );
		          if (accept)
                             stuck_since=0;
                          else
			    if ( ++ stuck_since > abort_stuck )
                              {
                                G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters",
                                                       Lisa_ExceptionList::WARNING);
                                abort_algorithm = TRUE;
                              }
  			  break;
		 case SA: accept = ( NB->get_objective_value(WORK_SOLUTION)
				   < NB->get_objective_value(ORIG_SOLUTION) );
			  if ( !accept )
			    {
			      accept = ( lisa_random( 0, 1000000, &seed ) <
			  1000000*exp(-(NB->get_objective_value(WORK_SOLUTION)
				- NB->get_objective_value(ORIG_SOLUTION))/t));
			    }
			  if (++total_stuck>=abort_stuck)
			    {
			      G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters.",
                                   Lisa_ExceptionList::WARNING);
			      abort_algorithm = TRUE;
			    }
			  if (++stuck_since>=max_stuck)  
			    {
			      if ( anti_neighbour == TRUE )
				{
				  NB->anti_neighbour();
				  NB->set_objective( objective_type, 
						     ORIG_SOLUTION );
				}
			      last_break_value = 
				2*NB->get_objective_value(ORIG_SOLUTION);
			      stuck_since = 0;
			      t = t_first;
			      //t = t_old;
			      if ( steps > 5 )
				decr = 1/exp( log(t/t_end) / steps );
			    }
			  break;
		 case TA: accept = ( NB->get_objective_value(WORK_SOLUTION)
			       - NB->get_objective_value(ORIG_SOLUTION) < t );
			  if (++total_stuck>=abort_stuck)
			    {
			      G_ExceptionList.lthrow("Iteration aborted early because algorithm is stuck for too long. You might want to set other parameters.",Lisa_ExceptionList::WARNING);
			      abort_algorithm = TRUE;
			    }
			  if (++stuck_since>=max_stuck)  
			    {
			      last_break_value = 
				NB->get_objective_value(ORIG_SOLUTION);
			      stuck_since = 0;
			      t = t_first;
			      //t = t_old;
			      decr = t / steps;
			    }
 		} // switch ( method )
	      if ( accept )
		{
		 NB->accept_solution();
		 if (( NB->get_objective_value(WORK_SOLUTION) < best_value ))
		   {
		     total_stuck = 0;
		     best_value = NB->get_objective_value(ORIG_SOLUTION);
		     NB->put_orig_to_best();
		     if ( best_value <= abort_at_bound )
		       {
			 G_ExceptionList.lthrow("Iteration aborted early because objective reached lower bound. You might want to set other parameters.",Lisa_ExceptionList::WARNING);
			 abort_algorithm = TRUE;
		       }
		   }
		 if (( NB->get_objective_value(WORK_SOLUTION) 
		         < last_break_value ))
		   {
		     t_old = t;
		     stuck_since = 0;
		     last_break_value = NB->get_objective_value(ORIG_SOLUTION);
		   }
		}
	     } // if do_move ...
	  } // if prepare_move ...
	 if ( (test==NO_NGHBOURS) || (abort_algorithm) )
	   steps = 1;
	} // for ...
    else
      {
       // iteration loop for tabu search
       while( steps > 0 )
	 {
	   nn=number_neighbours;
	   best_step = MAXNUMBER;
	   NB->clean_tabu_param();
	   while ((nn>0)&&(NB->prepare_move(search_type)==OK))
	     {
	       NB->set_objective( objective_type, ORIG_SOLUTION );
	       if ( NB->use_tabulist() == OK )
		 {
		   nn--;
		   if ( NB->do_move() == OK )
		     {
		       steps--; 
		       if (!(steps%steps_per_output_line))
		       	 cout << "steps= " << steps << " OBJECTIVE= " 
		              << NB->get_objective_value( ORIG_SOLUTION )
		              << " best= " << best_value << endl;
		       non_move = max_non_move;
		       NB->set_objective( objective_type, WORK_SOLUTION );
		       if ( NB->get_objective_value( WORK_SOLUTION ) 
			                                     < best_step )
			 {
			   NB->put_work_to_best_ngh();
			   NB->store_tabu_param();
			   best_step=NB->get_objective_value( WORK_SOLUTION );
			 }
		     }
		   else
		     {
		       non_move--;
		       if ( non_move == 0 )
			 {
			   non_move = max_non_move;
			   steps--;
			   if (!(steps%steps_per_output_line))
			     cout << "steps= " << steps << " OBJECTIVE= " 
				  << NB->get_objective_value( ORIG_SOLUTION )
				  << " best= " << best_value << endl;
			 }
		     }
		 }
	       else
		 {
		   NB->clean_tabu_param();
		   NB->set_tabulist();
		 }
	     }
	   non_move--;
	   if ( non_move == 0 )
	     {
	       non_move = max_non_move;
	       steps--;
	       if (!(steps%steps_per_output_line))
		 cout << "steps= " << steps << " OBJECTIVE= " 
		      << NB->get_objective_value( ORIG_SOLUTION )
		      << " best= " << best_value << endl;
	     }
	   NB->set_objective( objective_type, ORIG_SOLUTION );
	   if ( best_step < MAXNUMBER )
	     NB->accept_best_ngh();
	   if ( best_step < best_value )
	     {
	       best_value = NB->get_objective_value( ORIG_SOLUTION );
	       NB->put_orig_to_best();
	       if ( best_value <= abort_at_bound )
		 {
		   G_ExceptionList.lthrow("Iteration aborted early because objective reached lower bound. You might want to set other parameters.",
                              Lisa_ExceptionList::WARNING);
		   abort_algorithm = TRUE;
		 }
	     }
	   NB->set_tabulist();
	   if (abort_algorithm)
	     steps = 0;
	 } // while( steps...
      } // else ...
    run_stop();
  }

//**************************************************************************

