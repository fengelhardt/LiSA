/*
 * ******************** osp_crft.cpp ******************************
 * 
 * description:      open shop critical-SHIFT neighborhood
 * 
 * owner:            Andreas Winkler
 *
 * date:             21.10.1998
 *
 */

#include "osp_crsh.hpp"

OSHOP_cr_shift_Ngbh::OSHOP_cr_shift_Ngbh(Lisa_OsSchedule *Plan,Lisa_OsProblem *PPi)
                                    : OSHOP_API_Ngbh( Plan, PPi )
  {
    cr_list_j = new Lisa_Vector<int>(PP->n*PP->m);
    cr_list_m = new Lisa_Vector<int>(PP->n*PP->m);
    count = 0;
    JOrd = new Lisa_Vector<int>(PP->n+1);   // JOrd[i] is the job on position i on the machine
    MOrd = new Lisa_Vector<int>(PP->m+1);   // MOrd[i] is ...
  }

OSHOP_cr_shift_Ngbh::~OSHOP_cr_shift_Ngbh()
  {
    delete cr_list_j;
    delete cr_list_m;
    delete JOrd;
    delete MOrd;
  }

int OSHOP_cr_shift_Ngbh::prepare_move( int typ )
  {
    // determs a possible move
    // typ=ENUM : enumerativ
    // typ=RAND : randomly
    int pos_job1=0, pos_job2, pos_mach1=0, pos_mach2;
    int test, succ, i, j, numb;
    test = OK;
    TIMETYP Cmax;

   if ( typ == RAND )
     {

       // determs the critical operations
       P[0]->SetValue(CMAX);
       Cmax = P[0]->GetValue();
       count = 0;
       for ( i=1; i<(PP->n+1); i++ )
	 for ( j=1; j<(PP->m+1); j++ )
	   if ( (*PP->sij)[i][j] != 0 )
	     {
	       if ( P[0]->GetHead(i,j) + (*PP->time)[i][j] 
		    + P[0]->GetTail(i,j) == Cmax )
		 {
		   (*cr_list_j)[count] = i;
		   (*cr_list_m)[count] = j;
		   count++;
		 }
	     }
       count--;

       // determs randomly an element
       numb = lisa_random( 0, count, &seed );
       machine1 = (*cr_list_m)[numb];
       job1     = (*cr_list_j)[numb];

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

       // determs randomly a second position in Ord
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
       G_ExceptionList.lthrow("enumerativ method not implemented in osp_crsh.cpp"); 
       exit(7);
      }

   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return !OK;
 }

int OSHOP_cr_shift_Ngbh::do_move()
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

    





