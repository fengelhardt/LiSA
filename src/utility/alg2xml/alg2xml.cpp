#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

#include "../../lisa/ptype.hpp"

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/tree.h>

#define LISA_XML_IMPL

#include "../../xml/LisaXmlFile.hpp"
#include "../../xml/xmldefs.hpp"

void trim(std::string& input, const string t = " ")
{
		input.erase(0, input.find_first_not_of(t));
		input.erase(input.find_last_not_of(t) + 1);
}


int lsa_to_xml(string lsa_file, string xml_file)
{
		Lisa_ExtAlg Alg;
		ifstream in(lsa_file.c_str());
		if(!in)
				{
						cerr << "Cannot read file \"" << lsa_file << "\"" << endl;
						return 1;
				}
		string S;
		stringstream ss;
		while(in.good() && S != "<GENERAL>")
				in >> S;

		if(!in.good())
				{
						cerr << "error : missing GENERAL section" << endl;
						return 1;
				}

		//read generals
		char buffer[256];
		char buffer2[256];
		*buffer = '\0';
		while(in.good())
				{
						//linewise scanning
						in.getline(buffer,256,'\n');
						S = string(buffer);
						trim(S);
						if(S == "</GENERAL>")
								break;
						if(S.empty())
								continue;
						S.replace(S.find("="),1,"\n"); //separate by newline
						ss.clear();
						ss.str(S);
						ss.getline(buffer2,256,'\n');
						string tag = string(buffer2);
						ss.getline(buffer2,256,'\n');
						string value = string(buffer2);
						trim(tag);
						trim(value);
						if(tag == "Name")
								{
										Alg.name = value;
								}
						else if(tag == "Type")
								{
										Alg.type = value;
								}
						else if(tag == "Call")
								{
										Alg.call = value;
								}
						else if(tag == "Code")
								{
										Alg.code = value;
								}
						else if(tag == "Help")
								{
										Alg.helpFile = value;
								}
						else
								cerr << "warning : unknown tag -> \"" << tag << "\"" << endl;
						if(value.empty())
								cerr << "warning : no value for tag \"" << tag << "\"" << endl;
				}
		in.seekg(0);
		in.clear();
		while(in.good() && S != "<EXACT>")
				in >> S;
		if(!in.good())
				{
						cerr << "error : missing EXACT section" << endl;
						return 1;
				}

		while(in.good() && S != "</EXACT>")
					{
							in >> S;
							if(S == "<PROBLEMTYPE>")
								{
										ss.clear();
										ss.str("");
										ss << S << endl;
										in >> S;
										while(S != "</PROBLEMTYPE>")
												{
														ss << ' ' << S;
														in >> S; 
												}
										ss << endl << S << endl;
										Lisa_ProblemType P;
										ss >> P;
										if(!P.valid())
												cerr << "error : inavlid Problem" << endl;
										else
												Alg.handle_exact.push_back(P);
								}
					}
		//read problems
		in.seekg(0);
		while(in.good() && S != "<HEURISTIC>")
				in >> S;
		if(!in.good())
				{
						cerr << "error : missing HEURISTIC section" << endl;
						return 1;
				}
		
			in.clear();
			while(in.good() && S != "</HEURISTIC>")
					{
							in >> S;
							if(S == "<PROBLEMTYPE>")
								{
										ss.clear();
										ss.str("");
										ss << S << endl;
										in >> S;
										while(S != "</PROBLEMTYPE>")
												{
														ss << ' ' << S;
														in >> S; 
												}
										ss << endl << S << endl;
										Lisa_ProblemType P;
										ss >> P;
										if(!P.valid())
												cerr << "error : inavlid Problem" << endl;
										else
												Alg.handle_heuristic.push_back(P);
								}
					}
			//read problems
			in.seekg(0);
			
			while(in.good() && S != "<PARAMETERS>")
					in >> S;
			
			//read parameters
			while(in.good())
					{
							//linewise scanning
							in.getline(buffer,256,'\n');
							S = string(buffer);
							trim(S);
							if(S == "</PARAMETERS>")
									break;
							if(S.empty())
									continue;
							ss.clear();
							ss.str(S);
							string entry;
							ss >> entry;
							if(entry == "long")
									{
											Integer_Control IC;
											ss >> entry;
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing variable name" << endl;
															continue;
													}
											IC.name = entry;
											ss >> IC.default_value;
											if( ss.fail())
													{
															cerr << "error : missing initial value" << endl;
															continue;
													}
											ss.getline(buffer,255);
											entry = string(buffer);
											trim(entry," \"");
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing caption" << endl;
															continue;
													}
											IC.caption = entry;
											Alg.Integer_Controls.push_back(IC);
									}
							else if(entry == "double")
									{
											Real_Control RC;
											ss >> entry;
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing variable name" << endl;
															continue;
													}
											RC.name = entry;
											ss >> RC.default_value;
											if( ss.fail())
													{
															cerr << "error : missing initial value" << endl;
															continue;
													}
											ss.getline(buffer,255);
											entry = string(buffer);
											trim(entry," \"");
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing caption" << endl;
															continue;
													}
											RC.caption = entry;
											Alg.Real_Controls.push_back(RC);
									}
							else if(entry == "string")
									{
											Choice_Control CC;
											ss >> entry;
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing variable name" << endl;
															continue;
													}
											CC.name = entry;
											//parse items
											unsigned br, adv = 0;
											string choice = string(buffer);
											br = choice.find_first_of("(") + 1;
											adv = br;
											if(br >= choice.size())
														{
																cerr << "error : malformed choice" << endl;
																continue;
														}
											choice.erase(0,br);
											br = choice.find_first_of(")");
											choice.erase(br);
											stringstream chss(choice);
											string ch;
											chss >> ch;
											while(chss.good())
													{
															CC.items.push_back(ch);
															chss >> ch;
													}
											while(ss.good() && (ss.get() != ')'));
											ss.getline(buffer,255);
											entry = string(buffer);
											trim(entry," \"");
											if(entry.empty() || ss.fail())
													{
															cerr << "error : missing caption" << endl;
															continue;
													}
											CC.caption = entry; 
											Alg.Choice_Controls.push_back(CC);  
									}
							else
									cerr << "error : \"" << entry << "\" unknown" << endl;
					}
			
			LisaXmlFile::initialize(".");
			LisaXmlFile::write_ExtAlg(Alg,xml_file);
			return 0;
}

void usage(ostream& o, char* prog)
{
						o << "Conversion tool : old .lsa format to XML" << endl;
						o << "Usage : " << prog
								<< " -s <infile> <outfile> | -b <file_1> ... <file_n> | -h" << endl << endl;
						o << "   -s <infile> <outfile>     ....  convert a single file." << endl;
						o << "   -b <file_1> ... <file_n>  ....  batch converting alg-files" << endl;
						o << "                                   extension is replaced by \".xml\"" << endl;
						o << "   -h ... print this help" << endl << endl;
}

int main(int argc, char** argv)
{
		if(argc < 2) 
				{
						usage(cerr,argv[0]);
						return 1;
				}
		//parse options
		string opt(argv[1]);
		vector<string> infiles;
		vector<string> outfiles;
		int count = 0;
		if(opt == "-h")
				{
						usage(cout,argv[0]);
						return 0;
				}
		else if(opt == "-s")
				{
						if(argc != 4)
								{
										usage(cerr,argv[0]);
										return 1;
								}
						infiles.push_back(string(argv[2]));
						outfiles.push_back(string(argv[3]));
						count = 1;
				}
		else if(opt == "-b")
				{
							if(argc < 3)
								{
										usage(cerr,argv[0]);
										return 1;
								}
							for(int i = 2; i < argc ; i++)
									{
											count ++;
											string infile(argv[i]);
											string outfile = infile;
											outfile.erase(outfile.find_last_of("."));
											outfile += ".xml";
											infiles.push_back(infile);
											outfiles.push_back(outfile);
									}
				}
		else {
				usage(cerr,argv[0]);
				return 1;
		}
		
		for(int i = 0; i < count; i++)
				{
						lsa_to_xml(infiles[i], outfiles[i]);
				}
		return 0;
}
		
