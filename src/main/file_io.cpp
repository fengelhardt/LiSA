/**
 * @author  Per Willenius
 * @version 2.3rc1
 */ 

#include <fstream>
#include <stdio.h>

#include "../lisa/ptype.hpp"
#include "../lisa/lvalues.hpp"
#include "../misc/except.hpp"
#include "../desktop/callback.hpp"
#include "../scheduling/xsched.hpp"
#include "../scheduling/schedule.hpp"

#include "file_io.hpp"

using namespace std;

//**************************************************************************

extern class Lisa_ProblemType G_ProblemType;
extern class Lisa_Values G_Values;
extern class Lisa_Schedule *G_Schedule;
extern class Lisa_List<Lisa_ScheduleNode> *G_ScheduleList;
extern class Lisa_XSchedule * G_XSchedule;

//**************************************************************************

int read(string dname) {
  int oldm=G_Values.get_m();
  int oldn=G_Values.get_n();
  
  ifstream fin(dname.c_str());
  if (fin==NULL) {
    G_ExceptionList.lthrow("Cannot open file '"+dname+"' for reading.",END_OF_FILE);
    return 1;
  }
  
  /// check what is in inputfile
  string S;
  bool fvalues=FALSE,fproblemtype=FALSE;
  int no_schedules=0;
  for (;;){
    S=""; 
    fin >> S;
    if (S=="") break;
    
    if (S=="<SCHEDULE>") no_schedules++;
    if (S=="<PROBLEMTYPE>") fproblemtype=TRUE;
    if (S=="<VALUES>") fvalues=TRUE ;
  }
  
  // read problemtype
  if (fproblemtype) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("Cannot open file '"+dname+"' for reading.",END_OF_FILE);
      return 1;
    }
    
    fin >> G_ProblemType;
  } else {
    G_ExceptionList.lthrow("No problem defined in file '"+dname+"'.",END_OF_FILE);
    return 1;
  }
  
  // read problem instance (values)
  if (fvalues) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("Cannot open file '"+dname+"' for reading.",END_OF_FILE);
      return 1;
    }
    
    fin >> G_Values;
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n()) new_mn(); 
    new_problemtype();
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n()) new_mn();
    new_values();
  } else {
    new_problemtype();
    return 0;
  }
  
  /// read values
  if (no_schedules) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("Cannot open file '"+dname+"' for reading.",END_OF_FILE);
      return 1;
    }
    
    Lisa_Schedule *mySchedule = 0;
    Lisa_ScheduleNode *myLisa_ScheduleNode;
    // clear old schedules in list
    while(!(G_ScheduleList->empty())){
      Lisa_ScheduleNode dummynode;
      dummynode=G_ScheduleList->pop();
      if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
    }
    
    // read new schedules
    for(int i=1;i<=no_schedules;i++) {
      mySchedule = new Lisa_Schedule(1,1);
      fin >> (*mySchedule);
      myLisa_ScheduleNode= new Lisa_ScheduleNode(mySchedule);
      G_ScheduleList->append(*myLisa_ScheduleNode);
    }
    
    // make last schedule read the current schedule
    G_Schedule=mySchedule;
    delete G_XSchedule;
    G_XSchedule = new Lisa_XSchedule(G_Schedule);
    new_schedule();
  }
  
  fin.close();
  return 0;
}

//**************************************************************************

int save(string dname) {
  
  ofstream fout(dname.c_str());
  if (fout==NULL) {
    G_ExceptionList.lthrow("Cannot open file: '"+dname+"' for writing.",END_OF_FILE);
    return 1;
  }
  
  if (G_ProblemType.valid()) {
    fout << G_ProblemType;
    if (G_Values.valid ) {
      fout << G_Values;
      if (G_Schedule->valid) 
        fout << *G_Schedule;
    }
  } 
  
  fout.close();
  return 0;
}

//**************************************************************************

