

#include <string>
#include <vector>
#include <sstream>
#include <map>
using namespace std;


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

#include "Lisa_NativeDataHandler.hpp"

//using namespace xmlpp;

using namespace std;

#ifndef WINDOWS
const std::string PATH_SEP = "/";
#else
const std::string PATH_SEP = "\\";
#endif

//static member initialization

bool LisaXmlFile::validate = true;

const std::string LisaXmlFile::DTD_EXTERN_PHRASE = "";
const std::string LisaXmlFile::DTD_SYSTEM_PHRASE = "LiSA.dtd";
const std::string LisaXmlFile::NAMESPACE = "http://lisa.math.uni-magdeburg.de";
const std::string LisaXmlFile::NAMESPACE_PREFIX = "LiSA";

const std::string LisaXmlFile::ENCODING = "ISO-8859-1";

std::string LisaXmlFile::DTD_PATH = "";

map< pair<int,int> , pair< string, string> > LisaXmlFile::WriteMap;
map< pair<string,string> , pair<int, int> >  LisaXmlFile::ReadMap;

std::vector<std::string> LisaXmlFile::DOC_TYPE_NAMES(LisaXmlFile::IMPLICIT);

bool LisaXmlFile::initialized = false;
xmlDtdPtr LisaXmlFile::DtdPtr = NULL;

std::string LisaXmlFile::dtd_file = "";


void LisaXmlFile::initialize(std::string dtd_path){
		DTD_PATH = "";
		if(dtd_path.empty()){
				//determine dtd_path from environment variable "LISA_DTD_PATH"
				char* dtd_env = getenv("LISA_DTD_PATH");
				DTD_PATH = (dtd_env) ? (dtd_env + PATH_SEP) : "";
		}
		else
				DTD_PATH = dtd_path + PATH_SEP;
		//test for dtd file path
				
		string file = 	DTD_PATH + DTD_SYSTEM_PHRASE;
		if(DtdPtr)
				xmlFreeDtd(DtdPtr);
		DtdPtr = xmlParseDTD( (const xmlChar*) DTD_EXTERN_PHRASE.c_str(),
																								(const xmlChar*) file.c_str());
		if(DtdPtr == NULL)
				{
						dtd_file = DTD_SYSTEM_PHRASE;
						if(validate)
								cerr << "WARNING : No Dtd for validation."<< endl;
				}
		else
				dtd_file = file;
		if(initialized)
				return;
		DOC_TYPE_NAMES[PREFERENCES] = "controls";
		DOC_TYPE_NAMES[PROBLEM]     = "problem";
		DOC_TYPE_NAMES[INSTANCE]    = "instance";
		DOC_TYPE_NAMES[SOLUTION]    = "solution";
		init_maps();
		initialized = true;
}

//constructor
LisaXmlFile::LisaXmlFile(DOC_TYPE doc_type)
{
		if(!initialized)
				initialize("..");
		type = doc_type;
		Doc = NULL;
		Hook = NULL;
		NsPtr = NULL;
		valid = true;
		coding = Lisa_NativeDataHandler::getName();
}

//destructor
LisaXmlFile::~LisaXmlFile(){
		//release libxml stuff
		if(Doc) 
				xmlFreeDoc(Doc);
}

bool LisaXmlFile::write(string filename)
{
		if(!valid)
				{
						raiseError(WRITE_INVALID_DOCUMENT,filename);
						return false;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_TO_WRITE,filename);
						return false;
				}
		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		if(Root == NULL)
				{
						raiseError(LIB_XML,"LisaXmlFile refuses to write empty document.");
						return false;
								}
		if(NsPtr == NULL)
				NsPtr = xmlNewNs(Root,
																					(const xmlChar*) LisaXmlFile::NAMESPACE.c_str(), 
																					(const xmlChar*) LisaXmlFile::NAMESPACE_PREFIX.c_str());
		if(DtdPtr == NULL)
				xmlCreateIntSubset(Doc,
																							(const xmlChar*) DOC_TYPE_NAMES[type].c_str(),
																							(const xmlChar*) DTD_EXTERN_PHRASE.c_str(),
																							(const xmlChar*) dtd_file.c_str());
		else
				xmlCreateIntSubset(Doc,
																							(const xmlChar*) DOC_TYPE_NAMES[type].c_str(),
																							DtdPtr->ExternalID,
																							DtdPtr->SystemID);
		if(validate)
				if(!validateDocument())
						{
								raiseError(LIB_XML,"Document is not dtd conform ... aborting.");
								return false;
						}
		xmlKeepBlanksDefault(0);
		xmlIndentTreeOutput = 1;
		if(xmlSaveFormatFileEnc(filename.c_str(), Doc, LisaXmlFile::ENCODING.c_str(),1) == -1)
				{
						raiseError(LIB_XML,"Failed to write file \"" + filename + "\"");
						return false;
				}
		return true;
}

bool LisaXmlFile::read(string filename)
{
		Doc = xmlParseFile(filename.c_str());
		if(Doc == NULL)
				{
						raiseError(LIB_XML,"Failed to parse file \"" + filename + "\"");
						valid = false;
						return false;
				}
		if(validate)
				if(!validateDocument())
						{
								raiseError(LIB_XML,"Document is not dtd conform ... aborting.");
								valid = false;
								xmlFreeDoc(Doc);
								Doc = NULL;
								return false;
						}
		
		xmlNodePtr root = NULL;
		root = xmlDocGetRootElement(Doc);
		if(root == NULL)
				{
						raiseError(READ_EMPTY_DOCUMENT,filename);
						xmlFreeDoc(Doc);
						Doc = NULL;
						valid = false;
						return false;
				}
		
		if(type != IMPLICIT)
				{
						//if(doc_name != DOC_TYPE_NAMES[type])
						if(xmlStrcmp(root->name, (const xmlChar *) DOC_TYPE_NAMES[type].c_str()))
								{
										raiseError(READ_BAD_DOC_TYPE,DOC_TYPE_NAMES[type],(char*) root->name);
										xmlFreeDoc(Doc);
										Doc = NULL;
										valid = false;
										return false;
								}
				}
		else //determine doc type
				{
						int i;
						for(i = ((int) IMPLICIT) - 1; i >= 0; i--)
								if(xmlStrEqual(root->name, (const xmlChar *) DOC_TYPE_NAMES[i].c_str()))
										break;
						if(i < 0)
								{
										raiseError(READ_BAD_DOC_TYPE,"[IMPLICIT]",(char*) root->name);
										xmlFreeDoc(Doc);
										Doc = NULL;
										valid = false;
										return false;
								}
						else
								type = (DOC_TYPE) i;
				}
		valid = true;
		return true;
}

LisaXmlFile& LisaXmlFile::operator<<(const Lisa_Preferences& Prefs)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) type = PREFERENCES;
		//bad document type -> invalidate object
		if(type != PREFERENCES)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,"controls");
						return *this;
				}
		// since this is a standalone entry replacement deletes document
		if(Doc)
				xmlFreeDoc(Doc);
		
		Doc = xmlNewDoc((const xmlChar* ) "1.0");
		Hook = xmlNewDocRawNode(Doc,
																										NULL,
																										(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																										NULL);
		
		xmlDocSetRootElement(Doc, Hook);
		write(Prefs);
		return *this;
}

LisaXmlFile& LisaXmlFile::operator >> (Lisa_Preferences& Prefs)
{
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != PREFERENCES)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,DOC_TYPE_NAMES[type]);
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}
		Hook = xmlDocGetRootElement(Doc);
		valid = read(Prefs);
		return *this;
}

LisaXmlFile& LisaXmlFile::operator<<(const Lisa_ProblemType& P)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) type = PROBLEM;
		//bad document type -> invalidate object
		if(type != PROBLEM && type != INSTANCE && type != SOLUTION)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,DOC_TYPE_NAMES[PROBLEM]);
						return *this;
				}
		// since this is not a standalone entry, replacement maintains document
		if(Doc == NULL)
				{
						Doc = xmlNewDoc((const xmlChar* ) "1.0");
						Hook = xmlNewDocRawNode(Doc,
																														NULL,
																														(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																														NULL);
						xmlDocSetRootElement(Doc, Hook);
				}
		else 	//replace evtl. existing entry
				{
						xmlNodePtr Root = xmlDocGetRootElement(Doc);
						if((xmlStrcmp(Root->name, (const xmlChar *) DOC_TYPE_NAMES[PROBLEM].c_str())) == 0)
								{
										xmlFreeDoc(Doc);
										Doc = xmlNewDoc((const xmlChar* ) "1.0");
										Hook = xmlNewDocRawNode(Doc,
																																		NULL,
																																		(const xmlChar* ) DOC_TYPE_NAMES[PROBLEM].c_str(),
																																		NULL);
										xmlDocSetRootElement(Doc, Hook);
								}
						else
								{
										xmlNodePtr cur = Root->children;
										while(cur != NULL)
												{
														if((xmlStrcmp(cur->name, (const xmlChar *) DOC_TYPE_NAMES[PROBLEM].c_str())) == 0)
																{
																		cur = cur->next;
																		xmlUnlinkNode(cur);
																		xmlFreeNode(cur);
																}
														else
																cur = cur->next;
												}
								}
				}
		Hook = xmlDocGetRootElement(Doc);
		write(P);
		return *this;
}

LisaXmlFile& LisaXmlFile::operator>>(Lisa_ProblemType& P)
{
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != PROBLEM && type != INSTANCE && type != SOLUTION)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,DOC_TYPE_NAMES[PROBLEM]);
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}
		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		xmlNodePtr cur = Root->children;
		//read the first problem to be found
		while(cur != NULL)
				{
						if((xmlStrcmp(cur->name, (const xmlChar *) DOC_TYPE_NAMES[PROBLEM].c_str())) == 0)
								break;
						cur = cur->next;
				}
		
		if(cur == NULL)
				{
						valid = false;
						return *this;
				}

		Hook = cur;
		valid = read(P);
		return *this;
}

LisaXmlFile& LisaXmlFile::operator<<(const Lisa_Values& V)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) type = INSTANCE;
		//bad document type -> invalidate object
		if(type != INSTANCE && type != SOLUTION)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,"values");
						return *this;
				}
		// since this is not a standalone entry, replacement maintains document
		if(Doc == NULL)
				{
						
						Doc = xmlNewDoc((const xmlChar* ) "1.0");
						Hook = xmlNewDocRawNode(Doc,
																														NULL,
																														(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																														NULL);
						xmlDocSetRootElement(Doc, Hook);
				}
		else 	//replace evtl. existing entry
				{
						Hook = xmlDocGetRootElement(Doc);
						xmlNodePtr cur = Hook->children;
						while(cur != NULL)
								{
										if(xmlStrcmp(cur->name, (const xmlChar *) "values") == 0)
												{
														cur = cur->next;
														xmlUnlinkNode(cur);
														xmlFreeNode(cur);
												}
										else
												cur = cur->next;
								}
				}
		write(V);
		return *this;
}

LisaXmlFile& LisaXmlFile::operator>>(Lisa_Values& V)
{
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != INSTANCE && type != SOLUTION)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,"values");
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}
		
		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		xmlNodePtr cur = Root->children;
		//read the first values to be found
		while(cur != NULL)
				{
						if(xmlStrcmp(cur->name, (const xmlChar *) "values") == 0)
								break;
						cur = cur->next;
				}
		
		if(cur == NULL)
				{
						valid = false;
						return *this;
				}
		
		Hook = cur;
		
		valid = read(V);
		return *this;
}


LisaXmlFile& LisaXmlFile::operator<<(const Lisa_ControlParameters& CP)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) type = INSTANCE;
		//bad document type -> invalidate object
		if(type != INSTANCE && type != SOLUTION)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,"controls");
						return *this;
				}
		// since this is not a standalone entry, replacement maintains document
		if(Doc == NULL)
				{
						Doc = xmlNewDoc((const xmlChar* ) "1.0");
						Hook = xmlNewDocRawNode(Doc,
																														NULL,
																														(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																														NULL);
						xmlDocSetRootElement(Doc, Hook);
				}
		else 	//replace evtl. existing entry
				{
						
						Hook = xmlDocGetRootElement(Doc);
						xmlNodePtr cur = Hook->children;
						while(cur != NULL)
								{
										if(xmlStrcmp(cur->name, (const xmlChar *) "controls") == 0)
												{
														cur = cur->next;
														xmlUnlinkNode(cur);
														xmlFreeNode(cur);
												}
										else
												cur = cur->next;
								}
				}				
		write(CP);
		return *this;
}
		
		
LisaXmlFile& LisaXmlFile::operator>>(Lisa_ControlParameters& CP)
{
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != INSTANCE && type != SOLUTION)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,"controls");
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}

		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		xmlNodePtr cur = Root->children;
		//read the first problem to be found
		while(cur != NULL)
				{
						if(xmlStrcmp(cur->name, (const xmlChar *) "controls") == 0)
								break;
						cur = cur->next;
				}
		
		if(cur == NULL)
				{
						valid = false;
						return *this;
				}
		
		Hook = cur;

		valid = read(CP);
		return *this;
}


LisaXmlFile& LisaXmlFile::operator<<(const Lisa_Schedule& S)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) 
				type = SOLUTION;
		//bad document type -> invalidate object
		if(type != SOLUTION)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,"schedule");
						return *this;
				}
		//no replacement -> add an entry
		if(Doc == NULL)
				{
						Doc = xmlNewDoc((const xmlChar* ) "1.0");
						Hook = xmlNewDocRawNode(Doc,
																														NULL,
																														(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																														NULL);
						xmlDocSetRootElement(Doc, Hook);
				}
		write(S);
		return *this;
}


LisaXmlFile& LisaXmlFile::operator>>(Lisa_Schedule& S)
{
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != SOLUTION)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,"schedule");
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}
		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		xmlNodePtr cur = Root->children;
		//read the first problem to be found
		while(cur != NULL)
				{
						if(xmlStrcmp(cur->name, (const xmlChar *) "schedule") == 0)
								break;
						cur = cur->next;
				}
		
		if(cur == NULL)
				{
						valid = false;
						return *this;
				}
		
		Hook = cur;
		valid = read(S);
		return *this;
}


LisaXmlFile& LisaXmlFile::operator<<(const Lisa_List<Lisa_ScheduleNode> & SL)
{
		if(!valid) 
				{
						raiseError(WRITE_ENTRY_TO_INVALID);
						return *this;
				}
		if(type == IMPLICIT) 
				type = SOLUTION;
		//bad document type -> invalidate object
		if(type != SOLUTION)
				{
						valid = false;
						raiseError(WRITE_DOC_OBJ_MISSMATCH,"schedule");
						return *this;
				}
		//no replacement -> add an entry
		if(Doc == NULL)
				{
						Doc = xmlNewDoc((const xmlChar* ) "1.0");
						Hook = xmlNewDocRawNode(Doc,
																														NULL,
																														(const xmlChar* ) DOC_TYPE_NAMES[type].c_str(),
																														NULL);
						xmlDocSetRootElement(Doc, Hook);
				}
		
		//add all entries
		//copy List to preserve state
		Lisa_List<Lisa_ScheduleNode> List(SL);
		List.reset();
		Hook = xmlDocGetRootElement(Doc);
		do write(List.get());
		while(List.next());
		return *this;
}


LisaXmlFile& LisaXmlFile::operator>>(Lisa_List<Lisa_ScheduleNode>& SL)
{
		SL.clear();
		SL.reset();
		if(!valid)
				{
						raiseError(READ_ENTRY_FROM_INVALID);
						return *this;
				}
		if(type != SOLUTION)
				{
						valid = false; //is that neccessary ????
						raiseError(READ_DOC_OBJ_MISSMATCH,"schedule");
						return *this;
				}
		if(Doc == NULL)
				{
						raiseError(NO_DATA_FOR_READ);
						return *this;
				}
		
		xmlNodePtr Root = xmlDocGetRootElement(Doc);
		xmlNodePtr cur = Root->children;
		//read the first problem to be found
		while(cur != NULL)
				{
						if(xmlStrcmp(cur->name, (const xmlChar *) "schedule") == 0)
								{
										Hook = cur;
										Lisa_ScheduleNode Nd;
										valid = read(Nd);
										if(!valid)
												{
														//remove all entries
														SL.clear();
														break;
												}
										SL.insert(Nd);
								}
						cur = cur->next;
				}
		
		SL.reset();
		return *this;
}
