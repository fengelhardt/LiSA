#ifndef _LISAXMLDEFS_HPP_
#define _LISAXMLDEFS_HPP_

/// file used internally for xml implementation.
/// containes definitions for algorithm description objects and helper functions

#include <string>
#include <vector>

class Lisa_ProblemType;

typedef struct {
		std::string name;
		long default_value;
		//evtl. add range information here
		std::string caption;
} Integer_Control;
		
typedef struct {
		std::string name;
		double default_value;
		//evtl. add range information here
		std::string caption;
} Real_Control;
		
typedef struct {
		std::string name;
		std::string caption;
		std::vector < std::string > items;
} Choice_Control;
				
struct Lisa_ExtAlg {
		//GENRAL
		std::string name;
		std::string type;
		std::string call;
		std::string code;
		std::string helpFile;
				
		std::vector <Lisa_ProblemType> handle_exact;
				
		std::vector <Lisa_ProblemType> handle_heuristic;
				
		std::vector <Integer_Control> Integer_Controls;
		std::vector <Real_Control> Real_Controls;
		std::vector <Choice_Control> Choice_Controls;

		typedef std::pair< std::string,std::string > Fixed_Control;

		std::vector< Fixed_Control  > Fixed_Controls;
	
} ;

xmlChar* string2xmlChar(const std::string& input);
bool xmlChar2string(const xmlChar* in , std::string& result);

#endif
