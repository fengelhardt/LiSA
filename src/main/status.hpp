
#ifndef _status_h 
#define _status_h 

/// maximum width for Lisa_Canvas
const int MAX_CANV_WIDTH = 200000;
/// maximum height for Lisa_Canvas
const int  MAX_CANV_HEIGHT = 150000;


/// contains information on LiSA system values
/** @author  Per Willenius
    @version 2.3rc1
  */
class Lisa_Status{
public:
  /// seed value for lisa_random()
  long seed;
  
  /// time seed 
  /** used for random generating of all types of data and for generating the 
      sequence */
  long time_seed;
  
  /// machine seed
  /** used in random generating the processing times to get the values of the 
      Taillard Examples */
  long mach_seed;
  
  /// a flag
  /** used to mark that the size and position of the Lisa_Canvas is set by 
      LiSA */
  bool zoom;
  
  /// width and height of the canvas in the main window
  float width,height;
  
  ///constructor
  /** sets default values */
  Lisa_Status();
};

#endif

