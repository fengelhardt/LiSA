#include <iostream>
#include <sstream>

#include "../../xml/LisaXmlFile.hpp"

#include "../../lisa/ptype.hpp" 
#include "../../lisa/lvalues.hpp" 
#include "../../lisa/ctrlpara.hpp" 
//#include "../../main/lisapref.hpp" 
#include "../../scheduling/schedule.hpp" 

#include "../../main/file_io.hpp" 
#include "../../main/global.hpp" 

#include <assert.h>

#include "../../classify/redgraph.hpp"
#include "../../classify/transform.hpp"
using namespace std;

void usage(int argc, char** argv){
		cout << "Transform test program." << endl;
		cout << "Usage : " << argv[0] << " <source_file> <target_file>" << endl; 
}

bool read(const char* f1,
										const char* f2,
										Lisa_ProblemType& r_P1, 
										Lisa_ProblemType& r_P2,
										Lisa_ProblemType& m_P1, 
										Lisa_ProblemType& m_P2,
										Lisa_Values& V1)
{
		//initialize class
		LisaXmlFile::initialize();
		//create Input handler
		LisaXmlFile xmlInput1(LisaXmlFile::IMPLICIT);
		LisaXmlFile xmlInput2(LisaXmlFile::IMPLICIT);
		Lisa_Values V2;
		bool have_val2 = true;
		if(!xmlInput1.read(f1) || !(xmlInput1))
				{
						cout << "Error : Cannot read source Problem file \"" << f1 << "\"" << endl;
						return false;
				}
		if(!xmlInput2.read(f2) || !(xmlInput2))
				{
						cout << "Error : Cannot read target Problem file \"" << f2 << "\"" << endl;
						return false;
				}
		//check types
		//get Problem
		if( !(xmlInput1 >> r_P1))
    {
						cout << "Error : Cannot read source Problem." << endl;
						return false;
				}
		if( !(xmlInput2 >> r_P2))
    {
						cout << "Error : Cannot read target Problem." << endl;
						return false;
				}
		if( !(xmlInput1 >> V1))
				{
						cout << "Error : Cannot read source values." << endl;
						return false;
				}
		if( !(xmlInput2 >> V2))
				have_val2 = false;
		cout << "Setting Machine/Job Numbers Property to \"VALUE\"" << endl;
		m_P1 = r_P1;
		m_P2 = r_P2;

		
		m_P1.set_property(M_NUMBER,M_VAL);
		m_P1.set_property(JOB_NR,J_VAL);
		m_P2.set_property(M_NUMBER,M_VAL);
		m_P2.set_property(JOB_NR,J_VAL);
		
		m_P1.m_no = V1.get_m();
		m_P1.n_no = V1.get_n();
		if(!have_val2)
				{
						cout << "No values for target problem. Using size of source problem." << endl;
						m_P2.m_no = r_P1.m_no;
						m_P2.n_no = r_P1.n_no;
				}
		else
				{
						m_P2.m_no = V2.get_m();	
						m_P2.n_no = V2.get_n();
				}
		return true;
}


int main(int argc, char** argv)
{
		if(argc != 3){
				usage(argc,argv);
				return 0;
		}
		Lisa_ProblemType origP1, origP2, modifP1, modifP2;
		Lisa_Values V;
		if(!read(argv[1],argv[2],origP1, origP2, modifP1, modifP2,V))
				{
						cout << "Failed to read input.\nAborting..." << endl;
						return 0;
				}
		//check reducibility
		int comp  = Lisa_RedGraph::compare(origP1,origP2);
		if((comp != FIRST_TO_SECOND) && (comp != IDENT))
				{
						cout << "Error : \"" <<  origP1.output_problem() << "\" is not reducible to \"" <<  origP2.output_problem() << "\"" << endl;
						return false;
				}
		
		//register the sample transform
		Lisa_ReductionTree::registerTransform(OBJECTIVE,SUM_CI,SUM_WICI,SumWiCi_Transform);
		

		const Lisa_ReductionTree* RL[TUPEL_INDEX];
		
		
		Lisa_RedGraph::getReductions(modifP1,modifP2,RL);
		
		Lisa_AtomicTransform::TransformObjects objects;
		objects.transf_problem = &modifP1;
		objects.target_problem = &modifP2;
		objects.values = &V;
		cout << "-----------------------------------------" << endl;
		cout << "Initial source problem : \"" <<  origP1.output_problem() << "\"   ->   \""
							<< modifP1.output_problem() << "\"" << endl;
		cout << "-----------------------------------------" << endl;
		cout << "Initial target problem : \"" <<  origP2.output_problem() << "\"   ->   \""
							<< modifP2.output_problem() << "\"" << endl;
		cout << "-----------------------------------------" << endl;
		cout << "Initial values : " << V <<  endl;
		cout << "-----------------------------------------" << endl;
		cout << "-----------------------------------------" << endl;
		cout << endl << endl;
		cout << "Looping over all graphs desperate to do transforms." << endl << endl;
		std::vector<std::pair<int,std::pair<int,int> > > FailedOps;
		for(int i = 0; i < TUPEL_INDEX; i++)
				{
						cout << "**************************************************" << endl;
						cout << "G = " << name_of_tupelindex[i] << " ... ";
						if(RL[i] == NULL){
								cout << "no transform implemented for (" 
													<< name_of_tupelentry[i][modifP1.get_property(i)]<< "," 
													<< name_of_tupelentry[i][modifP2.get_property(i)]<< ")." << endl;
								continue;
						}
						
						if( RL[i]->getRoot_ID() ==  RL[i]->getLeaf_ID() ){
								cout << "identity - no tree for ("
													<< name_of_tupelentry[i][RL[i]->getRoot_ID()] << "," 
													<< name_of_tupelentry[i][RL[i]->getLeaf_ID()] << ")." << endl;
								//continue;
						}
						cout << "printing reduction tree ..." << endl;
						cout << "---------------------------" << endl;
						cout << *(RL[i]) << endl;
						cout << "---------------------------" << endl;
						//search a path in the tree
						
						Lisa_List<Lisa_Pair> seq;
						Lisa_Pair p;
						//cast const away, because successor stuff is not const method
						Lisa_Graph *T = (Lisa_Graph*)  RL[i]->getGraph();
						//identity has NULL Graph -> make the sequence
						bool have_seq = true;
						if( T == NULL){
								p.x = RL[i]->getRoot_ID() + 1;
								p.y = RL[i]->getLeaf_ID() + 1;
								seq.append(p);
						}
						//find a path in the graph, just take the first one
						else
								{
										p.x = RL[i]->getRoot_ID() + 1;
										int n = T-> get_vertices();
										int source = p.x, target = source;
										while( (source != (RL[i]->getLeaf_ID() + 1)))
												{
														T->init_successor(source);
														target = T->next_successor(source);
														if(target == n+1) //no successor found
																{
																		cout << "Error : Graph traversal failed. " 
																							<< 	name_of_tupelentry[i][source - 1] << " doesn't have successor." 
																							<<  endl << "This is a bug, seriously." << endl;
																		have_seq = false;
																		break;
																}
														p.x = source;
														p.y = target;
														seq.append(p);
														source = target;
												}
										if(!have_seq)
												continue;
								}
						//apply transform
						cout << "Running forward transform with Sequence : {";
						//output sequence
						seq.reset();
						do
								{
										Lisa_Pair p = seq.get();
										cout << " [" << name_of_tupelentry[i][p.x - 1] << "," 
															<< name_of_tupelentry[i][p.y - 1] << "]";
								}while(seq.next());
						
						cout << " }  ... ";
						seq.reset();
						if(RL[i]->forwardTransform(seq,objects))
								cout << "okay, went well" << endl;
						else{
								cout << "failed." << endl;
								FailedOps.push_back(std::pair<int,std::pair<int,int> > (i,std::pair<int,int>( RL[i]->getRoot_ID(),  RL[i]->getLeaf_ID())));
						}
				}
		cout << endl << endl;
		if(!FailedOps.empty())
				{
						cout << "The following transforms failed :"<< endl;
						std::vector<std::pair<int,std::pair<int,int> > >::iterator it;
						for(it = FailedOps.begin(); it != FailedOps.end(); ++it){
								cout << "[ \"" << name_of_tupelentry[it->first][it->second.first] << "\" (";
								cout << it->second.first << ") ]  -> ";
								cout << "[ \"" << name_of_tupelentry[it->first][it->second.second] << "\" (";
								cout << it->second.second << ") ]  in  ";
								cout << "\"" << name_of_tupelindex[it->first] << "\" (";
								cout << it->first << ")" << endl;
						}
				}
		cout << endl << endl;

		cout << "Resetting Machine/Job Numbers Property to original state" << endl << endl;
		modifP1.set_property(M_NUMBER,origP1.get_property(M_NUMBER));
		modifP1.set_property(JOB_NR,origP1.get_property(JOB_NR));
		modifP2.set_property(M_NUMBER,origP2.get_property(M_NUMBER));
		modifP2.set_property(JOB_NR,origP2.get_property(JOB_NR));

		cout << "Here is what we finally got from the transforms :" << endl << endl;
		cout << "Final source problem : \"" <<  modifP1.output_problem() << "\"" << endl;
		cout << "Final target problem : \"" <<  modifP2.output_problem() << "\"" << endl;
		cout << "Final values : " << V <<  endl;
		

		
		return 1;
}
		
