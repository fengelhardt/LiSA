/*
 * ******************** fp_vector.cpp ******************************
 * 
 * description:  vector for football pool problem
 * 
 * @author        Thomas Tautenhahn
 * @version 2.3pre3
 *
 * date:         17. 11. 1998
 *
 */

#include "fp_vect.hpp"

// no return type specified:
fp_vector::fp_vector(int x)
 { 
   int i, j, rv;
   n=x;
   v=new Lisa_Vector<int>(n);
   v->fill(0);
   number=0;
   neighbour=new Lisa_Vector<int>(2*n+1);
   for (i=0, rv=1; i<n; i++, rv*=3)
      for (j=0; j<2 ; j++)
         (*neighbour)[2*i+j]=rv*(j+1);
   (*neighbour)[2*n]=0;
 }

const fp_vector& fp_vector::operator=(fp_vector& other)
 { 
   n=other.n;
   (*v)=(*(other.v));
   number=other.number;
   (*neighbour)=(*(other.neighbour));
   return *this;
 }

void fp_vector::set_element(int pos, int el)
 {
   int i, rv;
   rv=1; i=0;
   while (i<pos) { i++; rv*=3; }
   switch ((*v)[pos])
     { 
       case 0: (*neighbour)[2*pos]-=rv; (*neighbour)[2*pos+1]-=2*rv;
               break;
       case 1: (*neighbour)[2*pos+1]-=2*rv;
               break;
       case 2: (*neighbour)[2*pos+1]-=rv;
     }
   switch (el)
     { 
       case 0: (*neighbour)[2*pos]+=rv; (*neighbour)[2*pos+1]+=2*rv;
               break;
       case 1: (*neighbour)[2*pos+1]+=2*rv;
               break;
       case 2: (*neighbour)[2*pos+1]+=rv;
     }
   for (i=0; i<2*n+1; i++)
     if ((i!=2*pos) && (i!=2*pos+1))
       (*neighbour)[i]+=rv* (el-(*v)[pos]);
   (*v)[pos]=el;
  }

bool smalldistance(int i, int j)
  {
    if (i==j) return 1;
    if (i>j) i=i-j;
    else     i=j-i;
    while (i>1) {
                  if (i % 3) return 0;
                  i=i/3;
                }
    return 1;
  }
    




