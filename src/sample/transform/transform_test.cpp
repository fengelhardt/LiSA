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
										Lisa_ProblemType& P1, 
										Lisa_ProblemType& P2, 
										Lisa_Values& V1)
{
		//initialize class
		LisaXmlFile::initialize();
		//create Input handler
		LisaXmlFile xmlInput1(LisaXmlFile::IMPLICIT);
		LisaXmlFile xmlInput2(LisaXmlFile::IMPLICIT);
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
		if( !(xmlInput1 >> P1))
    {
						cout << "Error : Cannot read source Problem." << endl;
						return false;
				}
		if( !(xmlInput2 >> P2))
    {
						cout << "Error : Cannot read target Problem." << endl;
						return false;
				}
		if( !(xmlInput1 >> V1))
				{
						cout << "Error : Cannot read source values." << endl;
						return false;
				}
		return true;
}


int main(int argc, char** argv)
{
		if(argc != 3){
				usage(argc,argv);
				return 0;
		}
		Lisa_ProblemType P1, P2;
		Lisa_Values V;
		if(!read(argv[1],argv[2],P1,P2,V))
				{
						cout << "Failed to read input.\nAborting..." << endl;
						return 0;
				}
		
		//check reducibility
		int comp  = Lisa_RedGraph::compare(P1,P2);
		if((comp != FIRST_TO_SECOND) && (comp != IDENT))
				{
						cout << "Error : \"" <<  P1.output_problem() << "\" is not reducible to \"" <<  P2.output_problem() << "\"" << endl;
						return false;
				}
		
		//register the sample transform
		Lisa_ReductionTree::registerTransform(OBJECTIVE,SUM_CI,SUM_WICI,SumWiCi_Transform);
		

		const Lisa_ReductionTree* RL[TUPEL_INDEX];
		
		
		Lisa_RedGraph::getReductions(P1,P2,RL);
		
		Lisa_AtomicTransform::TransformObjects objects;
		objects.transf_problem = &P1;
		objects.target_problem = &P2;
		objects.values = &V;
		
		cout << "Initial source problem : \"" <<  P1.output_problem() << "\"" << endl;
		cout << "Initial target problem : \"" <<  P2.output_problem() << "\"" << endl;
		cout << "Initial values : " << V <<  endl;


		for(int i = 0; i < TUPEL_INDEX; i++)
				{
						cout << "G = " << name_of_tupelindex[i] << " ... ";
						if(RL[i] == NULL){
								cout << "no transform implemented for (" 
													<< name_of_tupelentry[i][P1.get_property(i)]<< "," 
													<< name_of_tupelentry[i][P2.get_property(i)]<< ")." << endl;
								continue;
						}
						
						if( RL[i]->getRoot_ID() ==  RL[i]->getLeaf_ID() ){
								cout << "identity - nothing to transform for ("
													<< name_of_tupelentry[i][RL[i]->getRoot_ID()] << "," 
													<< name_of_tupelentry[i][RL[i]->getLeaf_ID()] << ")." << endl;
								continue;
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
						if( T == NULL){
								p.x = RL[i]->getRoot_ID() + 1;
								p.y = RL[i]->getLeaf_ID() + 1;
								seq.append(p);
						}
						//find a path in the graph, just take the first one
						else
								{
										p.x = RL[i]->getRoot_ID() + 1;
										int source = p.x, target = source;
										while( (source != (RL[i]->getLeaf_ID() + 1)))
												{
														T->init_successor(source);
														target = T->next_successor(source);
														p.x = source;
														p.y = target;
														seq.append(p);
														source = target;
												}
								}
						//output sequence
						cout << "Transform sequence : {";
						seq.reset();
						do
								{
										Lisa_Pair p = seq.get();
										cout << " [" << name_of_tupelentry[i][p.x - 1] << "," << name_of_tupelentry[i][p.y - 1] << "]";
								}while(seq.next());
						cout << " }" << endl;
						seq.reset();
						//apply transform
						RL[i]->forwardTransform(seq,objects);
				}
		
		cout << "Final source problem : \"" <<  P1.output_problem() << "\"" << endl;
		cout << "Final target problem : \"" <<  P2.output_problem() << "\"" << endl;
		cout << "Final values : " << V <<  endl;

		return 1;
}
		
