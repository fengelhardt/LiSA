/**
 * @author  Per Willenius
 * @version 2.3rc1
 */

#include "status.hpp"

//**************************************************************************

Lisa_Status::Lisa_Status(){
   /// set random seed value in each start with the same values
  seed=476452;
  time_seed=560048;
  mach_seed=398583;
  zoom=0;
  width=height=500;
}

//**************************************************************************

