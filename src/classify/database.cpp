/**
 * @author  Martin Harborth
 * @version 2.3final
 */
#include <cstdio>
#include <cstring>
#include <ctype.h>

#include <string>
#include <fstream>

#include "../misc/int2str.hpp"
#include "database.hpp"

//**************************************************************************

Lisa_DataBase::Lisa_DataBase(std::string name){
  std::ifstream i_strm(name.c_str());
  if(!i_strm){
    G_ExceptionList.lthrow("Could not open '"+name+"' for reading.");
    return;
  }
  
  unsigned int lines = 0;
  entries = 0;
  std::string entry;
  char c;
  while(! i_strm.eof() ){
    
    do{
      i_strm.get(c);
      if(c == '\n') lines++;
    }while( !i_strm.eof() && c != '@');
    
    entry = "@";
    do{
      i_strm.get(c);
      if(c == '\n') lines++;
      if(c != '@') entry.append(1,c);
      else i_strm.putback(c);
      
    }while( !i_strm.eof() && c != '@');
    
    if(!i_strm.eof()){
      //std::cout << entry << std::endl << std::endl;
      bib_entry(entry.c_str(),entries);
      entries++;
    }
  }
  
  i_strm.close();
}

//**************************************************************************

using namespace std;

int Lisa_DataBase::bib_entry(const char *const bibstr, int i){  
  int j, k, pn=0;
  char *annote  = new char[ANNOTE];
  char *helpstr = new char[ANNOTE];
  records.push_back(Lisa_DBRecord());
  
  strncpy(records[i].bibentry,bibstr,BIBENTRY);
  
  if (strstr(bibstr,"AUTHOR =") || strstr(bibstr,"author ="))
  {
    if (strstr(bibstr,"AUTHOR ="))
    strncpy(records[i].author,strchr(strstr(bibstr,"AUTHOR ="),'{')+1,AUTHOR);
      else
      strncpy(records[i].author,strchr(strstr(bibstr,"author ="),'{')+1,AUTHOR);
        *(strchr(records[i].author,'='))='\0';
      *(strrchr(records[i].author,'}'))='\0';
      if (strstr(records[i].author," and"))
        if (strstr(strstr(records[i].author," and")+4," and"))
        {
          *(strstr(records[i].author," and"))='\0';
          strcat(records[i].author," et al.");
        }
  }
  else
  {
    // if not author is available, put title into author field!
    if  (strstr(bibstr,"TITLE =") || strstr(bibstr,"title ="))
    {
      if (strstr(bibstr,"TITLE ="))
        strncpy(records[i].author,strchr(strstr(bibstr,"TITLE ="),':')+1,AUTHOR);
      else
        strncpy(records[i].author,strchr(strstr(bibstr,"title ="),':')+1,AUTHOR); 
      *(strchr(records[i].author,'='))='\0';
    *(strrchr(records[i].author,'}'))='\0';
    }
    // error if author and title are not available
    else 
      error_output(i,-1,string("no author found"));
  }
  if (strstr(bibstr,"YEAR =") || strstr(bibstr,"year ="))
  {
    if (strstr(bibstr,"YEAR ="))
      strncpy(records[i].year,strstr(bibstr,"YEAR ="),YEAR);
    else
      strncpy(records[i].year,strstr(bibstr,"year ="),YEAR);
    *(strchr(records[i].year+6,'='))='\0';
    if (strchr(records[i].year,'{'))
    *(strrchr(records[i].year,'}'))='\0';
    else
      *(strrchr(records[i].year,','))='\0';
    strcpy(records[i].year,records[i].year+strlen(records[i].year)-4);
  }
  else
    strcpy(records[i].year,"no year available!");
  
  if (strstr(bibstr,"ANNOTE =") || strstr(bibstr,"annote ="))
  {
    j=0; k=0;
    if (strstr(bibstr,"ANNOTE ="))
    strncpy(annote,strchr(strstr(bibstr,"ANNOTE ="),'{')+1,ANNOTE);
      else
      strncpy(annote,strchr(strstr(bibstr,"annote ="),'{')+1,ANNOTE);
      if (strstr(annote,".}"))
    *(strstr(annote,".}"))='\0';  
  while (strchr(annote,'$'))
  {

    strncpy(helpstr,strchr(annote,'$')+1,ANNOTE);
    j++;
    if (j%2==0)
    {
      k++;
      *(strchr(annote,'$'))='\0';
      pn=(k-1)/2;
      records[i].texannotes.push_back(Lisa_TexAnnote());
      records[i].problems.push_back(Lisa_ProblemType());
      if (k%2!=0)
      {
        if ((strchr(annote,'|'))!=(strrchr(annote,'|')))       
        {
          strcpy(records[i].texannotes[pn].prbl,annote);
          prbl_into_tupel(annote,i,pn);	      
        }   
        else                 
          error_output(i,pn,string(annote));   
        // This error can occur if the leading '$' or a '|' 
        // is missing in the problem type.
      }
      else
        if (strcmp(annote,"*\\NP")==0)
          records[i].texannotes[pn].np_flag=STR_NP_HARD;
      else if (strcmp(annote,"P")==0)
        records[i].texannotes[pn].np_flag=POLYN;
      else if (strcmp(annote,"\\NP")==0)
        records[i].texannotes[pn].np_flag=NP_HARD;
      else if (strcmp(annote,"P_{pseudo}")==0)
      records[i].texannotes[pn].np_flag=PS_POLYN;
      else
        error_output(i,pn,string(annote));
    }
    strcpy(annote,helpstr);
  }
  if (k%2!=0)
    error_output(i,pn,string("k%2 != 0"));
  records[i].no_of_prbls=k/2;
  }
  else
  {
    error_output(i,-1,string("no annotate entry found"));
    return(1);
  }
  delete [] annote;
  delete [] helpstr;
  
  return(0);
}

//**************************************************************************

int 
Lisa_DataBase::prbl_into_tupel(char *prbl, int dsatz, int nr)
{
  char *alpha  = new char[LINE/2];
  char *beta   = new char[LINE/2];
  char *gamma  = new char[LINE/2];
  char *substr = new char[LINE];
  char *hlpstr = new char[LINE];
	      
  strncpy(alpha,prbl,LINE/2); *(strchr(alpha,'|'))='\0'; 
  strncpy(beta,strchr(prbl,'|')+1,LINE/2); *(strchr(beta,'|'))='\0'; 
  strncpy(gamma,strrchr(prbl,'|')+1,LINE/2);

  // alpha entry ---------------------------------------------------
  
  if (strstr(alpha,"MPT"))
     {
       if (*alpha=='M')
	 records[dsatz].problems[nr].set_property(M_ENV,MPT);
       else if (*alpha=='O')
	 records[dsatz].problems[nr].set_property(M_ENV,OMPT);
       else if (*alpha=='F')
	 records[dsatz].problems[nr].set_property(M_ENV,FMPT);
       else if (*alpha=='J')
	 records[dsatz].problems[nr].set_property(M_ENV,JMPT);
       else if (*alpha=='X')
	 records[dsatz].problems[nr].set_property(M_ENV,XMPT);
       else if (*alpha=='G')
	 records[dsatz].problems[nr].set_property(M_ENV,GMPT);
       if (*alpha=='M')
	 strcpy(alpha,alpha+3);
       else
	 strcpy(alpha,alpha+4);
     }
   else if (strstr(alpha,"MPM"))
     {
       if (*alpha=='O')
	 records[dsatz].problems[nr].set_property(M_ENV,OMPM);
       else if (*alpha=='F')
	 records[dsatz].problems[nr].set_property(M_ENV,FMPM);
       else if (*alpha=='J')
	 records[dsatz].problems[nr].set_property(M_ENV,JMPM);
       else if (*alpha=='X')
	 records[dsatz].problems[nr].set_property(M_ENV,XMPM);
       else if (*alpha=='G')
	 records[dsatz].problems[nr].set_property(M_ENV,GMPM);
       else if (*alpha=='Q')
	 records[dsatz].problems[nr].set_property(M_ENV,QMPM);
       else if (*alpha=='P')
	 records[dsatz].problems[nr].set_property(M_ENV,PMPM);
       strcpy(alpha,alpha+4);
     }
  else 
    {
      if (*alpha=='1') 
	records[dsatz].problems[nr].set_property(M_ENV,ONE);
      else if (*alpha=='O')
	records[dsatz].problems[nr].set_property(M_ENV,O);
      else if (*alpha=='F')
	records[dsatz].problems[nr].set_property(M_ENV,F);
      else if (*alpha=='J')
	records[dsatz].problems[nr].set_property(M_ENV,J);
      else if (*alpha=='X')
	records[dsatz].problems[nr].set_property(M_ENV,X);
      else if (*alpha=='G')
	records[dsatz].problems[nr].set_property(M_ENV,G);
      else if (*alpha=='P')
	records[dsatz].problems[nr].set_property(M_ENV,P);
      else if (*alpha=='Q')
	records[dsatz].problems[nr].set_property(M_ENV,Q);
      else if (*alpha=='R')
	records[dsatz].problems[nr].set_property(M_ENV,R);
      else
	error_output(dsatz,nr,string(alpha));
      strcpy(alpha,alpha+1);
    }
  if ((*alpha=='\0') || (*alpha==';'))
    {
      records[dsatz].problems[nr].set_property(M_NUMBER,M_ARB);
      if (*alpha==';')
	{      
	  strcpy(alpha,alpha+1);
	  if (strcmp(alpha,"R1")==0)
	    {
	      records[dsatz].problems[nr].set_property(M_ENV,F_SR);
	      strcpy(alpha,alpha+2);
	    }
	  else if (strcmp(alpha,"S1")==0)
	    {
	      records[dsatz].problems[nr].set_property(M_ENV,P_CS);
	      strcpy(alpha,alpha+2);
	    }
	  else 
	    error_output(dsatz,nr,string(alpha));
	}
    }   
  else if (*alpha=='m')
    {
      records[dsatz].problems[nr].set_property(M_NUMBER,M_FIX);
      strcpy(alpha,alpha+1);
    }
  else if ((*alpha >= '2') && (*alpha <= '9'))
    {
      sscanf(alpha,"%1u",&records[dsatz].problems[nr].m_no);
      records[dsatz].problems[nr].set_property(M_NUMBER,M_VAL);
      strcpy(alpha,alpha+1);
    }
  else 
    error_output(dsatz,nr,string(alpha));
  if (*alpha==';')
    {
      strcpy(alpha,alpha+1);
      if (strcmp(alpha,"R1")==0)
	records[dsatz].problems[nr].set_property(M_ENV,F_SR);
      else if (strcmp(alpha,"S1")==0)
	records[dsatz].problems[nr].set_property(M_ENV,P_CS);
      else 
	error_output(dsatz,nr,string(alpha));
    } 
  else if (*alpha!='\0')
    error_output(dsatz,nr,string(alpha));
  
  // beta entry ------------------------------------------------

  while (*beta!='\0')
    {
      if ((strchr(beta,';')))
	{
	  strncpy(substr,strchr(beta,';')+1,LINE);
	  *(strchr(beta,';'))='\0';
	}
      else 
	strcpy(substr,strchr(beta,'\0'));
      if (strcmp(beta,"pmtn")==0)
	records[dsatz].problems[nr].set_property(PMTN,true);
      else if (strcmp(beta,"intree")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,INTREE);
      else if (strcmp(beta,"outtree")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,OUTTREE);
      else if (strcmp(beta,"tree")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,TREE);
      else if (strcmp(beta,"sp-graph")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,SP_GRAPH);
      else if (strcmp(beta,"chains")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,CHAINS);
      else if (strcmp(beta,"prec")==0)
	records[dsatz].problems[nr].set_property(PRECEDENCE,PREC);
      else if (strchr(beta,'('))
	{
	  strncpy(hlpstr,strchr(beta,'('),LINE);
	  *(strchr(beta,'('))='\0';
	  if (strcmp(beta,"intree")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,INTREE);
	  else if (strcmp(beta,"outtree")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,OUTTREE);
	  else if (strcmp(beta,"tree")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,TREE);
	  else if (strcmp(beta,"sp-graph")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,SP_GRAPH);
	  else if (strcmp(beta,"chains")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,CHAINS);
	  else if (strcmp(beta,"prec")==0)
	    records[dsatz].problems[nr].set_property(PRECEDENCE,PREC);
	  else
	    error_output(dsatz,nr,string(beta));
	  if (strcmp(hlpstr,"(1)")==0)
	    records[dsatz].problems[nr].set_property(TIME_LAGS,UNIT_TL);
	  else if (strcmp(hlpstr,"(l)")==0)
	    records[dsatz].problems[nr].set_property(TIME_LAGS,CONST_TL);
	  else if (strcmp(hlpstr,"(l_{ij})")==0)
	    records[dsatz].problems[nr].set_property(TIME_LAGS,GENERAL_TL);
	  else
	    error_output(dsatz,nr,string(hlpstr));      
	}
      else if (strcmp(beta,"r_i")==0)
	records[dsatz].problems[nr].set_property(RI,true);
      else if (strcmp(beta,"d_i")==0)
	records[dsatz].problems[nr].set_property(DI,true);	  
      else if (strcmp(beta,"p_{ij}=1")==0 || strcmp(beta,"p_i=1")==0) // handle pij = pi as the same
	records[dsatz].problems[nr].set_property(PIJ,PIJ_1);  
      else if (strcmp(beta,"p_{ij}=p")==0 || strcmp(beta,"p_i=p")==0) // handle pij=pi as the same
	records[dsatz].problems[nr].set_property(PIJ,PIJ_P);  
      else if (strcmp(beta,"s-batch")==0)
	records[dsatz].problems[nr].set_property(BATCH,S_BATCH);
      else if (strcmp(beta,"p-batch")==0)
	records[dsatz].problems[nr].set_property(BATCH,P_BATCH);
      else if (strcmp(beta,"b<n")==0)
	records[dsatz].problems[nr].set_property(BOUNDED_BATCH,true);
      else if (strncmp(beta,"n=",2)==0)
	{
	  if (*(beta+2)=='k')
	    records[dsatz].problems[nr].set_property(JOB_NR,J_FIX);
	  else if ((*(beta+2) >= '2') && (*(beta+2) <= '9'))
	    {
	      sscanf(beta+2,"%1u",&records[dsatz].problems[nr].n_no);
	      records[dsatz].problems[nr].set_property(JOB_NR,J_VAL);
	    }
	}
      else if (strcmp(beta,"no-wait")==0)
	records[dsatz].problems[nr].set_property(NO_WAIT,true);
      else if (strcmp(beta,"size_i")==0)
	records[dsatz].problems[nr].set_property(SIZE,true);
      else if (strcmp(beta,"t_{ik}=T")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TIK_T);
      else if (strcmp(beta,"t_{ikl}=T")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TIKL_T);
      else if (strcmp(beta,"t_i\\in\\{T_1,T_2\\}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TI_IN);
      else if (strcmp(beta,"t_{kl}=t_{lk}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TKL_TLK);
      else if (strcmp(beta,"t_i")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TI);
      else if (strcmp(beta,"t_k")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TK);
      else if (strcmp(beta,"t_{kl}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TKL);
      else if (strcmp(beta,"t_{ikl}=t_{ilk}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TIKL_TILK);
      else if (strcmp(beta,"t_{ik}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TIK);
      else if (strcmp(beta,"t_{ikl}")==0)
	records[dsatz].problems[nr].set_property(TRANSPORTATION_DELAYS,TIKL);
      else if (strcmp(beta,"s_i")==0)
	records[dsatz].problems[nr].set_property(SERVER_FLAGS,SI);        
      else if (strcmp(beta,"s_i=1")==0)
	records[dsatz].problems[nr].set_property(SERVER_FLAGS,SI_1);      
  else if (strcmp(beta,"s_i=s")==0)
	records[dsatz].problems[nr].set_property(SERVER_FLAGS,SI_S);    
      else
	error_output(dsatz,nr,string(beta));
      strcpy(beta,substr);
    }

// gamma entry --------------------------------------------------
 
  if (strcmp(gamma,"C_{\\max}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,CMAX);
  else if (strcmp(gamma,"L_{\\max}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,LMAX);
  else if (strcmp(gamma,"\\sum{C_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_CI);
  else if (strcmp(gamma,"\\sum{w_iC_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_WICI);
  else if (strcmp(gamma,"\\sum{U_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_UI);
  else if (strcmp(gamma,"\\sum{w_iU_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_WIUI);
  else if (strcmp(gamma,"\\sum{T_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_TI);
  else if (strcmp(gamma,"\\sum{w_iT_i}")==0)
    records[dsatz].problems[nr].set_property(OBJECTIVE,SUM_WITI);
  else
    error_output(dsatz,nr,string(gamma)); 
  delete [] substr;

  return(0);
}

//**************************************************************************
/*
int
Lisa_DataBase::output(void)
{
  int j, k;
  for (j=0; j<entries; j++)
    {
      cout << "\n\n" << records[j].author << "\n" << records[j].year << ", j=" << j;
      for (k=0; k<records[j].no_of_prbls; k++)
	{
	  cout << "\n\n" << records[j].texannotes[k].prbl << " is ";
	  if (records[j].texannotes[k].np_flag==NP_HARD)
	    cout << "(ordinary) NP-hard"; 
	  else if (records[j].texannotes[k].np_flag==STR_NP_HARD)
	    cout << "strongly NP-hard";
	  else if (records[j].texannotes[k].np_flag==POLYN)
	    cout << "polynomially solvable";
	  else if (records[j].texannotes[k].np_flag==PS_POLYN)
	    cout << "pseudo-polynomially solvable";
	  else 
	    error_output(j,k,string("hardness unknown"));
	  cout << "\nM_ENV:           " 
	       << records[j].problems[k].get_property_old(M_ENV);
	  cout << "\nM_NUMBER:        " 
	       << records[j].problems[k].get_property_old(M_NUMBER);
	  cout << "\nPMTN:            " 
	       << records[j].problems[k].get_property_old(PMTN);
	  cout << "\nPRECEDENCE:      " 
	       << records[j].problems[k].get_property_old(PRECEDENCE);
	  cout << "\nRI               " 
	       << records[j].problems[k].get_property_old(RI);
	  cout << "\nDI:              " 
	       << records[j].problems[k].get_property_old(DI);
	  cout << "\nPIJ:             " 
	       << records[j].problems[k].get_property_old(PIJ);
	  cout << "\nBATCH:           " 
	       << records[j].problems[k].get_property_old(BATCH);
	  cout << "\nBOUNDED_BATCH:   " 
	       << records[j].problems[k].get_property_old(BOUNDED_BATCH);
          cout << "\nJOB_NR:          " 
	       << records[j].problems[k].get_property_old(JOB_NR);
          cout << "\nNO_WAIT:         " 
	       << records[j].problems[k].get_property_old(NO_WAIT);
          cout << "\nSIZE:         " 
	       << records[j].problems[k].get_property_old(SIZE);
          cout << "\nTIME_LAGS:    " 
	       << records[j].problems[k].get_property_old(TIME_LAGS);
          cout << "\nTRANSPORTATION_DELAYS:" 
	       << records[j].problems[k].get_property_old(TRANSPORTATION_DELAYS);
	  cout << "\nOBJECTIVE:    " 
	       << records[j].problems[k].get_property_old(OBJECTIVE);
	  cout << "\nm_no:         " 
	       << records[j].problems[k].m_no;
	  cout << "\nn_no:         " 
	       << records[j].problems[k].n_no;
	}
      cout << "\n" << records[j].bibentry;
    }
  
  return(0);
}
*/
//**************************************************************************

void Lisa_DataBase::error_output(int rec,int pn,string error_token){
 
  G_ExceptionList.lfthrow(
     "Error while reading BibTeX database! Syntax error in record "+
     ztos(rec+1)+": "+(string) records[rec].author+", "+
     (string) records[rec].year+", Problem Number "+ztos(pn+1)+
     "  Error at: "+error_token,Lisa_ExceptionList::SYNTAX_ERROR); 

}

//**************************************************************************

