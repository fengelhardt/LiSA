/**
 * @author  Per Willenius
 * @version 2.3final
 */ 

#include <fstream>
#include <vector>
#include <stdio.h>

#include "../xml/LisaXmlFile.hpp"
#include "../xml/xmldefs.hpp"


#include "../lisa/ptype.hpp"
#include "../lisa/ctrlpara.hpp"
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

int read(string dname){
  
  
  int oldm=G_Values.get_m();
  int oldn=G_Values.get_n();
  
  ifstream fin(dname.c_str());
  if (fin==NULL) {
    G_ExceptionList.lthrow("Cannot open file '"+dname+"' for reading.",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
  
  /// check what is in inputfile
  string S;
  bool fvalues=false,fproblemtype=false;
  int no_schedules=0;
  for (;;){
    S=""; 
    fin >> S;
    if (S=="") break;
    
    if (S=="<SCHEDULE>") no_schedules++;
    if (S=="<PROBLEMTYPE>") fproblemtype=true;
    if (S=="<VALUES>") fvalues=true ;
  }

  
  // reopen file, start over
  fin.close();
  ifstream in(dname.c_str());
  if (in==NULL) {
    G_ExceptionList.lthrow("Cannot reopen file '"+dname+"' for reading.",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }

  // read problemtype
  if (fproblemtype) { 
    in >> G_ProblemType;
  } else {
    G_ExceptionList.lthrow("No problem defined in file '"+dname+"'.",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
  
  // read problem instance (values)
  if (fvalues) {
    in >> G_Values;
    
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n()) new_mn(); 
    new_problemtype();
    if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n()) new_mn();
    new_values();
  } else {
    new_problemtype();
    return 0;
  }
  
  /// read schedule
  if (no_schedules) {
    
    Lisa_Schedule *mySchedule = 0;
    Lisa_ScheduleNode *myLisa_ScheduleNode;
    // clear old schedules in list
    while(!(G_ScheduleList->empty())){
      Lisa_ScheduleNode dummynode=G_ScheduleList->pop();
      if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
    }
    
    // read new schedules
    for(int i=1;i<=no_schedules;i++) {
      mySchedule = new Lisa_Schedule(1,1);
      in >> (*mySchedule);
      myLisa_ScheduleNode= new Lisa_ScheduleNode(mySchedule);
      G_ScheduleList->append(*myLisa_ScheduleNode);
    }
    
    // make last schedule read the current schedule
    G_Schedule=mySchedule;
    delete G_XSchedule;
    G_XSchedule = new Lisa_XSchedule(G_Schedule);
    new_schedule();
  }

  in.close();
  return 0;
}

//**************************************************************************

int read_xml(string dname) {
  int oldm=G_Values.get_m();
  int oldn=G_Values.get_n();
  ifstream fin(dname.c_str());
  if (fin==NULL) {
    G_ExceptionList.lthrow("cannot open file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
		fin.close();
		LisaXmlFile xmlInput;
		Lisa_ProblemType tmpProb;
		Lisa_Values tmpValues;
		//read the file
		if(!xmlInput.read(dname))
				{
						G_ExceptionList.lthrow("cannot read contents of file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
						return 1;
				}
		//get document type
		LisaXmlFile::DOC_TYPE docType = xmlInput.getDocumentType();
		//check if appropriate
		if( docType != LisaXmlFile::PROBLEM &&
						docType != LisaXmlFile::INSTANCE &&
						docType != LisaXmlFile::SOLUTION)
				{
						G_ExceptionList.lthrow("cannot read contents of file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
						return 1;
				}
		//read problem
		if(!(xmlInput >> tmpProb))
				{
						G_ExceptionList.lthrow("cannot read problem from file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
						return 1;
				}
		xmlInput >> G_ProblemType;
		//are we done ?
		if( docType == LisaXmlFile::PROBLEM)
				{
						new_problemtype();
						return 0;
				}
		//read values
		if(!(xmlInput >> tmpValues))
				{
						G_ExceptionList.lthrow("cannot read values from file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
						return 1;
				}	
		xmlInput >> G_Values;
		if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n())
				new_mn(); 
		new_problemtype();
		if(oldm!=G_Values.get_m()||oldn!=G_Values.get_n())
				new_mn();
		new_values();
		if(docType == LisaXmlFile::INSTANCE)
				return 0;
		//read schedules
		Lisa_List<Lisa_ScheduleNode> schedList;
		if(!(xmlInput >> schedList))
				{
						G_ExceptionList.lthrow("cannot read schedule from file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
						return 1;
				}
		if(schedList.empty())
				return 0;
		//delete all schedules except current one
		//Lisa_Schedule *mySchedule = NULL;
		//ScheduleNode *myScheduleNode;
		//int i;

		while(!(G_ScheduleList->empty())) 
				{
						Lisa_ScheduleNode dummynode;
						dummynode=G_ScheduleList->pop();
						if (dummynode.actual_schedule!=G_Schedule) 
								delete dummynode.actual_schedule;
				}
		//skiped that memory check
		while(!schedList.empty()) {
				//mySchedule = new Lisa_Schedule(1,1);
				//fin >> (*mySchedule);
				//      cout << "myschedule" << *mySchedule << endl;
				//myScheduleNode= new ScheduleNode(mySchedule);
				//G_ScheduleList->append(*myScheduleNode);
				G_ScheduleList->append(schedList.pop());
		}
		
		G_Schedule=G_ScheduleList->last().actual_schedule;
		delete G_XSchedule;
		G_XSchedule = new Lisa_XSchedule(G_Schedule);
		new_schedule();
  return 0;
}

//**************************************************************************

int save(string dname) {
  
  ofstream fout(dname.c_str());
  if (fout==NULL) {
    G_ExceptionList.lthrow("Cannot open file: '"+dname+"' for writing.",Lisa_ExceptionList::Lisa_ExceptionList::END_OF_FILE);
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

int save_xml(string dname) {
  ofstream fout(dname.c_str());
  if (fout==NULL) {
    G_ExceptionList.lthrow("cannot write file: "+dname+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
		fout.close();
		LisaXmlFile::DOC_TYPE docType = LisaXmlFile::IMPLICIT;
		if (G_ProblemType.valid()) {
				docType = LisaXmlFile::PROBLEM;
				if (G_Values.valid ) {
						docType = LisaXmlFile::INSTANCE;
						if (G_Schedule->valid) 
								docType = LisaXmlFile::SOLUTION;
				}
		}
		if(docType == LisaXmlFile::IMPLICIT)
				return 0;
		LisaXmlFile xmlOutput(docType);
		xmlOutput << G_ProblemType;
		if(docType == LisaXmlFile::PROBLEM)
				{
						xmlOutput.write(dname);
						return 0;
				}
		xmlOutput << G_Values;
		if(docType == LisaXmlFile::INSTANCE)
				{
						xmlOutput.write(dname);
						return 0;
				}
		xmlOutput << *G_Schedule;
		xmlOutput.write(dname);
		return 0;
}


void write_alg_call(std::string output_file,
																				Lisa_ProblemType& Problem,
																				Lisa_ControlParameters& Prefs, 
																				Lisa_Schedule& Schedule, 
																				Lisa_Values& Values)
{
		ofstream fout(output_file.c_str());

  fout << Problem;
  fout << Prefs;
  fout << Values;
  fout << Schedule;
  fout.close();
}


void write_alg_call_xml(std::string output_file,
																								Lisa_ProblemType& Problem,
																								Lisa_ControlParameters& Prefs, 
																								Lisa_Schedule& Schedule, 
																								Lisa_Values& Values)
{
		LisaXmlFile::DOC_TYPE type = LisaXmlFile::INSTANCE;
		if(Schedule.valid)
				type = LisaXmlFile::SOLUTION;
		LisaXmlFile xmlOut(type);

  xmlOut << Problem;
		xmlOut << Values;
		xmlOut << Prefs;
		if(Schedule.valid)
				xmlOut << Schedule;
		xmlOut.write(output_file);
}


int read_schedule(std::string fname){
		ifstream fin(fname.c_str());
  if (fin==NULL) { 
    G_ExceptionList.lthrow("file:"+ fname +" cannot be opened");
    return 1;
  }
  int no_schedules=0;
  string S;
  for (;;)
    {
      S=""; 
      fin >>S;
      if (S=="") 
								{ 
										break;
								} 
      if (S=="<SCHEDULE>") no_schedules++;
    }
		
  //  fin.seekg(0);
		fin.close();  
  if (no_schedules>0) {
    ifstream fin( fname.c_str());
    if (fin==NULL) { 
      G_ExceptionList.lthrow("file:"+ fname +" cannot be open");
      return 1;
    }
    //    G_ScheduleList->clear();
				while(!(G_ScheduleList->empty())) 
						{
								Lisa_ScheduleNode dummynode;
								dummynode=G_ScheduleList->pop();
								if (dummynode.actual_schedule!=G_Schedule) delete dummynode.actual_schedule;
						}
    Lisa_Schedule *mySchedule=0;
    Lisa_ScheduleNode *myScheduleNode;
    int i=0;
    for(i=1;i<=no_schedules;i++) {
      mySchedule = new Lisa_Schedule(1,1);
      fin >> (*mySchedule);
      myScheduleNode= new Lisa_ScheduleNode(mySchedule);
      G_ScheduleList->append(*myScheduleNode);
						delete myScheduleNode;
    }
    G_Schedule=mySchedule;
    new_schedule();
    if (G_ExceptionList.empty(Lisa_ExceptionList::END_OF_FILE)==0) {
      G_ExceptionList.lcatch(Lisa_ExceptionList::END_OF_FILE);
      G_ExceptionList.lthrow("no schedule read");
    } else  new_schedule();
  }
  fin.close();  
		return 0;
}

int read_schedule_xml(std::string fname)
{
		ifstream fin( fname.c_str());
  if (fin==NULL) { 
    G_ExceptionList.lthrow("file:"+ fname +" cannot be opened");
    return 1;
  }
  fin.close();
		LisaXmlFile xmlInput;
		if(!(xmlInput.read(fname)))
				{
						G_ExceptionList.lthrow("file:"+ fname +" cannot be read");
						return 1;
				}
		LisaXmlFile::DOC_TYPE docType = xmlInput.getDocumentType();
		Lisa_List<Lisa_ScheduleNode> schedList;
		if(docType != LisaXmlFile::SOLUTION || 
					!(xmlInput >> schedList) ||
					schedList.empty())
				{
						G_ExceptionList.lthrow("file:"+ fname +" contains no schedule");
						return 1;
				}
		
		while(!(G_ScheduleList->empty())) 
				{
						Lisa_ScheduleNode dummynode;
						dummynode=G_ScheduleList->pop();
						if (dummynode.actual_schedule!=G_Schedule) 
								delete dummynode.actual_schedule;
				}
		
		while(!schedList.empty())
				G_ScheduleList->append(schedList.pop());
		
		G_Schedule=G_ScheduleList->last().actual_schedule;
		new_schedule();
		if (G_ExceptionList.empty(Lisa_ExceptionList::END_OF_FILE)==0) {
				G_ExceptionList.lcatch(Lisa_ExceptionList::END_OF_FILE);
				G_ExceptionList.lthrow("no schedule read");
		} 
		else  new_schedule();
		return 0;
}

int get_problemList(std::string filename,
																				std::string type,
																				std::vector<Lisa_ProblemType>& Problems)
{
		ifstream fin(filename.c_str());
  
  if (fin==NULL) {
    G_ExceptionList.lthrow("cannot open file: "+filename+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
		
  string S;
		string starttag,endtag;
  
  if(type=="heuristic"){
    starttag="<HEURISTIC>";
    endtag="</HEURISTIC>";
  }else if(type=="exact"){
    starttag="<EXACT>";
    endtag="</EXACT>";
  }else{
    return 1;
  }
  int number_of_solv_probl = 0;
  // parse files for problem entries
  for (;;){
    
    S=""; 
    fin >> S;
    
    if (S==""){ 
      G_ExceptionList.lthrow("no extry: "+endtag+" in file: "+filename+"\n",Lisa_ExceptionList::END_OF_FILE);
      return 1;
    }else if(S=="<PROBLEMTYPE>"){
      number_of_solv_probl++;
    }else if(S==starttag){
      number_of_solv_probl=0;
    }else if(S==endtag){
      break;
    }
  }
  
  // none found
  if (number_of_solv_probl==0){
    return 1;
		}
		//no more test by reduction, alg problem type and current problem type 
		//have to match exactly
		
		//rewind file
		fin.seekg(0);
		for (;;){
				S=""; 
				fin >>S;
				if (S==starttag) break;
		}
		for (int j=0;j<number_of_solv_probl;j++) {
				Lisa_ProblemType myProblemType;
				fin >> myProblemType;
				Problems.push_back( myProblemType);
		}
		return 0;
}



int get_problemList_xml(std::string filename,
																								std::string htype,
																								std::vector<Lisa_ProblemType>& Problems)
{
		ifstream fin(filename.c_str());
		if (fin==NULL) {
    G_ExceptionList.lthrow("cannot open file: "+filename+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
  }
		fin.close();
		Lisa_ExtAlg Alg;
		if(!LisaXmlFile::read_ExtAlg(Alg,filename)){
				G_ExceptionList.lthrow("cannot parse file: "+filename+"\n",Lisa_ExceptionList::END_OF_FILE);
    return 1;
		}
		if(htype == "heuristic")
				Problems=Alg.handle_heuristic;
		else if(htype == "exact")
				Problems=Alg.handle_exact;
		else return 1;
		return 0;
}

//**************************************************************************

