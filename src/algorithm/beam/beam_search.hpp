#ifndef BEAM_SEARCH_HPP
#define BEAM_SEARCH_HPP

#include "../../main/global.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/ptype.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../misc/except.hpp"
#include "../../basics/pair.hpp"
#include "beam_orders.hpp"

class LisaXmlFile;

class BeamSearch {

public:
  explicit BeamSearch();
  explicit BeamSearch(const BeamSearch& other);
  
  typedef enum {
    insert1,
    insert2
  } InsertionMethod;

  typedef enum {
    CObjective,
    CLast,
    CLb_Sum_Ci 
  }  CostFunc;
  
  typedef enum {
    machines,
    jobs,
    both
  } AttachmentRule;

  typedef enum {
    INSERT,
    ATTACH 
  } Mode; 

  typedef enum {
    FCFS,
    RANDOM,
    LPT,
    SPT  } TieBreakMode;

  int problemtype;
  int beam_width;
  InsertionMethod insertionMethod;
  InsertionOrder iord;
  AttachmentRule attach;
  CostFunc costFunc;
  int destObjective;
  Mode mode;
  TieBreakMode tieBreak;
  
  
  bool init(LisaXmlFile& input);
  bool run();

  Lisa_ProblemType Problem;
  Lisa_ControlParameters Parameter;
  Lisa_Values Values;

  Lisa_OsProblem *problem;
  Lisa_Schedule * result;
  TIMETYP value;
  Lisa_Order *order;
  ~BeamSearch();
  TIMETYP guessObjective();

  int step, n_ops;
  

private:
  void clear();
  TIMETYP tie(int i, int j);
  typedef std::pair<int,int> Operation;
  typedef std::pair<int,int> OpRankPos;
  typedef std::pair<Operation, OpRankPos> OpInsertion;
  typedef std::vector<OpInsertion> InsertionList;
  bool getNextOp(class B_Node* parent, BeamSearch::Operation& next);
  void getDescendentInsertions(class B_Node& parent, const Operation& op, InsertionList& ins);
  void getDescendentAppendings(class B_Node& parent, const Operation& op, InsertionList& ins);
  

};


#endif
