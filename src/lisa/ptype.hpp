
#ifndef _ptype_h 
#define _ptype_h 

#include <string>
#include <iostream>

#include "../main/global.hpp"
#include "filentry.hpp"

/// Problem Description
/** contains machine environment, additional constraints and objective function 
    as tupel with TUPEL_INDEX entries
    
    @author Per Willenius, Thomas Tautenhahn, Martin Harborth
    @version 2.3rc1
*/
class Lisa_ProblemType : public Lisa_FileEntry{
private:

  /// set  maschine environment corresponding to al 
  int setalpha(std::string al); 

  /// set additional constraints corresponding to be 
  int setbeta(std::string be); 

  /// set objective function  corresponding to ga
  int setgamma(std::string ga);

  /// flag to indicate consistence of tupel
  bool vld;

  /// maschine environment, additional constraints and objective function
  int tupel[TUPEL_INDEX];

public:

  /// constructor 
  Lisa_ProblemType();

  /// returns if the current problemtype is valid
  bool inline valid() const { return vld; }

  /// Main manipulation method.
  /** Sets property and checks tuple for consistency. Older properties which 
      cannot be used in connection with new property are deleted or altered in 
      a sensible way. Return value is number of old properties which are
      altered. */  
  int set_property(const int prop, const int value);

  /// get tuple entry back
  int get_property(const int prop) const {return tupel[prop];}

  /// program parts which are not up to date need obsolet definitions:
  int get_property_old(const int prop) const;

  /// erase all tupel entries:
  void reset();

  /// Number of maschines (if fixed)
  unsigned int m_no; 

  /// Number of jobs (if fixed)
  unsigned int n_no; 

  /// write the whole problem in a string
  std::string output_problem() const;

  /// write the maschine environment in a string
  std::string output_alpha() const; 

  /// write the additional constraints in a string
  std::string output_beta() const; 

  ///  write the objective function in a string
  std::string output_gamma() const; 

  /// delete one additional constraint
  int delbeta(int nr);

  /// delete all additional constraints
  void delbeta(); 

  /// returns the name of the corresponding entry as string
  std::string name(int tupelEntry, int identifier ) const;  

  /// return the number of the corresponding entry to the string
  int name(int tupelEntry, std::string identifier) const;
  
  /// write object to a stream
  void write(std::ostream& = std::cout) const;

  /// write object to a stream
  void read(std::istream& = std::cin);
};

#endif

