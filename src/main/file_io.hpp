
#ifndef _file_io_h 
#define _file_io_h 

#include <string>
#include <vector>

class Lisa_ProblemType;
class Lisa_ControlParameters;
class Lisa_Values;
class Lisa_Schedule;

/// read the LiSA core variables from file
/** read Lisa_ProblemType, Lisa_Values and Lisa_Schedule from file
@author Per Willenius
@version 2.3final
*/
int read(std::string filename);

int read_xml(std::string filename);

/// write LiSA core variables to file
/** write Lisa_ProblemType, Lisa_Values and Lisa_Schedule to file
@author Per Willenius
@version 2.3final
*/
int save(std::string filename);

int save_xml(std::string filename);

void write_alg_call(std::string,
                    Lisa_ProblemType&,
                    Lisa_ControlParameters&, 
                    Lisa_Schedule&, 
                    Lisa_Values&);

void write_alg_call_xml(std::string,
                        Lisa_ProblemType&,
                        Lisa_ControlParameters&, 
                        Lisa_Schedule&, 
                        Lisa_Values&);

int read_schedule(std::string);

int read_schedule_xml(std::string);

int get_problemList(std::string,std::string,
                    std::vector<Lisa_ProblemType>& );

int get_problemList_xml(std::string,std::string,
                        std::vector<Lisa_ProblemType>& );

#endif

