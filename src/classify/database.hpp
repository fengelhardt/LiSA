/** @name Database of Problems with Known Complexity
    This section contains the classes, functions and varibales used for
    the database of minimal np-hard and maximal polynomial problems for 
    scheduling problem classification. 
*/
//@{

#ifndef _database_h
#define _database_h

#include "../misc/except.hpp"
#include "../main/global.hpp"
#include "../lisa/ptype.hpp"

#include "redgraph.hpp"

/// maximal number of BiBTeX entries.
const int MAX_REC=200;
/// maximal number of characters per line.
const int LINE=82;
/// maximal number of characters per BiBTeX entry.
const int BIBENTRY=60*LINE;
/// maximal characters per author item. 
const int AUTHOR=4*LINE;
/// maximal characters per year item.  
const int YEAR=LINE/2;
/// maximal characters per annote item. 
const int ANNOTE=40*LINE;
/// maximal number of problems per annote item.
const int MAX_PRBLMS=40;

const int NP_HARD=-1;
const int STR_NP_HARD=-2;
const int PS_POLYN=1;
const int POLYN=2;
const int OPEN=0;

const int NO_AUTHOR=-1;
const int NO_ANNOTE=-2;



/// BiBTeX Annote entry: contains the problem and its complexity status
class TexAnnote
{
private:
  
public:
  /// problem in ascii code
  char prbl[LINE];
  /// complexity status of prbl
  int  np_flag;
};


/// Class for records of BiBTeX entries.
class Record
{
 private:
  
 public:
  ///  whole bibtex entry as ascii text, read step by step
  char       bibentry[BIBENTRY];  
  ///  author item
  char       author[AUTHOR];     
  ///  year item
  char       year[YEAR];         
  ///  array of annote items
  TexAnnote  tex_an[MAX_PRBLMS];  
  ///  array of tupel items
  Lisa_ProblemType  problem[MAX_PRBLMS];
  /// number of annote and tupel items per bibtex entry
  int        no_of_prbls;       
};

 
/** Database class: minimal np-hard and maximal polynomial problems.
    A database object parses the BiBTeX file {\tt classify.bib}
    containing all known polynomially solvable, pseudopolynomially
    solvable, np-hard, and np-hard (in strong sense) problems.

    @see Lisa_classify
    @author Martin Harborth
    @version 2.3pre2
*/  
class Lisa_DataBase
{
 private:
  /// extracting author, year, etc. from bibtex entry
  int    bib_entry(char*,int);    
  /// translating problem given in annote notation into Lisa_Problem notation
  int    prbl_into_tupel(char*,int,int);
  /// error output function for errors while reading the database
  int    error_output(int,int);

 public:
  /// initializing and reading database
  Lisa_DataBase(string);  
  /// array of bibtex entries
  Record E[MAX_REC]; 
  /// total number of bibtex entries in database
  int    entries;   
  /// output of database
  int    output(void);
};

#endif
//@}




