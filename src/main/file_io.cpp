/**
 * @author  Per Willenius
 * @version 2.3pre3
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
extern class Lisa_List<ScheduleNode> *G_ScheduleList;
extern class Lisa_XSchedule * G_XSchedule;

//**************************************************************************

int read(string dname) {
  int oldm=G_Values.get_m();
  int oldn=G_Values.get_n();
  ifstream fin(dname.c_str());
  if (fin==NULL) {
    G_ExceptionList.lthrow("cannot open file: "+dname+"\n",END_OF_FILE);
    return 1;
  }  
  string S;
  bool fvalues=FALSE,fproblemtype=FALSE;
  int no_schedules=0;
  for (;;)
    {
      S=""; 
      fin >>S;
      if (S=="") 
	{ 
	  break;
	} 
      if (S=="<SCHEDULE>") no_schedules++;
      if (S=="<PROBLEMTYPE>") fproblemtype=TRUE;
      if (S=="<VALUES>") fvalues=TRUE ;
    }

  if (fproblemtype) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("cannot open file: "+dname,END_OF_FILE);
      return 1;
    }
    fin >> G_ProblemType;
  } else {
     G_ExceptionList.lthrow("no Problem defined in: "+dname,END_OF_FILE);
    return 1;
  }
  if (fvalues) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("cannot open file: "+dname,END_OF_FILE);
      return 1;
    }
    fin >> G_Values;
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n())
      new_mn(); 
    new_problemtype();
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n())
      new_mn();
    new_values();
  } else {
    new_problemtype();
    return 0;
  }
  if (no_schedules) {
    ifstream fin(dname.c_str());
    if (fin==NULL) {
      G_ExceptionList.lthrow("cannot open file: "+dname,END_OF_FILE);
      return 1;
    }

    Lisa_Schedule *mySchedule = 0;
    ScheduleNode *myScheduleNode;
    int i;
    //G_ScheduleList->clear();
    while(!(G_ScheduleList->empty())) 
	{
          ScheduleNode dummynode;
          dummynode=G_ScheduleList->pop();
          if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
	}

    if (no_schedules*G_Values.get_m()*G_Values.get_n()>MAX_MEMORY) {
      G_ExceptionList.lthrow("to much schedules to read",WARNING);
      int my_schedules=(int) MAX_MEMORY / (G_Values.get_m()*G_Values.get_n());
      int actual_number=0;
      for(i=1;i<=no_schedules && actual_number<my_schedules;i++) {
	mySchedule = new Lisa_Schedule(1,1);
	fin >> (*mySchedule);
	if (mySchedule->get_property(NO_OF_SINKS) <=1) {
	  myScheduleNode= new ScheduleNode(mySchedule);
	  G_ScheduleList->append(*myScheduleNode);
	  actual_number++;
	}
      }
      cout << actual_number << " schedules read, (maximal " << my_schedules << ")" << endl;
    } else {
      for(i=1;i<=no_schedules;i++) {
	mySchedule = new Lisa_Schedule(1,1);
	fin >> (*mySchedule);
	//      cout << "myschedule" << *mySchedule << endl;
	myScheduleNode= new ScheduleNode(mySchedule);
	G_ScheduleList->append(*myScheduleNode);
      }
    }
      G_Schedule=mySchedule;
      delete G_XSchedule;
      G_XSchedule = new Lisa_XSchedule(G_Schedule);
      new_schedule();
    } else return 0;
  return 0;
}

//**************************************************************************

int save(string dname) {
  ofstream fout(dname.c_str());
  if (fout==NULL) {
    G_ExceptionList.lthrow("cannot write file: "+dname+"\n",END_OF_FILE);
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
  return 0;
}

//**************************************************************************

