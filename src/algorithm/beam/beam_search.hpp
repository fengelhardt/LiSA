#ifndef BEAM_SEARCH_HPP
#define BEAM_SEARCH_HPP

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "beam_orders.hpp"

class LisaXmlFile;

class BeamSearch {

public:
  explicit BeamSearch();
  
  typedef enum {
    insert1,
    insert2
  } InsertionMethod;

  typedef enum {
    CObjective,
    CLast
  }  CostFunc;
  

  int problemtype;
  int beam_width;
  InsertionMethod insertionMethod;
  InsertionOrder iord;
  CostFunc costFunc;
  int destObjective;
  
  bool init(LisaXmlFile& input);
  bool run();

  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;

  Lisa_OsProblem *problem;
  Lisa_Schedule * result;
  ~BeamSearch();
  TIMETYP guessObjective();

private:
  void clear();

};


#endif
