
#ifndef _file_io_h 
#define _file_io_h 

#include <string>

/// read the LiSA core variables from file
/** read Lisa_ProblemType, Lisa_Values and Lisa_Schedule from file
    @author Per Willenius
    @version 2.3rc1
 */
int read(std::string filename);

/// write LiSA core variables to file
/** write Lisa_ProblemType, Lisa_Values and Lisa_Schedule to file
    @author Per Willenius
    @version 2.3rc1
 */
int save(std::string filename);

#endif

