/*
 * ****************** status.h *******************************
 * 
 * description:     spezific system values
 * 
 * @author           Per Willenius
 *
 * date:            16.09.1998
 *
 * @version 2.3pre3
*/

#ifndef _status_h 
#define _status_h 

//  ********************** defines ***********************
const int MAX_CANV_WIDTH = 200000;
const int  MAX_CANV_HEIGHT = 150000;

//  **************  Class Definitions  ******************

/** Contain informations of LiSA system values
  */
class Lisa_Status
{
public:
  /// seed value for Lisa_random()
  long seed;
  /// time seed (used for random generating of all types of data
  /// and for generating the sequence
  long time_seed;
  /// machine seed (used in random generating the processing times)
  /// to get the values of the Taillard Examples
  long mach_seed;
  /// A Flag which is used to mark that the size and Position of the
  /// Lisa_Canvas is set by Lisa
  bool zoom; 
  /// width and height of the Canvas in the main-Window
  float width,height;
  Lisa_Status();
};

#endif











