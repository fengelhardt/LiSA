/*
 * ************** status.C *******************************
 * 
 * description:     spezific system values
 * 
 * owner:           Per Willenius
 *
 * date:            16.09.1998
 *
 * version:         V 1.0
*/



// ************************ Local Includes ************************
#include "status.hpp"


// *********************** functions *************************

Lisa_Status::Lisa_Status(){
   /// set random seed value in each start with the same values
  seed=476452;
  time_seed=560048;
  mach_seed=398583;
  zoom=0;
  width=height=500;
}



