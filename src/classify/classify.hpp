
#ifndef _classify_h 
#define _classify_h 

#include <string>
#include <tk.h>

#include "../misc/except.hpp"
#include "../desktop/c_tcl.hpp"
#include "../lisa/ptype.hpp"
#include "database.hpp"

/// Scheduling problem classification - main function.  
/**  The function Lisa_classify initializes the database of minimal np-hard
    and maximal polynomial problems located in LISA_HOME/data/classify/classify.bib
    
    The function also initializes the reduction graphs.

    This function takes an instance of Lisa_Problem as input and decides
    whether this problem is polynomially solvable, pseudo-polynomially
    solvable, np-hard, np-hard in the strong sense, or open.

    TODO for future versions: 
    If a given problem is open, the function Lisa_classify should 
    also give the corresponding lists of maximall open and minimal open
    problems 

    @param G_Problem as input problem object
    @param home LISA_HOME path
    @param file relative path of the BibTeX data base file
    @return string classification result text
    @see Lisa_DataBase 
    @see Lisa_RedGraph
    @author Martin Harborth
    @version 2.3final
*/
std::string Lisa_classify(Lisa_ProblemType* G_Problem,
                          std::string home,std::string file);  

/// output of complete reference list concerning current problem
std::string Lisa_full_ref(Lisa_ProblemType* G_Problem,
                          std::string home,std::string file);  

/// output of results
std::string result_text(int,int);  

/// output of short references
std::string reference_output(int,int,Lisa_DataBase*);

#endif

