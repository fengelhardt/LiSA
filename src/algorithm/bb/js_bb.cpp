/*
 * ************** js_bb.h *******************************
 * 
 * branch and bound implementation for job shop
 *
 * @author Thomas Tautenhahn
 * @version 2.3pre3
 *
 * 19.9.1998
*/

#include "../../main/global.hpp"
#include "../../basics/order.hpp"

#include "js_bb.hpp"

// start the algorithm
void JS_BB::run(Lisa_JsProblem * Pi, int zfn, 
                Lisa_List<Lisa_Matrix<int>* > * results)
    { 
      // copy parameters:
      my_list=results;
      zfn_type=zfn;
      P=Pi;
            
      // set up insertion order (big operations first):
      int i, j; 
      long seed=123456L;
      order = new Lisa_Order(P->n, P->m);
      for (i=0; i<P->n ; i++ ) 
	for (j=0; j<P->m ; j++ )
	  switch (ins_order_type)
	    {
	    case LPT_ORDER: 
	      order->read_one_key(i, j, - (*(P->time))[i+1][j+1]);
	      break;
	    default:  
	      order->read_one_key(i, j, lisa_random(1,1000,&seed));
            }
      order->sort();
      // put non-existing operations to the end:
      number_ops=0;
      for (i=0; i<P->n ; i++ ) 
	for (j=0; j<P->m ; j++ )
	  if ((*(P->sij))[i+1][j+1])
	    {
	      order->read_one_key(i, j, 0);
	      number_ops++;
	    }
	  else 
	    order->read_one_key(i, j, 1);
      order->sort();  

      // start algorithm:
      Schedule =new Lisa_JsSchedule(P);
      run_start();
      Schedule->insert(order->row(0)+1, order->col(0)+1, SOURCE); 
      _run(1);
      run_stop();
      delete Schedule;
      delete order;
    }

void JS_BB::_run(int op_nr)
  {
    Lisa_JsSchedule *Schedule2;
    Lisa_Matrix<int> * erg;

    int pos, i, j;

    // get backup copy of Schedule:
    Schedule2 = new Lisa_JsSchedule(P);
    
    // find positions: 
    i=order->row(op_nr)+1; j=order->col(op_nr)+1; 
    pos=SOURCE;   
    do 
     {
       (*Schedule2)=(*Schedule);
       count_steps();
       if (Schedule->insert(i,j,pos)>0)
         {
           steps_ok++;
           Schedule->SetValue(zfn_type);
           if ( (Schedule->GetValue() < upper_bound) || 
               ((Schedule->GetValue() == upper_bound) && 
                 (my_list->length()<output_bound)
               )
              )
             { 
               if (op_nr+1 < (number_ops)) 
                 { 
                   if (!abort_algorithm) _run(op_nr+1);
                 }
	       else 
                 {                       
		   if (upper_bound>Schedule->GetValue())
                     {
                       upper_bound=Schedule->GetValue();
                       my_list->clear();
		     } 
                   erg=new Lisa_Matrix<int>(P->n, P->m);
                   Schedule->write_LR(erg);
                   my_list->append(erg); 
                   if ((Schedule->GetValue()<=given_lower_bound) &&
                       (my_list->length()>=output_bound))
		     abort_algorithm=TRUE;
                 }     // if schedule complete
	     }        // if value <= upper_bound       
         }             // if insertion ok
       (*Schedule)=(*Schedule2);
       pos=Schedule->GetJOsucc(pos,j);
     } 
    while (pos!=SINK);     
    delete Schedule2;
 }














