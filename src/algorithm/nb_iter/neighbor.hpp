/*
 * ******************** neighbor.hpp ******************************
 * 
 * description:      general problem independent neighborhood class
 * 
 * owner:            Andreas Winkler
 *
 * date:             14.12.1998
 *
 */




#ifndef _nghbour_h
#define _nghbour_h

#include <stdlib.h>
#include <stdio.h>

#include "../../main/global.hpp"

const int ORIG_SOLUTION     = 0;
const int WORK_SOLUTION     = 1;
const int BEST_NGH_SOLUTION = 3;
const int BEST_SOLUTION     = 2;
const int ENUM = 1;
const int RAND = 2;
const int NO_NGHBOURS = 2;
const int JO = 1;
const int MO = 2;

/** @name Neighborhoods
    These are the availible neighborhoods for the several problems.
    Observe, that every problem dependent neighborhood must be inherited
    from the problem independent class #Lisa_Neighborhood#.
*/

//@{
/** General Problem Independent Neighborhood Class.\\

    This is the problem independent neighborhood class.
    Observe, that every problem dependent neighborhood must be inherited
    from this class. All functions here are dummys.\\
    
    The communication between #prepare_move()#, #do_move()#, 
    #use_tabulist()#, #set_tabulist()# and all other functions
    has to be be implemented internally (not public).\\

    You should take care than every method does 
    what it should do ;). Otherwise the algorithms might
    (and most times will) not work. The comments 
    should make clear what every method is intended to do,
    additionally there are several examples which will show
    that it is not really that hard to do.\\

    The class should have the ability to represent and handle 
    at least 4 different solutions which are indicated by the flags 
    #ORIG_SOLUTION#,#WORK_SOLUTION#,#BEST_SOLUTION# and #BEST_NGH_SOLUTION#.\\

    #ORIG_SOLUTION# is the one for which neighbours are created. If enumerated 
    neighbours are created you need to keep track of that.

    Additionally 2 representations for moves or solutions that are 
    tabu (I will call them #work_tabu# and #copy_tabu#) and of course 
    a tabu list will be needed if you want to use this neighbourhood
    with tabu search.\\
    
    When creating a neighbourhood object you will have to initialize #work_tabu#, 
    #copy_tabu#, #ORIG_SOLUTION# and #WORK_SOLUTION# internally. The solution you want 
    to start with has to be placed in #ORIG_SOLUTION#. 

    A commented example can be found in #LiSA/src/utility/travel/#.
    The implementations for the Open Shop, Job Shop and Single Machine problems 
    found in subdirectories from the directory where this file is located
    can serve as examples as well.\\

    @see Lisa_Tabu
    @see Lisa_Iterator
    
    @author Thomas Tautenhahn and Andreas Winkler
    @version 2.3pre3
*/
class Lisa_Neighborhood{ 
protected:
  /** the objective type\\ */
  int objective_type;

public: 
  
  /** set the objective type ...\\
      
      LiSA's schedule objects support different objective types like #CMAX#, #LMAX#, etc. 
      The Iteration algorithm uses this method if it wants to set this for a 
      neighbourhood object and its underlying solutions (which are of 
      course represented by schedule objects) */ 
  virtual void set_objective_type(int objective_type) = 0;

  /** set the objective type for a given solution ...\\

      Just as #set_objective_type()# this function is used to set the objective 
      for a given solution (which can be #WORK_SOLUTION#,#BEST_SOLUTION#, etc).*/
  virtual void set_objective(int objective_type, int solution) = 0;
  
  /** return the objective for a given solution ...\\

      Objectives can be requested for #WORK_SOLUTION#,#BEST_SOLUTION#,
      #BEST_NGH_SOLUTION# and #ORIG_SOLUTION# */
  virtual TIMETYP get_objective_value(int solution) = 0;
  
  /** accept the current #WORK_SOLUTION# ...\\
      
      copy #WORK_SOLUTION# to #ORIG_SOLUTION# and reset enumeration
      since #ORIG_SOLUTION# has been changed. */
  virtual int accept_solution() = 0;

  /** accept the current #BEST_NGH_SOLUTION# ...\\

      copy #BEST_NGH_SOLUTION# to #ORIG_SOLUTION# and reset enumeration
      since #ORIG_SOLUTION# has been changed */
  virtual int accept_best_ngh() = 0;

  /** copy #ORIG_SOLUTION# to #BEST_SOLUTION#\\ */
  virtual int put_orig_to_best() = 0;

  /** copy #WORK_SOLUTION# to #BEST_NGH_SOLUTION#\\ */
  virtual int put_work_to_best_ngh() = 0;
 
  /** propose a possible move ...\\
      
      This function should find a neighbour of #ORIG_SOLUTION#. 
      If one was found it has to be stored so it can later be used 
      in #do_move()#. The tabu representation for the proposed 
      move/solution also has to be put in #work_tabu# so it can be 
      compared with the tabu list with a call to #use_tabulist()#\\ 

      In #ENUM# mode this function has to generate all neighbours of 
      #ORIG_SOLUTION# at successive calls. It has to reset enumeration and
      return #NO_NGHBOURS# if all neighbours have been created.\\
      
      The parameter is #ENUM# or #RAND# for enumerative or random
      generation of a neighbour. It returns #OK# if a neighbour was found,
      otherwise #!OK#. In #ENUM# mode #NO_NGHBOURS# should be returned if no 
      neighbours are left to enumerate. */
  virtual int prepare_move(int type) = 0;
  
  /** propose a move to an anti neighbour ...\\

      This Method should basically do the same as #prepare_move()#
      e.g. find a Neighbour of #ORIG_SOLUTION# and store it so it 
      can be used later. Additionally it has to put the tabu representation 
      of the proposed move/solution in #work_tabu#. An anti neighbour should 
      be a solution that is a little more far away from #ORIG_SOLUTION# so the 
      possibility exists to jump out of local minima but without destroying 
      the found #ORIG_SOLUTION# completly. 

      This method is only used in combination with the #SA_anti# algorithm.
      It returns #OK# if an anti neighbour was created otherwise #!OK#. 
      If no anti neighbours have been defined simply return #!OK# */
  virtual int anti_neighbor() = 0;
  
  /** do a proposed move ...\\
      
      This function has to put the last neighbour that was found in
      #prepare_move()# or #anti_neighbor()# into #WORK_SOLUTION#. The tabu 
      representation of #ORIG_SOLUTION# or the move back to #ORIG_SOLUTION# 
      has to be put into work_tabu so it can be put on the tabu list. It 
      returns #OK# if all that could be done otherwise #!OK# */ 
  virtual int do_move() = 0;


  /** create a tabu list ...\\

      This function should create a tabu list with the specified length.
      Please note that LiSA already has a tabu list #Lisa_Tabu# which can be used here.*/
  virtual int init_tabulist(unsigned int length) {return OK;}
  
  /** test if a move/solution is in the tabu list ...\\

      This function should check whether #work_tabu# is in the tabu list.
      It should return #OK# if work_tabu IS NOT in the tabu list, if 
      work_tabu IS in the list it should return #!OK#. */
  virtual int use_tabulist() {return OK;}

  /** put a move/solution in the tabu list ...\\
      
      Put #copy_tabu# into the tabu list. #OK# should be returned if that was done 
      successfully, otherwise #!OK# should be returned. */
  virtual int set_tabulist() {return OK;}
  
  /** handle tabu representations ...\\
      
      Copy #work_tabu# to #copy_tabu# so it can be put into the tabu list later. */
  virtual void store_tabu_param() {}

  /** clean tabu representations ...\\

      Reset #work_tabu# to some initial value. */
  virtual void clean_tabu_param() {}

  /** destructor ...\\

      Just a virtual destructor*/
  virtual ~Lisa_Neighborhood(){}
};

#endif

   /** @name Problem Dependent Neighborhoods  */
    //@{
    /** @name Single-Machine Neighborhoods  */
      //@{
      //@Include: one_mach/m1_api.hpp
      //@Include: one_mach/m1_shft.hpp
    //@}
    /** @name Open-Shop Neighborhoods  */
    //@{
      //@Include: osp/osp_api.hpp
      //@Include: osp/osp_shft.hpp
      //@Include: osp/osp_3api.hpp
      //@Include: osp/osp_3_cr.hpp
      //@Include: osp/osp_crpi.hpp
      //@Include: osp/osp_crsh.hpp
      //@Include: osp/osp_cr_r.hpp
      //@Include: osp/osp_blpi.hpp
      //@Include: osp/osp_blsh.hpp
    //@}
    /** @name Job-Shop Neighborhoods  */
    //@{
      //@Include: jsp/jsp_api.hpp
      //@Include: jsp/jsp_shft.hpp
      //@Include: jsp/jsp_3api.hpp
      //@Include: jsp/jsp_3_cr.hpp
      //@Include: jsp/jsp_crpi.hpp
      //@Include: jsp/jsp_crsh.hpp
      //@Include: jsp/jsp_blpi.hpp
      //@Include: jsp/jsp_blsh.hpp
    //@}
  //@}
//@}



