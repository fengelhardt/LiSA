/*
 * ************** status.cpp *******************************
 * 
 * description:     spezific system values
 * 
 * @author           Per Willenius
 *
 * date:            16.09.1998
 *
 * @version 2.3pre3
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



