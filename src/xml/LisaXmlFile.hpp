/** @name LisaXmlFile 
				This section contains LisaXmlFile class. 
				It is responsible for all LiSA's file IO.
*/
//@{
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
class ScheduleNode;
class Lisa_ControlParameters;
class Lisa_Preferences;
class Lisa_Graph;

#ifndef LISA_XML_IMPL

struct xmlDoc;
struct xmlNode;
struct xmlNs;
struct xmlDtd;

typedef xmlDoc*  xmlDocPtr ;
typedef xmlNode* xmlNodePtr ;
typedef xmlNs* xmlNsPtr ;
typedef xmlDtd* xmlDtdPtr ;
typedef  unsigned char xmlChar;

#endif


/** LisaXmlFile class.  
				This class represents an XML document that can be written to	or read from a file. 
				It provides functionality for submission and retrieval of LiSA objects.
				It also can validate documents against LiSA's DTD.
    @author Jan Tusch
    @version 2.3pre3
*/

class LisaXmlFile {

public:		
		
		/// write an algorithm description file
		static void write_ExtAlg(const struct Lisa_ExtAlg&, const std::string& file);
		

		/// read an algorithm description file
		static bool read_ExtAlg(struct Lisa_ExtAlg&, const std::string& file);
		
		//@{
		/** Enumeration of valid document types.
						See the DTD for more information.
			*/
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
		//@}

		/// The coding used for the textual representation of entries in the document
		typedef std::string WriteCoding;

public:
		
		/** Contructor for an empty document
						@param docType the desired document type
						@see DOC_TYPE
		*/
		explicit LisaXmlFile (DOC_TYPE docType = IMPLICIT);
		
		/// Destructor
		~LisaXmlFile();
		
		/** Write the internal document tree to a file.
						@param filename name of the output file
						@return true on success, false otherwise
			*/
		bool write(std::string filename);

		/** Read a document from a file
						@param filename name of the input file
						@return true on success, false otherwise
			*/
		bool read (std::string filename);
		
		/** Submit preferences to the internal document tree
						@param Pref reference to a preferences object
		*/
		LisaXmlFile& operator<<(const Lisa_Preferences& Pref);
		
		/** Retrieve preferences from the internal document tree
						@param Pref reference to a preferences object
		*/
		LisaXmlFile& operator>>(Lisa_Preferences& Pref);
		

		/** Set the coding for the document entries
						The default is LiSA's native coding.
						@param newCoding Alternative coding identifier
			*/
		LisaXmlFile& operator<<(const WriteCoding newCoding){
				coding = newCoding;
				return *this;
		}
		
		/** Submit problem to the internal document tree
						@param Problem reference to a problem object
			*/
		LisaXmlFile& operator<<(const Lisa_ProblemType& Problem);
		
		/** Retrieve problem from the internal document tree
						@param Problem reference to a problem object
		*/
		LisaXmlFile& operator>>(Lisa_ProblemType& Problem);
		
		/** Submit values to the internal document tree
						@param Values reference to a values object
			*/
		LisaXmlFile& operator<<(const Lisa_Values& Values);

		/** Retrieve values from the internal document tree
						@param Values reference to a values object
		*/
		LisaXmlFile& operator>>(Lisa_Values& Values);

		/** Submit controls to the internal document tree
						@param Controls reference to a controls object
		*/
		LisaXmlFile& operator<<(const Lisa_ControlParameters& Controls);

		/** Retrieve controls from the internal document tree
						@param Controls reference to a controls object
		*/
		LisaXmlFile& operator>>(Lisa_ControlParameters& Controls);
		
		/** Submit schedule to the internal document tree
						@param Schedule reference to a schedule object
			*/
		LisaXmlFile& operator<<(const Lisa_Schedule& Schedule);

		/** Retrieve a (the firts) schedule from the internal document tree
						@param Schedule reference to a schedule object
		*/
		LisaXmlFile& operator>>(Lisa_Schedule& Schedule);

		/** Submit a list of schedules to the internal document tree
						@param ScheduleList reference to a list of schedules
		*/
		LisaXmlFile& operator<<(const Lisa_List<ScheduleNode> & ScheduleList);

		/** Retrieve all schedules from the internal document tree
						@param ScheduleList reference to a list of schedules object
		*/
		LisaXmlFile& operator>>(Lisa_List<ScheduleNode>& ScheduleList);
		
		/** Determine internal state.
						This might change after any operation and should be checked frequently.
						Note: A once invalidated object cannot be used further and all subsequent 
						operations will fail.
		*/
		operator bool() {return valid;}
		
		/** Determine document type.
						This is either set in the contructor or (if used IMPLICIT) when reading from a file
						@return the type of the internal document tree
						@see DOC_TYPE
		*/
		DOC_TYPE getDocumentType(){return type;}

		/** Class initializer.
						If you use validation it sets the path to find the DTD
						@param TOP_PROGRAM_PATH LiSA's root directory
		*/
		static void initialize(std::string TOP_PROGRAM_PATH = "");
		
		/// Flag for validation of a document (both after reading, and before writing)
		static bool validate;

protected:

		
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

	
protected:
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
		
		/// LiSA's root directory (to locate the DTD)
		static std::string TOP_PROGRAM_PATH;

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
		
private:
		
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
		void write(const ScheduleNode&);

		/// find and obtain data from the internal document tree
		bool read(ScheduleNode&);
		
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
