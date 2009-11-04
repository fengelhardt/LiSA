/**
 * @author  Mathias Zimmermann
 * @version 3.0pre1
 */

// Include the header files for the used objects! Standard:
// LisaProblemtype requires global.hpp.
// The file global.hpp is needed by LisaProblemtype.
// The files ctrlpara.hpp, ptype.hpp and lvalues.hpp
// are used for parsing the input file.
// The object Lisa_Schedule (header: schedule.hpp)
// is necessary for writing the result into the output file.
// The file except.hpp is used for the exception handling.

#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>

#include "../../xml/LisaXmlFile.hpp"

#include "../../scheduling/matching.hpp"
#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../scheduling/schedule.hpp"

#include "eulersplit.hpp"

using namespace std;

//**************************************************************************

void sort_duedates(Lisa_Vector<TIMETYP>& D, int pos[]) {

    int n = D.get_m(); // length of the duedates vector

    int comp[n]; // same values as in D but in ordered form

    // initialize the array
    fill(comp, comp + n, 0);

    // copy the duedates to comp
    for (int i = 0; i < n; i++) {
	comp[i] = D[i];
    }

    // sort the duedates
    sort(comp, comp + n);

    // store the original positions of the duedates
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
	if(comp[i] == D[j]) {
	  pos[i] = j;
	  D[j] = -1;
	  break;
	}
      }
    }
    // copy the ordered duedates to the original field
    for (int i = 0; i < n; i++) {
	D[i] = comp[i];
    }
}

int compute_M(Lisa_Vector<TIMETYP>& dd, Lisa_Matrix<int> *M, int m, int nondelay) {

    int n = (*M).get_n(); // number of jobs
    int max = (*M).get_m(); // maximum number of columns of the job-time matrix M
    int realmax = 0; // the real number of columns of the job-time matrix M
    int ci = 0; // the end of the latest processing time of job i
    int d[n]; // copy of the duedates
    int h[max]; // h vector
    int pos[m]; // the positions of the m smallest values in h

    fill(d, d + n, 0);
    fill(h, h + max, 0);

    // if nondelay: compute new duedates
    if ((nondelay) && (n > 2*m-3)) {
      int diff = n-m;
      for (int i = 0; i < n; i++) {
	d[i] = i < diff ? (dd[i] < n ? dd[i] : n) : n;
      }
    }
    else {
      for (int i = 0; i < n; i++) {
	d[i] = dd[i];
      }
    }

    // compute ci
    for (int i = 0; i < n; i++) {
      if (d[i] < i + m) {
	int count = 0;
	int t = 0;
	while ((count < m) && (t < max)) {
	  if (h[t] < m) {
	    count++;
	  }
	  t++;
	}
	ci = d[i] < t ? t : d[i];
      }
      else {
	ci = i + m;
      }

      // get the real maximum time
      if (realmax < ci) {
	realmax = ci;
      }

      // copy the first ci values of h to h_ci
      int h_ci[ci];
      fill(h_ci, h_ci + ci, 0);
      for (int j = 0; j < ci; j++) {
	h_ci[j] = h[j];
      }

      // find the m positions with the smallest h(t) values
      for (int j = 0; j < m; j++) {
	pos[j] = 0;
	int comp = h_ci[0];
	for (int k = 1; k < ci; k++) {
	  if (h_ci[k] < comp) {
	    comp = h_ci[k];
	    pos[j] = k;
	  }
	}
	h_ci[pos[j]] = m;
      }

      // update M and h
      for (int j = 0; j < m; j++) {
	(*M)[i][pos[j]] = 1;
	h[pos[j]]++;
      }
    }

    return realmax;
}

int computeDegree(Lisa_Matrix<TIMETYP> *M) {

    int n = (*M).get_n();
    int m = (*M).get_m();
    int rd[n];
    int cd[m];
    int RD = 0;
    int CD = 0;
    for (int i = 0; i < n; i++) {
      rd[i] = 0;
      for (int j = 0; j < m; j++) {
	if((*M)[i][j] > 0) {
	  rd[i]++;
	}
      }
      if (rd[i] > RD) {
	RD = rd[i];
      }
    }

    for (int i = 0; i < m; i++) {
      cd[i] = 0;
      for (int j = 0; j < n; j++) {
	if((*M)[j][i] > 0) {
	  cd[i]++;
	}
      }
      if (cd[i] > CD) {
	CD = cd[i];
      }
    }

    // weight the edges depending on the degree of the incident vertices
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
	if((*M)[i][j] > 0) {
	  (*M)[i][j] = rd[i]+cd[j];
	}
      }
    }

    return (RD > CD ? RD : CD);
}

int edgecolor(Lisa_Matrix<int> *F, Lisa_Matrix<TIMETYP> *M, int color) {

    int n = (*M).get_n(); // number of vertices in I
    int m = (*M).get_m(); // number of vertices in J

    // compute the degree of the given graph and weight the edges
    int d = computeDegree(M);

    if (d % 2 == 1) {
      // compute a matching that contains all edges incident with a
      // vertex of maximum degree
      Lisa_Matching* match = new Lisa_WeightedMatching(M);
      const Lisa_Vector<int> *I_matched;
      I_matched = match->get_matching_I();
      color++;
      // fill F with the colored matching edges
      for (int i = 0; i < n; i++) {
	if ((*I_matched)[i] > -1) {
	  (*F)[i][(*I_matched)[i]] = color;
	  (*M)[i][(*I_matched)[i]] = Lisa_Matching::REMOVED;
	}
      }
    }

    if (d == 1) {
      return color;
    }

    Lisa_Matrix<TIMETYP> *M1;
    Lisa_Matrix<TIMETYP> *M2;

    // split the graph into two subgraphs containing round about
    // half of the edges of the original graph
    Eulersplit* split = new Eulersplit(M);
    split->splitGraph();
    M1 = split->get_G1();
    M2 = split->get_G2();

    // some necessary changes and computations before going on with
    // coloring the subgraphs
    int d1 = computeDegree(M1);
    int d2 = computeDegree(M2);
    int k = 0;
    int r = 0;
    int diff = -1;
    if (d1 > (d2 - 1)) {
      color = edgecolor(F, M1, color);
      int deg = d2;
      while (deg > 0) {
	if (deg > 1) {
	  if (deg % 2 == 1) {
	    diff = 0;
	  }
	}
	deg = deg / 2;
	k++;
      }
      k = k + diff;
      r = (int) pow(2.0,k) - d2;

      for (int i = 0; i < r; i++) {
	if (color > 0) {
	  for (int j = 0; j < n; j++) {
	    for (int k = 0; k < m; k++) {
	      if ((*F)[j][k] == color) {
		(*M2)[j][k] = color;
		(*F)[j][k] = Lisa_Matching::REMOVED;
	      }
	    }
	  }
	  color--;
	}
      }
      color = edgecolor(F, M2, color);
    }
    else {
      color = edgecolor(F, M2, color);
      int deg = d1;
      while (deg > 0) {
	if (deg > 1) {
	  if (deg % 2 == 1) {
	    diff = 0;
	  }
	}
	deg = deg / 2;
	k++;
      }
      k = k + diff;
      r = (int) pow(2.0,k) - d1;

      for (int i = 0; i < r; i++) {
	if (color > 0) {
	  for (int j = 0; j < n; j++) {
	    for (int k = 0; k < m; k++) {
	      if ((*F)[j][k] == color) {
		(*M1)[j][k] = color;
		(*F)[j][k] = Lisa_Matching::REMOVED;
	      }
	    }
	  }
	  color--;
	}
      }
      color = edgecolor(F, M1, color);
    }

    return color;
}

void compute_LR(Lisa_Matrix<int>& LR, Lisa_Matrix<int> *M, int pos[], int m, int realmax) {

    int n =  (*M).get_n(); // number of jobs

    // copy of M but with less columns (cause of known cmax)
    Lisa_Matrix<TIMETYP> *M_adj = new Lisa_Matrix<TIMETYP>(n, realmax);

    // colored adjacency matrix
    Lisa_Matrix<int> *F = new Lisa_Matrix<int>(n, realmax);

    (*M_adj).fill(0);
    (*F).fill(0);

    // fill M_adj with the values of M and some changes for the matching algorithm
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < realmax; j++) {
	(*M_adj)[i][j] = (*M)[i][j];
	if ((*M)[i][j] == 0) {
	  (*M)[i][j] =  Lisa_Matching::REMOVED;
	}
      }
    }

    // edge-color M
    edgecolor(F, M_adj, 0);

    // fill the sequence LR
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < realmax; j++) {
	if ((*F)[i][j] > 0) {
	  LR[pos[i]][(*F)[i][j]-1] = j+1;
	}
      }
    }
}

//**************************************************************************

int main(int argc, char *argv[]) {

    // print a message that the program has started:
    cout << "This is the LiSA os_sum_ti Module" << endl;

    // The Lisa_Exceptionlist is forced for writing
    // error messages to cout. Then LiSA is able
    // to show the error messages of an external module.
    G_ExceptionList.set_output_to_cout();

    // open files and assure existence:
    if (argc != 3)
      {
        cout << "\nUsage: " << argv[0] << " [input file] [output file]\n";
        exit(1);
      }

    cout << "PID= " << getpid() << endl;

    // The both parameter of the module call are the name of the
    // input file and the name of the output file:
    ifstream i_strm(argv[1]);
    ofstream o_strm(argv[2]);
    if (!i_strm){
      cout << "ERROR: cannot find input file " << argv[1] << "." << endl;
      exit(1);
    }
    if (!o_strm){
      cout << "ERROR: cannot find output file " << argv[1] << "." << endl;
      exit(1);
    }
    i_strm.close();
    o_strm.close();

    //initialize class
    LisaXmlFile::initialize();
    //create Input handler
    LisaXmlFile xmlInput(LisaXmlFile::IMPLICIT);
    //communication objects
    Lisa_ProblemType Problem;
    Lisa_ControlParameters Parameter;
    Lisa_Values Values;

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
    if( !(xmlInput >> Problem))
    {
      cout << "ERROR: cannot read ProblemType , aborting program." << endl;
      exit(1);
    }
    //get ControlParameters
    if( !(xmlInput >> Parameter))
    {
      cout << "ERROR: cannot read ControlParameters , aborting program." << endl;
      exit(1);
    }
    //get Values
    if( !(xmlInput >> Values))
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

    string schedule = "ACTIVE";
    if(Parameter.defined("SCHEDULE")) schedule = Parameter.get_string("SCHEDULE");
    cout << "schedule = " << schedule << endl;

    // Define the LiSA  schedule object for storing results
    Lisa_Schedule result(Values.get_n(), Values.get_m());

    result.make_LR();

    // **************************************************************
    // *************** Insert your algorithm here: ******************
    // **************************************************************

    // checks if a nondelay schedule schould be generated (works for p_ij = 1)
    int nondelay = 0;
    if (schedule == "NONDELAY") {
      nondelay = 1;
    }

    int n = Values.get_n(); // number of jobs
    int m = Values.get_m(); // number of machines
    int max = n * m; // maximum number of columns of the job-time matrix M
    if(nondelay) {
      max = n > m ? (n > 2*m-3 ? n : ((n / m) + 1) * m ) : m;
    }
    int realmax = 0; // the real number of columns of the job-time matrix M (equal to cmax)
    int pos[n]; // positions of the ordered elements in D
    fill(pos, pos + n, 0);

    // sort the duedates
    sort_duedates(*(Values.DD), pos);

    // the job-time Matrix
    Lisa_Matrix<int> M(n,max);
    M.fill(0);

    // main algorithm - compute the job-time matrix M
    realmax = compute_M(*(Values.DD), &M, m, nondelay);

    // compute the sequence out of the job-time matrix
    compute_LR(*(result.LR), &M, pos, m, realmax);

    // ***************************************************************
    // ********************* End of Algorithm ************************
    // ***************************************************************

    // The object out_schedule contain the result of this algorithm,
    // which is written into the output file
    //create xml output handler
    LisaXmlFile xmlOutput(LisaXmlFile::SOLUTION);
    //pipe objects to this
    xmlOutput << Problem << Values << Parameter << result;
    //write content to a file
    xmlOutput.write(argv[2]);
}

//**************************************************************************

