/*
 * ******************** gen_ptst.hpp ******************************
 * 
 * description:      convention for the neighborhood parameter test
 * 
 * owner:            Andreas Winkler
 *
 * date:             14.04.1999
 *
 */

#include "../../basics/matrix.hpp"
#include "../../misc/except.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../algorithm/nb_iter/iterate.hpp"

#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>
  const int MAXINT  = 214748000;
  const int MAXLONG = 214748000;

// definition of parameters for the file-controlling
// following parameters are possible:
  TIMETYP OBJ_VALUE, BEST_OBJECTIVE;

  int n;  // number of jobs
  int i, j, k;
  int count, succ;
  int numb_fix_l, numb_fix_d, numb_fix_s, numb_var;
  int no_objective;
  string S;
  string CONFIG_HOME_St, LISA_HOME_St, FILE_St;
  string DESCRIPTION, CALL;













