/* ************************************************************************ */
/*                            Filename: Read.c                              */
/*                                                                          */
/*     EXPORT: Procedure Read_Data:                                         */
/*                       Einlesen der Daten aus der Datei 'FileName'        */
/* ************************************************************************ */


#include <cstdlib>

#include <fstream>
#include <vector>
#include <utility>

#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../basics/pair.hpp"
#include "../../scheduling/schedule.hpp"

#include "../../xml/LisaXmlFile.hpp"

#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_read.hpp"
#include "wo_list.hpp"
#include "wo_heur.hpp"

/* ************************************************************************ */
/*                           Procedure  Read_Data()                         */
/*                                                                          */
/*   INPUT:  FileName:    Datei, die die Problemdaten enthaelt              */
/*                                                                          */
/*   FUNCTION: Liest die Daten aus der Datei und speichert sie in globalen  */
/*             Variablen.						    */
/*             Wenn das Datei-lesen fehlerfrei verlaeuft, so gibt Read_Data */
/*             den Wert TRUE zurueck.                                       */
/* ************************************************************************ */

//communication objects
Lisa_ProblemType Problem;
Lisa_ControlParameters Parameter;   
Lisa_Values Values;

//machine and job order
Lisa_Matrix<int> *MO,*JO;

//find machine and job for an operation
std::vector<std::pair<int,int> > lookup;

void Read_Data(char *FileName){
  
  std::ifstream i_strm(FileName);
  if (!i_strm){
    G_ExceptionList.lthrow((std::string)"Cannot open file "+FileName+" for reading.");    
    exit(1);
  }
  i_strm.close();
  
  //initialize class
  LisaXmlFile::initialize();
  //create Input handler
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
  
  //parse xml file
  xmlInput.read(FileName);
  //determine document type
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
  
  //check for successful parsing and valid document type
  if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION)){
    G_ExceptionList.lthrow("Cannot read input.",
    Lisa_ExceptionList::INCONSISTENT_INPUT);
  }
  if( !(xmlInput >> Problem)){ //get Problem
    G_ExceptionList.lthrow("Cannot read ProblemType.",
    Lisa_ExceptionList::INCONSISTENT_INPUT);
  }
  if( !(xmlInput >> Parameter)){ //get ControlParameters
    G_ExceptionList.lthrow("Cannot read ControlParameters.",
    Lisa_ExceptionList::INCONSISTENT_INPUT);
  }
  if( !(xmlInput >> Values)){ //get Values
    G_ExceptionList.lthrow("Cannot read Values.",
    Lisa_ExceptionList::INCONSISTENT_INPUT);
  }
  if (!G_ExceptionList.empty()) exit(1); // something went wrong
  
  NumOfMachines = Values.get_m();
  NumOfJobs = Values.get_n();
  
  if(NumOfMachines > MaxNumOfMachines || NumOfJobs > MaxNumOfMachines ||
     NumOfMachines*NumOfJobs > MaxNumOfOperations){
    G_ExceptionList.lthrow((std::string) "Can not handle more than "+
                           ztos(MaxNumOfMachines)+" machines and "+
                           ztos(MaxNumOfJobs)+" jobs, you may want to "+
                           "recompile.",Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(1);
  }
  
  //which heuristic ??
  minmax_match = 1;
  head_match = 0;
  Heuristic_Schedule = &Heuristic_Schedule_MATCHING;
  if(Parameter.defined("HEURISTIC") == Lisa_ControlParameters::STRING){
    if(Parameter.get_string("HEURISTIC") == "MIN_MATCHING"){
      Heuristic_Schedule = &Heuristic_Schedule_MATCHING;
      minmax_match = 1;
    }else if(Parameter.get_string("HEURISTIC") == "MAX_MATCHING"){
      Heuristic_Schedule = &Heuristic_Schedule_MATCHING;
      minmax_match = -1;
    }else if(Parameter.get_string("HEURISTIC") == "MIN_BOTTLE_MATCHING"){
      Heuristic_Schedule = &Heuristic_Schedule_BOTTLE_MATCHING;
      minmax_match = 1;
      head_match=0;
    }else if(Parameter.get_string("HEURISTIC") == "MAX_BOTTLE_MATCHING"){
      Heuristic_Schedule = &Heuristic_Schedule_BOTTLE_MATCHING;
      minmax_match = -1;
      head_match = 0;
    }else if(Parameter.get_string("HEURISTIC") == "MIN_BOTTLE_MATCHING_VAR"){
      Heuristic_Schedule = &Heuristic_Schedule_BOTTLE_MATCHING;
      minmax_match = 1;
      head_match = 1;
    }else if(Parameter.get_string("HEURISTIC") == "MAX_BOTTLE_MATCHING_VAR"){
      Heuristic_Schedule = &Heuristic_Schedule_BOTTLE_MATCHING;
      minmax_match = -1;
      head_match = 1;
    }else if(Parameter.get_string("HEURISTIC") == "LB_PREC_RULE"){
      Heuristic_Schedule = &Heuristic_Schedule_LB_PREC_RULE;
    }else if(Parameter.get_string("HEURISTIC") == "LB_PREC_RULE_VAR"){
      Heuristic_Schedule = &Heuristic_Schedule_LB_PREC_RULE_VAR;
    }else{
      G_ExceptionList.lthrow("Unknows value for HEURISTIC Parameter, using defaults.",
                             Lisa_ExceptionList::WARNING);  
    }
  }else{
    G_ExceptionList.lthrow("Could not find HEURISTIC Parameter, using defaults.",
                           Lisa_ExceptionList::WARNING);
  }
  
  // write data into algorithm data structures
  int sum = 0;
  for(int i=1;i<=NumOfJobs;i++){
    for(int j=1;j<=NumOfMachines;j++){
      
      if((*Values.SIJ)[i-1][j-1]){
        sum++;
        
        lookup.push_back(std::pair<int,int>(i-1,j-1));
        
        OpData[sum].process_time = (int) (*Values.PT)[i-1][j-1];
        OpData[sum].machine_nr = j;
        OpData[sum].job_nr = i;
        
        OpsOnMachine[OpData[sum].machine_nr] =
			     Insert(OpsOnMachine[OpData[sum].machine_nr],sum);
        OpsOfJob[OpData[sum].job_nr] = 
			     Insert(OpsOfJob[OpData[sum].job_nr],sum);
      }
    
    }
  }
  
  NumOfOperations = sum;
  MO = new Lisa_Matrix<int>(NumOfJobs,NumOfMachines);
  JO = new Lisa_Matrix<int>(NumOfJobs,NumOfMachines); 
}

void Set_Solution(List** machines, List** jobs){
  MO->fill(0);
  JO->fill(0);
  
  List* help;
  int pos;
  
  // create job and machine order
  for(int i=1;i<=NumOfJobs;i++){
    help = jobs[i];
    pos = 0;
    while(help != NIL){
      pos++;
      (*JO)[i-1][lookup[help->number-1].second] = pos;
      help = help->next;
    }
  }
  
  for(int i=1;i<=NumOfMachines;i++){
    help = machines[i];
    pos = 0;
    while(help != NIL){
      pos++;
      (*MO)[lookup[help->number-1].first][i-1] = pos;
      help = help->next;
    }
  }
  
}

void Write_Solution(char * FileName){

  Lisa_Schedule Schedule(NumOfJobs,NumOfMachines);
  Schedule.make_LR();
  bool feasible  = Schedule.MO_JO_to_LR(Schedule.LR,Values.SIJ,MO,JO);
  
  delete MO,JO;
    
  std::ofstream o_strm(FileName);
  if (!o_strm){
    G_ExceptionList.lthrow((std::string) "Could not open "+FileName+
                           " for writing.");
    exit(1);
  }
  o_strm.close();
  
  LisaXmlFile::DOC_TYPE type = LisaXmlFile::INSTANCE;
  if(feasible) type = LisaXmlFile::SOLUTION;
  LisaXmlFile xmlOutput(type);
  xmlOutput << Problem << Values << Parameter;
  if(feasible) xmlOutput << Schedule;
  xmlOutput.write(FileName);
}

