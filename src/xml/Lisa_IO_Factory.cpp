
#include <string>
#include "Lisa_IO_Factory.hpp"
#include "Lisa_NativeDataHandler.hpp"

Lisa_DataHandler* Lisa_IO_Factory::createHandler(std::string coding)
{
		if(coding == Lisa_NativeDataHandler::getName())
				return new Lisa_NativeDataHandler();
		else
				return NULL;
}
