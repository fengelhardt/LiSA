/*
 * ************** ptype.hpp *******************************
 * 
 * description:    definition of class Lisa_ProblemType, 
 *                 
 * owner:          Per Willenius
 *
 * date:           27.9.1999
 *
 * version:        V 1.0  
 * 
 * history:         7.  9. 98 kleinere Anpassungen TAU
 *                 19. 10. 99 Zugriffsfunktionen, Konsistenztest TAU
*/

/**  @name Classes for Communication of LiSA Modules
 
    Include file LiSA/src/lisa_dt/ptype.hpp, it uses iostream and string.

    @author Per Willenius, Thomas Tautenhahn and Martin Harborth
    @version 2.3pre1
*/ 

//@{


#ifndef _ptype_h 
#define _ptype_h 


//  ************** System Includes  ******************
#include "../basics/lisa_str.hpp"
#include <iostream.h>


//  ************** Lisa Includes  ******************
#include "../basics/global.hpp"
#include "../lisa_dt/filentry.hpp"


//  ************** Class Definitions ******************

//######################### Class Lisa_ProblemType ##########################
/** Problem Description
    contain machine environment, additional constraints and objective function 
    as tupel with TUPEL_INDEX entries
*/
class Lisa_ProblemType : public Lisa_FileEntry
{
private:
  // set  maschine environment corresponding to al 
  int setalpha(string al); 
  // set additional constraints corresponding to be 
  int setbeta(string be); 
  // set objective function  corresponding to ga
  int setgamma(string ga);
  // flag to indicate consistence of tupel
  bool vld;
  // maschine environment, additional constraints and objective function
  int tupel[TUPEL_INDEX];
public:
  Lisa_ProblemType();
  /// Lisa_ProblemType is valid iff valid==TRUE
  bool valid() const { return vld; }
  /** Main manipulation method, sets property and checks tuple for 
      consistency. Older properties which cannot be used in connection 
      with new property are deleted or altered in a sensible way. Return 
      value is number of old properties which are altered.              */  
  int set_property(const int prop, const int value);
  /// get tuple entry back:
  int get_property(const int prop) const {return tupel[prop];}
  /// program parts which are not up to date need obsolet definitions:
  int get_property_old(const int prop) const;
  /// erase all tupel entries:
  void reset();
  /// Number of maschines (if fixed)
  int m_no; 
  /// Number of jobs (if fixed)
  int n_no; 
  /// write the whole problem in a string
  string output_problem() const;
  /// write the maschine environment in a string
  string output_alpha() const; 
  /// write the additional constraints in a string
  string output_beta() const; 
  ///  write the objective function in a string
  string output_gamma() const; 
  /// delete one additional constraint
  int delbeta(int nr);
  /// delete all additional constraints
  void delbeta(); 
  /// returns the name of the corresponding entry as string
  string name(int tupelEntry, int identifier ) const;  
  /// return the number of the corresponding entry to the string
  int name(int tupelEntry, string identifier) const;
  void write(ostream& = cout) const;
  void read(istream& = cin);
};


#endif

//@}




























