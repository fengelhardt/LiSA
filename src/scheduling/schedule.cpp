/**
 * @author  Per Willenius
 * @version 2.3final
 */
 
#include <stdio.h>

#include "../main/global.hpp"
#include "../misc/except.hpp"

#include "schedule.hpp"

using namespace std;

//**************************************************************************

Lisa_ScheduleNode::Lisa_ScheduleNode() {
  actual_schedule=NULL;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  schedule_info->fill(0);
}

//**************************************************************************

int Lisa_ScheduleNode::sinfo_pointer = 0;

//**************************************************************************

Lisa_ScheduleNode::Lisa_ScheduleNode(Lisa_Schedule* l_schedule) {
  actual_schedule=l_schedule;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  schedule_info->fill(0);
}

//**************************************************************************

Lisa_ScheduleNode::Lisa_ScheduleNode(const Lisa_ScheduleNode &myschedulenode) {
  actual_schedule=myschedulenode.actual_schedule;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  (*schedule_info) = (*myschedulenode.schedule_info);
}

//**************************************************************************

bool Lisa_ScheduleNode::operator<=(const Lisa_ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ( (*schedule_info)[sinfo_pointer]<= (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

//**************************************************************************

bool Lisa_ScheduleNode::operator>=(const Lisa_ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]>= (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

//**************************************************************************

bool Lisa_ScheduleNode::operator<(const Lisa_ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]< (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

//**************************************************************************

bool Lisa_ScheduleNode::operator==(const Lisa_ScheduleNode & other) const {
 if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]== (*other.schedule_info)[sinfo_pointer]);
 return 1;
}

//**************************************************************************

bool Lisa_ScheduleNode::operator!=(const Lisa_ScheduleNode & other) const {
 if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]!= (*other.schedule_info)[sinfo_pointer]);
 return 1;
}

//**************************************************************************

const Lisa_ScheduleNode & Lisa_ScheduleNode::operator=( const Lisa_ScheduleNode & other) {
  delete actual_schedule;
  if (other.actual_schedule) {
  actual_schedule= new Lisa_Schedule(*other.actual_schedule);
  }
  if (other.schedule_info) {
    delete schedule_info;
    schedule_info= new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
    (*schedule_info)=(*other.schedule_info);
  }
 return  other;
}

//**************************************************************************

void Lisa_ScheduleNode::write(ostream & strm) const {
  if (actual_schedule)
    strm << *actual_schedule << endl;
}

//**************************************************************************

void Lisa_ScheduleNode::read(istream & strm) {
  strm >> *actual_schedule;
}

//**************************************************************************

Lisa_ScheduleNode::~Lisa_ScheduleNode(){
  delete schedule_info;
}

//**************************************************************************

Lisa_Schedule::Lisa_Schedule(int n_in,int m_in) {
  semiactive=TRUE;
  valid=FALSE;
  n=n_in; 
  m=m_in;
  LR=NULL;
  NMO=NULL;
  NJO=NULL;
  CIJ=NULL; 
}

//**************************************************************************

Lisa_Schedule::Lisa_Schedule(Lisa_Schedule & other) {
  semiactive=other.semiactive;
  valid=other.valid;
  n=other.n; 
  m=other.m;
  if (other.LR) {
    LR=new Lisa_Matrix<int>(n,m);
    (*LR)=(*other.LR);
  } else LR=NULL;
  if (other.NMO) {
    NMO=new Lisa_Matrix<int>(n,m);
    (*NMO)=(*other.NMO);
  } else NMO=NULL;
  if (other.NJO) {
    NJO=new Lisa_Matrix<int>(n,m);
    (*NJO)=(*other.NJO);
  } else NJO=NULL;
  if (other.CIJ) {
    CIJ=new Lisa_Matrix<TIMETYP>(n,m);
    (*CIJ)=(*other.CIJ);
  } else CIJ=NULL;
}

//**************************************************************************

void Lisa_Schedule::make_LR() {
  if (LR) return; LR=new Lisa_Matrix<int>(n,m); LR->fill(0); 
}

//**************************************************************************

void Lisa_Schedule::make_NMO() {
  if (NMO) return; NMO=new Lisa_Matrix<int>(n,m); NMO->fill(0); 
}

//**************************************************************************

void Lisa_Schedule::make_NJO() {
  if (NJO) return; NJO=new Lisa_Matrix<int>(n,m); NJO->fill(0); 
}

//**************************************************************************

void Lisa_Schedule::make_CIJ() {
  if (CIJ) return; CIJ=new Lisa_Matrix<TIMETYP>(n,m); CIJ->fill(0); 
}

//**************************************************************************

void Lisa_Schedule::init(int n_in, int m_in) {
  if (n!=n_in||m!=m_in){
    n=n_in; 
    m=m_in;
    
    if(LR){
      delete LR;
      LR= new Lisa_Matrix<int>(n,m);
      LR->fill(0);
    }
    
    if(NMO){
      delete NMO;
      NMO=new Lisa_Matrix<int>(n,m);
    }
    
    if(NJO){
      delete NJO;
      NJO=new Lisa_Matrix<int>(n,m);
    }
    
    if(CIJ){
      delete CIJ;
      CIJ=new Lisa_Matrix<TIMETYP>(n,m);
    } 
  }
  valid=FALSE;
}

//**************************************************************************

void Lisa_Schedule::write(ostream& strm) const {
  strm << endl << "<SCHEDULE>" << endl;
  
          strm << "m= "          << m          << endl;
          strm << "n= "          << n          << endl;
          strm << "semiactive= " << semiactive << endl;
  if(LR)  strm << "LR= "         << *LR        << endl; //latin rectangle
  if(NMO) strm << "NMO= "        << *NMO       << endl; //maschine order
  if(NJO) strm << "NJO= "        << *NJO       << endl; //job order
  if(CIJ) strm << "CIJ= "        << *CIJ        << endl; //end time for each operation
  
  strm << "</SCHEDULE>" << endl;
}

//**************************************************************************

int Lisa_Schedule::valid_LR(Lisa_Matrix<bool> *SIJ) { 
  int i=0,j=0;
  if (LR==NULL) {valid=FALSE;return !OK;} 
  if (SIJ==NULL) {  G_ExceptionList.lthrow("No valid SIJ",UNDEFINED_OBJECT);
  valid=FALSE; return !OK; 
  }
  for (i=0; i<n; i++) {
    for (j=0;j<m; j++) {
      if ( (*SIJ)[i][j]==1 && (*LR)[i][j]<1 ) 
	{valid=FALSE;return !OK;} 
    }
  }
  valid=TRUE;
  return OK;
}

//**************************************************************************

int Lisa_Schedule::get_property(int property) {
  int value=0;
  int i=0,j=0,i1=0,j1=0,test=0;
  if (LR==NULL) return 0; 
  if ( semiactive!=TRUE) return 0;
  switch (property)
    {
    case MAXRANK_INFO:
      value=0;
      for (i=0;i<n;i++) {
	for (j=0;j<m;j++) {
	  value = MAX(value, (*LR)[i][j]);          
        }
      }
      break;
    case NO_OF_SOURCES:
      for (i=0;i<n;i++) {
	for (j=0;j<m;j++) {
	  if ((*LR)[i][j]==1)
	    value++;
	}
      }
      break;
    case NO_OF_SINKS:
      for (i=0;i<n;i++) {
	for (j=0;j<m;j++) {
	  test=0;
	  for (i1=0;i1<n;i1++)
	    if ((*LR)[i1][j]> (*LR)[i][j]) test=1;
	  for (j1=0;j1<m;j1++)
	    if ((*LR)[i][j1]> (*LR)[i][j]) test=1;
	  if (test==0)
	    value++;
	}
      }
    }
  return value;
}

//**************************************************************************

void Lisa_Schedule::read(istream& strm) {
  
  // check input stream
  if (strm==NULL) {
    G_ExceptionList.lthrow("No valid stream in Lisa_Schedule::read().",ANY_ERROR);
    return;
  }
  
  // search for entry
  string S;
  int dim;
  for (;;){
    S=""; 
    strm >>S;
    if (S==""){ 
      G_ExceptionList.lthrow("No '<SCHEDULE>' entry found.",END_OF_FILE);
      return;
    } 
    if (S=="<SCHEDULE>") break;
  }
  
  bool semif=1,mf=1,nf=1;
  
  // start found ... read on
  for (;;){
    
    S=""; 
    strm >>S;  
    
    if (S==""){
      G_ExceptionList.lthrow("Closing tag '</SCHEDULE>' not found.",END_OF_FILE);
      return;
    }else if (S=="</SCHEDULE>"){
      break; 
    }else if (S=="m="){  
      mf=0;
      strm >> dim;
      init(n,dim);
    }else if (S=="n="){
      nf=0;
      strm >> dim;
      init(dim,m);
    }else if (S=="semiactive="){ 
      semif=0;
      strm >> semiactive;
    }else if (S=="LR="){ 
      if(mf || nf || semif){
        G_ExceptionList.lthrow("Problem size/semiactive not given before 'LR='. Skipping.",SYNTAX_ERROR);
      }else{
        if (!LR) make_LR();  
        strm >> *LR;
      }
    }else if (S=="NMO="){ 
      if(mf || nf || semif){
        G_ExceptionList.lthrow("Problem size/semiactive not given before 'NMO='. Skipping.",SYNTAX_ERROR);
      }else{
        if (!NMO) make_NMO();
        strm >> *NMO;
      }
    }else if (S=="NJO="){ 
      if(mf || nf || semif){
        G_ExceptionList.lthrow("Problem size/semiactive not given before 'NJO='. Skipping.",SYNTAX_ERROR);
      }else{
        if (!NJO) make_NJO();
        strm >> *NJO;
      }
    }else if (S=="CIJ="){
      if(mf || nf || semif){
        G_ExceptionList.lthrow("Problem size/semiactive not given before 'CIJ='. Skipping.",SYNTAX_ERROR);
      }else{      
        if (!CIJ) make_CIJ();
        strm >> *CIJ;
      }
    }else{
      G_ExceptionList.lthrow("Unknown token '"+S+"' found. Skipping.",SYNTAX_ERROR);
    }

  }
  
  if(mf || nf || semif){
    G_ExceptionList.lthrow("Problem size/semiactive not given in Lisa_Schedule::read().",SYNTAX_ERROR);
  }else{
    valid=TRUE;
  }
}

//**************************************************************************

Lisa_Schedule::~Lisa_Schedule() {
  delete LR;
  delete NMO;
  delete NJO;
  delete CIJ;
}

//**************************************************************************

