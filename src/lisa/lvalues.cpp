/**
 * @author  Per Willenius
 * @version 3.0pre1
 */

#include <stdio.h>

#include "../main/global.hpp"
#include "../misc/except.hpp"

#include "lvalues.hpp"

using namespace std;

//**************************************************************************

const int LENGHT_OF_EXTRA_VECTOR = 5;

//**************************************************************************

Lisa_Values::Lisa_Values()
  {
    valid=true;
    n=m=0;                       // startvalue 
    PT=NULL;
    MO=NULL;
    SIJ=NULL;
    RD=DD=NULL;
    WI=NULL;
    WI2=NULL;
    EXTRA=NULL;
  }

//**************************************************************************

void Lisa_Values::init(int nin, int min){
  
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
    for (i=ncpy; i--; ){
      for (j=mcpy; j--; ){
        (*PT)[i][j]=(*T1)[i][j];
      }
    }
    delete T1;
  }  
  
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
    for (i=ncpy; i--; ){
      for (j=mcpy; j--; ){
        (*SIJ)[i][j]=(*T3)[i][j];
      }
    }
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

  valid=true;
}

//**************************************************************************

void Lisa_Values::write(ostream& strm) const {
  if (valid==false) {
    G_ExceptionList.lthrow("Lisa_Values not valid. Nothing written.",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  strm << endl << "<VALUES>" << endl;
  
            strm << "m= "     << m      << endl;
            strm << "n= "     << n      << endl;
  if(PT)    strm << "PT= "    << *PT    << endl; // Processing times
  if(MO)    strm << "MO= "    << *MO    << endl; // maschine order
  if(SIJ)   strm << "SIJ= "   << *SIJ   << endl; // set of operations
  if(RD)    strm << "RD= "    << *RD    << endl; // Release Dates
  if(DD)    strm << "DD= "    << *DD    << endl; // Due Dates
  if(WI)    strm << "WI= "    << *WI    << endl; // weights
  if(WI2)   strm << "WI2= "   << *WI2   << endl; // second type of weights
  if(EXTRA) strm << "EXTRA= " << *EXTRA << endl; // extra information
  
  strm << "</VALUES>" << endl;
}

//**************************************************************************

void Lisa_Values::read(istream& strm) {
  
  // assure stream existence
  if (strm==NULL) {
    G_ExceptionList.lthrow("No valid stream in Lisa_Values::read().",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  string S;
  int dim;
  bool mfound=1,nfound=1;
  
  // find start of entry
  for (;;){
    S=""; 
    strm >>S;
    if (S==""){ 
      G_ExceptionList.lthrow("No '<VALUES>' entry in file.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    } 
    if (S=="<VALUES>") break;
  }
  
  // found entry, read on
  for (;;){
    S=""; 
    strm >>S;  
    
    if (S==""){ 
      G_ExceptionList.lthrow("Closing tag '</VALUES>' not found.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    }else if(S=="</VALUES>"){
      return; 
    }else if(S=="m="){  
      strm >> dim;
      init(n,dim);
      mfound = 0;
    }else if(S=="n="){
      strm >> dim; 
      init(dim,m);
      nfound = 0;
    }else if(S=="PT="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'PT='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if(!PT) make_PT();
        strm >> *PT;
      }
    }else if(S=="MO="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'MO='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if(!MO) make_MO();
        strm >> *MO;
      }
    }else if(S=="SIJ="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'SIJ='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{      
        if (!SIJ) make_SIJ();
        strm >> *SIJ;
      }
	  }else if(S=="RD="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'RD='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{      
        if (!RD) make_RD();
        strm >> *RD;
      }
    }else if(S=="DD="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'DD='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if(!DD) make_DD();
        strm >> *DD;
      }
	  }else if(S=="WI="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'WI='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if (!WI) make_WI();
        strm >> *WI;
      }
    }else if(S=="WI2="){
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'WI2='. Skipping.",
                                Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if (!WI2) make_WI2();
        strm >> *WI2;
      }
    }else if(S=="EXTRA="){ 
      if(mfound || nfound){
        G_ExceptionList.lthrow("Problem size not given before 'EXTRA='. Skipping.",
                               Lisa_ExceptionList::SYNTAX_ERROR);
      }else{
        if (!EXTRA) make_EXTRA();
        strm >> *EXTRA;
      }
    }else{
      G_ExceptionList.lthrow("Unknown token '"+S+"' found. Skipping.",
                             Lisa_ExceptionList::SYNTAX_ERROR);
    }
  }
  
  if(mfound || nfound){
    G_ExceptionList.lthrow("Problem size not given in Lisa_Values::read().",
                           Lisa_ExceptionList::SYNTAX_ERROR);
  }else{
    valid=true;
  }
}

//**************************************************************************

void Lisa_Values::make_PT(void) { 
  if(PT) return;
  PT=new Lisa_Matrix<TIMETYP>(n,m);
  PT->fill(0); 
}

//**************************************************************************

void Lisa_Values::make_MO(void) { 
  if(MO) return;
  MO=new Lisa_MO(n,m); 
}

//**************************************************************************

int Lisa_Values::valid_MO(void) { 
  
  if(MO==NULL) return !OK;
  
  if(SIJ==NULL){
    G_ExceptionList.lthrow("No valid SIJ in Lisa_Values::valid_MO().",
                           Lisa_ExceptionList::UNDEFINED_OBJECT);
    return !OK; 
  }
  
  Lisa_Matrix<int> myMO(n,m);
  myMO.fill(0);
  MO->write_rank(& myMO);
  
  for (int i=0; i<n; i++) {
    for (int j=0;j<m; j++) {
      if ( (*SIJ)[i][j]==1 && myMO[i][j]<1 ) 
        return !OK; 
    }
  }
  return OK;
}

//**************************************************************************

void Lisa_Values::no_MO(void) { 
  if(MO) delete MO;
  MO=NULL;
}

//**************************************************************************

void Lisa_Values::no_RD(void) { 
  if(RD) delete RD;
  RD=NULL;
}

//**************************************************************************

void Lisa_Values::no_DD(void) { 
  if(DD) delete DD;
  DD=NULL;
}

//**************************************************************************

void Lisa_Values::no_WI(void) { 
  if(WI) delete WI;
  WI=NULL;
}

//**************************************************************************

void Lisa_Values::make_SIJ(void) { 
  if(SIJ) return;
  SIJ=new Lisa_Matrix<bool>(n,m);
  SIJ->fill(0); 
}

//**************************************************************************

void Lisa_Values::make_RD(void) { 
  if(RD) return;
  RD=new Lisa_Vector<TIMETYP>(n);
  RD->fill(0); 
}

//**************************************************************************

void Lisa_Values::make_DD(void) { 
  if(DD) return;
  DD=new Lisa_Vector<TIMETYP>(n);
  DD->fill(MAXTIME); 
}

//**************************************************************************

void Lisa_Values::make_WI(void) { 
  if(WI) return;
  WI=new Lisa_Vector<double>(n);
  WI->fill(1); 
} 

//**************************************************************************

void Lisa_Values::make_WI2(void) { 
  if(WI2) return;
  WI2=new Lisa_Vector<double>(n);
  WI2->fill(1); 
} 

//**************************************************************************

void Lisa_Values::make_EXTRA(void) { 
  if(EXTRA) return; 
  EXTRA=new Lisa_Vector<double>(LENGHT_OF_EXTRA_VECTOR); 
  EXTRA->fill(1); 
} 

//**************************************************************************

void Lisa_Values::PT_to_SIJ() {

  if (SIJ==NULL) { 
    G_ExceptionList.lthrow("In Lisa_Values::PT_to_SIJ(): no SIJ.",
                           Lisa_ExceptionList::ANY_ERROR);
    return;
  }
  
  if (PT==NULL)  {  
    G_ExceptionList.lthrow("In Lisa_Values::PT_to_SIJ(): no PT.",
                           Lisa_ExceptionList::ANY_ERROR); 
    return;
  }
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){ 
      if( (*PT)[i][j]>0.001) (*SIJ)[i][j]=1;
      else  (*SIJ)[i][j]=0;  
    }
  }
  
}   

//**************************************************************************

Lisa_Values::~Lisa_Values(){ 
  if(PT) delete PT;
  if(RD) delete RD;
  if(MO) delete MO;
  if(DD) delete DD;
  if(SIJ) delete SIJ;
  if(WI) delete WI;
}

//**************************************************************************

