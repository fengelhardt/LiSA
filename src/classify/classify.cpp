/**
 * @author  Martin Harborth
 * @version 2.3final
 */
 

#include <cstring>

#include <string>
#include <fstream>
#include <locale>

#include "../misc/int2str.hpp"
#include "../desktop/c_tcl.hpp"

#include "classify.hpp"

//**************************************************************************

// flag: minmal NP-hard or maximal polynomial problem
const int MINMAX=true;
// flag: opposite of MINMAX
const int NORMAL=false;
// maximal number of characters of data base path name
//const int PATHMAX=500;

//**************************************************************************

Lisa_Classify* Lisa_Classify::make_instance(const std::string& file){
  while(!G_ExceptionList.empty()) G_ExceptionList.lcatch();
  Lisa_Classify *classify = new Lisa_Classify(file);
  
  if(!G_ExceptionList.empty()){
    while(!G_ExceptionList.empty()) G_ExceptionList.lcatch();
    delete classify;
    return 0;
  }
  
  return classify;
}

//**************************************************************************
  
Lisa_Classify::Lisa_Classify(const std::string& file){
  
  entries = 0; 
  
  std::ifstream i_strm(file.c_str());
  if(!i_strm){
    G_ExceptionList.lthrow("Could not open '"+file+"' for reading.");
    return;
  }
  
  // find records,we assume each record starts with an '@' and ends with an '}'
  // and nesting with '{' and '}' brackets is correct
  std::string entry;
  char c;
  while(! i_strm.eof() ){
    
    i_strm >> c;
    if( c != '@' && !i_strm.eof()){
      G_ExceptionList.lthrow("@ expected after "+entry,
                             Lisa_ExceptionList::WARNING); 
      continue;
    }
    
    entry = "@";
    unsigned int level=0;
    while(!i_strm.eof()){
      i_strm.get(c);
      
      if(c=='{') level++;
      else if(c=='}') level--;
      
      entry.append(1,c);
 
      if(level == 0 && c == '}'){
        parse_bibentry(entry);
        break;
      }
    };
    
  }
  
  i_strm.close();
}

//**************************************************************************

void Lisa_Classify::parse_bibentry(const std::string& entry){
  Lisa_ClassifyRecord record;
  record.mmap.add_key(std::string("BIBENTRY"),entry);
  
  //clean up entry string some
  // remove control characters and any whitespace outside of value part
  std::string copy = "";
  unsigned int level = 0;
  for(unsigned int i=0;i<entry.size();i++){
    const char c = entry[i];
    
    if(c == '{') level++;
    else if(c == '}') level--;
    
    if(!(iscntrl(c)) && !(isspace(c) && level<=1)) copy.append(1,c);
  }
  
  // remove line break - "not a line break" markers
  unsigned int pos = copy.find(std::string("\\\\"),0);
  while(pos != std::string::npos){
    copy.erase(pos,2);
    pos = copy.find(std::string("\\\\"),pos);
  }
  
  // remove multiple whitespaces
  pos = copy.find(std::string("  "),0);
  while(pos != std::string::npos){
    copy.erase(pos,1);
    pos = copy.find(std::string("  "),0);
  }
  
  //parse key value pairs ... We assume that they are of the form key={value}
  //seperated by a ','. We only check for ',' at a nesting level of 1 
  //concerning '{' and '}' brackets. We further assume that there is a first
  //token that is not a key value pair which we ignore anyway. Such we can start
  //at the first ','. Additionally we already have removed any whitespace.
  level = 0;
  bool start = 1;
  std::string keyvalue;
  for(unsigned int i=0;i<copy.size();i++){
    const char c = copy[i];
    
    if(c == '{') level++;
    else if(c == '}') level--;
    
    if(c == ',' && start && level == 1){
      start = 0;
      keyvalue = "";
    }else if(c == ',' && !start && level == 1 || c == '}' && !start && level == 0){
      //insert key-value
      pos = keyvalue.find('=',0);
      if(pos == std::string::npos){
        G_ExceptionList.lthrow("= expected in"+keyvalue+" in "
                                +entry,Lisa_ExceptionList::WARNING);
      }else if(pos == 0){
        G_ExceptionList.lthrow("Key expected in"+keyvalue+" in "
                                +entry,Lisa_ExceptionList::WARNING);
      }else if(pos == keyvalue.size()-1){
        G_ExceptionList.lthrow("Value expected in"+keyvalue+" in "
                                +entry,Lisa_ExceptionList::WARNING);
      }else{
        
        std::string key = keyvalue.substr(0,pos);
        for(unsigned j=0;j<key.size();j++) key[j] = toupper(key[j]);
        
        std::string value = keyvalue.substr(pos+1,std::string::npos);
        if(value[0] == '{') value.erase(0,1);
        if(value[value.size()-1] == '}') value.erase(value.size()-1,1);
        
        record.mmap.add_key(key,value);
      }
      start = 1;
      i--;
    }else if(!start){
      keyvalue.append(1,c);
    }
    
  }
    
  // check for author and title entry, if author not given use title
  if(record.mmap.defined(std::string("AUTHOR")) != Lisa_ControlParameters::STRING &&
     record.mmap.defined(std::string("TITLE")) != Lisa_ControlParameters::STRING){
    G_ExceptionList.lthrow("Neither AUTHOR not TITLE given in BibTex record: \n"+entry,
                           Lisa_ExceptionList::WARNING);
    record.mmap.add_key(std::string("AUTHOR"),std::string("author unknown"));
  }else if(record.mmap.defined(std::string("AUTHOR")) != Lisa_ControlParameters::STRING){
    record.mmap.add_key(std::string("AUTHOR"),record.mmap.get_string(std::string("TITLE")));
  }
  
  // check for year entry
  if(record.mmap.defined(std::string("YEAR")) != Lisa_ControlParameters::STRING){
    record.mmap.add_key(std::string("YEAR"),std::string("no year available"));
  }
  
  record.number_of_problems = 0;
  if(record.mmap.defined(std::string("ANNOTE")) != Lisa_ControlParameters::STRING){
    G_ExceptionList.lthrow("No ANNOTATE given in BibTex record "+entry,
                           Lisa_ExceptionList::WARNING);
     
  }else{
   parse_annote(&record); 
  }
  
  records.push_back(record);
  entries ++;
}

//**************************************************************************

void Lisa_Classify::parse_annote(Lisa_ClassifyRecord *const record){
  std::string annote = record->mmap.get_string("ANNOTE");
  
  unsigned int level = 0;
  std::string problem;
  for(unsigned int i=0;i<annote.size();i++){
    const char c = annote[i];
    
    if(c == '$') level = (level+1) % 4;
    
    if(level == 0 && c == ','){

      const unsigned int pos1 = problem.find('$',0);
      const unsigned int pos2 = problem.find('$',pos1+1);
      
      if(pos2 == std::string::npos){
        G_ExceptionList.lthrow("Parse erros in entry "
                               +record->mmap.get_string(std::string("BIBENTRY"))+" at '"
                               +problem+"'",Lisa_ExceptionList::WARNING);
      }else{

        COMPLEXITY cpl = OPEN;
        if(problem.find("$*\\NP$-hard") != std::string::npos) cpl = STR_NP_HARD;
        else if(problem.find("$\\NP$-hard") != std::string::npos) cpl = NP_HARD;
        else if(problem.find("$P_{pseudo}$") != std::string::npos) cpl = PS_POLYN;
        else if(problem.find("$P$") != std::string::npos) cpl = POLYN;
        
        if(cpl == OPEN){
          G_ExceptionList.lthrow("Couldn't find complexitiy in '"+problem
                                 +"' in entry "
                                 +record->mmap.get_string(std::string("BIBENTRY")),
                                 Lisa_ExceptionList::WARNING);
        }

        Lisa_ProblemType pt;
        int ok = parse_problem(problem.substr(pos1+1,pos2-pos1-1),&pt);
        /*
        if(!ok){
          G_ExceptionList.lthrow("Error while parsing problem '"
                                 +problem.substr(pos1+1,pos2-pos1-1)
                                 +"' in entry "
                                 +record->mmap.get_string(std::string("BIBENTRY")),
                                 Lisa_ExceptionList::WARNING);
        }
        */
        if(ok && cpl != OPEN){
          
          record->problems.push_back(pt);
          record->complexity.push_back(cpl);
          record->number_of_problems++;
        }
        
      }
      
      problem = "";
    }else{  
      problem.append(1,c);
    }
  }
}

//**************************************************************************

int Lisa_Classify::parse_problem(const std::string& problem, Lisa_ProblemType *const pt){
  const unsigned int size = problem.size();
  unsigned int error = 0;
  
  char *const alpha1  = new char[size];
  char *const beta   = new char[size];
  char *const gamma  = new char[size];
  char *const substr = new char[size];
  char *const hlpstr = new char[size];
	
  //work using alpha pointer
  //but keep alpha1 for cleanup later
  char* alpha = alpha1;
  
  // split problem into alpha beta and gamma component
  strncpy(alpha,problem.c_str(),size); 
  *(strchr(alpha,'|'))='\0';
  
  strncpy(beta,strchr(problem.c_str(),'|')+1,size); 
  *(strchr(beta,'|'))='\0';
  
  strncpy(gamma,strrchr(problem.c_str(),'|')+1,size);
  
  // alpha entry ---------------------------------------------------
  
  if (strstr(alpha,"MPT")){
         
         if (*alpha=='M') pt->set_property(M_ENV,MPT);
    else if (*alpha=='O') pt->set_property(M_ENV,OMPT);
    else if (*alpha=='F') pt->set_property(M_ENV,FMPT);
    else if (*alpha=='J') pt->set_property(M_ENV,JMPT);
    else if (*alpha=='X') pt->set_property(M_ENV,XMPT);
    else if (*alpha=='G') pt->set_property(M_ENV,GMPT);
    
    if (*alpha=='M') alpha+=3;
    else alpha+=4;
    
  }else if (strstr(alpha,"MPM")){
    
         if (*alpha=='O') pt->set_property(M_ENV,OMPM);
    else if (*alpha=='F') pt->set_property(M_ENV,FMPM);
    else if (*alpha=='J') pt->set_property(M_ENV,JMPM);
    else if (*alpha=='X') pt->set_property(M_ENV,XMPM);
    else if (*alpha=='G') pt->set_property(M_ENV,GMPM);
    else if (*alpha=='Q') pt->set_property(M_ENV,QMPM);
    else if (*alpha=='P') pt->set_property(M_ENV,PMPM);
   
    alpha+=4;
    
  }else{
    
         if (*alpha=='1') pt->set_property(M_ENV,ONE);
    else if (*alpha=='O') pt->set_property(M_ENV,O);
    else if (*alpha=='F') pt->set_property(M_ENV,F);
    else if (*alpha=='J') pt->set_property(M_ENV,J);
    else if (*alpha=='X') pt->set_property(M_ENV,X);
    else if (*alpha=='G') pt->set_property(M_ENV,G);
    else if (*alpha=='P') pt->set_property(M_ENV,P);
    else if (*alpha=='Q') pt->set_property(M_ENV,Q);
    else if (*alpha=='R') pt->set_property(M_ENV,R);
    else{
      error = 1;
      goto error_occured;
    }
    
    alpha++;
    
  }
  
  if ((*alpha=='\0') || (*alpha==';')){
    
    pt->set_property(M_NUMBER,M_ARB);
    if (*alpha==';'){
      
      alpha++;
      
      if (strcmp(alpha,"R1")==0){
	      pt->set_property(M_ENV,F_SR);
	      alpha+=2;
	    }else if (strcmp(alpha,"S1")==0){
	      pt->set_property(M_ENV,P_CS);
	      alpha+=2;
	    }else{ 
        error = 1;
        goto error_occured;
      }
    }
  
  }else if (*alpha=='m'){
    pt->set_property(M_NUMBER,M_FIX);
    alpha++;
  }else if ((*alpha >= '2') && (*alpha <= '9')){
    sscanf(alpha,"%1u",&pt->m_no);
    pt->set_property(M_NUMBER,M_VAL);
    alpha++;
  }else{
    error = 1;
    goto error_occured;
  }
  
  if (*alpha==';'){
    
    alpha++;
    
         if (strcmp(alpha,"R1")==0) pt->set_property(M_ENV,F_SR);
    else if (strcmp(alpha,"S1")==0) pt->set_property(M_ENV,P_CS);
    else{
      error = 1;
      goto error_occured;
    }
  
  }else if (*alpha!='\0'){
    error = 1;
    goto error_occured;
  }
  
  // beta entry ------------------------------------------------
  
  while (*beta!='\0'){
    
    if ((strchr(beta,';'))){
      strncpy(substr,strchr(beta,';')+1,size);
      *(strchr(beta,';'))='\0';
    }else{ 
      strcpy(substr,strchr(beta,'\0'));
    }
    
         if (strcmp(beta,"pmtn")==0) pt->set_property(PMTN,true);
    else if (strcmp(beta,"intree")==0) pt->set_property(PRECEDENCE,INTREE);
    else if (strcmp(beta,"outtree")==0) pt->set_property(PRECEDENCE,OUTTREE);
    else if (strcmp(beta,"tree")==0) pt->set_property(PRECEDENCE,TREE);
    else if (strcmp(beta,"sp-graph")==0) pt->set_property(PRECEDENCE,SP_GRAPH);
    else if (strcmp(beta,"chains")==0) pt->set_property(PRECEDENCE,CHAINS);
    else if (strcmp(beta,"prec")==0) pt->set_property(PRECEDENCE,PREC);
    else if (strchr(beta,'(')){
      
      strncpy(hlpstr,strchr(beta,'('),size);
      *(strchr(beta,'('))='\0';
      
      if (strcmp(beta,"intree")==0) pt->set_property(PRECEDENCE,INTREE);
      else if (strcmp(beta,"outtree")==0) pt->set_property(PRECEDENCE,OUTTREE);
      else if (strcmp(beta,"tree")==0) pt->set_property(PRECEDENCE,TREE);
      else if (strcmp(beta,"sp-graph")==0) pt->set_property(PRECEDENCE,SP_GRAPH);
      else if (strcmp(beta,"chains")==0) pt->set_property(PRECEDENCE,CHAINS);
      else if (strcmp(beta,"prec")==0) pt->set_property(PRECEDENCE,PREC);
      else{
        error = 1;
        goto error_occured;
      }
      
           if (strcmp(hlpstr,"(1)")==0) pt->set_property(TIME_LAGS,UNIT_TL);
      else if (strcmp(hlpstr,"(l)")==0) pt->set_property(TIME_LAGS,CONST_TL);
      else if (strcmp(hlpstr,"(l_{ij})")==0) pt->set_property(TIME_LAGS,GENERAL_TL);
      else{
        error = 1;
        goto error_occured;
      }
    
    }else if (strcmp(beta,"r_i")==0) pt->set_property(RI,true);
    else if (strcmp(beta,"d_i")==0) pt->set_property(DI,true);
	  
    // handle pij = pi as the same
    else if (strcmp(beta,"p_{ij}=1")==0 || strcmp(beta,"p_i=1")==0) pt->set_property(PIJ,PIJ_1);  
    else if (strcmp(beta,"p_{ij}=p")==0 || strcmp(beta,"p_i=p")==0) pt->set_property(PIJ,PIJ_P);
    
    else if (strcmp(beta,"s-batch")==0) pt->set_property(BATCH,S_BATCH);
    else if (strcmp(beta,"p-batch")==0) pt->set_property(BATCH,P_BATCH);
    else if (strcmp(beta,"b<n")==0) pt->set_property(BOUNDED_BATCH,true);
    else if (strncmp(beta,"n=",2)==0){
      
      if (*(beta+2)=='k') pt->set_property(JOB_NR,J_FIX);
      else if ((*(beta+2) >= '2') && (*(beta+2) <= '9')){
       sscanf(beta+2,"%1u",&pt->n_no);
       pt->set_property(JOB_NR,J_VAL);
      }
      
    }else if (strcmp(beta,"no-wait")==0) pt->set_property(NO_WAIT,true);
    else if (strcmp(beta,"size_i")==0) pt->set_property(SIZE,true);
    else if (strcmp(beta,"t_{ik}=T")==0) pt->set_property(TRANSPORTATION_DELAYS,TIK_T);
    else if (strcmp(beta,"t_{ikl}=T")==0) pt->set_property(TRANSPORTATION_DELAYS,TIKL_T);
    else if (strcmp(beta,"t_i\\in\\{T_1,T_2\\}")==0) pt->set_property(TRANSPORTATION_DELAYS,TI_IN);
    else if (strcmp(beta,"t_{kl}=t_{lk}")==0) pt->set_property(TRANSPORTATION_DELAYS,TKL_TLK);
    else if (strcmp(beta,"t_i")==0) pt->set_property(TRANSPORTATION_DELAYS,TI);
    else if (strcmp(beta,"t_k")==0) pt->set_property(TRANSPORTATION_DELAYS,TK);
    else if (strcmp(beta,"t_{kl}")==0) pt->set_property(TRANSPORTATION_DELAYS,TKL);
    else if (strcmp(beta,"t_{ikl}=t_{ilk}")==0) pt->set_property(TRANSPORTATION_DELAYS,TIKL_TILK);
    else if (strcmp(beta,"t_{ik}")==0) pt->set_property(TRANSPORTATION_DELAYS,TIK);
    else if (strcmp(beta,"t_{ikl}")==0) pt->set_property(TRANSPORTATION_DELAYS,TIKL);
    else if (strcmp(beta,"s_i")==0) pt->set_property(SERVER_FLAGS,SI);        
    else if (strcmp(beta,"s_i=1")==0) pt->set_property(SERVER_FLAGS,SI_1);      
    else if (strcmp(beta,"s_i=s")==0) pt->set_property(SERVER_FLAGS,SI_S);    
    else{
      error = 1;
      goto error_occured;
    }
    
    strcpy(beta,substr);
  }
  
  // gamma entry --------------------------------------------------
  
       if (strcmp(gamma,"C_{\\max}")==0) pt->set_property(OBJECTIVE,CMAX);
  else if (strcmp(gamma,"L_{\\max}")==0) pt->set_property(OBJECTIVE,LMAX);
  else if (strcmp(gamma,"\\sum{C_i}")==0) pt->set_property(OBJECTIVE,SUM_CI);
  else if (strcmp(gamma,"\\sum{w_iC_i}")==0) pt->set_property(OBJECTIVE,SUM_WICI);
  else if (strcmp(gamma,"\\sum{U_i}")==0) pt->set_property(OBJECTIVE,SUM_UI);
  else if (strcmp(gamma,"\\sum{w_iU_i}")==0) pt->set_property(OBJECTIVE,SUM_WIUI);
  else if (strcmp(gamma,"\\sum{T_i}")==0) pt->set_property(OBJECTIVE,SUM_TI);
  else if (strcmp(gamma,"\\sum{w_iT_i}")==0) pt->set_property(OBJECTIVE,SUM_WITI);
  else{
    error = 1;
    goto error_occured;
  }
  
  error_occured:
  
  delete[] alpha1; 
  delete[] beta;  
  delete[] gamma;
  delete[] substr;
  delete[] hlpstr;
  
  return !error;
}

//**************************************************************************

std::string Lisa_Classify::result_text(const int np,const int mm_flag){
  std::string s;

  if (np==OPEN)
    s=translate("OPEN_TEXT");
  else
    {
      s=translate("SENTENCE_START")+" ";
      switch (np)
	{
	case STR_NP_HARD:
	  if (!mm_flag)
	    s+=translate("STR_NP_HARD")+", "+translate("SENTENCE_CONT1")+
	      " "+translate("HARD_CONT1")+" "+translate("STR_NP_HARD")+":"; 
	  else
	    s+=translate("MIN")+" "+translate("STR_NP_HARD")+ ":";
	  break; 
	case NP_HARD:
	  if (!mm_flag)
	    s+=translate("NP_HARD")+", "+translate("SENTENCE_CONT1")+
	      " "+translate("HARD_CONT1")+" "+translate("NP_HARD")+":"; 
	  else
	    s+=translate("MIN")+" "+translate("NP_HARD")+ ":";
	  break;
	case PS_POLYN:
	  if (!mm_flag)
	    s+=translate("PS_POLYN")+", "+translate("SENTENCE_CONT1")+
	      " "+translate("EASY_CONT1")+" "+translate("PS_POLYN")+":"; 
	  else
	    s+=translate("MAX")+" "+translate("PS_POLYN")+ ":";
	  break;
	case POLYN:
	  if (!mm_flag)
	    s+=translate("POLYN")+", "+translate("SENTENCE_CONT1")+
	      " "+translate("EASY_CONT1")+" "+translate("POLYN")+":"; 
	  else
	    s+=translate("MAX")+" "+translate("POLYN")+ ":";
	  break;
	default: break;
	}
    }
  s+="\n\n";
  return s;
}

//**************************************************************************
 
std::string Lisa_Classify::reference_output(const int i ,const int j){
  return records[i].problems[j].output_problem()+"\n"+translate("SEE")+" "
        +records[i].mmap.get_string("AUTHOR")+", "
        +records[i].mmap.get_string("YEAR")+"\n\n\n\n";
}

//**************************************************************************

std::string Lisa_Classify::classify(const Lisa_ProblemType& G_Problem){
  int comp, cplx, open=true;
  
  std::string output;
  
  // start of the classification
  for (int i=0; i<entries; i++){
    for (int j=0; j<records[i].number_of_problems; j++){
      
      comp = redgraph.compare(G_Problem,records[i].problems[j]);
      cplx = records[i].complexity[j];
   	
      switch (comp){
        case IDENT: 
          output+=result_text(cplx,MINMAX)+reference_output(i,j); 
          open=false; 
          break;
        case FIRST_TO_SECOND: 
          if ((cplx==POLYN) || (cplx==PS_POLYN)){
            output+=result_text(cplx,NORMAL)+reference_output(i,j);
            open=false;
	        } 
          break;
        case NOT_CMP: 
          break;
        case SECOND_TO_FIRST: 
          if ((cplx==STR_NP_HARD) || (cplx==NP_HARD)){
            output+=result_text(cplx,NORMAL)+reference_output(i,j);
            open=false;
          } 
          break;
	      default: 
          G_ExceptionList.lthrow("Error in reduction return value.",
					                        Lisa_ExceptionList::UNDEFINED_OBJECT);
      } 
    }  
  } 
   
  if (open) output+=result_text(OPEN,NORMAL);
  
  return output;
}
  
//**************************************************************************
  
std::string Lisa_Classify::full_reference(const Lisa_ProblemType& G_Problem){
  int comp, cplx, open=true, notice=-1;
  std::string full_ref="";

  // collecting the requested references:
  for (int i=0; i<entries; i++){
    for (int j=0; j<records[i].number_of_problems; j++){
      comp = redgraph.compare(G_Problem,records[i].problems[j]);
      cplx = records[i].complexity[j];
      switch (comp){
        case IDENT:
          if (i!=notice) full_ref+=records[i].mmap.get_string("BIBENTRY")+"\n";
          open=false; 
          notice=i; 
          break;
        case FIRST_TO_SECOND:
          if ((i!=notice) && ((cplx==POLYN) || (cplx==PS_POLYN))){
            full_ref+=records[i].mmap.get_string("BIBENTRY")+"\n";
            open=false;
            notice=i; 
          } 
          break;
        case SECOND_TO_FIRST:
          if ((i!=notice) && ((cplx==STR_NP_HARD) || (cplx==NP_HARD))){
            full_ref+=records[i].mmap.get_string("BIBENTRY")+"\n";
            open=false;
            notice=i; 
          } 
          break;
        case NOT_CMP: 
          break;
        default:
          G_ExceptionList.lthrow("Error in reduction return value",
					                       Lisa_ExceptionList::UNDEFINED_OBJECT);
      } 
    }  
  }
  
  return full_ref;
}
  
//**************************************************************************

