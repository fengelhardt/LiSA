/**
 * @author  Thomas Tautenhahn
 * @version 2.3final
 */
 
#include <iostream>

#include "order.hpp"

using namespace std;

//**************************************************************************

Lisa_Order::Lisa_Order(int nin)
  {
    int h;
    n=nin; m=1; anz=n;
    index=new Lisa_Vector<int>(n+1);
    key=new Lisa_Vector<double>(n+1);
    for (h=n; ((*index)[h]=h); h-- );
  }

//**************************************************************************

Lisa_Order::Lisa_Order(int nin, int min)
  {
    int h;
    n=nin; m=min; anz=n*m;
    index=new Lisa_Vector<int>(anz+1);
    key=new Lisa_Vector<double>(anz+1);
    for (h=anz; ((*index)[h]=h); h-- );
  }

//**************************************************************************

Lisa_Order::~Lisa_Order()      
  {
    delete index;
    delete key;
  }

//**************************************************************************

/* up to now key is copied, perhaps better write an access function for 
   matrix or vector */
void Lisa_Order::read(Lisa_Matrix<int> * ma)
  {
    int h,k;
    
    for (h=0; h<n; h++)
       for (k=0; k<m; k++)
          { 
            (*key)[m*h+k]=(*ma)[h][k];
          } 
  }

//**************************************************************************  
  
void Lisa_Order::read(Lisa_Matrix<double> * ma)
  {
    int h,k;
    
    for (h=0; h<n; h++)
       for (k=0; k<m; k++)
          { 
            (*key)[m*h+k]=(*ma)[h][k];
          } 
  }

//**************************************************************************  
 
void Lisa_Order::read(Lisa_Matrix<float> * ma)
  {
    int h,k;
    
    for (h=0; h<n; h++)
       for (k=0; k<m; k++)
          { 
            (*key)[m*h+k]=(*ma)[h][k];
          } 
  }

//**************************************************************************  

void Lisa_Order::read(Lisa_Vector<int> * ve)
  {
    int h;
    
    for (h=0; h<n; h++)
       { 
        (*key)[h]=(*ve)[h];
       } 
  }

//**************************************************************************  

void Lisa_Order::read(Lisa_Vector<float> * ve)
  {
    int h;
    
    for (h=0; h<n; h++)
       { 
        (*key)[h]=(*ve)[h];
       } 
  }

//**************************************************************************  

void  Lisa_Order::sort(){

  for(int i=anz-2; i>=0; i--){
    
    const int vi=(*index)[i];
    const double v=(*key)[vi];
    int j=i;
    
    while ((j+1)<anz && (*key)[(*index)[j+1]]<v){
      (*index)[j]=(*index)[j+1];
      j++;
    }
    
    (*index)[j]=vi;
  }
}

//**************************************************************************  
   
void  Lisa_Order::group()
 {
   int i, j, k, l, vi;
   double aktuell;
   
   for(i=0; i<anz; )
     { 
       aktuell=(*key)[(*index)[i]];
       k=i+1;
       for (j=k; j<anz; j++)
          if (aktuell==(*key)[(vi=(*index)[j])])
            {
              l=j;
              while (l>k)
                   { 
                     (*index)[l]=(*index)[l-1];
                     l--;
                   }
              (*index)[k++]=vi;
            }
       i=k;
     }
 }

//**************************************************************************

