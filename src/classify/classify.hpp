
#ifndef _classify_h 
#define _classify_h 

#include <string>
#include <vector>

#include "../main/global.hpp"
#include "../misc/except.hpp"
#include "../misc/except.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/ctrlpara.hpp"

#include "redgraph.hpp"

/// Scheduling problem classification.  
/** The class Lisa_Classify initializes a database of minimal np-hard
    from the file $LISA_HOME/data/classify/classify.bib
    
    The function also initializes the reduction graphs.

    The classification takes an instance of Lisa_ProblemType as input and 
    decides whether this problem is polynomially solvable, pseudo-polynomially
    solvable, np-hard, np-hard in the strong sense, or open, using the database
    of known minimal np-hard and maximal polynomial problems and the
    reduction graph.

    TODO for future versions: 
    If a given problem is open, the function classify() should 
    also give the corresponding lists of maximal open and minimal open
    problems 

    @see Lisa_RedGraph
    @author Martin Harborth
    @version 2.3final
*/
class Lisa_Classify{
private:
  // complexity flags
  enum COMPLEXITY {STR_NP_HARD=-2,NP_HARD,OPEN,PS_POLYN,POLYN};

  ///struct for records of BiBTeX entries.
  struct Lisa_ClassifyRecord{ 
    /// holding key-value pairs, like title author, year, etc
    Lisa_ControlParameters mmap;
    /// vector of complexitiys for each problem
    std::vector<COMPLEXITY>  complexity;  
    /// vector of problems known in this record
    std::vector<Lisa_ProblemType>  problems;
    /// number of problems known in this record
    int number_of_problems;       
  };
  
  /// number of entries in database
  int entries;
  /// vector records 
  std::vector<Lisa_ClassifyRecord> records;
  
  /// graph storing reduction path's
  Lisa_RedGraph redgraph;
  
  /// private constructor, create class from a file
  Lisa_Classify(const std::string& file);
  /// extract author, year, etc. from one bibtex entry
  void parse_bibentry(const std::string& entry);
  /// extract problem types from annotate
  void parse_annote(Lisa_ClassifyRecord *const record);    
  /// translate problem given in annote notation into Lisa_ProblemType notation
  int parse_problem(const std::string& problem, Lisa_ProblemType *const pt);
  /// output of results
  std::string result_text(const int np,const int mm_flag);  
  /// output of short references
  std::string reference_output(const int i ,const int j);
public:

  /** create a new instance from a file .. returns 0 if no instance could be
      created .. deletion is on behalf of the caller */
  static Lisa_Classify * make_instance(const std::string& file);
  
  /** returns a string telling whether the problem is np-hard or polynomially
      and also from which publications this follows */
  std::string classify(const Lisa_ProblemType& G_Problem);
  
  /** gives full references in bibtex format for each publication which has
      implications on the given problem */
  std::string full_reference(const Lisa_ProblemType& G_Problem);
};

#endif

