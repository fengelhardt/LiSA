#ifndef LISA_DATA_HANDLER_HPP
#define LISA_DATA_HANDLER_HPP

#include <string>
#include <ostream>
#include <istream>

class Lisa_Schedule;
class Lisa_Values;
class Lisa_Graph;


class Lisa_DataHandler {
		
protected:

		virtual bool write_to_stream(const Lisa_Schedule&, std::string tag, std::ostream &) = 0;
		virtual bool write_to_stream(const Lisa_Values&,   std::string tag, std::ostream &) = 0;
		virtual bool write_to_stream(const Lisa_Graph&,    std::string tag, std::ostream &) = 0;
		
		virtual bool read_from_stream(Lisa_Schedule&,  std::string tag, std::istream &) = 0;
		virtual bool read_from_stream(Lisa_Values&,    std::string tag, std::istream &) = 0;
		virtual bool read_from_stream(Lisa_Graph&,     std::string tag, std::istream &) = 0;
		
public:
		
		static std::string getName() {return "";}
		
		bool write(const Lisa_Schedule& S, std::string tag, std::ostream &out){
				return write_to_stream(S,tag,out);
		}
		bool write(const Lisa_Values& V,   std::string tag, std::ostream &out){
				return write_to_stream(V,tag,out);
		}
		bool write(const Lisa_Graph& G,    std::string tag, std::ostream &out){
				return write_to_stream(G,tag,out);
		}
		
		bool read(Lisa_Schedule& S,  std::string tag, std::istream &in){
				return read_from_stream(S,tag,in);
		}
		bool read(Lisa_Values& V,    std::string tag, std::istream &in){
								return read_from_stream(V,tag,in);
		}
		bool read(Lisa_Graph& G,     std::string tag, std::istream &in){
				return read_from_stream(G,tag,in);
		}

};

#endif
