/**
 * @author  unknown
 * @version 3.0pre1
 */
 
#include <string>

#include "mo_jo.hpp"

using namespace std;

//**************************************************************************

Lisa_OrderWithoutRepetition::Lisa_OrderWithoutRepetition(){
  n = 0;
  succv = 0;
  start = 0;
}

//**************************************************************************

Lisa_OrderWithoutRepetition::Lisa_OrderWithoutRepetition(const int n_in){
    n=n_in;
    succv=new Lisa_Vector<int>(n);
    start=NO_SUCCESSOR;
    succv->fill(NO_SUCCESSOR);
}

//**************************************************************************

int Lisa_OrderWithoutRepetition::succ(int i) const
  {
    if (i==MO_SOURCE) return start;
    return (*succv)[i];
  }

//**************************************************************************
        
void Lisa_OrderWithoutRepetition::insert(const int i, const int j)
  {
    if (j==MO_SOURCE)
      {
        (*succv)[i]=start;
        start=i;
      }
    else
      {
        (*succv)[i]=(*succv)[j];
        (*succv)[j]=i;
      }
  }

//**************************************************************************
       
void Lisa_OrderWithoutRepetition::write_rank(Lisa_Vector<int>* r) const
 {
   int i=start;
   int rank=1;
   
   r->fill(0);
   while(i!=NO_SUCCESSOR)
      {
        (*r)[i]=rank;
        i=(*succv)[i]; rank++;
      }
 } 

//**************************************************************************
 
int Lisa_OrderWithoutRepetition::read_rank(Lisa_Vector<int>* rank)
 {
   int pred=MO_SOURCE;
   int r,pos;

   succv->fill(NO_SUCCESSOR);
   start=NO_SUCCESSOR;
   r=0;
   do 
     { 
       r++;
       pos=0; while ((pos<n)&&(*rank)[pos]!=r) pos++;
  
       if (pos<n)
	 {
           insert(pos,pred);
           pred=pos;
         }
     }
   while(pos<n);

   return true;
 }

//**************************************************************************

Lisa_OrderWithoutRepetition::~Lisa_OrderWithoutRepetition(){
    if(succv) delete succv;
}

//**************************************************************************

/// Classes Lisa_MO and Lisa_JO are not to be confused with each other!
Lisa_MO::Lisa_MO(const int n_in, const int m_in)
  {
    n=n_in; m=m_in;
    orders=new Lisa_OrderWithoutRepetition[n];
    
    for(int i=0;i<n;i++){
        orders[i].n=m;
        orders[i].succv=new Lisa_Vector<int>(m);
        orders[i].start=NO_SUCCESSOR;
        orders[i].succv->fill(NO_SUCCESSOR);
    }
    
  } 

//**************************************************************************

int Lisa_MO::succ(int i, int j) const
  { 
    return orders[i].succ(j);
  }

//**************************************************************************

void Lisa_MO::insert(const int i, const int j, const int k)
  {
    orders[i].insert(j,k);
  }

//**************************************************************************

void Lisa_MO::write_rank(Lisa_Matrix<int>* lr) const
  {
    int i;
    for (i=0; i<n; i++)
       orders[i].write_rank(&((*lr)[i]));
  }

//**************************************************************************

int Lisa_MO::read_rank(Lisa_Matrix<int>* lr)
  {
    int i;
    for (i=0; i<n; i++)
       orders[i].read_rank(&((*lr)[i]));
    return true;
  }

//**************************************************************************

void Lisa_MO::write (ostream& strm) const
  {
    Lisa_Vector<int> * rank=new Lisa_Vector<int>(m);
    int i;
    strm << "{\n";
    for (i=0 ; i<n ; i++) 
          { 
            orders[i].write_rank(rank);
            strm << " " << *rank;
          } 
    strm << "}\n";
    delete rank;
  }

//**************************************************************************

void Lisa_MO::read (istream& strm)
  {
    string S;
    Lisa_Vector<int> * rank=new Lisa_Vector<int>(m);
    int i;
       
    S="";strm >> S; 
    if(S!="{") cout << "{ erwartet \n";
    for (i=0 ; i<n ; i++) 
       { 
         strm >> *rank;
         orders[i].read_rank(rank);
       }
    S="";strm >> S; 
    if(S!="}") cout << "} erwartet \n";
    delete rank;
  } 

//**************************************************************************

Lisa_MO::~Lisa_MO()
  { 
    delete[] orders;
  }

//**************************************************************************

Lisa_JO::Lisa_JO(const int n_in, const int m_in){
    n=n_in; m=m_in;
    orders=new Lisa_OrderWithoutRepetition[m];

     for(int i=0;i<m;i++){
        orders[i].n=n;
        orders[i].succv=new Lisa_Vector<int>(n);
        orders[i].start=NO_SUCCESSOR;
        orders[i].succv->fill(NO_SUCCESSOR);
    }
} 

//**************************************************************************

int Lisa_JO::succ(int i, int j) const
  { 
    return orders[j].succ(i);
  }

//**************************************************************************

void Lisa_JO::insert(const int i, const int j, const int k)
  {
    orders[j].insert(i,k);
  }

//**************************************************************************

void Lisa_JO::write_rank(Lisa_Matrix<int>* lr) const
  {
    Lisa_Vector<int>* column=new Lisa_Vector<int>(n);
    int i,j;
    for (j=0; j<m; j++)
       {
         orders[j].write_rank(column);
         for (i=0; i<n; i++)
            (*lr)[i][j]=(*column)[i];
       } 
  }

//**************************************************************************

int Lisa_JO::read_rank(Lisa_Matrix<int>* lr)
  {
    Lisa_Vector<int>* column=new Lisa_Vector<int>(n);
    int i,j;
    for (j=0; j<m; j++)
       {
         for (i=0; i<n; i++)
            (*column)[i]=(*lr)[i][j];
         orders[j].read_rank(column);
       }
    return true; 
  }

//**************************************************************************

void Lisa_JO::write (ostream& strm) const
  {
    Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(n,m);
    write_rank(LR);
    strm << *LR;
    delete LR;
  }

//**************************************************************************

void Lisa_JO::read (istream& strm)
  {
    Lisa_Matrix<int> * LR=new Lisa_Matrix<int>(n,m);
    strm >> *LR;
    read_rank(LR);
    delete LR;
  }

//**************************************************************************

Lisa_JO::~Lisa_JO()
  { 
    delete[] orders;
  }

//**************************************************************************

