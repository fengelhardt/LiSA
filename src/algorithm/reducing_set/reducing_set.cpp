/**
 * @author  Ivo Roessling
 * @version 2.3pre3
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "../../scheduling/matching.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../main/global.hpp"
#include "../../basics/list.hpp"
#include "../../basics/pair.hpp"
#include "../../basics/matrix.hpp"
#include "../../basics/lmatrix.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"

using namespace std;

//**************************************************************************

bool nondelay=false;

TIMETYP LB, LV;
Lisa_Vector<TIMETYP> * rowsums;
Lisa_Vector<TIMETYP> * colsums;
Lisa_MatrixOfLists<TIMETYP> * result_pmtnP;
Lisa_MatrixOfLists<TIMETYP> * result_pmtnLR;


Lisa_Matrix<TIMETYP> * P_input;
Lisa_Matrix<bool> * MatchingMatrix;
int level;
int m,n;		  
Lisa_List<Lisa_Pair> * allMatchings;
Lisa_List<TIMETYP> * allDeltas;

//**************************************************************************

// first thing ever to do: calculating the sums for each col and row
void calc_RowAndColSums(const Lisa_Matrix<TIMETYP> & P)
{  
  // initializing the arrays with 0
  (*rowsums).fill(0);
  (*colsums).fill(0);

  // taking a primitive LB
  LB=P[0][0];
    
  // calculating rowsums, colsums and LB
  for(int j=0;j<n;j++) {
    for(int i=0;i<m;i++) {
      if (P[j][i]>0) {
	if (P[j][i]<LV) LV=P[j][i];
	(*colsums)[i]+=P[j][i];
	(*rowsums)[j]+=P[j][i];
	
	if ((*colsums)[i]>LB) LB=(*colsums)[i];
	if ((*rowsums)[j]>LB) LB=(*rowsums)[j];
      }
    }
  }
  return;
}

//**************************************************************************

// determining the maximum delta for calculated set of operations (=matching)
TIMETYP get_delta(Lisa_Matrix<TIMETYP> & P,
		  const Lisa_Vector<int> & matching_I, const Lisa_Vector<int> & matching_J)
{
  TIMETYP delta=LB;  
  for(int i=0;i<m;i++) {
    if (matching_J[i]>=0 && matching_J[i]<n && P[matching_J[i]][i]>0) {
      // && ((*colsums)[i]==LB || (*rowsums)[matching_J[i]]==LB)) {
      if (delta>P[matching_J[i]][i]) delta=P[matching_J[i]][i]; }
    else {
      if (delta>LB-(*colsums)[i]) delta=LB-(*colsums)[i]; }
  }
  for(int j=0;j<n;j++) {
    if (matching_I[j]>=0 && matching_I[j]<m && P[j][matching_I[j]]>0) {
      // && ((*rowsums)[j]==LB || (*colsums)[matching_I[j]]==LB )) {
      if (delta>P[j][matching_I[j]]) delta=P[j][matching_I[j]]; }
    else {
      if (delta>LB-(*rowsums)[j]) delta=LB-(*rowsums)[j]; }
  }
  return delta;
}

//**************************************************************************

// this is for updating everything needed in the general case...
void update(Lisa_Matrix<TIMETYP> & P,
	    const Lisa_Vector<int> & matching_I, const Lisa_Vector<int> & matching_J,
	    TIMETYP delta) 
{
  LB-=delta;
  if (n>=m) {
    for(int j=0;j<n;j++)
      if ((matching_I[j]>=0) && (matching_I[j]<m) && (P[j][matching_I[j]]>0)) {
	P[j][matching_I[j]]-=delta;
	if (P[j][matching_I[j]]<=0) (*MatchingMatrix)[j][matching_I[j]]=false;
	(*rowsums)[j]-=delta;
	(*colsums)[matching_I[j]]-=delta;
	(*result_pmtnLR)[j][matching_I[j]].append(level);
	(*result_pmtnP)[j][matching_I[j]].append(delta);
      }
  }
  else {
    for(int i=0;i<m;i++)
      if (matching_J[i]>=0 && matching_J[i]<n &&  P[matching_J[i]][i]>0) {
	P[matching_J[i]][i]-=delta; 
	if (P[matching_J[i]][i]<=0) {
          MatchingMatrix[matching_J[i]][i]=0;
     }
	(*colsums)[i]-=delta;
	(*rowsums)[matching_J[i]]-=delta;
	(*result_pmtnLR)[matching_J[i]][i].append(level);
	(*result_pmtnP)[matching_J[i]][i].append(delta);
      }
  }
  
  for(int j=0;j<n;j++)  {
       (*MatchingMatrix)[j][m+j]=(((*rowsums)[j]==LB) ? 0 : ((LB - (*rowsums)[j])>0));
  }
  for(int i=0;i<m;i++) {
       (*MatchingMatrix)[n+i][i]=(((*colsums)[i]==LB) ? 0 : ((LB - (*colsums)[i])>0));
  }
}

//**************************************************************************

// this is for updating everything needed in the case of nondelay...
void update_nondelay(Lisa_Matrix<TIMETYP> & P,
	    const Lisa_Vector<int> & matching_I, const Lisa_Vector<int> & matching_J,
	    TIMETYP delta) 
{
  Lisa_List<Lisa_Pair> * MatchingEdges=new Lisa_List<Lisa_Pair>();
  int edgecount=0;
  LB-=delta;
  if (n>=m) {
    for(int j=0;j<n;j++)
      if ((matching_I[j]>=0) && (matching_I[j]<m) && (P[j][matching_I[j]]>0)) {
	edgecount++;
	Lisa_Pair * edge= new Lisa_Pair();
	edge->x=matching_I[j];
	edge->y=j;
	MatchingEdges->append(*edge);
	delete edge;
	P[j][matching_I[j]]-=delta;
	if (P[j][matching_I[j]]<=0) (*MatchingMatrix)[j][matching_I[j]]=false;
	(*rowsums)[j]-=delta;
	(*colsums)[matching_I[j]]-=delta;
      }
  }
  else {
    for(int i=0;i<m;i++)
      if (matching_J[i]>=0 && matching_J[i]<n &&  P[matching_J[i]][i]>0) {
	edgecount++;
	Lisa_Pair * edge= new Lisa_Pair();
	edge->x=i;
	edge->y=matching_J[i];
	MatchingEdges->append(*edge);
	delete edge;
	P[matching_J[i]][i]-=delta; 
	if (P[matching_J[i]][i]<=0) MatchingMatrix[matching_J[i]][i]=0;
	(*colsums)[i]-=delta;
	(*rowsums)[matching_J[i]]-=delta;
      }
  }


  allDeltas->append(delta);

  Lisa_Pair *MatchingInfo=new Lisa_Pair();
  Lisa_Pair *EdgesAndDelta=new Lisa_Pair();

  EdgesAndDelta->x=level-1;
  EdgesAndDelta->y=(int)MatchingEdges;

  MatchingInfo->x=edgecount;
  MatchingInfo->y=(int)EdgesAndDelta;
  
  
  for(int j=0;j<n;j++)  (*MatchingMatrix)[j][m+j] = (((*rowsums)[j]==LB) ? 0 : ( (LB - (*rowsums)[j]) > 0 ));
  for(int i=0;i<m;i++)  (*MatchingMatrix)[n+i][i] = (((*colsums)[i]==LB) ? 0 : ( (LB - (*colsums)[i]) > 0 ));

  allMatchings->append(*MatchingInfo);
  //  delete EdgesAndDelta;
  delete MatchingInfo;
}

//**************************************************************************

int main(int argc, char *argv[]) 
{

    // Auskommentieren, falls die Fehlermeldungen weitergesendet werden sollen
    G_ExceptionList.set_output_to_cout();   

    Lisa_ProblemType * lpr = new Lisa_ProblemType;
    //    Lisa_ControlParameters * sp = new Lisa_ControlParameters;
    Lisa_Values * my_werte=new Lisa_Values;

    cout << "PID= " << getpid() << endl; 

    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);

    // read problem description 
    i_strm >> (*lpr);
    // read control parameters: 
    // i_strm >> (*sp);
    // read problem instance:
    i_strm >> (*my_werte);
    
    n=my_werte->get_n();
    m=my_werte->get_m();
    P_input=my_werte->PT;
    nondelay=lpr->get_property(NO_WAIT);

    MatchingMatrix=new Lisa_Matrix<bool>(m+n,m+n);
    result_pmtnP  = new Lisa_MatrixOfLists<TIMETYP>(my_werte->get_n(),my_werte->get_m());
    result_pmtnLR = new Lisa_MatrixOfLists<TIMETYP>(my_werte->get_n(),my_werte->get_m());

    if (nondelay) {
      allMatchings=new Lisa_List<Lisa_Pair>();
      allDeltas=new Lisa_List<TIMETYP>();
    }

    rowsums=new Lisa_Vector<TIMETYP>(my_werte->get_n());
    colsums=new Lisa_Vector<TIMETYP>(my_werte->get_m());

    calc_RowAndColSums(*P_input);

    for(int j=0;j<n;j++) for(int i=0;i<m;i++) {
     (*MatchingMatrix)[j][i]=((*P_input)[j][i]>0);
     (*MatchingMatrix)[n+i][m+j]=((*P_input)[j][i]>0);
    }
    for(int j=0;j<n;j++)  (*MatchingMatrix)[j][m+j]=(((*rowsums)[j]==LB) ? 0 : ((LB - (*rowsums)[j])>0));
    for(int i=0;i<m;i++)  (*MatchingMatrix)[n+i][i]=(((*colsums)[i]==LB) ? 0 : ((LB - (*colsums)[i])>0));


    /*********************************************************
     **  ok - here the algorithm itself starts...           **
     **                                                     **/

    TIMETYP delta;
    for (level=1;;level++) {
      
      // calculating a set of independend operations...
      Lisa_BipartMatching * myMatching = new Lisa_BipartMatching(MatchingMatrix);
      
      // ..., determining the maximum delta for that...
      delta=get_delta(*P_input,
		      *myMatching->get_matching_I(),
		      *myMatching->get_matching_J());

      // ..., and updating everything!
      if (!nondelay) update(*P_input,
			    *myMatching->get_matching_I(),
			    *myMatching->get_matching_J(),
			    delta);
      else update_nondelay(*P_input,
			   *myMatching->get_matching_I(),
			   *myMatching->get_matching_J(),
			   delta);

      delete myMatching;
      
      // we have to stop if either LB<=0 or delta<=0 
      // 2nd case should never occur - but well: safe is safe :-)
      if (LB<=0||delta<=0) break;      
    }



    // in general case we've got the preemptimve schedule here allready
    // - in the case of nondelay, we've to built them up, out of the
    // matchings having calculated ever before...
    if (nondelay) {
      
      // just a few variables for later use...
      Lisa_Pair* currMatchingInfo=new Lisa_Pair();
      Lisa_Pair* anEdge=new Lisa_Pair();
      
      // converting the list of deltas into an array - this makes the algorithm faster!
      Lisa_Vector<TIMETYP> * arrayOfDeltas=new Lisa_Vector<TIMETYP>(allMatchings->length());
      for (int level=0;level < allMatchings->length()>0;level++) {
	(*arrayOfDeltas)[level]=allDeltas->pop();
      }
      

      // ok: sorting the matchings by the amount of matched edges
      allMatchings->sort();
      // and now: going through all of the matchings...
      for (int level=1;allMatchings->length()>0;level++) 
	{
	  // extracting current matching with all additional information
	  // out of the queue
	  (*currMatchingInfo)=allMatchings->dequeue();
	  Lisa_Pair * EdgesAndDelta=(Lisa_Pair*)currMatchingInfo->y;
	  Lisa_List<Lisa_Pair> * currMatching=(Lisa_List<Lisa_Pair>*)EdgesAndDelta->y;

 	  // going through all edges of the current matching and 
	  // and updating the preemtive P and LR
	  while (currMatching->length()>0) {
	    (*anEdge)=currMatching->dequeue();
	    (*result_pmtnLR)[anEdge->y][anEdge->x].append(level);
	    (*result_pmtnP)[anEdge->y][anEdge->x].append((*arrayOfDeltas)[EdgesAndDelta->x]);
	  }
	  //cleaning up...
	  delete currMatching;
	  delete EdgesAndDelta;
	  //delete &currMatchingInfo;
	}
      //cleaning up...
      delete anEdge;
      delete currMatchingInfo;
      delete arrayOfDeltas;

      delete allMatchings;
      delete allDeltas;
      
    }

    /**                                                     **
     **  ok - here the algorithm itself finishes...         **
     *********************************************************/


    // writing the result to file or stdout...
    o_strm <<"pmtnP= " << *result_pmtnP;
    o_strm <<"pmtnLR= " << *result_pmtnLR;      
    o_strm <<"level= " << level<<endl;
    

    //cleaning up...
    delete P_input;
    delete MatchingMatrix;
    delete result_pmtnP;
    delete result_pmtnLR;
    delete rowsums;
    delete colsums;

}

//**************************************************************************

