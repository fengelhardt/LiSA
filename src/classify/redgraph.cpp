/**
 * @version 2.3pre3
 */
#include <stdio.h>

#include "redgraph.hpp"

Lisa_RedGraph::Lisa_RedGraph(void)
{
  int g,i,j;
  

  for (g=0; g<TUPEL_INDEX; g++)
    for (i=0; i<V_MAX; i++)
      for (j=0; j<SUCC_MAX; j++)
	Graph[g][i][j]=MY_NULL; 
  
  // initialization of the reduction graphs!

  Graph[M_ENV][ONE][0]=O; Graph[M_ENV][ONE][1]=F; Graph[M_ENV][ONE][2]=P;
  Graph[M_ENV][O][0]=X; Graph[M_ENV][F][0]=J; Graph[M_ENV][J][0]=X;
  Graph[M_ENV][X][0]=G; Graph[M_ENV][P][0]=Q; Graph[M_ENV][Q][0]=R;
  Graph[M_ENV][OMPT][0]=XMPT; Graph[M_ENV][FMPT][0]=JMPT; 
  Graph[M_ENV][JMPT][0]=XMPT; 
  Graph[M_ENV][OMPM][0]=XMPM; Graph[M_ENV][FMPM][0]=JMPM; 
  Graph[M_ENV][JMPM][0]=XMPM; Graph[M_ENV][PMPM][0]=QMPM; 

  Graph[M_NUMBER][M_VAL][0]=M_FIX; Graph[M_NUMBER][M_FIX][0]=M_ARB;
  
  Graph[PRECEDENCE][FALSE][0]=CHAINS;
  Graph[PRECEDENCE][CHAINS][0]=OUTTREE; Graph[PRECEDENCE][CHAINS][1]=INTREE;
  Graph[PRECEDENCE][OUTTREE][0]=TREE; Graph[PRECEDENCE][INTREE][0]=TREE;
  Graph[PRECEDENCE][TREE][0]=SP_GRAPH; Graph[PRECEDENCE][SP_GRAPH][0]=PREC; 
  
  Graph[RI][FALSE][0]=TRUE;
  
  Graph[DI][FALSE][0]=TRUE;
  
  Graph[PIJ][PIJ_1][0]=PIJ_P;
  Graph[PIJ][PIJ_P][0]=FALSE;
  
  Graph[BOUNDED_BATCH][FALSE][0]=TRUE;
  
  Graph[JOB_NR][J_VAL][0]=J_FIX; Graph[JOB_NR][J_FIX][0]=J_ARB;

  Graph[TIME_LAGS][FALSE][0]=UNIT_TL; Graph[TIME_LAGS][UNIT_TL][0]=CONST_TL;
  Graph[TIME_LAGS][CONST_TL][0]=GENERAL_TL;

  Graph[TRANSPORTATION_DELAYS][FALSE][0]=TIK_T; 
  Graph[TRANSPORTATION_DELAYS][FALSE][1]=TIKL_T;
  Graph[TRANSPORTATION_DELAYS][TIK_T][0]=TI_IN; 
  Graph[TRANSPORTATION_DELAYS][TIK_T][1]=TK; 
  Graph[TRANSPORTATION_DELAYS][TIKL_T][0]=TI_IN; 
  Graph[TRANSPORTATION_DELAYS][TIKL_T][1]=TKL_TLK; 
  Graph[TRANSPORTATION_DELAYS][TI_IN][0]=TI; 
  Graph[TRANSPORTATION_DELAYS][TK][0]=TIK;
  Graph[TRANSPORTATION_DELAYS][TKL_TLK][0]=TKL;
  Graph[TRANSPORTATION_DELAYS][TKL_TLK][1]=TIKL_TILK;
  Graph[TRANSPORTATION_DELAYS][TI][0]=TIKL_TILK;
  Graph[TRANSPORTATION_DELAYS][TI][1]=TIK;
  Graph[TRANSPORTATION_DELAYS][TIKL_TILK][0]=TIKL;
  Graph[TRANSPORTATION_DELAYS][TKL][0]=TIKL;

  Graph[OBJECTIVE][CMAX][0]=LMAX; Graph[OBJECTIVE][LMAX][0]=SUM_TI; 
  Graph[OBJECTIVE][LMAX][1]=SUM_UI; Graph[OBJECTIVE][SUM_CI][0]=SUM_WICI;
  Graph[OBJECTIVE][SUM_CI][1]=SUM_TI; Graph[OBJECTIVE][SUM_WICI][0]=SUM_WITI;
  Graph[OBJECTIVE][SUM_TI][0]=SUM_WITI; Graph[OBJECTIVE][SUM_UI][0]=SUM_WIUI;
}

int
Lisa_RedGraph::output(void) // debug output of reduction graphs
{
  int g,i,j;
  
  for (g=0; g<TUPEL_INDEX; g++)
    {
      printf("\n\n    ");
      for (i=1; i<=V_MAX; i++)
	{    
	  printf("\n %2d:",i);
	  for (j=0; j<SUCC_MAX; j++)
	    printf(" %2d",Graph[g][i][j]);
	}
    }
  printf("\n");
  return(0);
}

int
Lisa_RedGraph::visit(int GrNo, int first, int second)
{
  int k=0, node;

  if (first==second)
    return(TRUE);
  //  printf("Graphen Nummer:%3d, Knoten Nummer:%3d\n",GrNo,first); 
  while((k<SUCC_MAX) && ((node=Graph[GrNo][first][k])!=MY_NULL))
    {
      if (visit(GrNo,node,second))
	return(TRUE);
      k++;
    }
  return(FALSE);
}

int
Lisa_RedGraph::reducible(Lisa_ProblemType* first, Lisa_ProblemType* second)
{
  int i=0;

  if ((first->get_property_old(M_NUMBER)==M_VAL) && 
      (second->get_property_old(M_NUMBER)==M_VAL))
    if (first->m_no > second->m_no)
      return(FALSE);
  
  if ((first->get_property_old(JOB_NR)==J_VAL) && 
      (second->get_property_old(JOB_NR)==J_VAL))
    if (first->n_no > second->n_no)
      return(FALSE);
  
  while (i<TUPEL_INDEX && 
	 visit(i,first->get_property_old(i),second->get_property_old(i)))
    i++;
  if (i==TUPEL_INDEX)
    return(TRUE);
  return(FALSE);
}

int
Lisa_RedGraph::compare(Lisa_ProblemType* t1, Lisa_ProblemType* t2)
{
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




