/*
 * ******************** osp_shft.cpp ******************************
 * 
 * description:      open shop SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             21.10.1998
 *
 */

#include "osp_shft.hpp"

OSHOP_shift_Ngbh::OSHOP_shift_Ngbh( Lisa_OsSchedule *Plan, Lisa_OsProblem *PPi )
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    witch_swap = JO;
    job1 = 1; job2 = 1;
    machine1 = 1; machine2 = 0;
    JOrd = new Lisa_Vector<int>( PP->n+1 );   // JOrd[i] is the job on position i on the machine
    MOrd = new Lisa_Vector<int>( PP->m+1 );   // MOrd[i] is ...
    JOpos = new Lisa_Vector<int>( PP->n+1 );  // JOpos[i] is the position of job i on the machine
    MOpos = new Lisa_Vector<int>( PP->m+1 );  // MOpos[i] is ...
  };

OSHOP_shift_Ngbh::~OSHOP_shift_Ngbh()
  {
    delete JOrd;
    delete MOrd;
    delete JOpos;
    delete MOpos;
  };

int OSHOP_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int pos_job1, pos_job2, pos_mach1, pos_mach2;
    int test, succ, i;
    test = OK;

   if ( typ == RAND )
     {

       // determs randomly a machine and a job
       do
	 {
	   machine1 = lisa_random( 1, PP->m, &seed );
	   job1     = lisa_random( 1, PP->n, &seed );
	 }
       while ( (*PP->sij)[job1][machine2]==0 ); 
       // determs the order on the machine1
       succ = 0; i = 0;
       do
	 {
	   succ = P[0]->GetJOsucc( succ, machine1 );
	   (*JOrd)[i] = succ;
	   if ( succ == job1 )
	     pos_job1 = i+1;
	   i++;
	 }
       while (succ != 0);

       // determs randomly a second position in JOrd
       do
	 {
	   do
	     pos_job2 = lisa_random( 1, i-1, &seed );
	   while ( pos_job1 == pos_job2 );
	   job2 = (*JOrd)[pos_job2-1];
	 }
       while (0); // here could stay precedence constraints

       // determs the order on the job1
       succ = 0; i = 0;
       do
	 {
	   succ = P[0]->GetMOsucc( job1, succ );
	   (*MOrd)[i] = succ;
	   if ( succ == machine1 )
	     pos_mach1 = i+1;
	   i++;
	 }
       while (succ != 0);	

       // determs randomly a second position in MOrd
       do
	 {
	   do
	     pos_mach2 = lisa_random( 1, i-1, &seed );
	   while ( pos_mach1 == pos_mach2 );
	   machine2 = (*MOrd)[pos_mach2-1];
	 }
       while (0); // here could stay prcedence constraints

       if (( (*PP->sij)[job1][machine2]==0 ) 
	   || ( (*PP->sij)[job2][machine1]==0 ))
	 return !OK;
       // is this move setting to be tabu ? :
       tabu_param[0][0] = job1;
       tabu_param[0][1] = machine1;
       if (pos_job1<pos_job2) 
	 tabu_param[0][2] = 1;
       else
	 tabu_param[0][2] = -1;
       if (pos_mach1<pos_mach2) 
	 tabu_param[0][3] = 1;
       else
	 tabu_param[0][3] = -1;
       return OK;
     }


   if ( typ == ENUM )
     {
       int return_OK = !OK;
       while ( return_OK == !OK )
	 {
	   machine2 += 1;
	   if ( machine2 == machine1 )
	     machine2 += 1;
	   if ( machine2 >= PP->m+1 )
	     {
	       job2 += 1; machine2 = 1;
	       if ( job2 == job1 )
		 job2 += 1;
	       if ( job2 >= PP->n+1 )
		 {
		   job1 += 1; job2 = 1;
		   if ( job1 >= PP->n+1 )
		     {
		       machine1 += 1; job1 = 1;
		       if ( machine1 >= PP->m+1 )
			 {
			   machine1 = 1; machine2 = 2;
			 }
		     }
		 }
	     }
	   if (( (*PP->sij)[job1][machine1]==0 ) 
	       || ( (*PP->sij)[job2][machine1]==0 )
	       || ( (*PP->sij)[job1][machine2]==0 ))
	     continue;	   

	   // determs the order on machine1:
	   succ = 0; i = 0;
	   do
	     {
	       succ = P[0]->GetJOsucc( succ, machine1 );
	       (*JOpos)[succ] = i + 1;
	       i++;
	     }
	   while (succ != 0);
	   pos_job1 = (*JOpos)[job1];
	   pos_job2 = (*JOpos)[job2];

	   // determs the order on job1:
	   succ = 0; i = 0;
	   do
	     {
	       succ = P[0]->GetMOsucc( job1, succ );
	       (*MOpos)[succ] = i + 1;
	       i++;
	     }
	   while (succ != 0);
	   pos_mach1 = (*MOpos)[machine1];
	   pos_mach2 = (*MOpos)[machine2];

	   //cout<<" ("<<job1<<","<<machine1<<")-("
	   //    <<job2<<","<<machine2<<")";

	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = job1;
	   tabu_param[0][1] = machine1;
	   if (pos_job1<pos_job2) 
	     tabu_param[0][2] = 1;
	   else
	     tabu_param[0][2] = -1;
	   if (pos_mach1<pos_mach2) 
	     tabu_param[0][3] = 1;
	   else
	     tabu_param[0][3] = -1;

	   if ( use_tabulist() != OK )
	     continue;
	   return_OK = OK;
	 }
       return OK;
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+string(typ)+")");
   return !OK;
 };

int OSHOP_shift_Ngbh::do_move()
 {
   short predJ1;
   short predM1;
   *P[1]=*P[0];

   if (tabu_param[0][2]==1)
     predJ1 = job2;
   else
     predJ1 = P[1]->GetJOpred(job2,machine1);

   if (tabu_param[0][3]==1)
     predM1 = machine2; 
   else
     predM1 = P[1]->GetMOpred(job1,machine2);

   P[1]->exclude( job1, machine1 );
   if ( P[1]->insert(job1,machine1,predJ1,predM1) == CYCLE )
     return !OK;
   // the following means, that in next time the re-move is set 
   // to be tabu
   tabu_param[0][2] *= -1;
   tabu_param[0][3] *= -1;
   return OK;
 }

    





