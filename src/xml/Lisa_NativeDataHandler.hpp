#ifndef LISA_NATIVE_DATA_HANDLER_HPP
#define LISA_NATIVE_DATA_HANDLER_HPP

#include <string>
#include <ostream>
#include <istream>

#include "Lisa_DataHandler.hpp"

//@{
/**
 * Implementation of the native way of storing LiSA data.
 * Essentially this is done by writing matrices to stream in a textual fashion.
 *
 * The documentation is provided in the Lisa_DataHandler class.
 * 
 * @author Jan Tusch
 * @version 3.0pre1
*/
/// IO-Handler for Lisa Native Data Model
class Lisa_NativeDataHandler : public Lisa_DataHandler {
  
 public:		
  static std::string getName() {return "lisa_native";}
 protected:
  virtual bool write_to_stream(const Lisa_Schedule& S, std::string tag, std::ostream & out);
  virtual bool write_to_stream(const Lisa_Values& V,   std::string tag, std::ostream & out);
  virtual bool write_to_stream(const Lisa_Graph& G,    std::string tag, std::ostream & out);
  
  virtual bool read_from_stream(Lisa_Schedule& S,  std::string tag, std::istream & in);
  virtual bool read_from_stream(Lisa_Values& V,    std::string tag, std::istream & in);
  virtual bool read_from_stream(Lisa_Graph& G,     std::string tag, std::istream & in);
  
};


//@}
#endif
