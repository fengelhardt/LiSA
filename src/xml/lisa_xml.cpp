#include <string>
#include <vector>
#include <sstream>
#include <map>

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define LISA_XML_IMPL

#include "LisaXmlFile.hpp"
#include "xmldefs.hpp"


#include "../basics/matrix.hpp"
#include "../basics/pair.hpp"
 
#include "../basics/graph.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../main/lisapref.hpp"
#include "../scheduling/schedule.hpp"
#include "../main/global.hpp"

#include "xmldefs.hpp"

#include "Lisa_NativeDataHandler.hpp"
#include "Lisa_IO_Factory.hpp"

using namespace std;

#define MAP_PAIRS(PROP, PROP_VAL, ATTR, ATTR_VAL) 		WriteMap[pair<int,int>((PROP),(PROP_VAL))] = pair<string,string >((ATTR),(ATTR_VAL)); \
  ReadMap[pair<string,string>((ATTR),(ATTR_VAL))] = pair<int,int >((PROP),(PROP_VAL))

const string VALUE_ATTRIBUTE = "";

void LisaXmlFile::init_maps()
{
  //Problem Type Mapping
  //default values are commented out
		
  // +++++++++++++++++++++++++++++++
  // ++++++   ALPHA   ++++++++++++++
  // +++++++++++++++++++++++++++++++
		
  //Machine enironment
  MAP_PAIRS(M_ENV,ONE,"env","1");
  MAP_PAIRS(M_ENV,O,"env","O");
  MAP_PAIRS(M_ENV,F,"env","F");
  MAP_PAIRS(M_ENV,J,"env","J");
  MAP_PAIRS(M_ENV,X,"env","X");
  MAP_PAIRS(M_ENV,G,"env","G");
  MAP_PAIRS(M_ENV,P,"env","P");
  MAP_PAIRS(M_ENV,Q,"env","Q");
  MAP_PAIRS(M_ENV,R,"env","R");
		
  //Multi-purpose stuff
  MAP_PAIRS(M_MPT,1,"mpt","yes");
  //		MAP_PAIRS(M_MPT,FALSE,"mpt","no");
  MAP_PAIRS(M_MPM,1,"mpm","yes");
  //		MAP_PAIRS(M_MPM,FALSE,"mpm","no");
		
  //Machine number
  //		MAP_PAIRS(M_NUMBER,M_ARB,"m","arbitrary");
  MAP_PAIRS(M_NUMBER,M_VAL,"m",VALUE_ATTRIBUTE); // m is placed here
  MAP_PAIRS(M_NUMBER,M_FIX,"m","fixed");
		
  // +++++++++++++++++++++++++++++++
  // ++++++   BETA   +++++++++++++++
  // +++++++++++++++++++++++++++++++
		
  //Preemtion
  MAP_PAIRS(PMTN,1,"pmtn","yes");
  //		MAP_PAIRS(PMTN,FALSE,"pmtn","no");
		
  //Preceedence
  //		MAP_PAIRS(PRECEDENCE,FALSE,"prec","no");
  MAP_PAIRS(PRECEDENCE,INTREE,"prec","intree");
  MAP_PAIRS(PRECEDENCE,OUTTREE,"prec","outtree");
  MAP_PAIRS(PRECEDENCE,TREE,"prec","tree");
  MAP_PAIRS(PRECEDENCE,SP_GRAPH,"prec","sp_graph");
  MAP_PAIRS(PRECEDENCE,CHAINS,"prec","chains");
  MAP_PAIRS(PRECEDENCE,PREC,"prec","yes");
		
  //Release times
  MAP_PAIRS(RI,1,"release_times","yes");
  //		MAP_PAIRS(RI,FALSE,"release_times","no");
		
  //Due dates
  MAP_PAIRS(DI,1,"due_dates","yes");
  //		MAP_PAIRS(DI,FALSE,"due_dates","no");
		
  //Processing times
  //	MAP_PAIRS(PIJ,FALSE,"processing_times","arbitrary");
  MAP_PAIRS(PIJ,PIJ_1,"processing_times","unit");
  MAP_PAIRS(PIJ,PIJ_P,"processing_times","uniform");
		
  //Batch
  //		MAP_PAIRS(BATCH,FALSE,"batch","no");
  MAP_PAIRS(BATCH,S_BATCH,"batch","s_batch");
  MAP_PAIRS(BATCH,P_BATCH,"batch","p_batch");
		
  //Bounded Batch
  //		MAP_PAIRS(BOUNDED_BATCH,FALSE,"batch_bounded","no");
  MAP_PAIRS(BOUNDED_BATCH,1,"batch_bounded","yes");
		
  //Job number
  //		MAP_PAIRS(JOB_NR,J_ARB,"n","arbitrary");
  MAP_PAIRS(JOB_NR,J_VAL,"n",VALUE_ATTRIBUTE); // n is placed here
  MAP_PAIRS(JOB_NR,J_FIX,"n","fixed");
		
  //No wait
  MAP_PAIRS(NO_WAIT,1,"no-wait","yes");
  //		MAP_PAIRS(NO_WAIT,FALSE,"no-wait","no");

  //Size --what's that ?
  MAP_PAIRS(SIZE,1,"size","yes");
  //		MAP_PAIRS(SIZE,FALSE,"size","no");

  //Timelags
  //		MAP_PAIRS(TIME_LAGS,FALSE,"time-lags","no");
  MAP_PAIRS(TIME_LAGS,UNIT_TL,"time-lags","unit");
  MAP_PAIRS(TIME_LAGS,CONST_TL,"time-lags","constant");
  MAP_PAIRS(TIME_LAGS,GENERAL_TL,"time-lags","given");
	
  //Transportation delays
  //		MAP_PAIRS(TRANSPORTATION_DELAYS,FALSE,"transp-delays","no");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TIK_T,"transp-delays","t_ik--T");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TIKL_T,"transp-delays","t_ikl--T");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TI_IN,"transp-delays","t_i--T_1-T_2");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TKL_TLK,"transp-delays","t_kl--t_lk");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TIKL_TILK,"transp-delays","t_ikl--t_ilk");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TI,"transp-delays","t_i");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TK,"transp-delays","t_k");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TKL,"transp-delays","t_kl");
  MAP_PAIRS(TRANSPORTATION_DELAYS,TIK,"transp-delays","t_ik"); 
  MAP_PAIRS(TRANSPORTATION_DELAYS,TIKL,"transp-delays","t_ikl");
		
  //SERVER_FLAGS
  //		MAP_PAIRS(SERVER_FLAGS,FALSE,"server-flags","no");
  MAP_PAIRS(SERVER_FLAGS,SI,"server-flags","yes");
  MAP_PAIRS(SERVER_FLAGS,SI_1,"server-flags","1");
  MAP_PAIRS(SERVER_FLAGS,SI_S,"server-flags","s");

  // +++++++++++++++++++++++++++++++
  // ++++++  GAMMA   +++++++++++++++
  // +++++++++++++++++++++++++++++++

  //Objective
  MAP_PAIRS(OBJECTIVE,CMAX,"objective","Cmax");
  MAP_PAIRS(OBJECTIVE,LMAX,"objective","Lmax");
  MAP_PAIRS(OBJECTIVE,SUM_CI,"objective","Sum_Ci");
  MAP_PAIRS(OBJECTIVE,SUM_WICI,"objective","Sum_wiCi");
  MAP_PAIRS(OBJECTIVE,SUM_UI,"objective","Sum_Ui");
  MAP_PAIRS(OBJECTIVE,SUM_WIUI,"objective","Sum_wiUi");
  MAP_PAIRS(OBJECTIVE,SUM_TI,"objective","Sum_Ti");
  MAP_PAIRS(OBJECTIVE,SUM_WITI,"objective","Sum_wiTi");
  MAP_PAIRS(OBJECTIVE,IRREG1,"objective","irreg_1");
  MAP_PAIRS(OBJECTIVE,IRREG2,"objective","irreg_2");
}



// ++++++++++++  functions for internal use only   ++++++++++
// problemType - numeric handlers
string handleSpecialAttribute(const int prop, const Lisa_ProblemType& P){
  stringstream s;
  switch(prop)
    {
    case M_NUMBER:
      s << P.m_no;
      break;
    case JOB_NR:
      s << P.n_no;
      break;
    default:
      break;
    }
  return s.str();
}

bool handleSpecialAttribute(const string attr, const string attr_val, Lisa_ProblemType& P){
  stringstream s(attr_val);
  int value;
  s >> value;
  if(s.fail())
    return false;
  if(attr == "m")
    {
      P.set_property(M_NUMBER,M_VAL);
      P.m_no = value;
      return true;
    }
  if(attr == "n")
    {
      P.set_property(JOB_NR,J_VAL);
      P.n_no = value;
      return true;
    }
  return false;
}

//++++++++++++++++++++++++++++++++++++++++++++++++++

// each of the following functions hooks the objcect's XML-representation
// into the current XML-document
//
// both, <<- and >>-operator have to take care about LisaXmlFile's internal
// hook pointer, i.e. set it appropriately in advance to each call of one of
// these functions

// If you want to add a serialization of an object to XML
// you may want to declare a friend function in your class to access
// private and/or protected data of your class interface

//the following functions assume existence of a proper document
//and Hook set to the desired parent node
void LisaXmlFile::write(const Lisa_ProblemType& P)
{
  const char* node_names[3] = {"alpha", "beta", "gamma"};
  const int   advance_list[4] = {0, 
				 NUMBER_ALPHA , 
				 NUMBER_ALPHA + NUMBER_BETA , 
				 NUMBER_ALPHA + NUMBER_BETA + NUMBER_GAMMA };
  int tag = 0, curr_node = 0;
  map< pair<int,int> , pair< string, string> > ::iterator finder;
  xmlNodePtr problem;
  if(xmlStrcmp(Hook->name, (const xmlChar *) "problem") == 0)
    problem = Hook;
  else
    problem = xmlAddChild(Hook,xmlNewNode(NULL,(const xmlChar *) "problem"));
  xmlNodePtr current;
  for(curr_node = 0; curr_node < 3 ; curr_node++)
    {
      current = xmlAddChild(problem, xmlNewNode(NULL, (const xmlChar *)  node_names[curr_node]));
      for(tag = advance_list[curr_node]; tag < advance_list[curr_node + 1]; tag++)
	{
	  finder = WriteMap.find(pair<int,int>(tag,P.get_property(tag)));
	  if(finder == WriteMap.end()) // not found
	    continue; //throw something
	  if(finder->second.second == VALUE_ATTRIBUTE)
	    xmlSetProp(current, 
		       (const xmlChar *)  finder->second.first.c_str(),
		       (const xmlChar *)  handleSpecialAttribute(finder->first.first,P).c_str());
	  else
	    xmlSetProp(current, 
		       (const xmlChar *)  finder->second.first.c_str(),
		       (const xmlChar *)  finder->second.second.c_str());
	}
    }
}

bool LisaXmlFile::read(Lisa_ProblemType& P)
{
  P.reset();
  if(!xmlStrEqual(Hook->name, (const xmlChar *) "problem"))
    {
      raiseError(BAD_ROOT,"problem",(const char*) Hook->name);
      return false;
    }
		
  map< pair<string,string> , pair< int, int> > ::iterator finder;
		
  xmlChar* attValue;
  string value_str;
  for(xmlNodePtr cur = Hook->children;cur;cur=cur->next)
    {
      if(cur->type != XML_ELEMENT_NODE)
	continue;
      for(xmlAttrPtr attPtr = cur->properties; attPtr; attPtr=attPtr->next)
	{
	  attValue = xmlGetProp(cur, attPtr->name);
	  if(attValue)
	    {
	      value_str = string((char*) attValue);
	      xmlFree(attValue);
	    }
	  else
	    value_str = "";
	  finder = ReadMap.find(pair<string,string>((const char*) attPtr->name, value_str));
	  //ignore unknown elements
	  if(finder != ReadMap.end())
	    {
	      P.set_property(finder->second.first, finder->second.second);
	    }
	  else if(!handleSpecialAttribute( (const char*) attPtr->name , value_str, P))
	    {
	      raiseError(BAD_ATTRIBUTE,  (const char*) attPtr->name  );
	      return false;
	    }
	}
    }
  return true;
}


void LisaXmlFile::write(const Lisa_Values& V)
{		
  xmlNodePtr l_parent;
  stringstream pipe;
		
  if(xmlStrEqual(Hook->name, (const xmlChar *) "values"))
    l_parent = Hook;
  else
    l_parent = xmlAddChild(Hook,xmlNewNode(NULL,(const xmlChar *) "values"));
		
  pipe.str("");
  const int n = V.get_n() , m = V.get_m();
  pipe << m;
  xmlSetProp(l_parent,(const xmlChar *) "m", (const xmlChar *) pipe.str().c_str());
  pipe.str("");
  pipe << n;
  xmlSetProp(l_parent,(const xmlChar *) "n", (const xmlChar *) pipe.str().c_str());
  xmlNodePtr child;
  pipe.str("");		
  pipe.clear();

  Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(coding);
  if(dh == NULL)
    {
      raiseError(BAD_MODEL,coding);
      return;
    }
  if (V.PT)
    {
      dh->write(V,"processing_times",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "processing_times",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.SIJ)
    {
      dh->write(V,"operation_set",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "operation_set",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.MO)
    {
      dh->write(V,"machine_order",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "machine_order",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.RD)  
    {
      dh->write(V,"release_times",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "release_times",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.DD)  
    {
      dh->write(V,"due_dates",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "due_dates",
			      (const xmlChar *) pipe.str().c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.WI)  
    {
      dh->write(V,"weights",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "weights",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.WI2)  
    {
      dh->write(V,"weights2",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "weights_2",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if (V.EXTRA)  
    {
      dh->write(V,"extra",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "extra",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
    }
  delete dh;
}


void LisaXmlFile::write(const Lisa_Schedule& S)
{
  xmlNodePtr l_parent;
  stringstream pipe;
		
  if(xmlStrEqual(Hook->name, (const xmlChar *) "schedule"))
    l_parent = Hook;
  else
    l_parent = xmlAddChild(Hook,xmlNewNode(NULL,(const xmlChar *) "schedule"));
		
  int n = S.get_n(), m = S.get_m();
  pipe.str(""); pipe << m;
  xmlSetProp(l_parent,(const xmlChar *) "m", (const xmlChar *) pipe.str().c_str());
  pipe.str(""); pipe << n;
  xmlSetProp(l_parent,(const xmlChar *) "n", (const xmlChar *) pipe.str().c_str());
  pipe.str("");
  pipe.clear();
		
  if(S.semiactive == 1) 
    xmlSetProp(l_parent,(const xmlChar *) "semiactive", (const xmlChar *) "yes");
  else
    xmlSetProp(l_parent,(const xmlChar *) "semiactive", (const xmlChar *) "no");

  xmlNodePtr child;

  Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(coding);
  if(dh == NULL)
    {
      raiseError(BAD_MODEL,coding);
      return;
    }
  if(S.LR)
    {
      dh->write(S,"plan",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "plan",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if(S.NMO)
    {
      dh->write(S,"machine_sequences",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "machine_sequences",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if(S.NJO)
    {
      dh->write(S,"job_sequences",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "job_sequences",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
      pipe.str("");
      pipe.clear();
    }
  if(S.CIJ)
    {
      dh->write(S,"completion_times",pipe);
      child = xmlNewTextChild(l_parent,
			      NULL, 
			      (const xmlChar *) "completion_times",
			      (const xmlChar *) pipe.str().c_str());
      xmlSetProp(child,(const xmlChar *) "model", 	(const xmlChar *) coding.c_str());
    }
  delete dh;
}

void LisaXmlFile::write(const Lisa_ScheduleNode& S)
{
  if(S.actual_schedule)
    write(*(S.actual_schedule));
}

void LisaXmlFile::write(const Lisa_Preferences& P)
{
  write(P.contents);
}

void LisaXmlFile::write(const Lisa_ControlParameters& Cp)
{
  xmlNodePtr l_parent;
  stringstream pipe;
		
  if(xmlStrEqual(Hook->name, (const xmlChar *) "controls"))
    l_parent = Hook;
  else
    l_parent = xmlAddChild(Hook,xmlNewNode(NULL,(const xmlChar *) "controls"));
		
  //+++++++++++++++++++++++++++++++++++++++
  // ++ copied from lsa-write +++++++++++++
  //+++++++++++++++++++++++++++++++++++++++
		
  //copy all contents: this is totally stupid, but reading a list would 
  //     change its internal bookmark and we are in a const method here
  Lisa_List<string> kl, sl;
  Lisa_List<double> dl;
  Lisa_List<long> il;
		
  kl=Cp.IntKeyList;
  xmlNodePtr child;
  xmlChar* conv_name, *conv_value;
  if (!kl.empty())
    {  
      il=Cp.IntList; kl.reset(); il.reset();
      //do strm << "long " << kl.get() << " " << il.get() << "\n";
      do {
	pipe.str("");pipe << il.get();
	conv_name = string2xmlChar(kl.get());
	conv_value = string2xmlChar(pipe.str());
	if(!conv_name)
	  conv_name = xmlStrdup((const xmlChar*) kl.get().c_str());
	if(!conv_value)
	  conv_value = xmlStrdup((const xmlChar*) pipe.str().c_str());
	child = xmlAddChild(l_parent,
			    xmlNewNode(NULL,(const xmlChar *) "parameter"));
	xmlSetProp(child, (const xmlChar *) "type", (const xmlChar *) "integer");
	xmlSetProp(child, (const xmlChar *) "name", conv_name);
	xmlSetProp(child, (const xmlChar *) "value",conv_value);
	xmlFree(conv_name);
	xmlFree(conv_value);
      }
      while (kl.next(), il.next());
    }
		
  kl=Cp.DoubleKeyList;
  if (!kl.empty())
    {  
      dl=Cp.DoubleList; kl.reset(); dl.reset();
      do {
	pipe.str("");pipe << dl.get();
	conv_name = string2xmlChar(kl.get());
	conv_value = string2xmlChar(pipe.str());
	if(!conv_name)
	    conv_name = xmlStrdup((const xmlChar*) kl.get().c_str());
	if(!conv_value)
	  conv_value = xmlStrdup((const xmlChar*) pipe.str().c_str());
	child = xmlAddChild(l_parent,
			    xmlNewNode(NULL,(const xmlChar *) "parameter"));
	xmlSetProp(child, (const xmlChar *) "type", (const xmlChar *) "real");
	xmlSetProp(child, (const xmlChar *) "name", conv_name);
	xmlSetProp(child, (const xmlChar *) "value",conv_value);
	xmlFree(conv_name);
	xmlFree(conv_value);
      }
      while (kl.next(), dl.next());
    }
  kl=Cp.StringKeyList;
  if (!kl.empty())
    {  
      sl=Cp.StringList; kl.reset(); sl.reset();
      //do strm << "string " << kl.get() << " " << sl.get() << "\n";
      do {
								
	conv_name = string2xmlChar(kl.get());
	conv_value = string2xmlChar(sl.get());
	if(!conv_name)
	  conv_name = xmlStrdup((const xmlChar*) kl.get().c_str());
	if(!conv_value)
	  conv_value = xmlStrdup((const xmlChar*) sl.get().c_str());
	child = xmlAddChild(l_parent,
			    xmlNewNode(NULL,(const xmlChar *) "parameter"));
	xmlSetProp(child, (const xmlChar *) "type", (const xmlChar *) "string");
	xmlSetProp(child, (const xmlChar *) "name", conv_name);
	xmlSetProp(child, (const xmlChar *) "value",conv_value);
	xmlFree(conv_name);
	xmlFree(conv_value);
      }
      while (kl.next(), sl.next());
    }
		
  //++++++++++++++++++++++++++++++++++++++++++
}

void LisaXmlFile::write(const Lisa_Graph& G)
{
  xmlNodePtr graphPtr;
  stringstream pipe;
  pipe << G.get_vertices();
  xmlSetProp(graphPtr,(const xmlChar *) "n", (const xmlChar *) pipe.str().c_str());
  xmlSetProp(graphPtr,(const xmlChar *) "model", (const xmlChar *) coding.c_str());
  //get the hanlder for the model
  Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(coding);
  if(dh == NULL)
    {
      raiseError(BAD_MODEL,coding);
      return;
    }
  pipe.str("");
  pipe.clear();
		
  dh->write(G,"graph",pipe);
  delete dh;
  graphPtr = Hook;
		
  if(xmlStrEqual(Hook->name, (const xmlChar *) "graph"))
    graphPtr = xmlAddChild(Hook,xmlNewText((const xmlChar *) pipe.str().c_str()));
  else
    xmlNewTextChild(Hook,
		    NULL, 
		    (const xmlChar *) "graph",
		    (const xmlChar *) pipe.str().c_str());
}


bool LisaXmlFile::read(Lisa_Values& V)
{

  if(!xmlStrEqual(Hook->name, (const xmlChar *) "values"))
    {
      raiseError(BAD_ROOT,"values",(const char*) Hook->name);
      return false;
    }
  xmlNodePtr cur = Hook;
  stringstream pipe;
		
  int n,m;
  bool okay = true;
  xmlChar* attr_m = xmlGetProp(cur,(const xmlChar*) "m");
  xmlChar* attr_n = xmlGetProp(cur,(const xmlChar*) "n");
  if(attr_m)
    {
      pipe.clear();
      pipe.str("");
      pipe << (char*) attr_m;
      pipe >> m;
      if(pipe.fail())
	okay = false;
      xmlFree(attr_m);
    }
  else okay = false;
  if(attr_n)
    {
      pipe.clear();
      pipe.str("");
      pipe << (char*) attr_n;
      pipe >> n;
      if(pipe.fail())
	okay = false;
      xmlFree(attr_n);
    }
  else okay = false;
  if(okay)
    V.init(n,m);
  else
    {
      raiseError(MISSING_SIZE,"values");
      return false;
    }
		
  string node_name, content, model_name;
  xmlChar* c_model;
  for(xmlNodePtr child = cur->children;child;child=child->next)
    {
      if(child->type != XML_ELEMENT_NODE)
	continue;
      node_name = (const char*) child->name;
      c_model = xmlGetProp(child,(const xmlChar*) "model");
      if(!c_model)
	model_name = Lisa_NativeDataHandler::getName();
      else
	{
	  model_name = (const char*) c_model;
	  xmlFree(c_model);
	}
      Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(model_name);
      if(dh == NULL)
	{
	  raiseError(BAD_MODEL,model_name);
	  return false;
	}
						
      pipe.clear();
      pipe.str("");
      //retrieve all text content of this node
      content = "";
      for(xmlNodePtr text = child->children; text; text=text->next)
	{
	  if(text->type == XML_TEXT_NODE)
	    {
	      content = (text->content) ? (const char*) text->content : "";
	      pipe << content;
	    }
	}
      if(!dh->read(V,node_name,pipe))
	{
	  raiseError(BAD_NODE,node_name);
	  delete dh;
	  return false;
	}
      delete dh;
    }
  return true;
}


bool LisaXmlFile::read(Lisa_Schedule& S)
{	
		
  if(!xmlStrEqual(Hook->name, (const xmlChar *) "schedule"))
    {
      raiseError(BAD_ROOT,"schedule",(const char*) Hook->name);
      return false;
    }
		
  xmlNodePtr cur = Hook;
  stringstream pipe;
		
  int n,m;
  bool okay = true;
  xmlChar* attr_m = xmlGetProp(cur,(const xmlChar*) "m");
  xmlChar* attr_n = xmlGetProp(cur,(const xmlChar*) "n");
  xmlChar* attr_sem = xmlGetProp(cur,(const xmlChar*) "semiactive");
  if(attr_m)
    {
      pipe.clear();
      pipe.str("");
      pipe << (char*) attr_m;
      pipe >> m;
      if(pipe.fail())
	okay = false;
      xmlFree(attr_m);
    }
  else okay = false;
  if(attr_n)
    {
      pipe.clear();
      pipe.str("");
      pipe << (char*) attr_n;
      pipe >> n;
      if(pipe.fail())
	okay = false;
      xmlFree(attr_n);
    }
  else okay = false;
  if(okay)
    S.init(n,m);
  else
    {
      raiseError(MISSING_SIZE,"schedule");
      return false;
    }
  S.semiactive=0;
  if(attr_sem && xmlStrEqual(attr_sem, (const xmlChar *) "yes"))
    {
      S.semiactive=1;
      xmlFree(attr_sem);
    }
		
		
  string node_name, content, model_name;
  xmlChar *c_model;

  for(xmlNodePtr child = cur->children;child;child=child->next)
    {
      if(child->type != XML_ELEMENT_NODE)
	continue;
      node_name = (const char*) child->name;
      c_model = xmlGetProp(child,(const xmlChar*) "model");
      if(!c_model)
	model_name = Lisa_NativeDataHandler::getName();
      else
	{
	  model_name = (const char*) c_model;
	  xmlFree(c_model);
	}
      Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(model_name);
      if(dh == NULL)
	{
	  raiseError(BAD_MODEL,model_name);
	  return false;
	}
						
      pipe.clear();
      pipe.str("");
      //retrieve all text content of this node
      content = "";
      for(xmlNodePtr text = child->children; text; text=text->next)
	{
	  if(text->type == XML_TEXT_NODE)
	    {
	      content = (text->content) ? (const char*) text->content : "";
	      pipe << content;
	    }
	}
      if(!dh->read(S,node_name,pipe))		
	{
	  raiseError(BAD_NODE,node_name);
	  delete dh;
	  return false;
	}
      delete dh;
    }
  S.valid = 1;
  return true;
}

bool LisaXmlFile::read(Lisa_ScheduleNode& S)
{	
  if(!S.actual_schedule)
    S.actual_schedule = new Lisa_Schedule();
  if(!read(*(S.actual_schedule)))
    {
      delete S.actual_schedule;
      S.actual_schedule = NULL;
      return false;
    }
  return true;
}
bool LisaXmlFile::read(Lisa_Preferences& P)
{
  return read(P.contents);
}

bool LisaXmlFile::read(Lisa_ControlParameters& Cp)
{	
		
  if(!xmlStrEqual(Hook->name, (const xmlChar *) "controls"))
    {
      raiseError(BAD_ROOT,"controls",(const char*) Hook->name);
      return false;
    }
		
  xmlNodePtr cur = Hook;
  stringstream pipe;

  long vl;
  double vd;
  string para_type;
  string para_name;
  string para_value;
		
  xmlChar* c_name,*c_value,*c_type;;
  for(cur = cur->children; cur; cur = cur->next)
    {
      if(cur->type !=XML_ELEMENT_NODE)
	continue;
      if(!xmlStrEqual(cur->name, (const xmlChar*) "parameter"))
	continue;
      //get type
      para_type = "";
      if((c_type = xmlGetProp(cur, (const xmlChar*) "type")) != NULL){
	xmlChar2string(c_type,para_type);
	xmlFree(c_type);
      }
      para_name = "";
      if((c_name = xmlGetProp(cur, (const xmlChar*) "name")) != NULL){
	xmlChar2string(c_name,para_name);
	xmlFree(c_name);
      }
      para_value = "";
      if((c_value = xmlGetProp(cur, (const xmlChar*) "value")) != NULL){
	xmlChar2string(c_value,para_value);
	xmlFree(c_value);
      }
      pipe.clear();
      pipe.str("");
      pipe << para_value;					
      if(para_type == "integer")
	{
	  pipe >> vl;
	  if(!pipe.fail())
	    Cp.add_key(para_name,vl);
	  else
	    cerr << "conversion error attribute value : " << para_name << endl;
	}
      else if(para_type == "real")
	{
	  pipe >> vd;
	  if(!pipe.fail())
	    Cp.add_key(para_name,vd);
	  else
	    cerr << "conversion error in attribute value : " << para_name << endl;
	}
      else if(para_type == "string")
	{
	  Cp.add_key(para_name,pipe.str());
	}
      else
	continue;
    }
  return true;
}



bool LisaXmlFile::read(Lisa_Graph& G)
{			
  if(!xmlStrEqual(Hook->name, (const xmlChar *) "graph"))
    {
      raiseError(BAD_ROOT,"graph",(const char*) Hook->name);
      return false;
    }
		
  xmlNodePtr cur = Hook;
  stringstream pipe;
		
  int n;
  bool okay = true;
  xmlChar* attr_n = xmlGetProp(cur,(const xmlChar*) "n");
  if(attr_n)
    {
      pipe.clear();
      pipe.str("");
      pipe << (char*) attr_n;
      pipe >> n;
      if(pipe.fail())
	okay = false;
      xmlFree(attr_n);
    }
  else 
    okay = false;
		
  if(okay)
    G.init(n);
  else
    {
      raiseError(MISSING_SIZE,"graph");
      return false;
    }
  xmlChar* c_model = xmlGetProp(cur,(const xmlChar*) "model");
  string model_name;
  if(!c_model)
    model_name = Lisa_NativeDataHandler::getName();
  else
    {
      model_name = (const char*) c_model;
      xmlFree(c_model);
    }
		
  Lisa_DataHandler* dh = Lisa_IO_Factory::createHandler(model_name);
  if(dh == NULL)
    {
      raiseError(BAD_MODEL,model_name);
      return false;
    }
		
  pipe.clear();
  pipe.str("");

  string content;
  for(xmlNodePtr text = cur->children; text; text=text->next)
    {
      if(text->type == XML_TEXT_NODE)
	{
	  content = (text->content) ? (const char*) text->content : "";
	  pipe << content;
	}
    }
  if(!dh->read(G,"graph",pipe))
    {
      raiseError(BAD_NODE,"graph");
      delete dh;
      return false;
    }
  delete dh;
  return true;
}


bool LisaXmlFile::read_ExtAlg(Lisa_ExtAlg& Alg, const string& filename)
{	
  xmlDocPtr Document = xmlParseFile(filename.c_str());
  if(Document == NULL)
    {
      raiseError(LIB_XML,"Failed to parse file \"" + filename + "\"");
      return false;
    }
		
  xmlNodePtr root = NULL;
  stringstream pipe;
  root = xmlDocGetRootElement(Document);
  if(root == NULL || !xmlStrEqual(root->name, (const xmlChar*) "algorithm"))
    {
      if(!root)
	raiseError(READ_EMPTY_DOCUMENT,filename);
      else
	raiseError(BAD_ROOT,"algorithm",  (const char*) root->name);
      xmlFreeDoc(Document);
      return false;
    }
  string converted;
  xmlChar* prop = xmlGetProp(root,(const xmlChar*) "name");
  if(prop)
    {
      if(!xmlChar2string(prop,converted))
	converted = (const char*) prop;
      Alg.name = converted;
      xmlFree(prop);
    }
  prop = xmlGetProp(root,(const xmlChar*) "type");
  if(prop)
    {
      if(!xmlChar2string(prop,converted))
	converted = (const char*) prop;
      Alg.type = converted;
      xmlFree(prop);
    }
  prop = xmlGetProp(root,(const xmlChar*) "call");
  if(prop)
    {
      if(!xmlChar2string(prop,converted))
	converted = (const char*) prop;
      Alg.call = converted;
      xmlFree(prop);
    }
  prop = xmlGetProp(root,(const xmlChar*) "code");
  if(prop)
    {
      if(!xmlChar2string(prop,converted))
	converted = (const char*) prop;
      Alg.code = converted;
      xmlFree(prop);
    }
  prop = xmlGetProp(root,(const xmlChar*) "help_file");
  if(prop)
    {
      if(!xmlChar2string(prop,converted))
	converted = (const char*) prop;
      Alg.helpFile = converted;
      xmlFree(prop);
    }

  Alg.handle_heuristic.clear();
  Alg.handle_exact.clear();
		
  Lisa_ProblemType P;
  LisaXmlFile xmlFile;
  string node_name;
  for(xmlNodePtr child = root->children;child;child=child->next)
    {
      if(child->type != XML_ELEMENT_NODE)
	continue;
      node_name = (const char*) child->name;
      P.reset();
      if(node_name == "heuristic")
	{
	  for(xmlNodePtr prob = child->children;prob;prob=prob->next)
	    {
	      if(prob->type != XML_ELEMENT_NODE)
		continue;
	      if(xmlStrEqual(prob->name,(const xmlChar*) "problem"))
		{	
		  Alg.handle_heuristic.push_back(P);
		  //changing hook -> take care
		  xmlFile.Hook = prob;
		  if(!xmlFile.read(Alg.handle_heuristic.back()))
		    return false;
		}
	    }
	}
      else if(node_name == "exact")
	{
	  for(xmlNodePtr prob = child->children;prob;prob=prob->next)
	    {
	      if(prob->type != XML_ELEMENT_NODE)
		continue;
	      if(xmlStrEqual(prob->name,(const xmlChar*) "problem"))
		{	
		  Alg.handle_exact.push_back(P);
		  //changing hook -> take care
		  xmlFile.Hook = prob;
		  if(!xmlFile.read(Alg.handle_exact.back()))
		    return false;
		}
	    }
	}
      else if(node_name == "alg_controls")
	{
	  string cntr_type, cnt_name, cnt_caption;
	  for(xmlNodePtr cntr = child->children;cntr;cntr=cntr->next)
	    {
	      if(cntr->type != XML_ELEMENT_NODE)
		continue;
	      xmlChar* attr;
	      cntr_type = (const char*) cntr->name;
	      if(cntr_type == "integer")
		{
		  Integer_Control IntC;
		  //name attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "name");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      IntC.name = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"name");
		      return false;
		    }
		  //caption attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "caption");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      IntC.caption = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"caption");
		      return false;
		    }
		  //default_value
		  attr = xmlGetProp(cntr,(const xmlChar*) "default");
		  if(attr)
		    {
		      pipe.str("");
		      pipe.clear();
		      pipe << (char*) attr;
		      pipe >> IntC.default_value;
		      xmlFree(attr);
		      if(pipe.fail()) 
			{
			  raiseError(BAD_ATTR_VALUE,"default",(const char*) attr);
			  return false;
			}
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"default");
		      return false;
		    }
		  Alg.Integer_Controls.push_back(IntC);
		}
	      else if(cntr_type == "real")
		{
		  Real_Control RealC;
		  //name attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "name");
		  if(attr)
		    {	
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      RealC.name = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"name");
		      return false;
		    }
		  //caption attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "caption");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      RealC.caption = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"caption");
		      return false;
		    }
		  //default_value
		  attr = xmlGetProp(cntr,(const xmlChar*) "default");
		  if(attr)
		    {
		      pipe.str("");
		      pipe.clear();
		      pipe << (char*) attr;
		      pipe >> RealC.default_value;
		      xmlFree(attr);
		      if(pipe.fail()) 
			{
			  raiseError(BAD_ATTR_VALUE,"default",(const char*)attr);
			  return false;
			}
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"default");
		      return false;
		    }
		  Alg.Real_Controls.push_back(RealC);
		}
	      else if(cntr_type == "choice")
		{
		  Choice_Control ChoiceC;
		  //name attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "name");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      ChoiceC.name = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"name");
		      return false;
		    }
		  //caption attribute
		  attr = xmlGetProp(cntr,(const xmlChar*) "caption");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      ChoiceC.caption = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"caption");
		      return false;
		    }
		  //parse items
		  for(xmlNodePtr item=cntr->children;item;item=item->next)
		    {
		      if(item->type != XML_ELEMENT_NODE)
			continue;
		      attr = xmlGetProp(item,(const xmlChar*) "name");
		      if(attr)
			{
			  if(!xmlChar2string(attr,converted))
			    converted = (const char*) prop;
			  ChoiceC.items.push_back(converted);
			  xmlFree(attr);
			}
		      else
			{
			  raiseError(MISSING_ATTRIBUTE,"name");
			  return false;
			}
		    }
		  Alg.Choice_Controls.push_back(ChoiceC);
		}
	      else if(cntr_type == "fixed")
		{
		  Lisa_ExtAlg::Fixed_Control FixC;
		  attr = xmlGetProp(cntr,(const xmlChar*) "name");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      FixC.first = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"name");
		      return false;
		    }
		  attr = xmlGetProp(cntr,(const xmlChar*) "value");
		  if(attr)
		    {
		      if(!xmlChar2string(attr,converted))
			converted = (const char*) prop;
		      FixC.second = converted;
		      xmlFree(attr);
		    }
		  else
		    {
		      raiseError(MISSING_ATTRIBUTE,"name");
		      return false;
		    }
		  Alg.Fixed_Controls.push_back(FixC);
		}
	      else
		{
		  raiseError(BAD_NODE,cntr_type);
		  return false;
		}
	    }
	}
      //ignore other stuff
    }
  return true;
}

void LisaXmlFile::write_ExtAlg(const Lisa_ExtAlg& Alg, const string& filename)
{	
		
		
  xmlDocPtr Document = xmlNewDoc((const xmlChar* ) "1.0");
  xmlNodePtr Root = xmlNewDocRawNode(Document,
				     NULL,
				     (const xmlChar* ) "algorithm",
				     NULL);
  xmlDocSetRootElement(Document, Root);
  //fill out the form
  Root = xmlDocGetRootElement(Document);

  stringstream pipe;
  LisaXmlFile xmlFile;
  xmlChar* converted;
		
  converted = string2xmlChar(Alg.name);
  if(!converted)
    converted = xmlStrdup((const xmlChar*) Alg.name.c_str());
  xmlSetProp(Root, (const xmlChar*) "name", converted);
  xmlFree(converted);
		
  converted = string2xmlChar(Alg.type);
  if(!converted)
    converted = xmlStrdup((const xmlChar*) Alg.type.c_str());
  xmlSetProp(Root, (const xmlChar*) "type", (const xmlChar*) Alg.type.c_str());
  xmlFree(converted);

  converted = string2xmlChar(Alg.call);
  if(!converted)
    converted = xmlStrdup((const xmlChar*) Alg.call.c_str());
  xmlSetProp(Root, (const xmlChar*) "call", (const xmlChar*) Alg.call.c_str());
  xmlFree(converted);
		
  converted = string2xmlChar(Alg.code);
  if(!converted)
    converted = xmlStrdup((const xmlChar*) Alg.code.c_str());
  xmlSetProp(Root, (const xmlChar*) "code", (const xmlChar*) Alg.code.c_str());
  xmlFree(converted);
		
  converted = string2xmlChar(Alg.helpFile);
  if(!converted)
    converted = xmlStrdup((const xmlChar*) Alg.helpFile.c_str());
  xmlSetProp(Root, (const xmlChar*) "help_file", (const xmlChar*) Alg.helpFile.c_str());
  xmlFree(converted);
		
  xmlNodePtr cur;
  if(!Alg.handle_heuristic.empty())
    {
      cur = xmlAddChild(Root,xmlNewNode(NULL,(const xmlChar*) "heuristic"));
      for(unsigned i = 0; i < Alg.handle_heuristic.size(); i++)
	{
	  //changing hook -> take care
	  xmlFile.Hook = cur;
	  xmlFile.write(Alg.handle_heuristic[i]);
	}
    }
  if(!Alg.handle_exact.empty())
    {
      cur = xmlAddChild(Root,xmlNewNode(NULL,(const xmlChar*) "exact"));
      for(unsigned i = 0; i < Alg.handle_exact.size(); i++)
	{
	  //changing hook -> take care
	  xmlFile.Hook = cur;
	  xmlFile.write(Alg.handle_exact[i]);
	}
    }
  cur = xmlAddChild(Root,xmlNewNode(NULL,(const xmlChar*) "alg_controls"));
  xmlNodePtr cntr;
  for(unsigned i = 0; i < Alg.Integer_Controls.size(); i++)
    {
      cntr = xmlAddChild(cur,xmlNewNode(NULL,(const xmlChar*) "integer"));
						
      converted = string2xmlChar(Alg.Integer_Controls[i].name);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Integer_Controls[i].name.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "name",  converted);
      xmlFree(converted);

      converted = string2xmlChar(Alg.Integer_Controls[i].caption);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Integer_Controls[i].caption.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "caption", converted);
      xmlFree(converted);
						
      pipe.str("");
      pipe << Alg.Integer_Controls[i].default_value;
      xmlSetProp(cntr, (const xmlChar*) "default",  (const xmlChar*) pipe.str().c_str());
    }
		
  for(unsigned i = 0; i < Alg.Real_Controls.size(); i++)
    {
      cntr = xmlAddChild(cur,xmlNewNode(NULL,(const xmlChar*) "real"));
						
      converted = string2xmlChar(Alg.Real_Controls[i].name);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Real_Controls[i].name.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "name",  converted);
      xmlFree(converted);
						
      converted = string2xmlChar(Alg.Real_Controls[i].caption);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Real_Controls[i].caption.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "caption", converted);
      xmlFree(converted);
						
      pipe.str("");
      pipe << Alg.Real_Controls[i].default_value;
      xmlSetProp(cntr, (const xmlChar*) "default",  (const xmlChar*) pipe.str().c_str());
    }
  for(unsigned i = 0; i < Alg.Choice_Controls.size(); i++)
    {
      cntr = xmlAddChild(cur,xmlNewNode(NULL,(const xmlChar*) "choice"));
						
      converted = string2xmlChar(Alg.Choice_Controls[i].name);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Choice_Controls[i].name.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "name",  converted);
      xmlFree(converted);
						
      converted = string2xmlChar(Alg.Choice_Controls[i].caption);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Choice_Controls[i].caption.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "caption", converted);
      xmlFree(converted);

      xmlNodePtr item;
      for(unsigned j = 0; j < Alg.Choice_Controls[i].items.size(); j++)
	{
	  item = xmlAddChild(cntr,xmlNewNode(NULL,(const xmlChar*) "item"));
	  xmlSetProp(item, (const xmlChar*) "name",  (const xmlChar*) Alg.Choice_Controls[i].items[j].c_str());
	}
    }
  for(unsigned i = 0; i < Alg.Fixed_Controls.size(); i++)
    {
      cntr = xmlAddChild(cur,xmlNewNode(NULL,(const xmlChar*) "fixed"));
						
      converted = string2xmlChar(Alg.Fixed_Controls[i].first);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Fixed_Controls[i].first.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "name",  converted);
      xmlFree(converted);

      converted = string2xmlChar(Alg.Fixed_Controls[i].second);
      if(!converted)
	converted = xmlStrdup((const xmlChar*) Alg.Fixed_Controls[i].second.c_str());
      
      xmlSetProp(cntr, (const xmlChar*) "value",  converted);
      xmlFree(converted);
    }
		
  //okay flush everything to the file
  xmlNewNs(xmlDocGetRootElement(Document),
	   (const xmlChar*) LisaXmlFile::NAMESPACE.c_str(), 
	   (const xmlChar*) LisaXmlFile::NAMESPACE_PREFIX.c_str());
		
  xmlCreateIntSubset(Document,
		     (const xmlChar*) "algorithm",
		     (const xmlChar*) DTD_EXTERN_PHRASE.c_str(),
		     (const xmlChar*) DTD_SYSTEM_PHRASE.c_str());
		
  xmlKeepBlanksDefault(0);
  xmlIndentTreeOutput = 1;
  if(xmlSaveFormatFileEnc(filename.c_str(), 
			  Document, 
			  LisaXmlFile::ENCODING.c_str(),
			  1) == -1)
    {
      raiseError(LIB_XML,"Failed to write file \"" + filename + "\"");
    }
  xmlFreeDoc(Document);
}

void  LisaXmlFile::raiseError(ERROR_MODE error, 
			      string info1, 
			      string info2)
{
  stringstream output;
  switch(error)
    {
    case LIB_XML:
      output << "Error : " << info1;
      break;
    case MISSING_ATTRIBUTE:
      output << "LisaXMLFile - error : " << "missing attribute \"" << info1 << '\"';
      break;
    case BAD_ROOT:
      output << "LisaXMLFile - error : Bad root-node; expected \"" << info1 << "\" - got \"" << info2 << '"';
      break;
    case BAD_NODE:
      output << "LisaXMLFile - error : Bad node : " << info1;
      break;
    case BAD_ATTRIBUTE:
      output << "LisaXMLFile - error : Unknown attribute : " << info1;
      break;
    case BAD_ATTR_VALUE:
      output << "LisaXMLFile - error : Value \"" << info2 << "\" is not valid for \"" << info1 <<"\"-attribute";
      break;
    case MISSING_SIZE:
      output << "LisaXMLFile - error : missing proper size specification in \"" << info1 << '\"';
      break;
    case NO_DATA_TO_WRITE:
      output << "LisaXMLFile - error: trying to write empty document to file  \"" << info1 << "\"";
      break;
    case NO_DATA_FOR_READ:
      output << "LisaXMLFile - error: trying retrieve data from empty document";
      break;
    case WRITE_INVALID_DOCUMENT:
      output << "LisaXMLFile - error: attempt to write invalid document to file \"" << info1 << "\"";
      break;
    case READ_EMPTY_DOCUMENT:
      output << "LisaXMLFile - error: attempt to read empty document from file \"" << info1 << "\"";
      break;
    case READ_DOC_OBJ_MISSMATCH:
      output << "LisaXMLFile - error: attempt to retrieve \"" << info1 << "\"-entry from not matching document type";
      break;
    case WRITE_DOC_OBJ_MISSMATCH:
      output << "LisaXMLFile - error: attempt to add \"" << info1 << "\"-entry to not matching document type" ;
      break;
    case READ_BAD_DOC_TYPE:
      output << "LisaXMLFile - error: expected \"" << info1 << "\"-document instead of \"" << info2 << "\"";
      break;
    case READ_ENTRY_FROM_INVALID:
      output << "LisaXMLFile - error: attempt to retrieve data in inconsitent state";
      break;
    case WRITE_ENTRY_TO_INVALID:
      output << "LisaXMLFile - error: attempt to submit data when state is inconsitent";
      break;
    case BAD_MODEL:
      output << "LisaXMLFile - error: cannot handle data model \"" << info1 << "\"";
      break;
    default:
      output << "error: LisaXmlFile encountered unknown error";
      break;
    }
  cerr << output.str() << endl;
}



xmlChar* string2xmlChar(const string& input){
  //cout << "converting string \"" << input << "\"\t\t->  ";
  int temp, size, out_size;
  temp = size = input.size() + 1; /*terminating null included*/
  out_size = size*2-1; /*terminating null is just one byte*/
  xmlChar* out = (xmlChar*)  xmlMalloc((sizeof(xmlChar))* out_size);
  int ret = isolat1ToUTF8(out, &out_size, (const unsigned char*) input.c_str(), &temp);
  
  if ((LIBXML_VERSION >= 20616 && ret <= 0 ) || ret != 0 || temp-size) {
    xmlFree(out);
    cerr << "Warning : string2xmlChar : Failed to encode token \"" << input << "\"" << endl;
    return NULL;
  }
  //cout << '\"' << (char*) out << '\"' << endl;
  return out;
}

bool xmlChar2string(const xmlChar* in , string& result)
{
  //cout << "converting string \"" << (const char*) in  << "\"\t\t->  ";
  int chars   = xmlUTF8Strlen(in);
  int in_len  = xmlUTF8Strsize(in, chars);
  int out_len = chars + 1;
  unsigned char* out = new unsigned char[out_len];

  int ret = UTF8Toisolat1(out, &out_len, in,  &in_len);   
  if ((LIBXML_VERSION >= 20616 && ret <= 0 ) || ret != 0 ) // libxml changed return value from version 2.6.16 on -marc- 
    {	
      cerr << "Warning : xmlChar2string : Failed to decode token \"" << (char*) in << "\"" << endl;
      return false;	
    }
  out[out_len] = '\0';
  result = reinterpret_cast<char*>(out);
  delete[] out;
  return true;
}

bool LisaXmlFile::validateDocument()
{
  if(Doc == NULL)
    return false;
  //get the document's dtd
  xmlDtdPtr doc_dtd = xmlGetIntSubset(Doc);
  xmlDtdPtr parsed_dtd = NULL;
  //try to use system dtd if not found
  if(doc_dtd) //foun dtd ... try parse it
    {
      parsed_dtd = xmlParseDTD (doc_dtd->ExternalID, doc_dtd->SystemID);
    }
		
  xmlValidCtxt context;
  context.userData = stderr;
  context.error = (xmlValidityErrorFunc) fprintf;
  context.warning = (xmlValidityWarningFunc) fprintf;
		
  if(parsed_dtd == NULL) //no dtd found
    cout << "LisaXmlFile:: WARNING : Dtd of document not available, using internal Dtd." << endl;
  else
    {
      int ret = xmlValidateDtd(&context, Doc, parsed_dtd);
      xmlFreeDtd(parsed_dtd);
      if(ret)
	return true;
      return false;
    }
		
  if(DtdPtr == NULL)
    {
      cout << "LisaXmlFile:: ERROR : Don't have Dtd to validate." << endl;
      return false;
    }
  if(xmlValidateDtd(&context, Doc, DtdPtr))
    return true;
  return false;
}
