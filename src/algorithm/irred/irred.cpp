/**
 * @author  Marc Moerig
 * @version 2.3final
 */

#include <stdlib.h>
#include <unistd.h>

#include <iostream>
#include <fstream>

#include "../../main/global.hpp"
#include "../../basics/list.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/mo_jo.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

#include "../../xml/LisaXmlFile.hpp"

#include "convertgraph.hpp"
#include "irrednode.hpp"
#include "irredtest.hpp"

using namespace std;

//**************************************************************************

int main(int argc, char *argv[]){

  G_ExceptionList.set_output_to_cout();
  cout << "PID= " << getpid() << endl; 
  
  // open files and assure existence:
  if (argc != 3) {
    cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
    exit(1);
  }

  ifstream i_strm(argv[1]);
  ofstream o_strm(argv[2]);
  if (!i_strm){
    cout << "ERROR: cannot open file " << argv[1] << " for reading." << endl;
    exit(1);
  }
  if(!o_strm){
    cout << "ERROR: cannot open file " << argv[2] << " for writing." << endl;
    exit(1);
  }
  i_strm.close();
  o_strm.close();
  
  Lisa_ProblemType lpr;
  Lisa_ControlParameters sp;
  Lisa_Values my_werte; 
  Lisa_Schedule my_schedule; 
  
  //initialize class
  LisaXmlFile::initialize();
  //create Input handler
  LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    
  //parse xml file
  xmlInput.read(argv[1]);
  //determine document type
  LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
     
  //check for successful parsing and valid document type
  if (!xmlInput || !type == LisaXmlFile::SOLUTION){
    cout << "ERROR: cannot read input, aborting program." << endl;
    exit(1); 
  }
  //get Problem
  if( !(xmlInput >> lpr)){
    cout << "ERROR: cannot read ProblemType, aborting program." << endl;
    exit(1);
  }
  //get ControlParameters
  if( !(xmlInput >> sp)){
    cout << "ERROR: cannot read ControlParameters, aborting program." << endl;
    exit(1);
  }
  //get Values
  if( !(xmlInput >> my_werte)){
    cout << "ERROR: cannot read Values, aborting program." << endl;
    exit(1);
  }
  if( !(xmlInput >> my_schedule)){
    cout << "ERROR: cannot read Schedule, aborting program." << endl;
    exit(1);
  }
  // if something else went wrong
  if (!G_ExceptionList.empty()){
    cout << "ERROR: cannot read input, aborting program." << endl;
    exit(1);
  }
  
  // parse control parameters: 
  int alg_param = Lisa_IrreducibilityTest::JUST_TEST;
  if (sp.defined("TYPE")==Lisa_ControlParameters::STRING){
    if (sp.get_string("TYPE") == "ALL_REDUCING"){
      alg_param = Lisa_IrreducibilityTest::GENERATE_ALL;
    }else if(sp.get_string("TYPE") == "ITERATIVE_REDUCING"){
    }else if(sp.get_string("TYPE") == "SIMILAR"){
      alg_param = Lisa_IrreducibilityTest::GENERATE_SIMILAR;
    }else{
      cout << "WARNING: TYPE value out of Range, using default." << endl;
    }
  }else cout <<"WARNING: Could not read TYPE parameter, using default."<< endl;
  
  bool return_all = 0;
  if (sp.defined("RETURN")==Lisa_ControlParameters::STRING){
    if (sp.get_string("RETURN") == "ALL"){
      return_all = 1;
    }else if(sp.get_string("RETURN") == "ONLY_IRREDUCIBLE"){
    }else{
      cout << "WARNING: RETURN value out of Range, using default." << endl;
    }
  }else cout<<"WARNING: Could not read RETURN parameter, using default."<<endl;
  if (alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR) return_all = 1;

  if (sp.defined("RNDM")==Lisa_ControlParameters::STRING){
    if (sp.get_string("RNDM") == "YES"){ 
      if(alg_param==Lisa_IrreducibilityTest::JUST_TEST){ 
        alg_param = Lisa_IrreducibilityTest::JUST_TEST_RANDOM;
      }
    }else if(sp.get_string("RNDM") == "NO"){
    }else{
      cout << "WARNING: RNDM value out of Range, using default." << endl;
    }
  }else cout << "WARNING: Could not read RNDM parameter, using default."<<endl;
 



  delete my_schedule.CIJ;
  my_schedule.CIJ = 0;

  Lisa_ConvertGraph* convert = 
              Lisa_ConvertGraph::make_object(&lpr,my_werte.SIJ,my_werte.MO);
  
  if (!G_ExceptionList.empty()) exit(1);
 
  int v = (*convert->get_disjkt()).get_vertices();
  Lisa_MatrixGraph pg(v);
  convert->plan2graph(my_schedule.LR,&pg);
  
  Lisa_IrredResult* res;
  if (alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR){ 
    res=new Lisa_IrredResult(Lisa_IrredResult::NO_FILTER);
  }else if(return_all){ 
    res =new Lisa_IrredResult(Lisa_IrredResult::ALL);
  }else{ 
    res = new Lisa_IrredResult(Lisa_IrredResult::POTENTIALLY_IRREDUCIBLE);
  }

  Lisa_IrreducibilityTest my_test(convert->get_disjkt());
  my_test.set_output_to(res);
  bool irre = my_test.test(&pg,alg_param);

  Lisa_List<Lisa_ScheduleNode> SchedList;  
    
  if (irre && alg_param!=Lisa_IrreducibilityTest::GENERATE_SIMILAR){
    cout << "WARNING: Plan is irreducible !" << endl;
    SchedList.append(Lisa_ScheduleNode(&my_schedule));
  }else{
    if(alg_param==Lisa_IrreducibilityTest::JUST_TEST ||
       alg_param==Lisa_IrreducibilityTest::JUST_TEST_RANDOM){
         
      res->results->reset();
      do{
        Lisa_IrredNode* curr = res->results->get();
        if(curr->status == Lisa_IrredNode::UNKNOWN){
          irre = my_test.test(curr->plangraph,curr->compgraph,alg_param);
          if (irre) curr->status = Lisa_IrredNode::IRREDUCIBLE;
          else curr->status = Lisa_IrredNode::NOT_IRREDUCIBLE;
        }
      }while(res->results->next());
      
    }else{
      
      if(!return_all){
        res->compare_all();
        res->delete_reducible();
        res->results->reset();
        do{
          res->results->get()->status = Lisa_IrredNode::IRREDUCIBLE; 
        }while(res->results->next());
      }
      
    }
    
    if(alg_param==Lisa_IrreducibilityTest::GENERATE_SIMILAR){ 
      if(irre) cout << "WARNING: Plan is irreducible !" << endl;
      SchedList.append(Lisa_ScheduleNode(&my_schedule));
    }
    
    res->results->reset();
    do{
      if(res->results->get()->status == Lisa_IrredNode::IRREDUCIBLE){
        convert->graph2plan(res->results->get()->plangraph,my_schedule.LR);
        SchedList.append(Lisa_ScheduleNode(&my_schedule));
      }else if(return_all){
        convert->graph2plan(res->results->get()->plangraph,my_schedule.LR);
        SchedList.append(Lisa_ScheduleNode(&my_schedule));
      }
    }while(res->results->next());
    //  cout << "WARNING: Plan is NOT irreducible ! " << count 
    //       << " irreducible plan(s) found." <<endl;
  }
  
  LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
  xmlOutput << lpr << my_werte << sp << SchedList;
  xmlOutput.write(argv[2]);
   
  delete convert;
  delete res;
  
}

//**************************************************************************

