/**
 * @author  Andreas Winkler
 * @version 2.3rc1
 */

#include "m1_shft.hpp"

using namespace std;

//**************************************************************************

shift_Neighbourhood::shift_Neighbourhood( Lisa_1Schedule *Plan, Lisa_1Problem *PPi )
	: API_Neighbourhood( Plan, PPi )
  {
   pos1 = 1;
   pos2 = 1;
   inc_dec = 1;
  }

//**************************************************************************

int shift_Neighbourhood::prepare_move( int typ )
  {
   int i;
   // determs a possible move
   // typ=ENUM : enumerativ
   // typ=RAND : randomly
   int test, number, nend;
   number = 0; nend = 20;
   int richtung;

   if ( typ == ENUM )
     {
       do
	 {
	   test = OK;
	   number++;
	   pos2 += inc_dec;
	   if ( (pos1==2) && (pos2==0) )
	     {
	       pos1 = 1;
	       pos2 = 1;
	       inc_dec = 1;
	       return NO_NGHBOURS;
	     }
	   if ( (pos2==PP->n+1) || (pos2==0) )
	     {
	       pos1 += inc_dec;
	       if ( (pos1==PP->n) || (pos1==1) )
		 inc_dec *= -1;
	       pos2 = pos1 + inc_dec;
	     }
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos1);
	   if ( pos1 < pos2 )
	     tabu_param[0][1] = 1;
	   else
	     tabu_param[0][1] = -1;
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   if ( pos1 < pos2 )
	     {
	       for ( i=pos1+1; i<=pos2; i++ )
		 if ( (*PP->prec)[ P[0]->get_sequ(pos1) ]
		      [ P[0]->get_sequ(i)    ] == 1 )
		   test = !OK;
	     }
	   else
	     {
	       for ( i=pos2; i<pos1; i++ )
		 if ( (*PP->prec)[ P[0]->get_sequ(i)    ]
		      [ P[0]->get_sequ(pos1) ] == 1 )
		   test = !OK;
	     }
	 }
       while ( (test==!OK) && (number < nend) );
       if ( test==OK )
	 return OK;
       else
	 {
	   cout << "NoNghb  ";
	   return NO_NGHBOURS;
	 }
     }

   if ( typ == RAND )
     {
       do
	 {
	   test = OK;
	   number++;
	   pos1 = lisa_random( 1, PP->n, &seed );
	   richtung = lisa_random( 0, 1, &seed );
	   if ( richtung==0 )
	     {
	       if ( pos1==1 )
		 {
		   pos1 = 2;
		   pos2 = 1;
		 }
	       else
		 pos2 = lisa_random( 1, pos1-1, &seed );
	     }
	   else
	     {
	       if ( pos1==PP->n )
		 {
		   pos1 = PP->n-1;
		   pos2 = PP->n;
		 }
	       else
		 pos2 = lisa_random( pos1+1, PP->n, &seed );
	     }
	   // is this move setting to be tabu ? :
	   tabu_param[0][0] = P[0]->get_sequ(pos1);
	   if ( pos1 < pos2 )
	     tabu_param[0][1] = 1;
	   else
	     tabu_param[0][1] = -1;
	   tabu_param[0][2] = 0;
	   tabu_param[0][3] = 0;
	   if ( pos1 < pos2 )
	     {
	       for ( i=pos1+1; i<=pos2; i++ )
		 if ( (*PP->prec)[ P[0]->get_sequ(pos1) ]
		      [ P[0]->get_sequ(i)    ] == 1 )
		   test = !OK;
	     }
	   else
	     {
	       for ( i=pos2; i<pos1; i++ )
		 if ( (*PP->prec)[ P[0]->get_sequ(i)    ]
		      [ P[0]->get_sequ(pos1) ] == 1 )
		   test = !OK;
	     }
	 }
       while ( (test==!OK) && (number < nend) );
       if ( test==OK )
	 return OK;
       else
	 return NO_NGHBOURS;
     }
   G_ExceptionList.lthrow("wrong parameter in prepare_move("+ztos(typ)+")");
   return NO_NGHBOURS;
  }

//**************************************************************************

int shift_Neighbourhood::do_move()
  {
    *P[1]=*P[0];
    // the following means, that in next time the re-move is set 
    // to be tabu
    tabu_param[0][1] = (-1) * tabu_param[0][1];
    return P[1]->shift( pos1, pos2 );
  }

//**************************************************************************

