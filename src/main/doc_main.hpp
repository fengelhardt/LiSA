/*
 * ************** doc_main.h *******************************
 * 
 * This file is only a starting point for the doc++ documentation.
 * It has no meaning in the program.
 *
 * Owner: Thomas Tautenhahn
 *
 * 19.9.1998 
 *
 * modified by Martin Harborth, 27.10.1998
 * modified by Per Willenius, 3.5.2000
*/

/** @name LiSA - Library of Scheduling Algorithms */
//@{
  /** @name General Information */
  //@{
    //@Include: doc_general.hpp
    //@Include: doc_tcltk.hpp
  //@}
  /** @name Basic Data Types and Definitions 
      Contains lists, matrices, graphs, orders and so on. For sake of 
      compatibility of program parts, please stick to these classes as far 
      as possible. If there are errors or missing features, please inform the 
      Owner of the file as soon as possible! */
  //@{
    //@Include: ../basics/matrix.hpp
    //@Include: ../basics/graph.hpp
    //@Include: ../basics/list.hpp
    //@Include: ../basics/order.hpp
    //@Include: ../basics/except.hpp
   //@Include: ../basics/lisa_str.hpp
   //@Include: ../basics/matching.hpp
  //@}     
  /** @name LiSA Data Classes */
  //@{ 
    //@Include: ../lisa_dt/lsaobjct.hpp
    //@Include: ../lisa_dt/filentry.hpp
    //@Include: ../lisa_dt/ptype.hpp
    //@Include: ../lisa_dt/lvalues.hpp
    //@Include: ../lisa_dt/schedule.hpp
    //@Include: ../lisa_dt/xsched.hpp
    //@Include: ../lisa_dt/ctrlpara.hpp
    //@Include: ../lisa_dt/mo.hpp
    //@Include: ../lisa_dt/lisapref.hpp
    //@Include: ../basics/status.hpp
    //@Include: ../lisa_dt/tclvar.hpp
     /** @name Performance Oriented Schedule Classes
         Use these classes to run in algorithms! */
    //@{
      //@Include: ../lisa_dt/schedule/sp_sched.hpp
      //@Include: ../lisa_dt/schedule/os_sched.hpp
      //@Include: ../lisa_dt/schedule/js_sched.hpp
      //@Include: ../lisa_dt/schedule/m1_sched.hpp
    //@} 
  //@}  

  /** @name Data Types to handle window in and output    
      */
  //@{ 
    //@Include: ../desktop/basicwin.hpp
    //@Include: ../desktop/win_obj.hpp
    //@Include: ../graphics/gantt.hpp
  //@}   
  /** @name Scheduling Problem Classification 
      This section contains classes, functions and variables used for
      the scheduling problem classification. */
  //@{
    //@Include: ../classify/classify.hpp
    //@Include: ../classify/database.hpp
    //@Include: ../classify/redgraph.hpp
  //@}
  /** @name Communication with external algorithms */
  //@{
   //@Include: ../communic/alg_call.hpp
   //@Include: ../communic/ngbhptst.hpp
  //@}
  /** @name internal algorithms */
  //@{
   //@Include: ../algo/dispatch.hpp
  //@}
  /** @name Functions */
  //@{ 
    //@Include: ../desktop/callback.hpp
    //@Include: ../desktop/file_io.hpp
    //@Include: ../misc/int2str.hpp
    //@Include: ../desktop/tcl_c.hpp
    //@Include: ../desktop/c_tcl.hpp
  //@}   

  /** @name External Modules */
  //@{
    //@Include: ../external/neighbor/doc_nghb.hpp
    //@Include: ../external/irred/irred.cpp
    //@Include: ../external/match/match.cpp
    //@Include: ../external/bb/bb_main.hpp
    //@Include: ../external/bottle/bottle.cpp
    //@Include: ../external/single/single.cpp
  //@}

  // and so on
//@}

