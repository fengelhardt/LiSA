/**
 * @author  Ivo Roessling
 * @version 3.0pre1
 */

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include <fstream>

#include "../../xml/LisaXmlFile.hpp"

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

// A 'Lisa_Pair'-like object with one integer and one pointer.
// This class is needed as a fix for this algorithm due to a compiler error on 64bit systems.
class Lisa_Pair_fix : public Lisa_Object {
public:
	Lisa_Pair_fix() {
		// do nothing
	}

	virtual ~Lisa_Pair_fix() {
		// do nothing
	}

	int x;
	void* y;

	void write (std::ostream &strm=std::cout) const {
		// do nothing
	}

	void read (std::istream &strm=std::cin) {
		// do nothing
	}

	bool operator!=(const Lisa_Pair_fix &other) {
		if(x == other.x && y == other.y) return false;
		else return true;
	}

	bool operator==(const Lisa_Pair_fix &other) {
		if(x == other.x && y == other.y) return true;
		else return false;
	}

	bool operator<(const Lisa_Pair_fix &other) {
		return x < other.x;
	}

	bool operator>(const Lisa_Pair_fix &other) {
		return x > other.x;
	}
};

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
Lisa_List<Lisa_Pair_fix> * allMatchings;
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

  Lisa_Pair_fix *MatchingInfo=new Lisa_Pair_fix();
  Lisa_Pair_fix *EdgesAndDelta=new Lisa_Pair_fix();

  EdgesAndDelta->x=level-1;
  EdgesAndDelta->y=(void*)MatchingEdges;

  MatchingInfo->x=edgecount;
  MatchingInfo->y=(void*)EdgesAndDelta;


  for(int j=0;j<n;j++)  (*MatchingMatrix)[j][m+j] = (((*rowsums)[j]==LB) ? 0 : ( (LB - (*rowsums)[j]) > 0 ));
  for(int i=0;i<m;i++)  (*MatchingMatrix)[n+i][i] = (((*colsums)[i]==LB) ? 0 : ( (LB - (*colsums)[i]) > 0 ));

  allMatchings->append(*MatchingInfo);
  //  delete EdgesAndDelta;
  delete MatchingInfo;
}

//**************************************************************************

int main(int argc, char *argv[]){

    // write error messages so lisa can pick them up
    G_ExceptionList.set_output_to_cout();

    if(argc < 3){
      cout << "Usage: " << argv[0] << " [input file] [output file]" << endl;
      exit(1);
    }

    cout << "PID= " << getpid() << endl;

    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);
    if(!i_strm){
      cout << "Could not open '" << argv[1] << "' for reading." << endl;
      exit(1);
    }
    if(!o_strm){
      cout << "Could not open '" << argv[2] << "' for writing." << endl;
      exit(1);
    }
    o_strm.close();
    i_strm.close();

    Lisa_ProblemType lpr;
    Lisa_ControlParameters sp;
    Lisa_Values my_werte;

    // read problem instance:
   //initialize class
   LisaXmlFile::initialize();
   //create Input handler
   LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);

   //parse xml file
    xmlInput.read(argv[1]);
    //determine document type
    LisaXmlFile::DOC_TYPE type = xmlInput.getDocumentType();

    //check for successful parsing and valid document type
    if (!xmlInput || !(type == LisaXmlFile::INSTANCE || type == LisaXmlFile::SOLUTION))
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }
    //get Problem
    if( !(xmlInput >> lpr))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      exit(1);
    }
    //get ControlParameters
    if( !(xmlInput >> sp))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      exit(1);
    }
    //get Values
    if( !(xmlInput >> my_werte))
    {
      cout << "ERROR: cannot read Values , aborting program." << endl;
      exit(1);
    }
    // if something else went wrong
    if (!G_ExceptionList.empty())
    {
      cout << "ERROR: cannot read input , aborting program." << endl;
      exit(1);
    }



    n=my_werte.get_n();
    m=my_werte.get_m();
    P_input=my_werte.PT;
    nondelay=lpr.get_property(NO_WAIT);

    MatchingMatrix=new Lisa_Matrix<bool>(m+n,m+n);
    result_pmtnP  = new Lisa_MatrixOfLists<TIMETYP>(my_werte.get_n(),my_werte.get_m());
    result_pmtnLR = new Lisa_MatrixOfLists<TIMETYP>(my_werte.get_n(),my_werte.get_m());

    if (nondelay) {
      allMatchings=new Lisa_List<Lisa_Pair_fix>();
      allDeltas=new Lisa_List<TIMETYP>();
    }

    rowsums=new Lisa_Vector<TIMETYP>(my_werte.get_n());
    colsums=new Lisa_Vector<TIMETYP>(my_werte.get_m());

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
    	Lisa_Pair_fix* currMatchingInfo=new Lisa_Pair_fix();
      Lisa_Pair* anEdge=new Lisa_Pair();

      // converting the list of deltas into an array - this makes the algorithm faster!
      Lisa_Vector<TIMETYP> * arrayOfDeltas=new Lisa_Vector<TIMETYP>(allMatchings->length());
      for (int level=0;level < allMatchings->length() && allMatchings->length() > 0;level++) {
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
	  Lisa_Pair_fix * EdgesAndDelta=(Lisa_Pair_fix*)currMatchingInfo->y;
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
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::INSTANCE);
    //pipe objects to this
    xmlOutput << lpr << my_werte << sp;
    //write content to a file
    xmlOutput.write(argv[2]);

    ofstream oo_strm(argv[2],ios::out|ios::app);
    if(!oo_strm){
      cout << "Could not open '" << argv[2] << "' for writing." << endl;
      exit(1);
    }

    // add real solution as comment
    oo_strm << "<!--" << endl
            << "pmtnP= " << *result_pmtnP << endl
            << "pmtnLR= " << *result_pmtnLR << endl
            << "level= " << level << endl
            << "-->";
    oo_strm.close();

    //cleaning up...
    delete MatchingMatrix;
    delete result_pmtnP;
    delete result_pmtnLR;
    delete rowsums;
    delete colsums;

}

//**************************************************************************

