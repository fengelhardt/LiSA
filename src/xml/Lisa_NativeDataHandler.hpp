#ifndef LISA_NATIVE_DATA_HANDLER_HPP
#define LISA_NATIVE_DATA_HANDLER_HPP

#include <string>
#include <ostream>
#include <istream>

#include "Lisa_DataHandler.hpp"

class Lisa_NativeDataHandler : public Lisa_DataHandler {

public:		

		static std::string getName() {return "lisa_native";}

		virtual bool write_to_stream(const Lisa_Schedule&, std::string tag, std::ostream &);
		virtual bool write_to_stream(const Lisa_Values&,   std::string tag, std::ostream &);
		virtual bool write_to_stream(const Lisa_Graph&,    std::string tag, std::ostream &);
		
		virtual bool read_from_stream(Lisa_Schedule&,  std::string tag, std::istream &);
		virtual bool read_from_stream(Lisa_Values&,    std::string tag, std::istream &);
		virtual bool read_from_stream(Lisa_Graph&,     std::string tag, std::istream &);
		
};

#endif
