#ifndef LISA_IO_FACTOR_HPP
#define LISA_IO_FACTOR_HPP

#include <string>
#include "Lisa_DataHandler.hpp"

class Lisa_IO_Factory {
public:
		
		static Lisa_DataHandler* createHandler(std::string coding = "lisa_native");
		
};

#endif
