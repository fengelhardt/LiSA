/* ************************************************************************ */
/*                            Filename: Read.c                              */
/*                                                                          */
/*     EXPORT: Procedure Read_Data:                                         */
/*                       Einlesen der Daten aus der Datei 'FileName'        */
/* ************************************************************************ */


#include <cstdio>
#include <cstdlib>

#include <fstream>

#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

#include "wo_data.hpp"
#include "wo_table.hpp"
#include "wo_read.hpp"
#include "wo_list.hpp"

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

void Read_Data(char *FileName){
  
  std::ifstream i_strm(FileName);
  if (!i_strm){
    std::cout << "ERROR: cannot open file " << FileName << " for reading." << 
    std::endl;
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
  }
  
  int sum = 0;
  
  for(int i=1;i<=NumOfJobs;i++){
    for(int j=1;j<=NumOfMachines;j++){
      
      if((*Values.SIJ)[i-1][j-1]){
        sum++;
        
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
/*  
 int      i,j,num_of_op_for_job;
 sum = 1;
 FILE     *fp;

 if ((fp = fopen(FileName,"r")) == NIL)
    exit(1);   
*/
 /* ermitteln des Problems (m x n - Problem);
    erfassen der Daten in den globalen Variablen NumOfMachines, NumOfJobs;    
 *//*
 if ( !fscanf(fp,"%d %d ", &NumOfMachines, &NumOfJobs))  
    exit(1);

 for (i = 1; i <= NumOfJobs; ++i) 
 {
    if (!fscanf(fp,"%d", &num_of_op_for_job)) 
      exit(1);

 */  /* ermitteln der Bearbeitungszeit u. Maschinennummer einer Operation;
       erfassen der Daten in der globalen Variablen OpData[];
       Operationen, die auf derselben Maschine bearbeitet werden, kommen in
       eine gemeinsame globale Liste OpsOnMachine[];
       Operationen, die zum selben Job gehoeren, kommen in
       eine gemeinsame globale Liste OpsOfJob[] 
    *//*
    for (j = 1; j <= num_of_op_for_job; ++j)
    {
       if (!fscanf(fp,"%d %d ",&OpData[sum].process_time,
                               &OpData[sum].machine_nr) )
          exit(1);

       OpData[sum].job_nr = i;

       OpsOnMachine[OpData[sum].machine_nr] =
			     Insert(OpsOnMachine[OpData[sum].machine_nr],sum);
       OpsOfJob[OpData[sum].job_nr] = 
			     Insert(OpsOfJob[OpData[sum].job_nr],sum);

       sum ++;
    
    }
 }
 fclose(fp); 
 NumOfOperations = --sum;
  */
}
