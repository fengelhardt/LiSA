/*
 * ************** lvalues.cpp *******************************
 * 
 * description:    definition of class Lisa_Values
 *
 * @author          Per Willenius
 *
 * date:           27.9.1999
 *
 *  @version 2.3pre3
 */

#include <stdio.h>

#include "../main/global.hpp"
#include "../misc/except.hpp"

#include "lvalues.hpp"

// ########################### Class Lisa_Values ######################################

Lisa_Values::Lisa_Values()
  {
    valid=TRUE;
    n=m=0;                       // startvalue 
    PT=NULL;
    //PTl=NULL;
    MO=NULL;
    SIJ=NULL;
    RD=DD=NULL;
    WI=NULL;
    WI2=NULL;
    EXTRA=NULL;
  }

void Lisa_Values::init(int nin, int min)
{
  int ncpy, mcpy, i, j;
  bool fill;
  Lisa_Matrix<TIMETYP> * T1;
  Lisa_Matrix<bool>    * T3; 
  Lisa_Vector<TIMETYP> * V1;
  Lisa_Vector<double>   * V2; 
  
  mcpy=MIN(m,min); ncpy=MIN(n,nin);
  fill = (mcpy<min || ncpy < nin); 
  m=min; n=nin;  
  
  if (PT) { 
    T1= PT;
    PT=new Lisa_Matrix<TIMETYP>(n,m);
    if (fill) PT->fill(0);
    for (i=ncpy; i--; )
      for (j=mcpy; j--; )
	(*PT)[i][j]=(*T1)[i][j];
    delete T1;
  }  
 //  if (PTl) { 
//     Lisa_List<TIMETYP> mylist;
//     mylist.append(0.0);
//     Lisa_MatrixOfLists<TIMETYP> *T6;
//     T6= PTl;
//     PTl=new Lisa_MatrixOfLists<TIMETYP>(n,m);
//     for (i=0; i<ncpy; i++ )
//       for (j=0; j<=mcpy; j++ )
// 	(*PTl)[i][j]=(*T6)[i][j];
//     i=ncpy+1; 
//     while (i<n) 
//       {
// 	for (j=0; j<m; j++) (*PTl)[i][j]=mylist;
// 	i++;
//       }
//     j=mcpy+1;
//     while (j<m) 
//       {
// 	for (i=0; i<ncpy; i++) (*PTl)[i][j]=mylist;
// 	j++;
//       }
//     delete T6;
//   }      
  if (MO) { 
    if (MO->get_n()!=n ||MO->get_m()!=m) {
      delete MO;
      MO=new Lisa_MO(n,m);
    }
  }        
  if (SIJ) { 
    T3= SIJ;
    SIJ=new Lisa_Matrix<bool>(n,m);
    if (fill) SIJ->fill(1);
    for (i=ncpy; i--; )
      for (j=mcpy; j--; ) (*SIJ)[i][j]=(*T3)[i][j];
    delete T3;
  }         
  if (RD) { 
    V1= RD;
    RD= new Lisa_Vector<TIMETYP>(n);
    if (ncpy<n) RD->fill(0);
    for (i=ncpy; i--; ) (*RD)[i]=(*V1)[i];
    delete V1;
  } 
  if (DD) { 
    V1= DD;
    DD= new Lisa_Vector<TIMETYP>(n);
    if (ncpy<n) DD->fill(MAXTIME);
    for (i=ncpy; i--; ) (*DD)[i]=(*V1)[i];
    delete V1;
  }   
  if (WI) { 
    V2= WI;
    WI= new Lisa_Vector<double>(n);
    if (ncpy<n) WI->fill(1.);
    for (i=ncpy; i--; ) (*WI)[i]=(*V2)[i];
    delete V2;
  } 
  valid=TRUE;
}

void Lisa_Values::write(ostream& strm) const {
  if (valid==FALSE) {
    G_ExceptionList.lthrow("Lisa_Values not valid",ANY_ERROR);
    return;
  }
  strm << "\n<VALUES>\n";
  strm << "m= " << m << "\n";
  strm << "n= " << n << "\n";
  if (PT)  strm << "PT= " << *PT << "\n"; // Processing times
  //  if (PTl) strm << "PTl= " << *PTl << "\n"; // Preemtion- Processing Times
  if (MO)  strm << "MO= " << *MO << "\n"; // maschine order
  if (SIJ)  strm << "SIJ= " << *SIJ << "\n"; // set of operations
  if (RD)  strm << "RD= " << *RD << "\n"; // Release Dates
  if (DD)  strm << "DD= " << *DD << "\n"; // Due Dates
  if (WI)  strm << "WI= " << *WI << "\n"; // weights
  if (WI2)  strm << "WI2= " << *WI2 << "\n"; // second type of weights
  if (EXTRA)  strm << "EXTRA= " << *EXTRA << "\n"; // second type of weights
  strm << "</VALUES>\n ";
}

void Lisa_Values::read(istream& strm) {
  if (strm==NULL) {
    G_ExceptionList.lthrow("error in Lisa_Values::read (no valid stream)",ANY_ERROR);
    return;}
  string S;
  int dim;

  for (;;)
    {
      S=""; 
      strm >>S;
      if (S=="") 
	{ 
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Values.read",
				 END_OF_FILE);
	  return;
	} 
      if (S=="<VALUES>") break;
    }
  // d.h. ich soll weiterlesen:
   for (;;)
    {
      S=""; 
      strm >>S;  
      if (S=="") 
	{ 
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Values.read",
				 END_OF_FILE);
	  return;
	} 
      if (S=="</VALUES>") return; 
      // d.h. ich soll weiterlesen:  
       if (S=="m=") 
         {  
           strm >> dim; init(n,dim);  //break;
         } 
       else if (S=="n=")
	 {
           strm >> dim; init(dim,m);  //break; 
	 }
       else if (S=="PT=")
         { 
           if (!PT) make_PT();   strm >> *PT;   //break;  
	 }
     //   else if (S=="PTl=")
//          { 
//            if (!PTl) make_PTl();   strm >> *PTl;   //break;  
// 	 }
       else if (S=="MO=")
         { 
           if (!MO) make_MO();   strm >> *MO;   //break;  
	 }
       else if (S=="SIJ=")
         { 
           if (!SIJ) make_SIJ();   strm >> *SIJ;   //break;  
	 }
       else if (S=="RD=")
         { 
           if (!RD) make_RD();   strm >> *RD;   //break;  
	 } 
       else if (S=="DD=")
         { 
           if (!DD) make_DD();   strm >> *DD;   //break;  
	 } 
       else if (S=="WI=")
         { 
           if (!WI) make_WI();   strm >> *WI;   //break;  
	 }
       else if (S=="WI2=")
         { 
           if (!WI2) make_WI2();   strm >> *WI2;   //break;  
	 }
       else if (S=="EXTRA=")
         { 
           if (!EXTRA) make_EXTRA();   strm >> *EXTRA;   //break;  
	 }
    }
   valid=TRUE;
}


void Lisa_Values::make_PT(void) { 
  if (PT) return; PT=new Lisa_Matrix<TIMETYP>(n,m); PT->fill(0); 
}

// void Lisa_Values::make_PTl(void) { 
//   Lisa_List<TIMETYP> mylist;
//   mylist.append(0.0);
//   if (PTl) return; 
//   PTl=new Lisa_MatrixOfLists<TIMETYP>(n,m);
// }


void Lisa_Values::make_MO(void) { 
  if (MO) return; MO=new Lisa_MO(n,m); 
}

int Lisa_Values::valid_MO(void) { 
  int i=0,j=0;
  if (MO==NULL) return !OK; 
  if (SIJ==NULL) {  G_ExceptionList.lthrow("No valid SIJ",UNDEFINED_OBJECT);
  return !OK; 
  }
  Lisa_Matrix<int> myMO(n,m);
  myMO.fill(0);
  MO->write_rank(& myMO);
  for (i=0; i<n; i++) {
    for (j=0;j<m; j++) {
      if ( (*SIJ)[i][j]==1 && myMO[i][j]<1 ) 
	return !OK; 
    }
  }
  return OK;
}

void Lisa_Values::no_MO(void) { 
  if (MO) delete MO;
  MO=NULL;
}

void Lisa_Values::no_RD(void) { 
  if (RD) delete RD;
  RD=NULL;
}

void Lisa_Values::no_DD(void) { 
  if (DD) delete DD;
  DD=NULL;
}

void Lisa_Values::no_WI(void) { 
  if (WI) delete WI;
  WI=NULL;
}

void Lisa_Values::make_SIJ(void) { 
  if (SIJ) return; SIJ=new Lisa_Matrix<bool>(n,m); SIJ->fill(0); 
}

void Lisa_Values::make_RD(void) { 
  if (RD) return; RD=new Lisa_Vector<TIMETYP>(n); RD->fill(0); 
}

void Lisa_Values::make_DD(void) { 
  if (DD) return; DD=new Lisa_Vector<TIMETYP>(n); DD->fill(MAXTIME); 
}

void Lisa_Values::make_WI(void) { 
  if (WI) return; WI=new Lisa_Vector<double>(n); WI->fill(1); 
} 

void Lisa_Values::make_WI2(void) { 
  if (WI2) return; WI2=new Lisa_Vector<double>(n); WI2->fill(1); 
} 

void Lisa_Values::make_EXTRA(void) { 
  if (EXTRA) return; 
  EXTRA=new Lisa_Vector<double>(LENGHT_OF_EXTRA_VECTOR); 
  EXTRA->fill(1); 
} 

void Lisa_Values::PT_to_SIJ() {
  int i,j;
  if (SIJ==NULL) { 
    G_ExceptionList.lthrow("(in Lisa_Values::PT_to_SIJ) no SIJ",ANY_ERROR);
    return;
  }
  if (PT==NULL)  {  
    G_ExceptionList.lthrow("(in Lisa_Values::PT_to_SIJ) no PT",ANY_ERROR); 
    return;
  }
  for(i=0;i<n;i++) 
    {
	  for(j=0;j<m;j++) 
	    { 
	      if( (*PT)[i][j]>0.001) (*SIJ)[i][j]=1;
	      else  (*SIJ)[i][j]=0;  
	    }
    }
}   

Lisa_Values::~Lisa_Values()
    { 
       if (PT) delete PT;       if (RD) delete RD;
       if (MO) delete MO;       if (DD) delete DD;
       if (SIJ) delete SIJ;     if (WI) delete WI;
     //   if (PTl) delete PTl;
    }




