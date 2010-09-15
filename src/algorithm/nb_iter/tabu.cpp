/**
 * @author  Andreas Winkler
 * @version 3.0
 */
 
#include <stdlib.h>

#include "tabu.hpp"

//**************************************************************************

Lisa_Tabu::Lisa_Tabu( unsigned int ilength )
  {
   length = ilength;
   tabu=0;
   if ( !(tabu_1 = new Lisa_Vector<int>(length) ) )
     {
       G_ExceptionList.lthrow("out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !(tabu_2 = new Lisa_Vector<int>(length) ) )
     {
       G_ExceptionList.lthrow("out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !(tabu_3 = new Lisa_Vector<int>(length) ) )
     {
       G_ExceptionList.lthrow("out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
       exit( 7 );
     }
   if ( !(tabu_4 = new Lisa_Vector<int>(length) ) )
     {
      G_ExceptionList.lthrow("out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
      exit( 7 );
     }
   pos = 0;
   tabu_1->fill(0);
   tabu_2->fill(0);
   tabu_3->fill(0);
   tabu_4->fill(0);
  }

//**************************************************************************

Lisa_Tabu::Lisa_Tabu( unsigned int ilength, unsigned int ivector_length )
  {
    length = ilength;
    vec_length = ivector_length;
    tabu_1=tabu_2=tabu_3=tabu_4=0;
    if ( !(tabu = new Lisa_Matrix<int>( length, vec_length ) ) )
      {
	G_ExceptionList.lthrow("out of memory.",Lisa_ExceptionList::NO_MORE_MEMORY);
	exit( 7 );
      }
    pos = 0;
    tabu->fill(0);
  }

//**************************************************************************

Lisa_Tabu::~Lisa_Tabu()
  {
    if ( tabu_1 != NULL )
      delete tabu_1;
    if ( tabu_2 != NULL )
      delete tabu_2;
    if ( tabu_3 != NULL )
      delete tabu_3;
    if ( tabu_4 != NULL )
      delete tabu_4;
    if ( tabu != NULL )
      delete tabu;
  }

//**************************************************************************

int Lisa_Tabu::set( int a, int b, int c, int d )
  {
    (*tabu_1)[pos] = a;
    (*tabu_2)[pos] = b;
    (*tabu_3)[pos] = c;
    (*tabu_4)[pos] = d;
    if ( ++pos >= int(length) )
      pos=0;
    return OK;
  }

//**************************************************************************

int Lisa_Tabu::set_vector( Lisa_Vector<int> *entry )
  {
    (*tabu)[pos] = *entry;
    if ( ++pos >= int(length) )
      pos = 0;
    return OK;
  }

//**************************************************************************

int Lisa_Tabu::use( int a, int b, int c, int d )
  {
   int i;
   i=length;
   for ( i=length-1; i>=0 ; i-- )
     if ( ( (*tabu_1)[i]==a ) && ( (*tabu_2)[i]==b ) && 
	  ( (*tabu_3)[i]==c ) && ( (*tabu_4)[i]==d ) )
       return !OK;
   return OK;
  }

//**************************************************************************

int Lisa_Tabu::use_vector( Lisa_Vector<int> *entry )
  {
    int i;
    i = length;
    for ( i=length-1; i>=0 ; i-- )
      if ( (*tabu)[i] == *entry )
	return !OK;
    return OK;
  }

//**************************************************************************

