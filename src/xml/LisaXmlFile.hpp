#ifndef _LISAXMLFILE_HPP_
#define _LISAXMLFILE_HPP_

#include <map>
#include <string>
#include <vector>

//forward declarations to avoid unneccessary includes

template <class T> class Lisa_List;

class Lisa_ProblemType;
class Lisa_Values;
class Lisa_Schedule;
class Lisa_ScheduleNode;
class Lisa_ControlParameters;
class Lisa_Preferences;
class Lisa_Graph;

#ifndef LISA_XML_IMPL

struct xmlDoc;
struct xmlNode;
struct xmlNs;
struct xmlDtd;
struct xmlCharEncodingHandler;

typedef xmlDoc*  xmlDocPtr ;
typedef xmlNode* xmlNodePtr ;
typedef xmlNs* xmlNsPtr ;
typedef xmlDtd* xmlDtdPtr ;
typedef unsigned char xmlChar;
typedef void* xmlCharEncodingHandlerPtr;

#endif



//@{
/*! 
 *  This class represents an XML ( http://www.xmlsoft.org ) document that can be written to	or read from a file.\n
 *		It provides functionality for submission and retrieval of LiSA objects.\n
 *  Here are some things to notice befor using this class.
 *
 *  - Document types\n
 *  XML imposes a document type on each document. 
 *  The type of a document in turn imposes a structure on the document itself.
 *  This structure is given in a so called Document Type Description (DTD).
 *  You define the type of a document at construction time. 
 *  Hence, you only can submit or retrieve data that is given in a document of such a type.
 *  After reading a document you should ask for the type to know what information is provided.
 *  
 *		- Validation\n
 *		It also can validate documents against LiSA's DTD.
 *		If validation is active it first attempts to validate the document against the DTD
 *		the document commits to. On failure another validation against the internal DTD is done.
 *		Validation is done both before writing and after reading a document.
 *
 *  - Consistency\n
 *  You may permorm subsequent sumissions to a single document. 
 *  If there may be only one of the object you submit the current (if existing) one is replaced.
 *  Otherwise it is inserted in the tree.
 *  The consitency is checked roughly at submission time. 
 *  An inaproriate operation will invalidate the object and make it unusable.
 *
 *  - Encoding\n
 *  To know what character set is used in a document it crucial for further processing the text.
 *  The current implementation uses the "ISO-8859-1" character set for central europe.
 *  You should avoid to change the encoding if not neccessary.
 *
 *  - Error Handling\n
 *  After every operation you perform on the object you should check its validity.
 *  If an error occured the object is invalidated and an error message is printed 
 *  to the Standard Output.
 *  
 *
 *		@author Jan Tusch
 *		@version 2.3pre3
 */
//@}



//@{

/// Handler class for XML based file IO.
class LisaXmlFile {

 public:		
		
  /// write an algorithm description file
  static void write_ExtAlg(const struct Lisa_ExtAlg&, const std::string& file);
		

  /// read an algorithm description file
  static bool read_ExtAlg(struct Lisa_ExtAlg&, const std::string& file);
		
  /** The structure of a document with one of the enumerated types is imposed by the DTD.
      See the DTD for more information.
  */
  /// Enumeration of valid document types.
  enum DOC_TYPE {
    /// LiSA configuration document.
    PREFERENCES = 0,
    /// Document contains a problem definition.
    PROBLEM,
    /// Document contains a problem instance, i.e. problem + values.
    INSTANCE,
    /// Document contains a solution for a problem, i.e. problem + values + schedule.
    SOLUTION,
    /// Use this type to determine the actual type at read -time.
    IMPLICIT //must be last one -> does not have a name !!
  };
  
  /** The coding used for the textual representation of the entries in the document.
      The string is used to identify the model to store the data.
  */
  /// type for a data model identifier
  typedef std::string WriteCoding;

 public:
		
  /** If you use docType = IMPLICIT the object will be usable for reading a document from a file and obtain the
      thereby given document type.
      Note that you can not change the document type after contruction.   
      @param docType the desired document type
      @see DOC_TYPE, getDocumentType()
  */
  /// Contructor for an empty document tree.
  explicit LisaXmlFile (DOC_TYPE docType = IMPLICIT);
		
  /// Destructor
  ~LisaXmlFile();
		
  /*! Errors occur in the following cases :
   *  - File handling failed (e.g. permission or other I/O problems)
   *  - An attempt to write an invalid tree
   *  - If validation is active and (guess what ...) the document ist invalid
   *  @param filename name of the output file
   *  @return true on success, false otherwise
  */
  /// Write the internal document tree to a file.
  bool write(std::string filename);

  /*! Errors occur in the following cases :
   *  - File handling failed (e.g. permission or other I/O problems)
   *  - The object's document type does not match the type of the document to be read (use IMPLICT, to avoid this).\n
   *    How could you know the type in advance? 
   *  - If validation is active and (guess what ...) the document ist invalid
   *  @param filename name of the input file
   *  @return true on success, false otherwise
  */
  /// Read a document from a file.
  bool read (std::string filename);
		
  /** @param Pref reference to a preferences object
  */
  /// Submit preferences object's data to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_Preferences& Pref);
		
  /** @param Pref reference to a preferences object
  */
  /// Retrieve preferences from the internal document tree.
  LisaXmlFile& operator>>(Lisa_Preferences& Pref);
		

  /** The default is LiSA's native coding.
      @param newCoding Alternative coding identifier
  */
  /// Set the coding for the document entries.
  LisaXmlFile& operator<<(const WriteCoding newCoding){
    coding = newCoding;
    return *this;
  }
		
  /** @param Problem reference to a problem object
  */
  /// Submit problem to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_ProblemType& Problem);
		
  /** @param Problem reference to a problem object
  */
  /// Retrieve problem from the internal document tree.
  LisaXmlFile& operator>>(Lisa_ProblemType& Problem);
		
  /** @param Values reference to a values object
  */
  /// Submit values to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_Values& Values);

  /** @param Values reference to a values object
  */
  /// Retrieve values from the internal document tree.
  LisaXmlFile& operator>>(Lisa_Values& Values);

  /** @param Controls reference to a controls object
  */
  /// Submit controls to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_ControlParameters& Controls);

  /** @param Controls reference to a controls object
  */
  /// Retrieve controls from the internal document tree.
  LisaXmlFile& operator>>(Lisa_ControlParameters& Controls);
		
  /** @param Schedule reference to a schedule object
  */
  /// Submit schedule to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_Schedule& Schedule);

  /** In case there are multiple schedules listed, the data of the first schedule found is returned. 
      @param Schedule reference to a schedule object
  */
  /// Retrieve schedule from the internal document tree.
  LisaXmlFile& operator>>(Lisa_Schedule& Schedule);

  /** @param ScheduleList reference to a list of schedules
  */
  /// Submit a list of schedules to the internal document tree.
  LisaXmlFile& operator<<(const Lisa_List<Lisa_ScheduleNode> & ScheduleList);

  /** @param ScheduleList reference to a list of schedule objects
  */
  /// Retrieve all schedules from the internal document tree.
  LisaXmlFile& operator>>(Lisa_List<Lisa_ScheduleNode>& ScheduleList);
		
  /** This might change after any operation and should be checked frequently.\n
      This is the operation state of the object and has nothing to to with validity in terms of the XML tree.\n
      Note: A once invalidated object cannot be used further and all subsequent 
      operations will fail.
  */
  /// Determine internal validity state.
  operator bool() {return valid;}
		
  /** This is either set in the contructor or (if used IMPLICIT) when reading from a file
      @return the type of the internal document tree
  */
  /// Determine document type.
  DOC_TYPE getDocumentType(){return type;}

  /** If you use validation it sets the path to find the internal DTD.
      @param DTD_PATH Path to LiSA's dtd
  */
  /// Class initializer.
  static void initialize(std::string DTD_PATH = "");
		
  /// Flag for validation of a document (both after reading, and before writing)
  static bool validate;

 private:
		
  /// Mappings from problem tupels to xml-tags
  static std::map< std::pair<int,int> , std::pair <std::string ,std::string> > WriteMap;
  /// Mappings from xml-tags to problem tupels
  static std::map< std::pair<std::string,std::string> , std::pair <int ,int> > ReadMap;
  /// xml-tags for the different document types
  static std::vector<std::string> DOC_TYPE_NAMES;
		
  /// internal error codes
  enum ERROR_MODE {
    UNKNOWN,
    LIB_XML,
    MISSING_ATTRIBUTE,
    BAD_ROOT,
    BAD_NODE,
    BAD_ATTRIBUTE,
    BAD_ATTR_VALUE,
    MISSING_SIZE,
    NO_DATA_TO_WRITE,
    NO_DATA_FOR_READ,
    WRITE_INVALID_DOCUMENT,
    READ_EMPTY_DOCUMENT,
    READ_DOC_OBJ_MISSMATCH,
    WRITE_DOC_OBJ_MISSMATCH,
    READ_BAD_DOC_TYPE,
    READ_ENTRY_FROM_INVALID,
    WRITE_ENTRY_TO_INVALID,
    BAD_MODEL
  };

	
  /// XML : DTD PUBLIC ID
  static const std::string DTD_EXTERN_PHRASE;
  /// XML : DTD SYSTEM ID
  static const std::string DTD_SYSTEM_PHRASE;
  /// XML: Namesace URL
  static const std::string NAMESPACE;
  /// XML : Namespace prefix
  static const std::string NAMESPACE_PREFIX;
		
  /// Encoding (using latin-1, i.e. western-europe character set)
  static const std::string ENCODING;
  static xmlCharEncodingHandlerPtr encoder;
		
  /// Path to locate the DTD
  static std::string DTD_PATH;

  /// Document type
  DOC_TYPE type;
		
  /// Checks validity of the internal document tree
  bool validateDocument();
		
  /// XML : Pointer to the document structure
  xmlDocPtr Doc;
  /// XML : Pointer to a xml-node structure for attatchment of further entries
  xmlNodePtr Hook;
  /// XML : Pointer to Namespace structure
  xmlNsPtr NsPtr;
  /// XML : Pointer to DTD structure
  static xmlDtdPtr DtdPtr;
		
  /// Internal validity flag
  bool valid;

  /// Coding identifier (posted to the IO-Factory)
  WriteCoding coding;
		
  /// Method to raise an error
  static void raiseError(ERROR_MODE = UNKNOWN,
			 std::string info1 = std::string(),
			 std::string info2 = std::string());

  /// Initialize the mappings between LiSA-objects and xml-tags
  static	void init_maps();
		
  /// Flag to make sure class is initialized
  static bool initialized;

  /// Filename of the DTD, i.e. the XML-SystemID
  static std::string dtd_file;

  /// add entry to the internal document tree
  void write(const Lisa_ProblemType&);
		
  /// find and obtain data from the internal document tree
  bool read(Lisa_ProblemType&);
		
  /// add entry to the internal document tree		
  void write(const Lisa_Values&);

  /// find and obtain data from the internal document tree
  bool read(Lisa_Values&);
		
  /// add entry to the internal document tree
  void write(const Lisa_Schedule&);

  /// find and obtain data from the internal document tree
  bool read(Lisa_Schedule&);
		
  /// add entry to the internal document tree
  void write(const Lisa_ScheduleNode&);

  /// find and obtain data from the internal document tree
  bool read(Lisa_ScheduleNode&);
		
  /// add entry to the internal document tree
  void write(const Lisa_Preferences&);

  /// find and obtain data from the internal document tree
  bool read(Lisa_Preferences&);
		
  /// add entry to the internal document tree
  void write(const Lisa_Graph&);
		
  /// find and obtain data from the internal document tree
  bool read(Lisa_Graph&);
		
  /// add entry to the internal document tree
  void write(const Lisa_ControlParameters&);

  /// find and obtain data from the internal document tree
  bool read(Lisa_ControlParameters&);
		
}; // end class


#endif
//@}
