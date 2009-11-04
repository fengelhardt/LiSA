/**
 * @author  Thomas Tautenhahn
 * @version 3.0pre1
 */

#include "../../main/global.hpp"

#include "nb_fp.hpp"

using namespace std;

//**************************************************************************

FP_Neighbourhood::FP_Neighbourhood(int ni, int sigmai)
 {
   n=ni; sigma=sigmai;
   fixed=1;
   for (int h=0; h<n/2; h++) fixed*=3;
   seed=1234567;
   solution= new FP_Set(n, sigma);
   best_solution= new FP_Set(n, sigma);
   modification= new FP_Vector(n);
   best_modification= new FP_Vector(n);
 }  

//**************************************************************************

TIMETYP FP_Neighbourhood::get_objective_value( int i )
 {
   switch (i) {
                case ORIG_SOLUTION: return solution->get_slack();
                case WORK_SOLUTION: return work_value;
                case BEST_SOLUTION: return best_solution->get_slack();
                case BEST_NGH_SOLUTION: return best_step_value;
                default: cerr << "\nERROR: wrong solution specified." << endl;
                         return 0;
              }
 }   

//**************************************************************************

int FP_Neighbourhood::accept_solution()
 {
   modification=solution->replace_vector(repl_i, modification);
   return 0;
 }

//**************************************************************************
  
int FP_Neighbourhood::put_orig_to_best()
 {
   (*best_solution)=(*solution);
   return 0;
 }

//**************************************************************************

int FP_Neighbourhood::accept_best_ngh()
 {
   best_modification=solution->replace_vector(best_repl_i, best_modification);
   return 0;
 }

//**************************************************************************

int FP_Neighbourhood::put_work_to_best_ngh()
 {
   (*best_modification)=(*modification);
   best_repl_i=repl_i;
   best_chpos=chpos;
   return 0;
 }

//**************************************************************************
   
int   FP_Neighbourhood::prepare_move( int )
 {
   repl_i=lisa_random(0, sigma-1, &seed);
   chpos=lisa_random(0, n-1, &seed);
   elem=(solution->get_vector(repl_i)).get_element(chpos);
   if (lisa_random(0, 1, &seed)) elem++; else elem--; 
   if (elem==3) elem=0;
   if (elem==-1) elem=2;
   return OK;
 }

//**************************************************************************

int FP_Neighbourhood::do_move( )
 {
   int e;
   (*modification)=solution->get_vector(repl_i);
   modification->set_element(chpos,elem);
   work_value=solution->get_slack();
   for (e=0; e<=2*n; e++) 
      {
        if (solution->get_cover(modification->get_neighbour(e))==0)
          work_value--;
        if (solution->get_cover(
                      (solution->get_vector(repl_i)).get_neighbour(e))==1)
          work_value++;
      }
   if (solution->get_cover(modification->get_neighbour(2*chpos))==1) 
      work_value--;
   if (solution->get_cover(modification->get_neighbour(2*chpos+1))==1) 
      work_value--;
   if (solution->get_cover(modification->get_neighbour(2*n))==1) 
      work_value--;   
   return OK;
 }	  

//**************************************************************************

int FP_Neighbourhood::anti_neighbour()
 {
   return OK;
 }

//**************************************************************************

void FP_Neighbourhood::set_objective_type( int o )
 {
   objective_type = o;
 }

//**************************************************************************

int FP_Neighbourhood::init_tabulist( unsigned int i)
 {
   delete tabu;
   tabu=new Lisa_Tabu(i);
   return 0;
 }

//**************************************************************************

int FP_Neighbourhood:: use_tabulist()
 { 
   return tabu->use(repl_i, chpos, elem,0);
 }

//**************************************************************************

int    FP_Neighbourhood::set_tabulist()
 {
   return tabu->set(best_repl_i, best_chpos, old_elem,0);
 }

//**************************************************************************
	
void    FP_Neighbourhood::store_tabu_param()
 {
   old_elem=(solution->get_vector(repl_i)).get_element(chpos);
 }

//**************************************************************************
       
void   FP_Neighbourhood:: clean_tabu_param() 
 {
   old_elem=3;
 }

//**************************************************************************

FP_Neighbourhood::~FP_Neighbourhood()
 { 
   delete solution;
   delete best_solution;
   delete modification;
   delete best_modification;
 }

//**************************************************************************

