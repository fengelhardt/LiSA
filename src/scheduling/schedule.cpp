/*
 * ************** schedule.cpp *******************************
 * 
 * description:    definition of class Lisa_Schedule
 *
 * owner:          Per Willenius
 *
 * date:           27.9.1999
 *
 * version:        V 1.0  
*/


// **************************** System Includes ****************************
#include <stdio.h>


// **************************** Project Includes ****************************
#include "../main/global.hpp"
// error handling:
#include "../misc/except.hpp"

// **************************** Local Includes ****************************
#include "schedule.hpp"


// *********************** functions *************************


//######################## ScheduleNode #####################################

ScheduleNode::ScheduleNode() {
  actual_schedule=NULL;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  schedule_info->fill(0);
}

int ScheduleNode::sinfo_pointer =0;

ScheduleNode::ScheduleNode(Lisa_Schedule* l_schedule) {
  actual_schedule=l_schedule;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  schedule_info->fill(0);
}

ScheduleNode::ScheduleNode(const ScheduleNode &myschedulenode) {
  actual_schedule=myschedulenode.actual_schedule;
  schedule_info=new Lisa_Vector<int>(LENGHT_OF_INFO_VECTOR);
  (*schedule_info) = (*myschedulenode.schedule_info);
}


bool ScheduleNode::operator<=(const ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ( (*schedule_info)[sinfo_pointer]<= (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

bool ScheduleNode::operator>=(const ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]>= (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

bool ScheduleNode::operator<(const ScheduleNode &other ) const {
  if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]< (*other.schedule_info)[sinfo_pointer]);
return 1;
} 

bool ScheduleNode::operator==(const ScheduleNode & other) const {
 if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]== (*other.schedule_info)[sinfo_pointer]);
 return 1;
}

bool ScheduleNode::operator!=(const ScheduleNode & other) const {
 if (schedule_info!=NULL && other.schedule_info!=NULL && 
      sinfo_pointer>=0 && sinfo_pointer<=LENGHT_OF_INFO_VECTOR)
    return ((*schedule_info)[sinfo_pointer]!= (*other.schedule_info)[sinfo_pointer]);
 return 1;
}

const ScheduleNode & ScheduleNode::operator=( const ScheduleNode & other) {
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

void ScheduleNode::write(ostream & strm) const {
  if (actual_schedule)
    strm << *actual_schedule << endl;
}

void ScheduleNode::read(istream & strm) {
  strm >> *actual_schedule;
}

//ScheduleNode * ScheduleNode::copy() {
//   ScheduleNode * myScheduleNode;
//   myScheduleNode = new ScheduleNode;
//   (*myScheduleNode)= *this;
//   return myScheduleNode;
// return NULL;
//}

ScheduleNode::~ScheduleNode(){
 //  cout << "delete" << endl;
//   cout << *actual_schedule;
//   if (actual_schedule==NULL) 
//     cout << "actual_schedule==NULL" << endl;
//   else
//     delete actual_schedule;
  delete schedule_info;
}

//######################## Lisa_Schedule #####################################

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


void Lisa_Schedule::make_LR() {
  if (LR) return; LR=new Lisa_Matrix<int>(n,m); LR->fill(0); 
}

void Lisa_Schedule::make_NMO() {
  if (NMO) return; NMO=new Lisa_Matrix<int>(n,m); NMO->fill(0); 
}
void Lisa_Schedule::make_NJO() {
  if (NJO) return; NJO=new Lisa_Matrix<int>(n,m); NJO->fill(0); 
}
void Lisa_Schedule::make_CIJ() {
  if (CIJ) return; CIJ=new Lisa_Matrix<TIMETYP>(n,m); CIJ->fill(0); 
}

void Lisa_Schedule::init(int n_in, int m_in) {
  if (n!=n_in||m!=m_in)
    {
      n=n=n_in; 
      m=m=m_in;
      if(LR)  {delete LR; LR= new Lisa_Matrix<int>(n,m);LR->fill(0);}
      if(NMO) {delete NMO; NMO=new Lisa_Matrix<int>(n,m);}
      if(NJO) {delete NJO; NJO=new Lisa_Matrix<int>(n,m);}
      if(CIJ) {delete CIJ;CIJ=new Lisa_Matrix<TIMETYP>(n,m);} 
    }
  valid=FALSE;
}

void Lisa_Schedule::write(ostream& strm) const {
  strm << "\n<SCHEDULE>\n";
  strm << "m= " << m << "\n";
  strm << "n= " << n << "\n";
  strm << "semiactive= " << semiactive << "\n";
  if (LR)  strm << "LR= " << *LR << "\n"; // the  Latin Rectangle
  //if (LRpmt)  strm << "LRpmt= " << *LRpmt << "\n"; // the pmt Latin Rectangle
  if (NMO)  strm << "NMO= " << *NMO << "\n"; // maschine order
  if (NJO)  strm << "NJO= " << *NJO << "\n"; // job order
  if (CIJ) strm << "CIJ= " << *CIJ << "\n"; // Matrix with the end time for each operation
  strm << "</SCHEDULE>\n ";
}

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

void Lisa_Schedule::read(istream& strm) {
  if (strm==NULL) {
    G_ExceptionList.lthrow("(in Lisa_Schedule::read) no valid stream",ANY_ERROR);
    return;
  }
  string S;
  int dim;
  bool semiflag=FALSE;
  for (;;)
    {
      S=""; 
      strm >>S;
      if (S=="") 
	{ 
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Schedule.read",END_OF_FILE);
	  return;
	} 
      if (S=="<SCHEDULE>") break;
       }
      // d.h. ich soll weiterlesen:  
   for (;;)
    {
      S=""; 
      strm >>S;  
      if (S=="") 
	{
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Schedule.read",END_OF_FILE);
	  return;
	} 
      if (S=="</SCHEDULE>") break; 
      // d.h. ich soll weiterlesen:  
       if (S=="m=") 
         {  
           strm >> dim; init(n,dim);  //break;
         } 
       if (S=="n=")
	 {
           strm >> dim; init(dim,m);  //break; 
	 }
       if (S=="LR=")
         { 
           if (!LR) make_LR();  
	   strm >> *LR;
//break;  
	 }
       if (S=="NMO=")
         { 
           if (!NMO) make_NMO();   strm >> *NMO;   //break;  
	 }
       if (S=="NJO=")
         { 
           if (!NJO) make_NJO();   strm >> *NJO;   //break;  
	 }
       if (S=="CIJ=")
	 {
	   if (!semiflag) semiactive=FALSE; 
	   if (!CIJ) make_CIJ();
	   strm >> *CIJ; 
	   //break;  
	 }
       if (S=="semiactive=")
	 { 
	   semiflag=TRUE;
           strm >> semiactive;   //break;  
	 }
    }
   valid=TRUE;
}

Lisa_Schedule::~Lisa_Schedule() {
  delete LR;
  delete NMO;
  delete NJO;
  delete CIJ;
}




