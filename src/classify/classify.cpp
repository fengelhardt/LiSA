#include <stdio.h>
#include <string.h>
#include "classify.hpp"
#include "redgraph.hpp"
#include "database.hpp"

string 
Lisa_classify(Lisa_ProblemType *G_Problem,string home,string file) 
{
  int           i, j, comp, cplx, open=TRUE;
  string        db_path, output="";
  Lisa_RedGraph *mygraph=0;
  Lisa_DataBase *mybase;

  // sets the database path:
  db_path=home+file;
  
  if (!(mybase=new Lisa_DataBase(db_path)) || !(mygraph=new Lisa_RedGraph))
    {
      G_ExceptionList.lthrow("No more memory available!\n",NO_MORE_MEMORY);
      return("No more memory available!\n");
    }

  // reduction graph
  // mygraph->output();

  // data base output
  // mybase->output();

  // start of the classification
  for (i=0; i<mybase->entries; i++)
    for (j=0; j<mybase->E[i].no_of_prbls; j++)
      {
   	comp = mygraph->compare(G_Problem,&mybase->E[i].problem[j]);
	cplx = mybase->E[i].tex_an[j].np_flag;
   	switch (comp)
   	  {
	  case IDENT: 
	    output+=result_text(cplx,MINMAX)+reference_output(i,j,mybase); 
	    open=FALSE; 
	    break;
	  case FIRST_TO_SECOND: 
	    if ((cplx==POLYN) || (cplx==PS_POLYN))
	      {
		output+=result_text(cplx,NORMAL)+reference_output(i,j,mybase);
		open=FALSE;
	      } 
	    break;
	  case NOT_CMP: 
	    break;
	  case SECOND_TO_FIRST: 
	    if ((cplx==STR_NP_HARD) || (cplx==NP_HARD))
	      {
		output+=result_text(cplx,NORMAL)+reference_output(i,j,mybase);
		open=FALSE;
	      } 
	    break;
	  default: G_ExceptionList.lthrow("Error in reduction return value",
					  UNDEFINED_OBJECT);
   	  } 
      }  
  if (open)
    output+=result_text(OPEN,NORMAL);
      
  delete mybase; 
  delete mygraph; 
  return(output);
}

string
Lisa_full_ref(Lisa_ProblemType *G_Problem,string home,string file) 
{
  int           i, j, comp, cplx, open=TRUE, notice=-1;
  string        db_path, full_ref="";
  Lisa_RedGraph *mygraph=0;
  Lisa_DataBase *mybase;

  // sets the database path:
  db_path=home+file;
  
  if (!(mybase=new Lisa_DataBase(db_path)) || !(mygraph=new Lisa_RedGraph))
    {
      G_ExceptionList.lthrow("No more memory available!\n",NO_MORE_MEMORY);
      return("No more memory available!\n");
    }

  // collecting the requested references:
  for (i=0; i<mybase->entries; i++)
    for (j=0; j<mybase->E[i].no_of_prbls; j++)
      {
   	comp = mygraph->compare(G_Problem,&mybase->E[i].problem[j]);
	cplx = mybase->E[i].tex_an[j].np_flag;
   	switch (comp)
   	  {
	  case IDENT:
	    if (i!=notice)
	      full_ref+=(string) mybase->E[i].bibentry;
	    open=FALSE; notice=i; 
	    break;
	  case FIRST_TO_SECOND:
	    if ((i!=notice) && ((cplx==POLYN) || (cplx==PS_POLYN)))
	      {
		full_ref+=(string) mybase->E[i].bibentry;
		open=FALSE; notice=i; 
	      } 
	    break;
	  case SECOND_TO_FIRST:
	    if ((i!=notice) && ((cplx==STR_NP_HARD) || (cplx==NP_HARD)))
	      {
		full_ref+=(string) mybase->E[i].bibentry;
		open=FALSE; notice=i; 
	      } 
	    break;
	  case NOT_CMP: 
	    break;
	  default: G_ExceptionList.lthrow("Error in reduction return value",
					  UNDEFINED_OBJECT);
   	  } 
      }  
  
  delete mybase; 
  delete mygraph; 
  return(full_ref);
}

string
result_text(int np,int mm_flag)
{
  string s;

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
  return(s);
}

string
reference_output(int i,int j,Lisa_DataBase* mybase)
{
  string s;

  s=(string) mybase->E[i].tex_an[j].prbl+"\n"+translate("SEE")+" "+
    (string) mybase->E[i].author+", "+(string) mybase->E[i].year+
    "\n\n\n\n";
  return(s);
}


