/**
 * @author  Marc Moerig
 * @version 2.3final
 */ 

//**************************************************************************

#include <cstdlib>
#include <ctime>

#include <string>
#include <list>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>

#include "../../misc/int2str.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../scheduling/os_sched.hpp"
#include "../../xml/LisaXmlFile.hpp"

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

//**************************************************************************

int m=10,n=10;
long timeseed,machseed;
int minpt=1,maxpt=100;
int numberproblems=1;
int numberalgorithms=1;
long seedDD, seedWI, seedRI, modeRIDI=0;
int iMinDD=1,iMaxDD=99,iMinWi=1,iMaxWi=99,iMinRI=1,iMaxRI=99;
bool bNeedDD=false,bNeedWI=false,bNeedMO=false;
bool bIsSetDD=false,bIsSetWI=false;
double dTFactor=1.5;
std::string algin,algout;

//**************************************************************************

bool child_terminated=false;

void child_terminates(int i){ child_terminated=true; }


//**************************************************************************

// get parameters into global var's, if undefined and add default/generated
// values to controlparameters, so they are in the output later
void parseParameters(Lisa_ControlParameters &cp){
  
  if(cp.defined("TIMESEED")==Lisa_ControlParameters::LONG){
    timeseed = cp.get_long("TIMESEED");
  }else{
    timeseed = time(0);
    cp.add_key("TIMESEED",timeseed);
    G_ExceptionList.lthrow((std::string)"No TIMESEED parameter defined,"+
                           " generated '"+ztos(timeseed)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("MACHSEED")==Lisa_ControlParameters::LONG){
    machseed = cp.get_long("MACHSEED");
  }else{
    srand(time(0));
    machseed = rand();
    cp.add_key("MACHSEED",machseed);
    G_ExceptionList.lthrow((std::string)"No MACHSEED parameter defined,"+
                           " generated '"+ztos(machseed)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("DDSEED")==Lisa_ControlParameters::LONG){
    seedDD = cp.get_long("DDSEED");
  }else{
    seedDD = time(0);
    cp.add_key("DDSEED",seedDD);
    G_ExceptionList.lthrow((std::string)"No DDSEED parameter defined,"+
                           " generated '"+ztos(seedDD)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("WISEED")==Lisa_ControlParameters::LONG){
    seedWI = cp.get_long("WISEED");
  }else{
    seedWI = time(0);
    cp.add_key("WISEED",seedWI);
    G_ExceptionList.lthrow((std::string)"No WISEED parameter defined,"+
                           " generated '"+ztos(seedWI)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("RISEED")==Lisa_ControlParameters::LONG){
    seedRI = cp.get_long("RISEED");
  }else{
    seedRI = time(0);
    cp.add_key("RISEED",seedRI);
    G_ExceptionList.lthrow((std::string)"No RISEED parameter defined,"+
                           " generated '"+ztos(seedRI)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("M")==Lisa_ControlParameters::LONG){
    m = cp.get_long("M");
  }else{
    cp.add_key("M",(long)m);
    G_ExceptionList.lthrow((std::string)"No M parameter defined,"+
                           " using default '"+ztos(m)+"'.",Lisa_ExceptionList::WARNING);
  }
  
  if(cp.defined("N")==Lisa_ControlParameters::LONG){
    n = cp.get_long("N");
  }else{
    cp.add_key("N",(long)n);
    G_ExceptionList.lthrow((std::string)"No N parameter defined,"+
                           " using default '"+ztos(n)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("MINPT")==Lisa_ControlParameters::LONG){
    minpt = cp.get_long("MINPT");
  }else{
    cp.add_key("MINPT",(long)minpt);
    G_ExceptionList.lthrow((std::string)"No MINPT parameter defined,"+
                           " using default '"+ztos(minpt)+"'.",Lisa_ExceptionList::WARNING);
  }  
  
  if(cp.defined("MAXPT")==Lisa_ControlParameters::LONG){
    maxpt = cp.get_long("MAXPT");
  }else{
    cp.add_key("MAXPT",(long)maxpt);
    G_ExceptionList.lthrow((std::string)"No MAXPT parameter defined,"+
                           " using default '"+ztos(maxpt)+"'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("NUMBERPROBLEMS")==Lisa_ControlParameters::LONG){
    numberproblems = cp.get_long("NUMBERPROBLEMS");
  }else{
    cp.add_key("NUMBERPROBLEMS",(long)numberproblems);
    G_ExceptionList.lthrow((std::string)"No NUMBERPROBLEMS parameter "+
                           "defined, using default '"+ztos(numberproblems)+
                           "'.",Lisa_ExceptionList::WARNING);
  }  
  
  if(cp.defined("NUMBERALGORITHMS")==Lisa_ControlParameters::LONG){
    numberalgorithms = cp.get_long("NUMBERALGORITHMS");
  }else{
    cp.add_key("NUMBERALGORITHMS",(long)numberalgorithms);
    G_ExceptionList.lthrow((std::string)"No NUMBERALGORITHMS parameter "+
                           "defined, using default '"+ztos(numberalgorithms)+
                           "'.",Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("MINDD")==Lisa_ControlParameters::LONG){
    iMinDD = cp.get_long("MINDD");
  }

  if(cp.defined("MAXDD")==Lisa_ControlParameters::LONG){
	bIsSetDD = true;
    iMaxDD = cp.get_long("MAXDD");
  }

  if(cp.defined("MINWI")==Lisa_ControlParameters::LONG){
    iMinWi = cp.get_long("MINWI");
  }

  if(cp.defined("MAXWI")==Lisa_ControlParameters::LONG){
    iMaxWi = cp.get_long("MAXWI");
	bIsSetWI=true;
  }

  if(cp.defined("MINRI")==Lisa_ControlParameters::LONG){
    iMinRI = cp.get_long("MINRI");
  }

  if(cp.defined("MAXRI")==Lisa_ControlParameters::LONG){
    iMaxRI = cp.get_long("MAXRI");
  }

  if(cp.defined("RIDIMODE")==Lisa_ControlParameters::LONG){
    modeRIDI = cp.get_long("RIDIMODE");
  }else{
    cp.add_key("RIDIMODE",(long)modeRIDI);
    G_ExceptionList.lthrow((std::string)"No RIDIMODE parameter defined. Using default r_i//d_i generation.",
		                   Lisa_ExceptionList::WARNING);
  }

  if(cp.defined("TFACTOR")==Lisa_ControlParameters::DOUBLE){
    dTFactor = cp.get_double("TFACTOR");
  }
}

//**************************************************************************

// check if we can handle given problemtype, otherwise exit
void checkProblemType(Lisa_ProblemType &pt){
  Lisa_ProblemType compt;
  
  for(int i=0;i<6;i++){
	  switch(i){
	  case 0:	compt.set_property(M_ENV,O);
				break;
	  case 1:	compt.set_property(M_ENV,J);
				bNeedMO=true;
				break;
	  case 2:	compt.set_property(M_ENV,F);
				break;
	  case 3:	compt.set_property(RI,true);
                compt.set_property(M_ENV,O);
                bNeedMO=false;
				break;
	  case 4:	compt.set_property(RI,true);
                compt.set_property(M_ENV,J);
				bNeedMO=true;
				break;
	  case 5:	compt.set_property(RI,true);
                compt.set_property(M_ENV,F);
				break;
	  }
	  compt.set_property(OBJECTIVE,CMAX);
	  if(pt.output_problem() == compt.output_problem()) return;

	  compt.set_property(OBJECTIVE,LMAX);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedDD=true;
		  return;
	  }

	  compt.set_property(OBJECTIVE,SUM_CI);
	  if(pt.output_problem() == compt.output_problem()) return;
 
	  compt.set_property(OBJECTIVE,SUM_WICI);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedWI=true;
		  return;
	  }
 
	  compt.set_property(OBJECTIVE,SUM_TI);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedDD=true;
		  return;
	  }
 
	  compt.set_property(OBJECTIVE,SUM_WITI);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedDD=true;
		  bNeedWI=true;
		  return;
	  }
	  compt.set_property(OBJECTIVE,SUM_UI);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedDD=true;
		  return;
	  }
 
	  compt.set_property(OBJECTIVE,SUM_WIUI);
	  if(pt.output_problem() == compt.output_problem()){
		  bNeedDD=true;
		  bNeedWI=true;
		  return;
	  }
  }
 
  G_ExceptionList.lthrow((std::string)"Cannot handle '"+pt.output_alpha()+
                          " / "+pt.output_beta()+" / "+pt.output_gamma()+
                          "'.",Lisa_ExceptionList::INCONSISTENT_INPUT);
  exit(-1);
}

//**************************************************************************

//struct to hold parameters for one algorithm
struct algParameters{
  Lisa_ControlParameters cp;
  std::string executable;
  long timelimit;
  std::list<int> startfrom;

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  
  // get numbers from sf string, syntax has to be
  // {2,4,5,6} _without_ spaces
  void parseStartFrom(const std::string& sf,const int i){
    startfrom.clear();
    
    std::stringstream strm(sf);
    
    char c= ' ';
    int j = 0;
    strm >> c;
    if(c != '{'){
      G_ExceptionList.lthrow((std::string)"Could not parse AUTOALG_START_FROM parameter '"+sf+"' for algorithm "+ztos(i+1)+" ."
                             ,Lisa_ExceptionList::INCONSISTENT_INPUT);
      exit(-1);
    }      
    
    do{
      
      strm >> j;
      if(!strm.fail()){
        if(j-1>=i || j-1 < -1){
          G_ExceptionList.lthrow((std::string)"Can not use output from algorithm "+ztos(j)+" as input for algorithm "+ztos(i+1)+" ."
                                 ,Lisa_ExceptionList::INCONSISTENT_INPUT);
          exit(-1);
        }
        if(j-1 > 0) startfrom.push_back(j-1);
      }else{
        G_ExceptionList.lthrow((std::string)"Could not parse AUTOALG_START_FROM parameter '"+sf+"' for algorithm "+ztos(i+1)+" ."
                               ,Lisa_ExceptionList::INCONSISTENT_INPUT);
        exit(-1); 
      }
      
      strm >> c;
      if(c != '}' && c != ','){
        G_ExceptionList.lthrow((std::string)"Could not parse AUTOALG_START_FROM parameter '"+sf+"' for algorithm "+ztos(i+1)+" ."
                               ,Lisa_ExceptionList::INCONSISTENT_INPUT);
        exit(-1);
      }  
      
    }while(c != '}' && !strm.eof());
    
    if(strm.eof()){
      G_ExceptionList.lthrow((std::string)"Could not parse AUTOALG_START_FROM parameter '"+sf+"' for algorithm "+ztos(i+1)+" ."
                             ,Lisa_ExceptionList::INCONSISTENT_INPUT);
      exit(-1);    
    }else{
      strm >> c;
      if(!strm.fail()){
        G_ExceptionList.lthrow((std::string)"Could not parse AUTOALG_START_FROM parameter '"+sf+"' for algorithm "+ztos(i+1)+" ."
                               ,Lisa_ExceptionList::INCONSISTENT_INPUT);
        exit(-1);
      }
    }
      
  }
  
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

  //check if controlparameters contain an algorithm and if the executable exists
  //exit otherwise
  void checkAlgo(const int i){
  
    if(cp.defined("AUTOALG_EXECUTABLE")==Lisa_ControlParameters::STRING){
      executable = cp.get_string("AUTOALG_EXECUTABLE");
      cp.remove_key("AUTOALG_EXECUTABLE");
    }else{
      G_ExceptionList.lthrow((std::string)"No AUTOALG_EXECUTABLE parameter defined for algorithm "+ztos(i+1)+"."
                             ,Lisa_ExceptionList::INCONSISTENT_INPUT);
      exit(-1);
    }
    
    if(cp.defined("AUTOALG_TIMELIMIT")==Lisa_ControlParameters::LONG){
      timelimit = cp.get_long("AUTOALG_TIMELIMIT");
      cp.remove_key("AUTOALG_TIMELIMIT");
    }else{
      G_ExceptionList.lthrow((std::string)"No AUTOALG_TIMELIMIT parameter defined for algorithm "+ztos(i+1)+" '"+
                             executable +"', using default '0'.",Lisa_ExceptionList::WARNING);
      timelimit = 0;
    }
  
    if(cp.defined("AUTOALG_START_FROM")==Lisa_ControlParameters::STRING){
      std::string sf = cp.get_string("AUTOALG_START_FROM");
      cp.remove_key("AUTOALG_START_FROM");
      parseStartFrom(sf,i);
    }else{
      G_ExceptionList.lthrow((std::string)"No AUTOALG_START_FROM parameter defined for algorithm "+ztos(i+1)+" '"+
                             executable +"', using default '{"+ztos(i)+"}'.",Lisa_ExceptionList::WARNING);
      if(i>0) startfrom.push_back(i-1);
    }
    
  /*add some useful !!! testing if executable exists here -marc-  
    std::ifstream testfile(executable.c_str());
    if(testfile){
      testfile.close();
    }else{
      G_ExceptionList.lthrow((std::string)"Executable '"+
                             executable+"' doesn't seem to exist.",
                             Lisa_ExceptionList::INCONSISTENT_INPUT);
      exit(-1); 
    } */
  }

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

};

//**************************************************************************

// generate values with respect to seed,m,n,minpt,maxpt and the 
// problemtype .. only problemtypes that can be handled here should be accepted 
// by checkProblemType()
void generateValues(Lisa_Values &val,Lisa_ProblemType &pt){
  long lSumPij=0;
  int i,j;

  val.init(n,m);
  val.make_PT();
  val.make_SIJ();
  val.SIJ->fill(1);

  
  if(bNeedWI){
	  if(!bIsSetWI)
		G_ExceptionList.lthrow((std::string)"No MAXWI parameter defined,"+
                           " using default '"+ztos(iMaxWi)+"'.",Lisa_ExceptionList::WARNING);
	  val.make_WI();
	  if(iMinWi==iMaxWi){
		  val.WI->fill(iMinWi);
	  }
	  else{
		  Lisa_Vector<double> SwapVector(n);
		  for(i=0; i<n; i++){
			  SwapVector[i] = lisa_random((long)iMinWi,(long)iMaxWi, &seedWI);
		  }
 		  *val.WI = SwapVector;
	  }
  }

  if(bNeedMO){
	  int s,t;
	  val.make_MO();
	  Lisa_Matrix<int> MO(n,m);
	  MO.fill(0);
	  if(pt.get_property(M_ENV) == J){
		  // taken from tcl_c.cpp, TC_gen_mo
		  int k,index;
		  for (j=0; j<n; j++){
			  for (k=1; k<=m; k++){
				  for (index=lisa_random((long)0,(long)m,&timeseed); index <m;index++){
					  if(MO[j][index]==0)
						  break;
				  }
				if(index>=m){
					for (index=0; index <m;index++)
						if(MO[j][index]==0)
							break;
				}
				MO[j][index]=k;
			  }
		  }
	  }
	  else{
		  for (s=0; s<n; s++){
			  for(t=0; t<m; t++){
				MO[s][t]=t+1;
			  }
		  }
	  }
	  val.MO->read_rank(&MO);
  }

  // taken from tcl_c.cpp, TC_genpt()
  if(minpt==maxpt){
    val.PT->fill(minpt);
  }else{
    
    Lisa_Vector<int> zeg(m), mg(m);
    for(j=0; j<n; j++) {
      
      for(i=0; i<m; i++) {
        zeg[i] = lisa_random((long)minpt,(long)maxpt, &timeseed);
        mg[i]=i;
      }
      
      for(i=0; i<m; i++) {
        int u = lisa_random(i+1, m, &machseed) -1 ;
        int temp = mg[i];
        mg[i]=mg[u];
        mg[u]=temp;
      }
      
      for(i=0; i<m; i++) if((*val.SIJ)[j][mg[i]]) (*val.PT)[j][mg[i]]= zeg[i];
    }
  
  }

  if(pt.get_property(RI)){
	  if(!(iMinRI+iMaxRI))
		  pt.set_property(RI,false);
	  else{
		  val.make_RD();
		  if(iMinRI==iMaxRI)
			  val.RD->fill(iMinWi);
		  else{
			  Lisa_Vector<double> SwapVector(n);
			  if(modeRIDI == 1){
				  iMinRI = 0;
				  for(j=0; j<n; j++)
					  for(i=0; i<m; i++){
						  lSumPij += long((*val.PT)[j][i]);
					  }
				  iMaxRI = lSumPij /(2*n);
			  }
			  for(i=0; i<n; i++)
				  SwapVector[i] = lisa_random((long)iMinRI,(long)iMaxRI, &seedRI);
			  *val.RD = SwapVector;
		  }
	  }
  }


  if(bNeedDD){
	  if(!bIsSetDD){
		G_ExceptionList.lthrow((std::string)"No MAXDD parameter defined,"+
                           " using default '"+ztos(iMaxDD)+"'.",Lisa_ExceptionList::WARNING);
	  }
	  val.make_DD();
	  if(iMinDD==iMaxDD){
		val.DD->fill(iMinDD);
	  }
	  else{
		  //TODO:Couldnt directly change the values of the DD/WI vector?!?
		  Lisa_Vector<double> SwapVector2(n);
		  if(modeRIDI == 1){
			  for(i=0; i<n; i++){
				  lSumPij = 0;
				  for(j=0;j<m;j++)
					  lSumPij += (long)(*val.PT)[i][j];
				  SwapVector2[i] = long((*val.RD)[i] + dTFactor*lSumPij);
			  }
		  }
		  else{
			  for(i=0; i<n; i++) {
				SwapVector2[i] = lisa_random((long)iMinDD,(long)iMaxDD, &seedDD);
			  }
		  }
		  *val.DD = SwapVector2;
	  }
  }
}

//**************************************************************************

//create an input file, clean output file
void writeAlgInput(Lisa_ProblemType &pt,Lisa_ControlParameters &cp,
                   Lisa_Values &val,Lisa_List<Lisa_ScheduleNode> &sched){

  std::ofstream out_file(algout.c_str());
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for writing.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  out_file.close();
  
  std::ofstream in_file(algin.c_str());
  if(!in_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algin+
                           "' for writing.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  in_file.close();
  
  LisaXmlFile::DOC_TYPE type = LisaXmlFile::INSTANCE;
  if(!sched.empty()) type = LisaXmlFile::SOLUTION;
  
  LisaXmlFile xmlOutput(type);
  xmlOutput << pt << val << cp;
  if(!sched.empty()) xmlOutput << sched;
  xmlOutput.write(algin);
}

//**************************************************************************
    
void readAlgOutput(Lisa_Schedule &sched){
  std::ifstream out_file(algout.c_str());
  if(!out_file){
    G_ExceptionList.lthrow((std::string)"Could not open '"+algout+
                           "' for reading.",Lisa_ExceptionList::FILE_NOT_FOUND);
    exit(-1);
  }
  out_file.close();
  
 LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
 xmlInput.read(algout.c_str());
 
 //check for successful parsing and valid document type   
 LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();
 if( !xmlInput || !(type == LisaXmlFile::SOLUTION) || !(xmlInput >> sched)){
   G_ExceptionList.lthrow("Could not read solution.",
                          Lisa_ExceptionList::INCONSISTENT_INPUT);
   exit(-1);
  }
  
  if(!G_ExceptionList.empty()) exit(-1);
  
  if(sched.get_m() != m || sched.get_n() != n){
    G_ExceptionList.lthrow((std::string)
                           "Schedule size does not match problem.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    exit(-1);
  }
  
}
  
//**************************************************************************

TIMETYP sum(Lisa_Matrix<TIMETYP>* pt){
  TIMETYP retval = 0;
  
  for(int i=0;i<n;i++){
    for(int j=0;j<m;j++){
      retval += (*pt)[i][j];
    }
  }
  
  return retval;
}

//**************************************************************************

// convert int to string .. add leading zeros with respect to numberproblems,
// so outputfiles are correctly sorted by filesystem
std::string str_prob(const int i){
  const static unsigned int l = ((std::string) ztos(numberproblems)).length(); 
  
  std::string retval(ztos(i));
  while(retval.length() < l) retval = "0"+retval;
  
  return retval;
}

//**************************************************************************

// convert int to string .. add leading zeros with respect to numberalgorithms,
std::string str_alg(const int i){
  const static unsigned int l = ((std::string) ztos(numberalgorithms)).length(); 
  
  std::string retval(ztos(i));
  while(retval.length() < l) retval = "0"+retval;
  
  return retval;
}
  
//**************************************************************************

void system_limit(std::string executable,int limit){
  
  pid_t cpid;
  int status;
  
  child_terminated = false;
  
  switch(cpid = fork()){
   case -1:
     G_ExceptionList.lthrow("Could not fork.",Lisa_ExceptionList::ANY_ERROR);
     exit(-1);
   case 0: // child process
     execl(executable.c_str(),executable.c_str(),algin.c_str(),algout.c_str(),0);
     G_ExceptionList.lthrow("Could not exec '"+executable+" "+algin+" "+algout+"'.",
                            Lisa_ExceptionList::ANY_ERROR);
     exit(-1);
   default: // parent process
     if(limit > 0){ 
       while(limit > 0 && !child_terminated){
         sleep(1);
         limit--;
       }
       kill(cpid,SIGINT);
     }
     wait(&status);
  }
  
}

//**************************************************************************

int main(int argc, char *argv[]){
 time_t all_start = time(0);
 
 //make sure we report any errors
 G_ExceptionList.set_output_to_cout();
 
 //we want to be informed about terminating childs
 signal(SIGCHLD, &child_terminates);
 
 if(argc < 2){
  G_ExceptionList.lthrow((std::string)"Usage: "+argv[0]+
                         " [input file]",Lisa_ExceptionList::ANY_ERROR);
  exit(-1);  
 }
 
 //initialize xml input stuff
 LisaXmlFile::initialize();
 
 for(int i=1;i<argc;i++){
	 algin +=(std::string)argv[i];
	 algout +=(std::string)argv[i];
	 if(i<argc-1){
		 algin +=" ";
		 algout +=" ";
	 }
 }
 std::string strAutoAlgFilename;
 
 strAutoAlgFilename += algin;
 algin += "in.xml";
 algout += "out.xml";
 
 //open input file
 std::ifstream in_file(strAutoAlgFilename.c_str());
 if(! in_file){
   G_ExceptionList.lthrow((std::string)"Could not open '"+strAutoAlgFilename+
                          "' for reading.",Lisa_ExceptionList::FILE_NOT_FOUND);
   exit(-1);   
 }

 // get problemtype
 Lisa_ProblemType pt;
 in_file >> pt;
 if(!G_ExceptionList.empty()) exit(-1);
 checkProblemType(pt);
  
 // get parameters into global variables 
 Lisa_ControlParameters cp;
 in_file >> cp;
 if(!G_ExceptionList.empty()) exit(-1);
 parseParameters(cp);
 
 // clean up warnings
 do{}while(G_ExceptionList.lcatch(Lisa_ExceptionList::WARNING) != "No error of this kind in list.");
 
 // read parameters for algorithms to call
 algParameters* aps = new algParameters[numberalgorithms];
 for(int i=0;i<numberalgorithms;i++){
   in_file >> aps[i].cp;
   if(!G_ExceptionList.empty()) exit(-1);
   aps[i].checkAlgo(i);
   do{}while(G_ExceptionList.lcatch(Lisa_ExceptionList::WARNING) != "No error of this kind in list.");
 }

 // were done reading
 in_file.close();
   
 for(int i=0;i<numberproblems;i++){
   
   //save current seeds to pt so problem can be reconstructed
   cp.add_key("TIMESEED",timeseed);
   cp.add_key("MACHSEED",machseed);
   
   // generate instance 
   Lisa_Values val;
   generateValues(val,pt);
   Lisa_Schedule sched(n,m); // result from one algorithm
   
   //need those two to calculate objective
   Lisa_OsProblem op(&val);
   Lisa_OsSchedule os(&op);
           
   // run all the algorithms .. first one should be constructive, the others 
   // can be iterative to improve the solution or contructive to create a new
   // solution ...
   Lisa_List<Lisa_ScheduleNode> OutSchedList; //"global", one result per algorithm
   Lisa_List<Lisa_ScheduleNode> InSchedList; //"local", all inputs forone algorithm
   
   for(int j=0;j<numberalgorithms;j++){
    
    //pick input schedules
    InSchedList.clear();
    std::list<int>::iterator it;
    for(it=aps[j].startfrom.begin();it!=aps[j].startfrom.end();it++){
      OutSchedList.locate(*it);
      
      Lisa_Schedule* lsch = OutSchedList.get().actual_schedule;
      if(lsch->valid) InSchedList.append(Lisa_ScheduleNode(lsch)); 
    }
    
    writeAlgInput(pt,aps[j].cp,val,InSchedList);
    time_t start = time(0);
    
    system_limit(aps[j].executable,aps[j].timelimit);
    
    time_t end = time(0);
    readAlgOutput(sched);
    OutSchedList.append(Lisa_ScheduleNode(&sched));
    
    os.read_LR(sched.LR);
    os.SetValue(pt.get_property(OBJECTIVE));
    std::cout << "AUTO_ALG: problem " << str_prob(i+1) << " algorithm "
              << str_alg(j+1) << " time " << end-start << " sek objective "
              << os.GetValue();

    switch(pt.get_property(OBJECTIVE)){
	    case CMAX : std::cout << "(Cmax)" << std::endl;
			break;
	    case LMAX : std::cout << "(Lmax)" << std::endl;
			break;
	    case SUM_CI : std::cout << "(SumCi)" << std::endl;
			break;
	    case SUM_WICI : std::cout << "(SumWiCi)" << std::endl;
			break;
	    case SUM_UI : std::cout << "(SumUi)" << std::endl;
			break;
	    case SUM_WIUI : std::cout << "(SumWiUi)" << std::endl;
			break;
	    case SUM_TI : std::cout << "(SumTi)" << std::endl;
			break;
	    case SUM_WITI : std::cout << "(SumWiTi)" << std::endl;
			break;
	  }

    
  os.SetValue(CMAX);
	std::cout << " Cmax " << os.GetValue() << std::endl;
    os.SetValue(SUM_CI);
	std::cout << " SumCi " << os.GetValue() << std::endl;
	os.SetValue(SUM_WICI);
	std::cout << " SumWiCi " << os.GetValue() << std::endl;
	os.SetValue(LMAX);
	std::cout << " Lmax " << os.GetValue() << std::endl;
	os.SetValue(SUM_UI);
	std::cout << " SumUi " << os.GetValue() << std::endl;
	os.SetValue(SUM_WIUI);
	std::cout << " SumWiUi " << os.GetValue() << std::endl;
	os.SetValue(SUM_TI);
	std::cout << " SumTi " << os.GetValue() << std::endl;
	os.SetValue(SUM_WITI);
	std::cout << " SumWiTi " << os.GetValue() << std::endl;
  }
   
   //open output file, write generated problem + comments
   std::string out_file_name = strAutoAlgFilename+"."+str_prob(i+1)+".xml";
   std::ofstream out_file(out_file_name.c_str());
   if(!out_file){
     G_ExceptionList.lthrow("Could not open '"+out_file_name+"' for writing.",
                            Lisa_ExceptionList::FILE_NOT_FOUND);
     exit(-1);
   }
   out_file.close();
   
   LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
   xmlOutput << pt << val << OutSchedList;
   xmlOutput.write(out_file_name);
   
   std::ofstream out_file1(out_file_name.c_str(),std::ios::out|std::ios::app);
   if(!out_file1){
     G_ExceptionList.lthrow("Could not open '"+out_file_name+"' for appending.",
                            Lisa_ExceptionList::FILE_NOT_FOUND);
     exit(-1);
   }
   
   out_file1 
   << "<!--" << std::endl
   << "This problem was created from the following initial values:" << std::endl
   << pt << std::endl
   << cp << std::endl  
   << "-->";
   
   out_file1.close();
   
 }

 delete[] aps;
 time_t all_end = time(0);
 std::cout << "AUTO_ALG: overall time " << all_end-all_start << " sek" << std::endl;
 return 0;
}

