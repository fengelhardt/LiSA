/**
 * @author  Thomas Tautenhahn
 * @version 2.3pre3
 */
 
#include <stdlib.h>
#include <string>

#include "irregjs.hpp"

using namespace std;

NM_JsProblem::NM_JsProblem(Lisa_Values * Pin) : Lisa_JsProblem( Pin ) 
  {
  }

NM_JsSchedule::NM_JsSchedule(Lisa_JsProblem * Pin) : Lisa_JsSchedule( Pin ) 
  {
  }

void NM_JsProblem::read(istream& strm) 
     {      
       int i,j,k,kalt;
       string S;
       for (i=1; i<=8; i++)
           strm >> S;
       strm >> w_early;
       strm >> w_late;
       for (i=1; i<=7; i++)
         strm >> S;
       for (i=1; i<=20; i++)
	  {
            strm >> (*di)[i]; strm >> (*di)[i];
            strm >> (*wi_early)[i]; strm >> (*wi)[i]; 
          }
       for (i=1; i<=4; i++)
          strm >> S;       
       for (i=1; i<=20; i++)
          {
            kalt=SOURCE;
            for (j=1; j<=5; j++)
	       {
                 strm >> S; strm >> S;
                 k=((int) S.at(1) )-48;
                 (*MOsucc)[i][kalt]=k;
                 (*MOpred)[i][k]=kalt;
                 kalt=k;
                 strm >> (*time)[i][k];
               }
            (*MOsucc)[i][kalt]=SINK;
            (*MOpred)[i][SINK]=kalt;    
          }  
       ri->fill(0);
     }

void NM_JsSchedule::read(istream& strm) 
  {      
    int i,j,k;
    string S;
    Lisa_Matrix<TIMETYP> * cij=new Lisa_Matrix<TIMETYP>( P->n, P->m);    
 
    for (i=1; i<=5; i++)
         strm >> S;
    for (i=1; i<=20; i++)
       for (j=1; j<=5; j++)
	  {
            strm >> S; strm >> S;
            k=(int) S.at(1) - 48;
            strm >> (*cij)[i-1][k-1];     
            (*cij)[i-1][k-1]+=(*(P->time))[i][k];
          }
    read_Cij(cij); // cout << "ERROR: input not correct" << endl;
    delete cij;
  }
