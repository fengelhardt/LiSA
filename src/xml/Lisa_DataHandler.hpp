#ifndef LISA_DATA_HANDLER_HPP
#define LISA_DATA_HANDLER_HPP

#include <string>
#include <ostream>
#include <istream>

class Lisa_Schedule;
class Lisa_Values;
class Lisa_Graph;

//@{
/**
 * Class is comprised by a public and a protected interface.
 * Pure virtual functions of the latter are implemented by child classes to perform object en-/decoding.
 * The former interface is just a wrapper for the sake of further extension to other sources and destinations than streams.
 *
 * Objects are obtained via Lisa_IO_Factory::createHandler . 
 * 
 * @author Jan Tusch
 * @version 3.0
 */
/// Abstract base class for IO data handler.
class Lisa_DataHandler {

public:
  virtual ~Lisa_DataHandler(){}
  
 protected:
  
  /**
   * @param S Schedule holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Schedule to stream according to given tag
  virtual bool write_to_stream(const Lisa_Schedule& S, std::string tag, std::ostream &out) = 0;
  
  /**
   * @param V Values holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Values to stream according to given tag
  virtual bool write_to_stream(const Lisa_Values& V,   std::string tag, std::ostream &out) = 0;
  
  /**
   * @param G Graph holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Graph to stream according to given tag
  virtual bool write_to_stream(const Lisa_Graph& G,    std::string tag, std::ostream &out) = 0;
	
  /**
   * @param S Schedule to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */	
  /// read Schedule from stream according to given tag
  virtual bool read_from_stream(Lisa_Schedule& S,  std::string tag, std::istream &in) = 0;
  
  /**
   * @param V Values to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */
  /// read values from stream according to given tag
  virtual bool read_from_stream(Lisa_Values& V,    std::string tag, std::istream &in) = 0;
  
  /**
   * @param G Graph to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */
  /// read Graph from stream according to given tag
  virtual bool read_from_stream(Lisa_Graph& G,     std::string tag, std::istream &in) = 0;
  
 public:
  
  /**
   * Every model implementation should have a unique name to be identified by the Lisa_IO_Factory.
   * @return Model Identifier of class
   */
  /// obtain data model identifier
  static std::string getName() {return "";}
   /**
   * @param S Schedule holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Schedule to stream according to given tag
  inline  bool write(const Lisa_Schedule& S, std::string tag, std::ostream &out){
    return write_to_stream(S,tag,out);
  }
   
  /**
   * @param V Values holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Values to stream according to given tag
  inline bool write(const Lisa_Values& V,   std::string tag, std::ostream &out){
    return write_to_stream(V,tag,out);
  }
  
  /**
   * @param G Graph holding the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param out stream to recieve data
   * @return true on succes, false otherwise
   */
  /// write Graph to stream according to given tag
  inline bool write(const Lisa_Graph& G,    std::string tag, std::ostream &out){
    return write_to_stream(G,tag,out);
  }
  
	
  /**
   * @param S Schedule to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */	
  /// read Schedule from stream according to given tag
  inline bool read(Lisa_Schedule& S,  std::string tag, std::istream &in){
    return read_from_stream(S,tag,in);
  }
  
  /**
   * @param V Values to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */
  /// read values from stream according to given tag
  inline bool read(Lisa_Values& V,    std::string tag, std::istream &in){
    return read_from_stream(V,tag,in);
  }
  
  /**
   * @param G Graph to store the data
   * @param tag Information about what to handle (e.g. the node name for XML)
   * @param in stream providing data
   * @return true on succes, false otherwise
   */
  /// read Graph from stream according to given tag
  inline bool read(Lisa_Graph& G,     std::string tag, std::istream &in){
    return read_from_stream(G,tag,in);
  }

};
//@}
#endif
