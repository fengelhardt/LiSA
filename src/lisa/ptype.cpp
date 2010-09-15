/**
 * @author  Per Willenius
 * @version 3.0
 */

#include <stdio.h>

#include "../main/global.hpp"
#include "../misc/except.hpp"

#include "ptype.hpp"

using namespace std;

//**************************************************************************

Lisa_ProblemType::Lisa_ProblemType() 
{
  reset();  
} 

//**************************************************************************

Lisa_ProblemType::Lisa_ProblemType(const Lisa_ProblemType& other){
  for(int i=0;i<TUPEL_INDEX;i++) tupel[i] = other.tupel[i];
  vld = other.vld;
  m_no = other.m_no;
  n_no = other.n_no;
}

//**************************************************************************

void Lisa_ProblemType::reset()
{
  for(int i=0; i<TUPEL_INDEX; i++) tupel[i]=EMPTY;
  vld=false;
  m_no = 0;
  n_no = 0;
}

//**************************************************************************

int Lisa_ProblemType::set_property(const int prop, const int value)
{
  int rv=0;
  // what we are here to do:
  if ((prop>=TUPEL_INDEX)||(value>=NUMBER[prop])) 
    {
      G_ExceptionList.lthrow("cannot set this property in problem type",
                             Lisa_ExceptionList::INCONSISTENT_INPUT);
      return rv;
    }
  tupel[prop]=value;
  
  // now check for consistency and set the valid flag:
  switch (prop)
    {
    case M_NUMBER:
      if (tupel[M_ENV]!=0) {
	switch (tupel[M_ENV]) {
	case ONE:
	  if (tupel[M_NUMBER]!=M_VAL) rv++;
	  tupel[M_NUMBER]=M_VAL; m_no=1;
	  if (tupel[M_MPM]) { tupel[M_MPM]=false; rv++; }
	  if (tupel[M_MPT]) { tupel[M_MPT]=false; rv++; }
	case P:
	case Q:
	case R:
	  break;
	case O:
	case J:
	case F:
	case X:
	case G:
	  if (m_no<=1) { tupel[M_NUMBER]=EMPTY;  m_no=2; }
	  break;
	}
      }
    break;
    case M_ENV:
      switch (value)
	{
	case ONE:
	  if (tupel[M_NUMBER]!=M_VAL) rv++;
	  tupel[M_NUMBER]=M_VAL; m_no=1;
	  if (tupel[M_MPM]) { tupel[M_MPM]=false; rv++; }
	  if (tupel[M_MPT]) { tupel[M_MPT]=false; rv++; }
	case P:
	case Q:
	case R:
	  break;
	case O:
	case J:
	case F:
	case X:
	case G:
	  if (m_no<=1) { tupel[M_NUMBER]=EMPTY;  m_no=2; }
	  break;
	}
      break;
    case M_MPT:
    case M_MPM:
      if (tupel[M_ENV]==ONE) { tupel[M_ENV]=P; rv++; }  
    case PIJ:
        tupel[PIJ]=value;
      break;
    case BATCH:
      if ((value==EMPTY)&& tupel[BOUNDED_BATCH])
	{ tupel[BOUNDED_BATCH]=false; rv++; }
      break;
    case BOUNDED_BATCH:
      if ((value==true) && (tupel[BATCH]==EMPTY))
	{ tupel[BATCH]=S_BATCH; rv++; }
      break;
    }
  
  // special correction of obsolete problem types:
  if (prop==M_ENV)
    switch (value)
      {
      case MPT: tupel[M_ENV]=P; tupel[M_MPT]=true; break;
      case OMPT: tupel[M_ENV]=O; tupel[M_MPT]=true; break;
      case FMPT: tupel[M_ENV]=F; tupel[M_MPT]=true; break;
      case JMPT: tupel[M_ENV]=J; tupel[M_MPT]=true; break;
      case XMPT: tupel[M_ENV]=X; tupel[M_MPT]=true; break;
      case GMPT: tupel[M_ENV]=G; tupel[M_MPT]=true; break;
      case OMPM: tupel[M_ENV]=O; tupel[M_MPM]=true; break;
      case FMPM: tupel[M_ENV]=F; tupel[M_MPM]=true; break;
      case JMPM: tupel[M_ENV]=J; tupel[M_MPM]=true; break;
      case XMPM: tupel[M_ENV]=X; tupel[M_MPM]=true; break;
      case GMPM: tupel[M_ENV]=G; tupel[M_MPM]=true; break;
      case PMPM: tupel[M_ENV]=P; tupel[M_MPM]=true; break;
      case QMPM: tupel[M_ENV]=Q; tupel[M_MPM]=true; break;
      }
  
  // set the valid flag:
  vld=((tupel[M_ENV]!=EMPTY)&&(tupel[OBJECTIVE]!=EMPTY));
  //cout << "set( " << prop << "," << value << ") called with " 
  //     << tupel[M_ENV] <<  tupel[M_MPT] << tupel[M_MPM] << endl; 
  return rv;
}

//**************************************************************************

int Lisa_ProblemType::get_property_old(const int prop) const
{
  if ((prop!=M_ENV) || ((!tupel[M_MPT]) && (!tupel[M_MPM])))
    return tupel[prop];
  if (tupel[M_MPT] && (!tupel[M_MPM]))
    switch (tupel[M_ENV])
    {
    case P: return MPT;
    case O: return OMPT;
    case F: return FMPT;
    case J: return JMPT;
    case X: return XMPT;
    case G: return GMPT;
      default: G_ExceptionList.lthrow("no old expression for this problem");
    }
  if (tupel[M_MPM] && (!tupel[M_MPT]))
    switch (tupel[M_ENV])
    {
    case O: return OMPM;
    case F: return FMPM;
    case J: return JMPM;
    case X: return XMPM;
    case G: return GMPM;
    case P: return PMPM;
    case Q: return QMPM;
      default: G_ExceptionList.lthrow("no old expression for this problem");
    }
  G_ExceptionList.lthrow("problem too complex for Brucker's classification");
  return false;
}

//**************************************************************************

string Lisa_ProblemType::output_alpha() const
{
  string str="";
  str+=name(M_ENV,tupel[M_ENV]);
  if (tupel[M_MPT]) str+="MPT";
  if (tupel[M_MPM]) str+="MPM";
  if (tupel[M_ENV]!=ONE)
    str+=name(M_NUMBER,tupel[M_NUMBER]);
  return str;
}

//**************************************************************************

string Lisa_ProblemType::output_beta() const 
{
  string str="";
  int number=0,i=0;
  for (i=NUMBER_ALPHA; i<NUMBER_ALPHA+NUMBER_BETA; i++) 
    {
      if(tupel[i]!=EMPTY) 
	{ 
	  if(number) str+="; "; 
	  number++;
	  str+=name(i,tupel[i]);
	}
    }
  return str;
}

//**************************************************************************

string Lisa_ProblemType::output_gamma() const 
{
  string str="";
  str+=name(OBJECTIVE,tupel[OBJECTIVE]);
  return str;
}

//**************************************************************************

string Lisa_ProblemType::output_problem() const {
  string str;
  if (output_alpha()=="" || output_gamma()=="") return("no valid Problem");
  str=output_alpha();
  str+=" | ";
  str+=output_beta();  
  str+=" | ";
  str+=output_gamma();
  return(str);
}

//**************************************************************************

int Lisa_ProblemType::setalpha(string al){
  
  string str,m_number;
  int i, letters_done;
  // cout << "analyzing " << al << endl;
  // erase old entries
  
  tupel[M_ENV]=EMPTY;
  tupel[M_NUMBER]=M_ARB;
  tupel[M_MPT]=false;
  tupel[M_MPM]=false;

  // find the machine environment
  letters_done=0;
  for(i=ONE; i<=R; i++) 
    if (al.substr(0,1)==name(M_ENV,i))
      tupel[M_ENV]=i;
  
  // could be normal
  letters_done=1;
  
  // or non-conform problem type
  if (al.substr(0,5)==name(M_ENV,F_SR)){
      tupel[M_ENV]=F_SR;
      letters_done=5; 
  }
  
  if (al.substr(0,5)==name(M_ENV,P_CS)){
      tupel[M_ENV]=P_CS;
      letters_done=5;
  } 
  
  // now look for MPT and MPM
  if (al.substr(letters_done,3)=="MPT"){
      tupel[M_MPT]=true;
      letters_done+=3;
  }
  
  if (al.substr(letters_done,3)=="MPM"){
      tupel[M_MPM]=true;
      letters_done+=3;
  }

  // finally, there could be a machine number left
  m_number=al.substr(letters_done, al.length() - letters_done);
  if(m_number==""){
    tupel[M_NUMBER]=M_ARB;
  }else{  
    if (m_number=="m"){
      tupel[M_NUMBER]=M_FIX;
    }else{
      tupel[M_NUMBER]=M_VAL;
      sscanf((char*) m_number.c_str(),"%u",&m_no);
    }
  }
  
  //  cout << "got M_ENV=" << name(M_ENV, tupel[M_ENV])
  //     << " MPT=" << tupel[M_MPT] << " MPM=" << tupel[M_MPM] 
  //     << " m_type=" << tupel[M_NUMBER] << " m=" << m_no << endl;
  
  return (tupel[M_ENV]==EMPTY);
}

//**************************************************************************

int Lisa_ProblemType::setbeta(string be) 
{
  int tuplepos, meaning=0, i,j;
  
  // if there is a , or ; on the end of the string, then remove it
  if (be[be.length()-1]==','){ 
      be.erase(be.length()-1,1);
  }else if (be[be.length()-1]==';'){ 
      be.erase(be.length()-1,1);
  }
  
  // find out where the string fits 
  tuplepos=0;
  for (i=NUMBER_ALPHA; i<NUMBER_ALPHA+NUMBER_BETA; i++){
    for (j=1; j<=NUMBER[i]; j++){
      if (be==name(i,j)){
          tuplepos=i; 
          meaning=j;
        }
    }
  }
        
  // special procedure for n=[number]:
  if( (!tuplepos) && (be.substr(0,2)=="n=")){       
    if (sscanf((char*) (be.substr(2,be.length()-2)).c_str(),"%u",&n_no)){
      tuplepos=JOB_NR; meaning=VALUE;
    } 
  }
  
  // if no tuplepos was found this means the given beta parameter is unknown
  if (!tuplepos) return true;
  
  // replace tuple entry:
  tupel[tuplepos]=meaning;

  return false;

}

//**************************************************************************

void Lisa_ProblemType::delbeta() {
  int i;
  for (i=NUMBER_ALPHA; i<NUMBER_ALPHA+NUMBER_BETA; i++)
    tupel[i]=0;
}

//**************************************************************************

int Lisa_ProblemType::delbeta(int nr) {
  tupel[nr]=0;
  return(0);
}

//**************************************************************************

int Lisa_ProblemType::setgamma(string ga) {
  string str;
  int i;
  
  tupel[OBJECTIVE]=0;
  for(i=1;i<=NUMBER[OBJECTIVE];i++) {
    str=name(OBJECTIVE,i);
    if(ga==str) {tupel[OBJECTIVE]=i;return 0;} 
  }
  if (tupel[OBJECTIVE]==0) cerr << "'" << ga << "' as objective function unknown\n";
  return(1);
}

//**************************************************************************

string Lisa_ProblemType::name(int tupelEntry, int identifier)const{
  char s[50];
  
  switch(tupelEntry){
    
    case M_ENV:
    switch(identifier){
      case EMPTY:return("");
      case ONE:  return("1");
      case O:    return("O");
      case F:    return("F");
      case J:    return("J");
      case X:    return("X");
      case G:    return("G");
      case P:    return("P");
      case Q:    return("Q");
      case R:    return("R");
      case MPT:  return("MPT");
      case OMPT: return("OMPT");
      case FMPT: return("FMPT");
      case JMPT: return("JMPT");
      case XMPT: return("XMPT");
      case GMPT: return("GMPT");
      case OMPM: return("OMPM");
      case FMPM: return("FMPM");
      case JMPM: return("JMPM");
      case XMPM: return("XMPM");
      case GMPM: return("GMPM");
      case PMPM: return("PMPM");
      case QMPM: return("QMPM");
      case F_SR: return("F;R1:");
      case P_CS: return("P;S1:");
    }    
    return("error in M_ENV");
    
    case M_NUMBER:
    switch(identifier){
      case M_ARB: return("");
      case M_VAL: {
        sprintf(s,"%d",m_no);
        return( (string) s);
      }
      case M_FIX: return("m");
    }
    cerr << "Identifier:" << identifier << "not found\n";
    return("error in M_NUMBER");
    
    case PMTN:
    switch(identifier){
      case EMPTY: return "";
      case SET: return "pmtn";
    }
    return("error in PMTN");
    
    case PRECEDENCE:
    switch(identifier){
      case EMPTY: return "";
      case INTREE: return "intree";
      case OUTTREE: return "outtree";
      case TREE: return "tree";
      case SP_GRAPH: return "sp_graph";
      case CHAINS: return "chains";
      case PREC: return "prec";
    }
    return("error in PRECEDENCE");
    
    case RI:
    switch(identifier){
      case EMPTY: return "";
      case SET: return "r_i";
    }
    return("error in RI");
    
    case DI:
    switch(identifier) {
      case EMPTY: return "";
      case SET: return "d_i";
    }
    return("error in DI");
    
    case PIJ:
    switch(identifier){
      case EMPTY: return "";
      case PIJ_1: return "p_ij=1";
      case PIJ_P: return "p_ij=p";
    }
    return("error");
    
    case BATCH:
    switch(identifier){
      case EMPTY: return "";
      case S_BATCH: return "s-batch";
      case P_BATCH: return "p-batch";
    }
    return("error in BATCH");
    
    case BOUNDED_BATCH:
    switch(identifier){
      case EMPTY: return "";
      case SET: return "b<n";
    }
    return("error in BOUNDED_BATCH");
    
    case JOB_NR:
    switch(identifier){
      case J_ARB: return "";
      case J_VAL: {
        sprintf(s,"n=%d",n_no);
        return( (string) s);
      }
      case J_FIX: return "n=k";
    }
    return("error in JOB_NR");
    
    case NO_WAIT:
    switch(identifier){
      case EMPTY: return "";
      case SET:  return "no-wait";
    }
    return("FEHLER in NO_WAIT");
    
    case SIZE:
    switch(identifier){
      case EMPTY: return "";
      case SET:  return "size_i";
    }
    return("FEHLER in SIZE");
    
    case TIME_LAGS:
    switch(identifier){
      case EMPTY: return "";
      case UNIT_TL:  return "prec(1)";
      case CONST_TL:  return "prec(l)";
      case GENERAL_TL:  return "prec(l_ij)";
    }
    return("FEHLER in TIME_LAGS");
    
    case TRANSPORTATION_DELAYS:
    switch(identifier){
      case EMPTY: return "";
      case TIK_T:  return "t_ik=T";
      case TIKL_T:  return "t_ikl=T";
      case TI_IN:  return "t_i in {T_1,T_2}";
      case TKL_TLK:  return "t_kl=t_lk";
      case TIKL_TILK:  return "t_ikl=t_ilk";
      case TI:  return "t_i";
      case TK:  return "t_k";
      case TKL:  return "t_kl";
      case TIK:  return "t_ik";
      case TIKL:  return "t_ikl";
    }
    return("FEHLER in TRANSPORTATION_DELAYS");
    
    case SERVER_FLAGS:
    switch(identifier){
      case EMPTY: return "";
      case SI:  return "s_i";
      case SI_1:  return "s_i=1";
      case SI_S:  return "s_i=s";
    }
    return("FEHLER in SERVER_FLAGS");
    
  case OBJECTIVE:
    switch(identifier)      {
      case EMPTY: return "";
      case CMAX: return "Cmax";
      case LMAX: return "Lmax";
      case SUM_CI: return "SumCi";
      case SUM_WICI: return "SumWiCi";
      case SUM_UI: return "SumUi";
      case SUM_WIUI: return "SumWiUi";
      case SUM_TI: return "SumTi";
      case SUM_WITI: return "SumWiTi";
      case IRREG1: return "Irreg1";
      case IRREG2: return "Irreg2";
    }
    return("error in OBJECTIVE");
  }
  return "TupelEntry unknown";
}

//**************************************************************************

int Lisa_ProblemType::name(int tupelEntry, string identifier)const{
  
  for(int i=0;i<NUMBER[tupelEntry];i++)
    if(identifier==name(tupelEntry,i)) return i;
    
  cerr << "'" << identifier << "' not found." << endl;
  return -1;
}

//**************************************************************************
  
void Lisa_ProblemType::write(ostream& strm) const{
  if (vld){
    strm << endl << "<PROBLEMTYPE>" << endl;
    strm << "Lisa_ProblemType= { " << output_alpha() <<" / " << output_beta() << " / " << output_gamma() << " }" << endl;
    strm << "</PROBLEMTYPE>" << endl;
  } else cerr << "Lisa_ProblemType not valid." << endl;
}

//**************************************************************************

void Lisa_ProblemType::read(istream& strm){
  
  // assure existence of input stream:
  if(strm==NULL){
    G_ExceptionList.lthrow("No valid stream in Lisa_ProblemType::read().");
    return;
  }
  
  // locate begin of problem type entry:
  string S="";
  strm >> S;
  while(S != "<PROBLEMTYPE>"){
    if(S == ""){
      G_ExceptionList.lthrow("No '<PROBLEMTYPE>' entry found in input file.",
                             Lisa_ExceptionList::END_OF_FILE);
      return;
    }
    S = "";
    strm >> S;
  }
    
  //check for 'Lisa_ProblemType='
  S="";
  strm >> S;
  if(S!="Lisa_ProblemType="){
    G_ExceptionList.lthrow("'Lisa_ProblemType=' expected in <PROBLEMTYPE>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  // check delimiter
  S = "";
  strm >> S;
  if(S!="{"){
    G_ExceptionList.lthrow("'{' expected in <PROBLEMTYPE>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  // read alpha
  S="";
  strm >> S; 
  if(setalpha(S)){
    G_ExceptionList.lthrow("Incorrect alpha entry '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  // check delimiter
  S="";
  strm >> S; 
  if (S!="/"){
    G_ExceptionList.lthrow("'/' expected in <PROBLEMTYPE>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  // read beta
  delbeta();
  S="";
  strm >> S;
  while (S!="/" && S!=""){
    if(setbeta(S))  G_ExceptionList.lthrow("Unknown beta restriction '"+S+"'. Ignoring it.",
                                           Lisa_ExceptionList::ANY_ERROR);
    S="";
    strm >> S;
  }

  // check delimiter
  if (S!="/"){
    G_ExceptionList.lthrow("'/' expected in <PROBLEMTYPE>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  // read gamma
  S="";
  strm >> S;
  if(setgamma(S)){
    G_ExceptionList.lthrow("Incorrect gamma entry '"+S+"'.", 
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
    return;
  }
  
  S="";
  strm >> S;
  if(S!="}"){
    G_ExceptionList.lthrow("'}' expected in <PROBLEMTYPE>, found '"+S+"'.",
                           Lisa_ExceptionList::INCONSISTENT_INPUT);
  }
  
  S="";
  strm >> S;
  if(S != "</PROBLEMTYPE>"){
    G_ExceptionList.lthrow("'</PROBLEMTYPE>' expected, found '"+S+"'.",
                           Lisa_ExceptionList::END_OF_FILE);
  }
  
  vld=true;
}

//**************************************************************************

