/**
 * @author  Thomas Tautenhahn
 * @version 2.3pre3
 */

#include "ctrlpara.hpp"

using namespace std;

void Lisa_ControlParameters::add_key(const string name, long int value) 
 {
   if (IntKeyList.search_first(name)) 
     {
       IntList[IntKeyList.get_index()];
       IntKeyList.exclude();
       IntList.exclude();
     } 
   IntList.append(value);
   IntKeyList.append(name);
 }

void Lisa_ControlParameters::add_key(const string name, double value) 
 {
   if (DoubleKeyList.search_first(name)) 
     {
       DoubleList[DoubleKeyList.get_index()];
       DoubleKeyList.exclude();
       DoubleList.exclude();
     } 
   DoubleList.append(value);
   DoubleKeyList.append(name);
 }

void Lisa_ControlParameters::add_key(const string name, string value) 
 {
   if (StringKeyList.search_first(name)) 
     {
       StringList[StringKeyList.get_index()];
       StringKeyList.exclude();
       StringList.exclude();
     } 
   StringList.append(value);
   StringKeyList.append(name);
 }

long int  Lisa_ControlParameters::get_int(const string name)
 {
  if (!IntKeyList.search_first(name)) 
    {
      cerr << "ERROR: keyword int " << name << " not defined!\n";
      return 0;
    }
  return IntList[IntKeyList.get_index()]; 
 }

double Lisa_ControlParameters::get_double(const string name)
 {
  if (!DoubleKeyList.search_first(name)) 
    {
      cerr << "ERROR: keyword double " << name << " not defined!\n";
      return 0.0;
    }
  return DoubleList[DoubleKeyList.get_index()]; 
 }

string Lisa_ControlParameters::get_string(const string name)
 {
   if (!StringKeyList.search_first(name)) 
     {
       cerr << "ERROR: keyword string " << name << " not defined!\n";
       return "ERROR";
     }
  return StringList[StringKeyList.get_index()]; 
 }

int Lisa_ControlParameters::defined(const string name)
 {
   if (IntKeyList.search_first(name)) return INT;
   if (DoubleKeyList.search_first(name)) return DOUBLE;
   if (StringKeyList.search_first(name)) return STRING;
   return UNDEFINED;
 }

void Lisa_ControlParameters::write(ostream & strm) const
 {
   //copy all contents: this is totally stupid, but reading a list would 
   //     change its internal bookmark and we are in a const method here
   Lisa_List<string> kl, sl;
   Lisa_List<double> dl;
   Lisa_List<long> il;

   strm << "<CONTROLPARAMETERS>\n";
   kl=IntKeyList;
   if (!kl.empty())
     {  
       il=IntList; kl.reset(); il.reset();
       do strm << "long " << kl.get() << " " << il.get() << "\n";
       while (kl.next(), il.next());
     }
   kl=DoubleKeyList;
   if (!kl.empty())
     {  
       dl=DoubleList; kl.reset(); dl.reset();
       do strm<< "double " << kl.get() << " " << dl.get()<< "\n";
       while (kl.next(), dl.next());
     }
   kl=StringKeyList;
   if (!kl.empty())
     {  
       sl=StringList; kl.reset(); sl.reset();
       do 
	 strm << "string " << kl.get() << " " << sl.get() << "\n";
       while (kl.next(), sl.next());
     }
   strm << "</CONTROLPARAMETERS>\n";
 }

void Lisa_ControlParameters::read(istream& strm) 
 {
   string S;
   long longEntry;
   double doubleEntry;
   string stringEntry;
 
   // find beginning of <CONTROLPARAMETERS> entry:
   do {
        S=""; strm >>S;
        //cout << "[" << S << "]" << endl;
        if (S=="") 
          {
            cerr << "ERROR: no <CONTROLPARAMETERS> found in input file.\n";
            return;
	  } 
      }  
   while (S!="<CONTROLPARAMETERS>");
   
   // read entry (3 words) stop if first word is  </CONTROLPARAMETERS>:
   while(1)
    { 
      S=""; strm >> S;
      if (S=="") 
          {
            cerr << "ERROR: unexpected end of file while reading <CONTROLPARAMETERS>.\n";
            return;
	  } 
      else 
      if (S=="</CONTROLPARAMETERS>") return;
      else
      if (S=="long")
        { 
	  S="";strm >> S;
	  longEntry=0; strm >> longEntry;
	  IntList.append(longEntry);
          IntKeyList.append(S);
	}
      else
      if (S=="double")
	{ 
	  S=""; strm >> S;
	  doubleEntry=0.0; strm >> doubleEntry;
	  DoubleList.append(doubleEntry);
          DoubleKeyList.append(S);
	}
      else
      if (S=="string")
	{ 
	  S="";strm >> S;
	  stringEntry=""; strm >> stringEntry;
	  StringList.append(stringEntry);
          StringKeyList.append(S);
        }
      else 
        { 
          cerr << "ERROR: Unknown type identifier "<< S << 
                    " while reading <CONTROLPARAMETERS>.\n";
          return;
        }
    }
 }








