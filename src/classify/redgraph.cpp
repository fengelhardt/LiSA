#include <stdio.h>
#include "redgraph.hpp"
#include "reduction.hpp"
#include "transform.hpp"
#include <set>

using namespace std;

#define ADD_REDUCTION(G,S,T) Graph[(G)][(S) + (NUMBER[(G)]*(T))] = 1
#define GET_REDUCTION(G,S,T) ((Graph[(G)][(S) + (NUMBER[(G)]*(T))] == 0) ? false : true)

bool Lisa_RedGraph::isInitialized = false;
unsigned char* Lisa_RedGraph::Graph[TUPEL_INDEX];

void Lisa_RedGraph::initializeGraph(){
		if(isInitialized)
				return;
		isInitialized = true;
		int g,i;

  for (g=0; g<TUPEL_INDEX; g++)
				{
						Graph[g] = new unsigned char[NUMBER[g] * NUMBER[g]];
						for (i=0; i< NUMBER[g]* NUMBER[g]; i++)
								Graph[g][i]=0; 
				}
  
  // initialization of the reduction graphs!
  //Graph[M_ENV][ONE][0]=O; 
		ADD_REDUCTION(M_ENV,ONE,O);
		//Graph[M_ENV][ONE][1]=F; 
		ADD_REDUCTION(M_ENV,ONE,F);
		//Graph[M_ENV][ONE][2]=P;
		ADD_REDUCTION(M_ENV,ONE,P);
  //Graph[M_ENV][O][0]=X;
		ADD_REDUCTION(M_ENV,O,X);
		//Graph[M_ENV][F][0]=J; 
		ADD_REDUCTION(M_ENV,F,J);
		//Graph[M_ENV][J][0]=X;
		ADD_REDUCTION(M_ENV,J,X);
  //Graph[M_ENV][X][0]=G; 
		ADD_REDUCTION(M_ENV,X,G);
		//Graph[M_ENV][P][0]=Q; 
		ADD_REDUCTION(M_ENV,P,Q);
		//Graph[M_ENV][Q][0]=R;
		ADD_REDUCTION(M_ENV,Q,R);
  //Graph[M_ENV][OMPT][0]=XMPT;
		ADD_REDUCTION(M_ENV,OMPT,XMPT); 
		//Graph[M_ENV][FMPT][0]=JMPT; 
		ADD_REDUCTION(M_ENV,FMPT,JMPT);
  //Graph[M_ENV][JMPT][0]=XMPT; 
		ADD_REDUCTION(M_ENV,JMPT,XMPT);
  //Graph[M_ENV][OMPM][0]=XMPM; 
		ADD_REDUCTION(M_ENV,OMPM,XMPM);
		//Graph[M_ENV][FMPM][0]=JMPM; 
		ADD_REDUCTION(M_ENV,FMPM,JMPM);
  //Graph[M_ENV][JMPM][0]=XMPM; 
		ADD_REDUCTION(M_ENV,JMPM,XMPM);
		//Graph[M_ENV][PMPM][0]=QMPM; 
		ADD_REDUCTION(M_ENV,PMPM,QMPM);

  
		//Graph[M_NUMBER][M_VAL][0]=M_FIX; 
		ADD_REDUCTION(M_NUMBER,M_VAL,M_FIX);
		//Graph[M_NUMBER][M_FIX][0]=M_ARB;
		ADD_REDUCTION(M_NUMBER,M_FIX,M_ARB);
		
  //Graph[PRECEDENCE][0][0]=CHAINS;
		ADD_REDUCTION(PRECEDENCE,0,CHAINS);
  //Graph[PRECEDENCE][CHAINS][0]=OUTTREE; 
		ADD_REDUCTION(PRECEDENCE,CHAINS,OUTTREE);
		//Graph[PRECEDENCE][CHAINS][1]=INTREE;
		ADD_REDUCTION(PRECEDENCE,CHAINS,INTREE);
  //Graph[PRECEDENCE][OUTTREE][0]=TREE; 
		ADD_REDUCTION(PRECEDENCE,OUTTREE,TREE);
		//Graph[PRECEDENCE][INTREE][0]=TREE;
		ADD_REDUCTION(PRECEDENCE,INTREE,TREE);
  //Graph[PRECEDENCE][TREE][0]=SP_GRAPH; 
		ADD_REDUCTION(PRECEDENCE,TREE,SP_GRAPH);
		//Graph[PRECEDENCE][SP_GRAPH][0]=PREC; 
  ADD_REDUCTION(PRECEDENCE,SP_GRAPH,PREC);


  //Graph[RI][0][0]=1;
  ADD_REDUCTION(RI,0,1);

  //Graph[DI][0][0]=1;
  ADD_REDUCTION(DI,0,1);

  //Graph[PIJ][PIJ_1][0]=PIJ_P;
		ADD_REDUCTION(PIJ,PIJ_1,PIJ_P);
  //Graph[PIJ][PIJ_P][0]=0;
		ADD_REDUCTION(PIJ,PIJ_P,0);
  
  //Graph[BOUNDED_BATCH][0][0]=1;
		ADD_REDUCTION(BOUNDED_BATCH,0,1);
  
  //Graph[JOB_NR][J_VAL][0]=J_FIX; 
		ADD_REDUCTION(JOB_NR,J_VAL,J_FIX);
		//Graph[JOB_NR][J_FIX][0]=J_ARB;
		ADD_REDUCTION(JOB_NR,J_FIX,J_ARB);

  //Graph[TIME_LAGS][0][0]=UNIT_TL; 
		ADD_REDUCTION(TIME_LAGS,0,UNIT_TL);
		//Graph[TIME_LAGS][UNIT_TL][0]=CONST_TL;
		ADD_REDUCTION(TIME_LAGS,UNIT_TL,CONST_TL);
  //Graph[TIME_LAGS][CONST_TL][0]=GENERAL_TL;
		ADD_REDUCTION(TIME_LAGS,CONST_TL,GENERAL_TL);

  //Graph[TRANSPORTATION_DELAYS][0][0]=TIK_T; 
		ADD_REDUCTION(TRANSPORTATION_DELAYS,0,TIK_T);
  //Graph[TRANSPORTATION_DELAYS][0][1]=TIKL_T;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,0,TIKL_T);
  //Graph[TRANSPORTATION_DELAYS][TIK_T][0]=TI_IN;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TIK_T,TI_IN);
  //Graph[TRANSPORTATION_DELAYS][TIK_T][1]=TK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TIK_T,TK);
		//Graph[TRANSPORTATION_DELAYS][TIKL_T][0]=TI_IN; 
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TIKL_T,TI_IN);
  //Graph[TRANSPORTATION_DELAYS][TIKL_T][1]=TKL_TLK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TIKL_T,TKL_TLK);
  //Graph[TRANSPORTATION_DELAYS][TI_IN][0]=TI; 
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TI_IN,TI);
  //Graph[TRANSPORTATION_DELAYS][TK][0]=TIK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TK,TIK);
  //Graph[TRANSPORTATION_DELAYS][TKL_TLK][0]=TKL;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TKL_TLK,TKL);
  //Graph[TRANSPORTATION_DELAYS][TKL_TLK][1]=TIKL_TILK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TKL_TLK,TIKL_TILK);
  //Graph[TRANSPORTATION_DELAYS][TI][0]=TIKL_TILK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TI,TIKL_TILK);
  //Graph[TRANSPORTATION_DELAYS][TI][1]=TIK;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TI,TIK);
  //Graph[TRANSPORTATION_DELAYS][TIKL_TILK][0]=TIKL;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TIKL_TILK,TIKL);
  //Graph[TRANSPORTATION_DELAYS][TKL][0]=TIKL;
		ADD_REDUCTION(TRANSPORTATION_DELAYS,TKL,TIKL);

  //Graph[OBJECTIVE][CMAX][0]=LMAX; 
		ADD_REDUCTION(OBJECTIVE,CMAX,LMAX);
		//Graph[OBJECTIVE][LMAX][0]=SUM_TI; 
		ADD_REDUCTION(OBJECTIVE,LMAX,SUM_TI);
  //Graph[OBJECTIVE][LMAX][1]=SUM_UI; 
		ADD_REDUCTION(OBJECTIVE,LMAX,SUM_UI);
		//Graph[OBJECTIVE][SUM_CI][0]=SUM_WICI;
		ADD_REDUCTION(OBJECTIVE,SUM_CI,SUM_WICI);
  //Graph[OBJECTIVE][SUM_CI][1]=SUM_TI; 
		ADD_REDUCTION(OBJECTIVE,SUM_CI,SUM_TI);
		//Graph[OBJECTIVE][SUM_WICI][0]=SUM_WITI;
		ADD_REDUCTION(OBJECTIVE,SUM_WICI,SUM_WITI);
  //Graph[OBJECTIVE][SUM_TI][0]=SUM_WITI; 
		ADD_REDUCTION(OBJECTIVE,SUM_TI,SUM_WITI);
		//Graph[OBJECTIVE][SUM_UI][0]=SUM_WIUI;
		ADD_REDUCTION(OBJECTIVE,SUM_UI,SUM_WIUI);
}

int
Lisa_RedGraph::output(void) // debug output of reduction graphs
{
  initializeGraph();
		int g,i,j;
  for (g=0; g<TUPEL_INDEX; g++)
    {
      printf("\n\n    ");
      for (i=1; i<=NUMBER[g]; i++)
								{    
										printf("\n %2d:",i);
										for (j=0; j< NUMBER[g]; j++)
												printf(" %2d",GET_REDUCTION(g,i,j));
								}
    }
  printf("\n");
  return(0);
}

int
Lisa_RedGraph::visit(int GrNo, int first, int second)
{
  initializeGraph();
  if (first==second)
    return(1);
  //  printf("Graphen Nummer:%3d, Knoten Nummer:%3d\n",GrNo,first); 
  for(int k = 0; k<NUMBER[GrNo]; k++)
				{
						if( GET_REDUCTION(GrNo,first,k) && visit(GrNo,k,second))
								return(1);
				}
		return(0);
}

int
Lisa_RedGraph::reducible(const Lisa_ProblemType& first, const Lisa_ProblemType& second)
{
  initializeGraph();
  int i=0;
		if( !first.valid() || !second.valid())
				return false;
  if ((first.get_property_old(M_NUMBER)==M_VAL) && 
      (second.get_property_old(M_NUMBER)==M_VAL))
    if (first.m_no > second.m_no)
      return(0);
  
  if ((first.get_property_old(JOB_NR)==J_VAL) && 
      (second.get_property_old(JOB_NR)==J_VAL))
    if (first.n_no > second.n_no)
      return(0);
  
  while (i<TUPEL_INDEX && 
									visit(i,first.get_property_old(i),second.get_property_old(i)))
    i++;
  if (i==TUPEL_INDEX)
    return(1);
  return(0);
}

int
Lisa_RedGraph::compare(const Lisa_ProblemType& t1, const Lisa_ProblemType& t2)
{
  initializeGraph();
  if (reducible(t1,t2))
    if (reducible(t2,t1))
      return (IDENT);
    else
      return(FIRST_TO_SECOND);
  else 
    if (reducible(t2,t1))
      return(SECOND_TO_FIRST);
    else
      return(NOT_CMP);
}

		
void Lisa_RedGraph::getReductions(const Lisa_ProblemType& from,
																																		const Lisa_ProblemType& to,
																																		const Lisa_ReductionTree*  reductions[TUPEL_INDEX])
{
  initializeGraph();
		for(int i = 0; i < TUPEL_INDEX; reductions[i++] =NULL);
		if(!reducible(from,to))
				return;
		int source, target;
		Lisa_ReductionTree* t;
		for(int i = 0; i < TUPEL_INDEX; i++)
				{
						source = from.get_property(i);
						target = to.get_property(i);
						t =  new Lisa_ReductionTree(i,source,target);
						if(t->isSane())
								reductions[i] = t;
						else
								delete t;
				}
}
		
bool Lisa_ReductionTree::initialized = false;
Lisa_Transform** Lisa_ReductionTree::TransformGraph[TUPEL_INDEX];

void Lisa_ReductionTree::init(){
		if(initialized)
				return;
		initialized = true;
		int g,i;
		// negate all transform pointers
  for (g=0; g<TUPEL_INDEX; g++)
				{
						TransformGraph[g] = new Lisa_Transform* [NUMBER[g] * NUMBER[g]];
						for (i=0; i< NUMBER[g]* NUMBER[g]; i++)
								TransformGraph[g][i]= NULL; 
				}
		// add all identity transform
		for (g=0; g<TUPEL_INDEX; g++)
				{
						for (i=0; i< NUMBER[g]; i++)
								//TransformGraph[g][i + i*NUMBER[g]] = IdentityTransform; 
								registerTransform(g,i,i,IdentityTransform);
				}
}

void Lisa_ReductionTree::registerTransform(int graph,
																																											int source,
																																											int target,
																																											Lisa_Transform* trans)
{
		init();
		if(0 > graph  || graph  >= TUPEL_INDEX ||
					0 > source || source >= NUMBER[graph] ||
					0 > target || target >= NUMBER[graph])
				return;
		TransformGraph[graph][source + target*NUMBER[graph]] = trans;
}

Lisa_ReductionTree::Lisa_ReductionTree()
{
		init();
		valid = false;
		graph_ID = -1;
		root_ID = -1;
		leaf_ID = -1;
		g = NULL;
}

Lisa_ReductionTree::Lisa_ReductionTree(const Lisa_ReductionTree& other)
{		
		init();
		valid = other.valid;
		graph_ID = other.graph_ID;
		root_ID = other.root_ID;
		leaf_ID = other.leaf_ID;
		g = NULL;
		if(other.g)
				g = new Lisa_MatrixGraph(other.g);
}

const Lisa_ReductionTree& Lisa_ReductionTree::operator=(const Lisa_ReductionTree& other)
{
		init();
		valid = other.valid;
		graph_ID = other.graph_ID;
		root_ID = other.root_ID;
		leaf_ID = other.leaf_ID;
		if(g)
				delete g;
		g = NULL;
		if(other.g)
				g = new Lisa_MatrixGraph(other.g);
		return *this;
}

bool Lisa_ReductionTree::visit(int gr,int s,int t)
{
		if (s==t)
    return true;
  for(int k = 0; k<NUMBER[gr]; k++)
				{
						if( (s == k) ||
										((TransformGraph[gr][s + k*NUMBER[gr]] != NULL) &&
											visit(gr,k,t)))
								return true;
				}
		return false;
}

Lisa_ReductionTree::Lisa_ReductionTree(int G, int source, int target)
{
		init();
		graph_ID = G;
		root_ID = source;
		leaf_ID = target;
		valid = false;
		g = NULL;
		//check for valid input
		if(0 > G      || G      >= TUPEL_INDEX || 
					0 > source || source >= NUMBER[G] || 
					0 > target || target >= NUMBER[G])
				return;
		if(source == target)
				{
						valid = true;
						return;
				}
		Lisa_RedGraph redG;
		//not reducible - tree is insane
		if( (!redG.visit(G,source,target)) || 
						(!visit(G,source,target)))
				{
						valid = false;
						return;
				}
		g = new Lisa_MatrixGraph(NUMBER[G]);
		std::vector<int> node_list(1,source);
		int node;
		while( !node_list.empty())
				{ 
						node = node_list.back();
						node_list.pop_back();
						for(int i = 0; i < NUMBER[G]; i++)
								{
										if(  (Lisa_RedGraph::Graph[G][node + i*NUMBER[G]] == 1) && //is reducible
															(redG.visit(G,i,target)) && //doesn't have dead end in reducibility
															(TransformGraph[G][node + i*NUMBER[G]] != NULL) && //have transform
															(visit(G,i,target) )) // no dead end in transform path
												{
														//that node has to be in the tree
														if(i != target)
																node_list.push_back(i);
														g->insert_arc(node + 1,i + 1);
												}
								}
				}
		valid = true;
}

Lisa_Transform* Lisa_ReductionTree::getTransform(int source_node, int target_node) const
{
		if((!valid) ||
					1 > source_node || source_node > NUMBER[graph_ID] || 
					1 > target_node || target_node > NUMBER[graph_ID])
				return NULL;
		return TransformGraph[graph_ID][source_node - 1 + (target_node -1) * NUMBER[graph_ID]];
}


bool 
Lisa_ReductionTree::forwardTransform(Lisa_List<Lisa_Pair>& nodes,
																																					Lisa_AtomicTransform::TransformObjects& to) const
{
		if(!valid)
				return false;
		//first check node integrity
		if(nodes.empty())
				return false;
		Lisa_Pair current = nodes.first();
		if(current.x != root_ID + 1)
				return false;
		current = nodes.last();
		if(current.y != leaf_ID +1)
				return false;
		int source, target;

		if(root_ID == leaf_ID) //identity
				if(nodes.length() == 1) //just one transform
						return getTransform(root_ID + 1,leaf_ID + 1)->forward->transform(graph_ID,
																																																																							root_ID,
																																																																							leaf_ID,
																																																																							to,
																																																																							Lisa_AtomicTransform::FORWARD);
				else
						return false;
		nodes.reset();
		do {
				current = nodes.get(); 
				source = current.x - 1;
				target = current.y - 1;
				if(0 > source || source >= NUMBER[graph_ID] ||
							0 > target || target >= NUMBER[graph_ID])
						return false;
				if(g->get_connection(source+1,target+1) != Lisa_Graph::ARC)
						return false;
				if(! getTransform(source + 1,target + 1)->forward->transform(graph_ID,
																																																																	source,
																																																																	target,
																																																																	to,
																																																																	Lisa_AtomicTransform::FORWARD))
						return false;
		} while (nodes.next());
		return true;
}

bool 
Lisa_ReductionTree::backwardsTransform(Lisa_List<Lisa_Pair>& nodes,
																																							Lisa_AtomicTransform::TransformObjects& to) const
{
			if(!valid)
				return false;
		//first check node integrity
		if(nodes.empty())
				return false;
		Lisa_Pair current = nodes.first();
		if(current.x != root_ID + 1)
				return false;
		current = nodes.last();
		if(current.y != leaf_ID +1)
				return false;
		int source, target;
		if(root_ID == leaf_ID) //identity
				if(nodes.length() == 1) //just one transform
						return getTransform(root_ID + 1,leaf_ID + 1)->backwards->transform(graph_ID,
																																																																									root_ID,
																																																																									leaf_ID,
																																																																									to,
																																																																									Lisa_AtomicTransform::BACKWARDS);
				else
						return false;
		nodes.locate(nodes.length()-1);
		do {
				current = nodes.get(); 
				source = current.x - 1;
				target = current.y - 1;
				if(0 > source || source >= NUMBER[graph_ID] ||
							0 > target || target >= NUMBER[graph_ID])
						return false;
				if(g->get_connection(source+1,target+1) != Lisa_Graph::ARC)
						return false;
				if(! getTransform(source + 1,target + 1)->backwards->transform(graph_ID,
																																																																			source,
																																																																			target,
																																																																			to,
																																																																			Lisa_AtomicTransform::FORWARD))
						return false;
		} while (nodes.previous());
		return true;
}




Lisa_ReductionTree::~Lisa_ReductionTree()
{
		if(g)
				delete g;
}

void Lisa_ReductionTree::write(ostream& out) const {
		if(!valid)
				{
						out << "Reduction tree is insane !" << endl;
						out << "Graph  : " << graph_ID << endl;
						out << "Source : " << root_ID << endl;
						out << "Target : " << leaf_ID << endl;
						return;
				}
		out << "Graph  : " << name_of_tupelindex[graph_ID] << endl;
		out << "Source : " << name_of_tupelentry[graph_ID][root_ID] << endl;
		out << "Target : " << name_of_tupelentry[graph_ID][leaf_ID] << endl;
		if(root_ID == leaf_ID)
				return;
		int succ = 0;
		std::set<int> nodes;
		for(int i = 0; i < NUMBER[graph_ID];i++)
				if(g->get_successors(i + 1) > 0)
						nodes.insert(i + 1);
		
		for(std::set<int>::iterator n_it = nodes.begin(); n_it != nodes.end(); n_it++)
				{
						g->init_successor(*n_it);
						out << name_of_tupelentry[graph_ID][*n_it - 1] << " -> {";
						while( (succ = g->next_successor(*n_it)) != NUMBER[graph_ID] +1)
								out << "[ " << name_of_tupelentry[graph_ID][succ - 1] << "," << TransformGraph[graph_ID][*n_it - 1 + (succ-1)*NUMBER[graph_ID]]->costs << "] ; ";
						out << "}" << endl;
				}
}

