/*                             Filename: Main.c                              */
/*                                                                           */
/*                               main program                                */
/*                                                                           */
//   @version 2.3pre3
/* ************************************************************************* */


#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>

#include <iostream>
#include <fstream>

#include "../../basics/matrix.hpp"
#include "../../scheduling/js_sched.hpp"
#include "../../lisa/ptype.hpp"
#include "../../scheduling/schedule.hpp"
#include "../../lisa/lvalues.hpp"
#include "../../lisa/ctrlpara.hpp"
#include "../../misc/except.hpp"

#include "jb_block.hpp"
#include "jb_brnch.hpp"
#include "jb_dtstr.hpp"
#include "jb_fix.hpp"
#include "jb_hd_tl.hpp"
#include "jb_heur.hpp"
#include "jb_init.hpp"
#include "jb_list.hpp"
#include "jb_low.hpp"
#include "jb_rd_dt.hpp"
#include "jb_selct.hpp"
#include "jb_sort.hpp"
#include "jb_stack.hpp"
#include "jb_table.hpp"

