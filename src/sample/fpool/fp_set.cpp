/*
 * ******************** fp_set.cpp ******************************
 * 
 * description:  vector set for football pool problem
 * 
 * @author        Thomas Tautenhahn
 * @version 2.3pre3
 *
 * date:         17. 11. 1998
 *
 */

#include "../../main/global.hpp"

#include "fp_set.hpp"

using namespace std;

fp_set::fp_set(int ni, int sigmai)
  {
    int i;
    n=ni;
    sigma=sigmai;
    vectors=new fp_vector*[sigma];
    for (i=0; i<sigma; i++)
       vectors[i]=new fp_vector(n);
    slack=1;
    for (i=0; i<n; i++) slack*=3;
    covered = new Lisa_Vector<int>(slack);
    covered->fill(0);
    slack-=2*n+1;
    for (i=0; i<2*n+1; i++)
       (*covered)[vectors[0]->get_neighbour(i)]=sigma;
  } 

const fp_set& fp_set::operator=(fp_set& other)
 { 
   int i;
   n=other.n;
   sigma=other.sigma;
   for (i=0; i<sigma; i++)
      (*(vectors[i]))= (*(other.vectors[i]));
   slack=other.slack;
   (*covered)=(*(other.covered));
   return *this;
 }
fp_vector* fp_set::replace_vector(int pos , fp_vector * nv)
  {
    int i;
    fp_vector * ov;
    ov=vectors[pos];      
    for (i=0; i<2*n+1; i++)
       if (!(--((*covered)[ov->get_neighbour(i)]))) slack++;    
    for (i=0; i<2*n+1; i++)
       if (!((*covered)[nv->get_neighbour(i)]++)) slack--;
         vectors[pos]=nv;
    return ov;
  }

void fp_set::spread()
  {
    int i,j,k, elem;
    long seed=12345671l;
    fp_vector * other=new fp_vector(n);
    Lisa_Matrix<int> needed(n,3);
    for (i=1; i<sigma; i++)
       {  
         // what does a typical non-coverd vector look like?
         needed.fill(0);
         for (k=0; k<covered->get_m(); k++)
            if (!(*covered)[k])
              {
                int k2;
                k2=k;
                for (j=0; j<n; j++)
                   { 
                     needed[j][k2 % 3]++;
                     k2=k2/3;
                   }
              }
         for (j=0; j<n; j++)
	    {
              for (k=0; k<3; k++) 
                 needed[j][k]+=lisa_random(0,1, &seed);
              if (needed[j][0]<needed[j][1])
                { 
                  elem=1; 
                  needed[j][0]=needed[j][1];
                }
              else elem=0;
              if (needed[j][0]<needed[j][2])
                elem=2;
              other->set_element(j, elem);
            }
         other=replace_vector(i,other);
       }
    delete other;  
  }

void fp_set::random(long * seed)
  {
    int i,j;
    fp_vector * other=new fp_vector(n);
    for (i=1; i<sigma; i++)
       {  
         for (j=0; j<n; j++)
	    other->set_element(j, lisa_random(0,2,seed));
         other=replace_vector(i,other);
       }
    delete other;  
  }

void fp_set::random2(long * seed)
  {
    int i,j,k,l;
    k=1;
    for (l=0; l< n/2; l++) k*=3;
    fp_vector * other=new fp_vector(n);
    for (i=1; i<sigma; i++)
       {  
         for (j=0,l=1; j<n; j++,l*=3)
	    if ((i<k) && (j<n/2))
               other->set_element(j, (i/l)%3);
            else 
               other->set_element(j, lisa_random(0,2,seed));
         other=replace_vector(i,other);
       }
    delete other;  
  }

void fp_set::write(ostream& strm) const
  {
    int i;
    for (i=0; i<sigma; i++)
      strm << *(vectors[i]);
    strm << *covered;
    strm << slack << " vectors not covered.\n";
  }

fp_set::~fp_set()
  {
    int i; 
    for (i=0; i<sigma; i++)
       delete vectors[i];
    delete[] vectors;
    delete covered;
  }

