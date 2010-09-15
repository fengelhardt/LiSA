#ifndef LISA_IO_FACTOR_HPP
#define LISA_IO_FACTOR_HPP

#include <string>
#include "Lisa_DataHandler.hpp"


//@{
/**
 * An ordinary Factory class.
 * The class constists of single static funtion to create a DataHandler for IO of LiSA objects.
 *
 * @author Jan Tusch
 * @version 3.0
*/
//@}

//@{

/// Factory class to create Lisa_DataHandler.
class Lisa_IO_Factory {
 public:
  /*!
     Function to obtain a handler identified by the given string.
     This is called by LisaXmlFile to pick the right text-data-translation according to the model tag of the nodes.
     If you want to add a new model do the following:
     - Derive a class from Lisa_DataHandler performing actual en-/decoding.
     - Add the neccessary lines to the implementation of this function to introduce your class to the factory.
     - Use your coding identifier with LisaXmlFile class.
     @param coding Identifier of the data model
     @return Pointer to DataHandler if available, NULL otherwise.
  */
  /// factory function
  static Lisa_DataHandler* createHandler(std::string coding = "lisa_native");
};

//@}
#endif
