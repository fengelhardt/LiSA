#include <stdio.h>
#include "../basics/lisa_str.hpp"
#include <string.h>
#include <fstream.h>
#include "database.hpp"


Lisa_DataBase::Lisa_DataBase(string name)
{
  int      i=0;
  ifstream file((char*) name.c_str());
  char     *line   = new char[LINE];
  char     *bibstr = new char[BIBENTRY]; *bibstr='\0';
  
  file.getline(line,LINE,'\n'); 
  while (!strchr(line,'@'))
    file.getline(line,LINE,'\n'); 
  strcat(bibstr,"\n"); strcat(bibstr,line);
  while (file.getline(line,LINE,'\n'))
    {
      if (strchr(line,'@')) // if '@' is found, take next input as
	{	            // new bib-entry.
	  bib_entry(bibstr,i); 
	  *bibstr='\0';
	  i++;
	}
      if (!strchr(line,'%'))
	  {
	    strcat(bibstr,"\n"); strcat(bibstr,line);
	  }
    }
  bib_entry(bibstr,i); // take also the last entry ('@' cannot be found)! 
  i++;                   
  entries=i;

  delete [] line;
  delete [] bibstr;
}

int
Lisa_DataBase::bib_entry(char *bibstr, int i)
{  
  int j, k, pn;
  char *annote  = new char[ANNOTE];
  char *helpstr = new char[ANNOTE];
  
  strncpy(E[i].bibentry,bibstr,BIBENTRY);

  if (strstr(bibstr,"AUTHOR =") || strstr(bibstr,"author ="))
    {
      if (strstr(bibstr,"AUTHOR ="))
	strncpy(E[i].author,strchr(strstr(bibstr,"AUTHOR ="),'{')+1,AUTHOR);
      else
	strncpy(E[i].author,strchr(strstr(bibstr,"author ="),'{')+1,AUTHOR);
      *(strchr(E[i].author,'='))='\0';
      *(strrchr(E[i].author,'}'))='\0';
      if (strstr(E[i].author," and"))
	if (strstr(strstr(E[i].author," and")+4," and"))
	  {
	    *(strstr(E[i].author," and"))='\0';
	    strcat(E[i].author," et al.");
	  }
    }
  else
    {
      // if not author is available, put title into author field!
      if  (strstr(bibstr,"TITLE =") || strstr(bibstr,"title ="))
	{
	  if (strstr(bibstr,"TITLE ="))
	    strncpy(E[i].author,strchr(strstr(bibstr,"TITLE ="),':')+1,AUTHOR);
	  else
	    strncpy(E[i].author,strchr(strstr(bibstr,"title ="),':')+1,AUTHOR); 
	  *(strchr(E[i].author,'='))='\0';
	  *(strrchr(E[i].author,'}'))='\0';
	}
      // error if author and title are not available
      else 
	error_output(i,NO_AUTHOR);
    }
  if (strstr(bibstr,"YEAR =") || strstr(bibstr,"year ="))
    {
      if (strstr(bibstr,"YEAR ="))
	strncpy(E[i].year,strstr(bibstr,"YEAR ="),YEAR);
      else
	strncpy(E[i].year,strstr(bibstr,"year ="),YEAR);
      *(strchr(E[i].year+6,'='))='\0';
      if (strchr(E[i].year,'{'))
	*(strrchr(E[i].year,'}'))='\0';
      else
	*(strrchr(E[i].year,','))='\0';
      strcpy(E[i].year,E[i].year+strlen(E[i].year)-4);
    }
  else
    strcpy(E[i].year,"no year available!");

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
	      *(strchr(annote,'$'))='\0'; pn=(k-1)/2;
	      if (k%2!=0)
		{
		  if ((strchr(annote,'|'))!=(strrchr(annote,'|')))       
		    {
		      strcpy(E[i].tex_an[pn].prbl,annote);
		      prbl_into_tupel(annote,i,pn);	      
		    }   
		  else                 
		    error_output(i,pn);   
		    // This error can occur if the leading '$' or a '|' 
                    // is missing in the problem type.
		}
	      else
		if (strcmp(annote,"*\\NP")==0)
		  E[i].tex_an[pn].np_flag=STR_NP_HARD;
		else if (strcmp(annote,"P")==0)
		  E[i].tex_an[pn].np_flag=POLYN;
		else if (strcmp(annote,"\\NP")==0)
		  E[i].tex_an[pn].np_flag=NP_HARD;
		else if (strcmp(annote,"P_{pseudo}")==0)
		  E[i].tex_an[pn].np_flag=PS_POLYN;
		else
		  error_output(i,pn);
	    }
	  strcpy(annote,helpstr);
	}
      if (k%2!=0)
	error_output(i,pn);
      E[i].no_of_prbls=k/2;
    }
  else
    {
      error_output(i,NO_ANNOTE);
      return(1);
    }
  delete [] annote;
  delete [] helpstr;
  return(0);
}


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
	 E[dsatz].problem[nr].set_property(M_ENV,MPT);
       else if (*alpha=='O')
	 E[dsatz].problem[nr].set_property(M_ENV,OMPT);
       else if (*alpha=='F')
	 E[dsatz].problem[nr].set_property(M_ENV,FMPT);
       else if (*alpha=='J')
	 E[dsatz].problem[nr].set_property(M_ENV,JMPT);
       else if (*alpha=='X')
	 E[dsatz].problem[nr].set_property(M_ENV,XMPT);
       else if (*alpha=='G')
	 E[dsatz].problem[nr].set_property(M_ENV,GMPT);
       if (*alpha=='M')
	 strcpy(alpha,alpha+3);
       else
	 strcpy(alpha,alpha+4);
     }
   else if (strstr(alpha,"MPM"))
     {
       if (*alpha=='O')
	 E[dsatz].problem[nr].set_property(M_ENV,OMPM);
       else if (*alpha=='F')
	 E[dsatz].problem[nr].set_property(M_ENV,FMPM);
       else if (*alpha=='J')
	 E[dsatz].problem[nr].set_property(M_ENV,JMPM);
       else if (*alpha=='X')
	 E[dsatz].problem[nr].set_property(M_ENV,XMPM);
       else if (*alpha=='G')
	 E[dsatz].problem[nr].set_property(M_ENV,GMPM);
       else if (*alpha=='Q')
	 E[dsatz].problem[nr].set_property(M_ENV,QMPM);
       else if (*alpha=='P')
	 E[dsatz].problem[nr].set_property(M_ENV,PMPM);
       strcpy(alpha,alpha+4);
     }
  else 
    {
      if (*alpha=='1') 
	E[dsatz].problem[nr].set_property(M_ENV,ONE);
      else if (*alpha=='O')
	E[dsatz].problem[nr].set_property(M_ENV,O);
      else if (*alpha=='F')
	E[dsatz].problem[nr].set_property(M_ENV,F);
      else if (*alpha=='J')
	E[dsatz].problem[nr].set_property(M_ENV,J);
      else if (*alpha=='X')
	E[dsatz].problem[nr].set_property(M_ENV,X);
      else if (*alpha=='G')
	E[dsatz].problem[nr].set_property(M_ENV,G);
      else if (*alpha=='P')
	E[dsatz].problem[nr].set_property(M_ENV,P);
      else if (*alpha=='Q')
	E[dsatz].problem[nr].set_property(M_ENV,Q);
      else if (*alpha=='R')
	E[dsatz].problem[nr].set_property(M_ENV,R);
      else
	error_output(dsatz,nr);
      strcpy(alpha,alpha+1);
    }
  if ((*alpha=='\0') || (*alpha==';'))
    {
      E[dsatz].problem[nr].set_property(M_NUMBER,M_ARB);
      if (*alpha==';')
	{      
	  strcpy(alpha,alpha+1);
	  if (strcmp(alpha,"R1")==0)
	    {
	      E[dsatz].problem[nr].set_property(M_ENV,F_SR);
	      strcpy(alpha,alpha+2);
	    }
	  else if (strcmp(alpha,"S1")==0)
	    {
	      E[dsatz].problem[nr].set_property(M_ENV,P_CS);
	      strcpy(alpha,alpha+2);
	    }
	  else 
	    error_output(dsatz,nr);
	}
    }   
  else if (*alpha=='m')
    {
      E[dsatz].problem[nr].set_property(M_NUMBER,M_FIX);
      strcpy(alpha,alpha+1);
    }
  else if ((*alpha >= '2') && (*alpha <= '9'))
    {
      sscanf(alpha,"%1u",&E[dsatz].problem[nr].m_no);
      E[dsatz].problem[nr].set_property(M_NUMBER,M_VAL);
      strcpy(alpha,alpha+1);
    }
  else 
    error_output(dsatz,nr);
  if (*alpha==';')
    {
      strcpy(alpha,alpha+1);
      if (strcmp(alpha,"R1")==0)
	E[dsatz].problem[nr].set_property(M_ENV,F_SR);
      else if (strcmp(alpha,"S1")==0)
	E[dsatz].problem[nr].set_property(M_ENV,P_CS);
      else 
	error_output(dsatz,nr);
    } 
  else if (*alpha!='\0')
    error_output(dsatz,nr);
  
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
	E[dsatz].problem[nr].set_property(PMTN,TRUE);
      else if (strcmp(beta,"intree")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,INTREE);
      else if (strcmp(beta,"outtree")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,OUTTREE);
      else if (strcmp(beta,"tree")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,TREE);
      else if (strcmp(beta,"sp-graph")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,SP_GRAPH);
      else if (strcmp(beta,"chains")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,CHAINS);
      else if (strcmp(beta,"prec")==0)
	E[dsatz].problem[nr].set_property(PRECEDENCE,PREC);
      else if (strchr(beta,'('))
	{
	  strncpy(hlpstr,strchr(beta,'('),LINE);
	  *(strchr(beta,'('))='\0';
	  if (strcmp(beta,"intree")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,INTREE);
	  else if (strcmp(beta,"outtree")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,OUTTREE);
	  else if (strcmp(beta,"tree")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,TREE);
	  else if (strcmp(beta,"sp-graph")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,SP_GRAPH);
	  else if (strcmp(beta,"chains")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,CHAINS);
	  else if (strcmp(beta,"prec")==0)
	    E[dsatz].problem[nr].set_property(PRECEDENCE,PREC);
	  else
	    error_output(dsatz,nr);
	  if (strcmp(hlpstr,"(1)")==0)
	    E[dsatz].problem[nr].set_property(TIME_LAGS,UNIT_TL);
	  else if (strcmp(hlpstr,"(l)")==0)
	    E[dsatz].problem[nr].set_property(TIME_LAGS,CONST_TL);
	  else if (strcmp(hlpstr,"(l_{ij})")==0)
	    E[dsatz].problem[nr].set_property(TIME_LAGS,GENERAL_TL);
	  else
	    error_output(dsatz,nr);      
	}
      else if (strcmp(beta,"r_i")==0)
	E[dsatz].problem[nr].set_property(RI,TRUE);
      else if (strcmp(beta,"d_i")==0)
	E[dsatz].problem[nr].set_property(DI,TRUE);	  
      else if (strcmp(beta,"p_i=1")==0)
	E[dsatz].problem[nr].set_property(PIJ,PI_1);
      else if (strcmp(beta,"p_i=p")==0)
	E[dsatz].problem[nr].set_property(PIJ,PI_P);
      else if (strcmp(beta,"p_{ij}=1")==0)
	E[dsatz].problem[nr].set_property(PIJ,PIJ_1);  
      else if (strcmp(beta,"p_{ij}=p")==0)
	E[dsatz].problem[nr].set_property(PIJ,PIJ_P);  
      else if (strcmp(beta,"s-batch")==0)
	E[dsatz].problem[nr].set_property(BATCH,S_BATCH);
      else if (strcmp(beta,"p-batch")==0)
	E[dsatz].problem[nr].set_property(BATCH,P_BATCH);
      else if (strcmp(beta,"b<n")==0)
	E[dsatz].problem[nr].set_property(BOUNDED_BATCH,TRUE);
      else if (strncmp(beta,"n=",2)==0)
	{
	  if (*(beta+2)=='k')
	    E[dsatz].problem[nr].set_property(JOB_NR,J_FIX);
	  else if ((*(beta+2) >= '2') && (*(beta+2) <= '9'))
	    {
	      sscanf(beta+2,"%1u",&E[dsatz].problem[nr].n_no);
	      E[dsatz].problem[nr].set_property(JOB_NR,J_VAL);
	    }
	}
      else if (strcmp(beta,"no-wait")==0)
	E[dsatz].problem[nr].set_property(NO_WAIT,TRUE);
      else if (strcmp(beta,"size_i")==0)
	E[dsatz].problem[nr].set_property(SIZE,TRUE);
      else if (strcmp(beta,"t_{ik}=T")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TIK_T);
      else if (strcmp(beta,"t_{ikl}=T")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TIKL_T);
      else if (strcmp(beta,"t_i\\in\\{T_1,T_2\\}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TI_IN);
      else if (strcmp(beta,"t_{kl}=t_{lk}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TKL_TLK);
      else if (strcmp(beta,"t_i")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TI);
      else if (strcmp(beta,"t_k")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TK);
      else if (strcmp(beta,"t_{kl}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TKL);
      else if (strcmp(beta,"t_{ikl}=t_{ilk}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TIKL_TILK);
      else if (strcmp(beta,"t_{ik}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TIK);
      else if (strcmp(beta,"t_{ikl}")==0)
	E[dsatz].problem[nr].set_property(TRANSPORTATION_DELAYS,TIKL);
      else if (strcmp(beta,"s_i")==0)
	E[dsatz].problem[nr].set_property(SERVER_FLAGS,SI);        
      else if (strcmp(beta,"s_i=1")==0)
	E[dsatz].problem[nr].set_property(SERVER_FLAGS,SI_1);      
      else if (strcmp(beta,"s_i=s")==0)
	E[dsatz].problem[nr].set_property(SERVER_FLAGS,SI_S);    
      else
	error_output(dsatz,nr);
      strcpy(beta,substr);
    }

// gamma entry --------------------------------------------------
 
  if (strcmp(gamma,"C_{\\max}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,CMAX);
  else if (strcmp(gamma,"L_{\\max}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,LMAX);
  else if (strcmp(gamma,"\\sum{C_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_CI);
  else if (strcmp(gamma,"\\sum{w_iC_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_WICI);
  else if (strcmp(gamma,"\\sum{U_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_UI);
  else if (strcmp(gamma,"\\sum{w_iU_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_WIUI);
  else if (strcmp(gamma,"\\sum{T_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_TI);
  else if (strcmp(gamma,"\\sum{w_iT_i}")==0)
    E[dsatz].problem[nr].set_property(OBJECTIVE,SUM_WITI);
  else
    error_output(dsatz,nr); 
  delete [] substr;

  return(0);
}

int
Lisa_DataBase::output(void)
{
  int j, k;
  for (j=0; j<entries; j++)
    {
      cout << "\n\n" << E[j].author << "\n" << E[j].year << ", j=" << j;
      for (k=0; k<E[j].no_of_prbls; k++)
	{
	  cout << "\n\n" << E[j].tex_an[k].prbl << " is ";
	  if (E[j].tex_an[k].np_flag==NP_HARD)
	    cout << "(ordinary) NP-hard"; 
	  else if (E[j].tex_an[k].np_flag==STR_NP_HARD)
	    cout << "strongly NP-hard";
	  else if (E[j].tex_an[k].np_flag==POLYN)
	    cout << "polynomially solvable";
	  else if (E[j].tex_an[k].np_flag==PS_POLYN)
	    cout << "pseudo-polynomially solvable";
	  else 
	    error_output(j,k);
	  cout << "\nM_ENV:           " 
	       << E[j].problem[k].get_property_old(M_ENV);
	  cout << "\nM_NUMBER:        " 
	       << E[j].problem[k].get_property_old(M_NUMBER);
	  cout << "\nPMTN:            " 
	       << E[j].problem[k].get_property_old(PMTN);
	  cout << "\nPRECEDENCE:      " 
	       << E[j].problem[k].get_property_old(PRECEDENCE);
	  cout << "\nRI               " 
	       << E[j].problem[k].get_property_old(RI);
	  cout << "\nDI:              " 
	       << E[j].problem[k].get_property_old(DI);
	  cout << "\nPIJ:             " 
	       << E[j].problem[k].get_property_old(PIJ);
	  cout << "\nBATCH:           " 
	       << E[j].problem[k].get_property_old(BATCH);
	  cout << "\nBOUNDED_BATCH:   " 
	       << E[j].problem[k].get_property_old(BOUNDED_BATCH);
          cout << "\nJOB_NR:          " 
	       << E[j].problem[k].get_property_old(JOB_NR);
          cout << "\nNO_WAIT:         " 
	       << E[j].problem[k].get_property_old(NO_WAIT);
          cout << "\nSIZE:         " 
	       << E[j].problem[k].get_property_old(SIZE);
          cout << "\nTIME_LAGS:    " 
	       << E[j].problem[k].get_property_old(TIME_LAGS);
          cout << "\nTRANSPORTATION_DELAYS:" 
	       << E[j].problem[k].get_property_old(TRANSPORTATION_DELAYS);
	  cout << "\nOBJECTIVE:    " 
	       << E[j].problem[k].get_property_old(OBJECTIVE);
	  cout << "\nm_no:         " 
	       << E[j].problem[k].m_no;
	  cout << "\nn_no:         " 
	       << E[j].problem[k].n_no;
	}
      cout << "\n" << E[j].bibentry;
    }
  
  return(0);
}

int 
Lisa_DataBase::error_output(int rec,int pn)
{
 
  G_ExceptionList.lfthrow(
     "Error while reading BibTeX database!\nSyntax error in record "+
     (string) (rec+1)+":\n\n"+(string) E[rec].author+", "+
     (string) E[rec].year+",\nProblem Number "+(string) (pn+1)+
     "\n\n",SYNTAX_ERROR); 
  if (pn==NO_ANNOTE)
    G_ExceptionList.lfthrow("ANNOTE entry is missing!\n\n",SYNTAX_ERROR);
  else if (pn==NO_AUTHOR)
    G_ExceptionList.lfthrow("AUTHOR and TITLE entry is missing!\n\n",
			   SYNTAX_ERROR);
  return(0);
}
