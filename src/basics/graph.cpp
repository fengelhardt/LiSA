#include"graph.hpp"

//Spalte 0 :
//x=Anfang der succ-Liste
//y=Anfang ist Ende der succ-Liste 

//Zeile 0 :
//x=Anfang der pred-Liste
//y=Anfang ist Ende der pred-Liste 

//matrix [i][j] i,j>0
//x Zeiger in Vorwaertsrichtung
//y Zeiger in Rueckwaertsrichtung 

Lisa_Graph::~Lisa_Graph()
{
  delete matrix;
  delete succ_pred_pointer;
}

Lisa_Graph::Lisa_Graph(int number_of_knots)
{
  matrix=0;
  succ_pred_pointer=0;
  init(number_of_knots);
};

Lisa_Graph::Lisa_Graph(const Lisa_Graph& othergraph)
{
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph.get_knots());
  
  for(int i=0; i<n+1; i++){
    for(int j=0; j<n+1; j++){
      (*matrix)[i][j].x=(*(othergraph.matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph.matrix))[i][j].y;
    }
  }
}

Lisa_Graph::Lisa_Graph(const Lisa_Graph* othergraph)
{
  matrix=0;
  succ_pred_pointer=0;
  init(othergraph->get_knots());
  
  for(int i=0; i<n+1; i++){
    for(int j=0; j<n+1; j++){
      (*matrix)[i][j].x=(*(othergraph->matrix))[i][j].x;
      (*matrix)[i][j].y=(*(othergraph->matrix))[i][j].y;
    }
  }
}

/*
Lisa_Graph::Lisa_Graph(Lisa_JsProblem* jpro)
{
  matrix=0;
  succ_pred_pointer=0;
  init((jpro->n)*(jpro->m));
  ix=jpro->n;
  iy=jpro->m;
*/

int Lisa_Graph::get_knots() const
{
  return n;
};


int Lisa_Graph::get_first_successor(int knot)
{
  return ((*matrix)[knot][0].x);
}

int Lisa_Graph::get_last_successor(int knot)
{
  return ((*matrix)[knot][0].y);
}

int Lisa_Graph::get_first_predecessor(int knot)
{
  return ((*matrix)[0][knot].x);
}

int Lisa_Graph::get_last_predecessor(int knot)
{
  return ((*matrix)[0][knot].y);
}


int Lisa_Graph::signum(int start, int end) const
{
  if(start==end) {return 0;};
 
  if(((*matrix)[start][end].x>0)&&((*matrix)[start][end].y>0))
    {
      return 1;
    }
  else
    {
      return -1;
    }
}


int  Lisa_Graph::get_connection(int start, int end)
{
  if((start>n)||(end>n)||(start==end)){return NOT_POSSIBLE;};
  
  if(((*matrix)[start][end].x==0)&((*matrix)[start][end].y==0))
    {
      return NO;
    }
  else
    {
      //if(((*matrix)[start][end].x<0)||((*matrix)[start][end].y<0))
      if(signum(start, end)==-1)
	{
	  return CRA;
	}
      else
	{
	  //if((matrix[end][start]->x<0)||(matrix[end][start]->y<0))
	  if(signum(end, start)==-1)
	    {
	      return ARC;
	    }
	  else
	    {
	      return EDGE;
	    }
	}
    }
}



bool Lisa_Graph::insert_arc(int start, int end)
{
  
  if((start>n)||(end>n)||(start==end)||(start==0)||(end==0)){return false;};
  
  int con=get_connection(start, end);
  
  //Test ob arc schon vorhanden
  if((con!=ARC)&(con!=EDGE)){
    if(con==CRA){
      exclude_arc(end,start);
      return (insert_edge(start,end));
    }
    else{
      //Einfuegen des Bogens
      //Aendern der Succ-Liste von start
      
      int end_of_Succ_List=(*matrix)[start][0].y;
      if( end_of_Succ_List== n+1){
	(*matrix)[start][0].x=end;
      }
      else{
	(*matrix)[start][end_of_Succ_List].x=end;
      };
	  
      (*matrix)[start][0].y=end;
      (*matrix)[start][end].x=n+1;
      (*matrix)[start][end].y=end_of_Succ_List;
      
      //Aendern der Pred-Liste von end
      
      int end_of_Pred_List=(*matrix)[0][end].y;
      if( end_of_Pred_List == n+1){
	(*matrix)[0][end].x=start;
      }
      else{
	(*matrix)[end][end_of_Pred_List].x=-start;
      };
      
      (*matrix)[0][end].y=start;
      (*matrix)[end][start].x=-(n+1);
      (*matrix)[end][start].y=-end_of_Pred_List;
      
      return true;
    }
  }
  return false;
}

bool Lisa_Graph::insert_edge(int start, int end)
{  
  if((start>n)||(end>n)||(start==end)||(start==0)||(end==0)){return false;};

  int con=get_connection(start, end);

  if(con!=EDGE){
    if(con==ARC){
      exclude_arc(start,end);
    };
    if(con==CRA){
      exclude_arc(end,start);
    };

    int end_of_NB_List;
      
    //Aendern der Kantenliste von start
    
    end_of_NB_List=(*matrix)[start][start].y;
      
    if( end_of_NB_List == n+1){
      (*matrix)[start][start].x=end;
    }
    else{
      (*matrix)[start][end_of_NB_List].x=end;
    };
      
    (*matrix)[start][start].y=end;
    (*matrix)[start][end].x=n+1;
    (*matrix)[start][end].y=end_of_NB_List;
    
    //Aendern der Kantenliste von end
    
    end_of_NB_List=(*matrix)[end][end].y;
    
    if( end_of_NB_List == n+1){
      (*matrix)[end][end].x=start;
    }
    else{
      (*matrix)[end][end_of_NB_List].x=start;
    };
      
    (*matrix)[end][end].y=start;
    (*matrix)[end][start].x=n+1;
    (*matrix)[end][start].y=end_of_NB_List;

    return true;
  }
  return false;
}

bool Lisa_Graph::exclude_edge(int start, int end)
{
  if((start>n)||(end>n)||(start==end)||(start==0)||(end==0)){return false;};
  
  //Test ob Kante vorhanden
  int con=get_connection(start,end);

  if(con==EDGE){
    int succ_of_edge=(*matrix)[start][end].x;
    int pred_of_edge=(*matrix)[start][end].y;
    
    if(succ_of_edge==n+1){
      (*matrix)[start][start].y=pred_of_edge;
    }
    else{
      (*matrix)[start][succ_of_edge].y=pred_of_edge;
    };
      
    if(pred_of_edge==n+1){
      (*matrix)[start][start].x=succ_of_edge;
    }
    else{
      (*matrix)[start][pred_of_edge].x=succ_of_edge;
    };
      
    (*matrix)[start][end].x=0;
    (*matrix)[start][end].y=0;
    
    //Aendern der end-Kantenliste
    
    succ_of_edge=(*matrix)[end][start].x;
    pred_of_edge=(*matrix)[end][start].y;
      
    if(succ_of_edge==n+1){
      (*matrix)[end][end].y=pred_of_edge;
    }
    else{
      (*matrix)[end][succ_of_edge].y=pred_of_edge;
    };
      
    if(pred_of_edge==n+1){
      (*matrix)[end][end].x=succ_of_edge;
    }
    else{
      (*matrix)[end][pred_of_edge].x=succ_of_edge;
    };
      
    (*matrix)[end][start].x=0;
    (*matrix)[end][start].y=0;
    
    return true;
  };
  return false;
}

bool Lisa_Graph::exclude_arc(int start, int end)
{

  if((start>n)||(end>n)||(start==end)||(start==0)||(end==0)){return false;};
  
  //Test ob Bogen vorhanden
  
  int con=get_connection(start,end);
  
  if(con==ARC)
    {
      int succ_of_arc=(*matrix)[start][end].x;
      int pred_of_arc=(*matrix)[start][end].y;
      
      if(succ_of_arc==n+1){
	(*matrix)[start][0].y=pred_of_arc;
      }
      else{
	(*matrix)[start][succ_of_arc ].y=pred_of_arc;
      };
      
      if(pred_of_arc==n+1){
	(*matrix)[start][0].x=succ_of_arc;
      }
      else{
	(*matrix)[start][pred_of_arc].x=succ_of_arc;
      };
      
      (*matrix)[start][end].x=0;
      (*matrix)[start][end].y=0;
      
      //Aendern der predecessor-Liste
      
      succ_of_arc=(*matrix)[end][start].x;
      pred_of_arc=(*matrix)[end][start].y;
      
      if(succ_of_arc==-(n+1)){
	(*matrix)[0][end].y=-pred_of_arc;
      }
      else{
	(*matrix)[end][-succ_of_arc].y=pred_of_arc;
      };
      
      if(pred_of_arc==-(n+1)){
	(*matrix)[0][end].x=-succ_of_arc;
      }
      else{
	(*matrix)[end][-pred_of_arc ].x=succ_of_arc ;
      };
      
      (*matrix)[end][start].x=0;
      (*matrix)[end][start].y=0;
      
      return true;
    };

  if(con==EDGE){
    exclude_edge(start, end);
    insert_arc(end, start);
    return true;
    };
  
  return false;
}



void Lisa_Graph::show()
{
  //cout<<"SURFENooooooooooooooooooooooo"<<endl;
  for (int i=0; i<=n; i++)
    {
      for (int j=0; j<=n; j++)
	{
	  cout<<(*matrix)[i][j].x<<"=="<<(*matrix)[i][j].y<<"   ";
	};
      cout<<endl;
    };
} 
  
bool Lisa_Graph::valid()
{
  int el_of_list;
  int knot_list[n];
  int count=-1;

  for(int knot=1; knot<=n; knot++)
    {
      //Ueberpruefen der Nachfolgerliste

      //Hinrichtung

      el_of_list=(*matrix)[knot][0].x;

      if(el_of_list==0){return false;};
    
      if(el_of_list==n+1)
	{	  
	  if((*matrix)[knot][0].y!=n+1){cout<<" matrix "<<endl;
	    return false;};
	}
      else
	{
	  if((*matrix)[knot][0].y==n+1){return false;};
	  
	  while(el_of_list!=n+1)
	    {
	      count++;
	      knot_list[count]=el_of_list;

	      if(el_of_list==0) {return false;};
	      
	      if(get_connection(el_of_list,knot)!=CRA){return false;};
	      
	      el_of_list=(*matrix)[knot][el_of_list].x;
	    };
	  
	  //Rueckrichtung

	  el_of_list=(*matrix)[knot][0].y;

	  if(el_of_list==0) {return false;};
	  
	  if((*matrix)[knot][el_of_list].x!=n+1){return false;};
	  
	  while(el_of_list!=n+1)
	    {
	      if(count<0){return false;};
	      
	      if(el_of_list!=knot_list[count]){return false;};
	          
	      knot_list[count]=0;
	      
	      count--;
	      
	      el_of_list=(*matrix)[knot][el_of_list].y;

	      if(el_of_list==0) {return false;};
	    };
	  count++;
	  if(count!=0){return false;};                      
	  
	};
    
      count=-1;
      
      //Ueberpruefen der Vorgaengerliste

      //Hinrichtung

      el_of_list=(*matrix)[0][knot].x;

      if(el_of_list==0) {return false;};

      if(el_of_list==(n+1))
	{
	  if((*matrix)[0][knot].y!=(n+1)){return false;};
	}
      else
	{
	  if((*matrix)[0][knot].y==(n+1)){return false;};

	  while(el_of_list!=(n+1))
	    {
	      count++;
	      knot_list[count]=el_of_list;
	      
	      if(get_connection(el_of_list,knot)!=ARC){return false;};

	      el_of_list=-((*matrix)[knot][el_of_list].x);
	      
	      if(el_of_list==0) {return false;};	      
	    }; 
	  
	  //Rueckrichtung

	  el_of_list=(*matrix)[0][knot].y;

	  if(el_of_list==0) {return false;};
	  
	  if((*matrix)[knot][el_of_list].x!=-(n+1)){return false;};
  
	  while(el_of_list!=(n+1))
	    {
	   
	      if(count<0){return false;};
	      
	      if(el_of_list!=knot_list[count]){return false;};
	      
	      knot_list[count]=0;
	      
	      count--;
	      
	      el_of_list=-((*matrix)[knot][el_of_list].y);

	      if(el_of_list==0) {return false;};
	    };

	  count++;
	  if(count!=0){return false;}
	
	}
      count=-1;

      //Ueberpruefen der Kantenliste

      //Hinrichtung

      el_of_list=(*matrix)[knot][knot].x;

      if(el_of_list==0) {return false;};

      if(el_of_list==(n+1))
	{
	  if((*matrix)[knot][knot].y!=(n+1)){return false;};
	}
      else
	{
	  if((*matrix)[knot][knot].y==(n+1)){return false;};

	  while(el_of_list!=(n+1))
	    {
	      count++;
	      knot_list[count]=el_of_list;
	      
	      if(get_connection(el_of_list,knot)!=EDGE){return false;};

	      el_of_list=(*matrix)[knot][el_of_list].x;
	      
	      if(el_of_list==0) {return false;};      
	    };
	  
	  //Rueckrichtung

	  el_of_list=(*matrix)[knot][knot].y;

	  if(el_of_list==0) {return false;};
	  
	  if((*matrix)[knot][el_of_list].x!=n+1){return false;};
  
	  while(el_of_list!=n+1)
	    {
	   
	      if(count<0){return false;};
	      
	      if(el_of_list!=knot_list[count]){return false;};
	      
	      knot_list[count]=0;
	      
	      count--;
	      
	      el_of_list=(*matrix)[knot][el_of_list].y;

	      if(el_of_list==0) {return false;};
	    };
	  
	  count++;
	  if(count!=0){return false;}
	  
	}
      count=-1;
    }    
  return true;
}
  
Lisa_Matrix<int>* Lisa_Graph::get_ADJ_Matrix( Lisa_Matrix<int> *adj) const
{
  //Lisa_Matrix<int> *adj=new Lisa_Matrix<int>(n,n);

  for(int i=1; i<=n; i++)
    {
      for(int j=1; j<=n; j++)
	{
	  if(signum(i,j)==1) {(*adj)[i-1][j-1]=1;}
	  else{(*adj)[i-1][j-1]=0;};
	};
    };
  return adj;
}


void Lisa_Graph::init(int n_in) 
{
  n=n_in;
  end=n+1;

  if(matrix){delete matrix;};
  matrix= new Lisa_Matrix<Lisa_Pair>(n+1,n+1);

  if(succ_pred_pointer){delete succ_pred_pointer;};
  succ_pred_pointer=new Lisa_Vector<Lisa_Pair>(n);

  for (int i=0; i<=n; i++){
    for (int j=0; j<=n; j++){
      if((i==0||j==0)||(i==j)){
	(*matrix)[i][j].x=n+1;
	(*matrix)[i][j].y=n+1;
      }
      else{
	(*matrix)[i][j].x=0;
	(*matrix)[i][j].y=0;
      }
    }
  }
  
  for (int i=0; i<n; i++){
    (*succ_pred_pointer)[i].x=i+1;
    (*succ_pred_pointer)[i].y=i+1;
  }
  
}

bool Lisa_Graph::init_succ_pointer(int knot)
{
  if((knot<=0)||(knot>n)){return false;};

  //(*list_pointer)[knot-1].x=0;
  (*succ_pred_pointer)[knot-1].x=knot;
  return true;
}

const Lisa_Graph& Lisa_Graph::operator=(const Lisa_Graph& other) 
{
  //cout<<"operator=================="<<endl;

  if (n!=other.get_knots()){
    G_ExceptionList.lthrow("wrong format argument to graph.operator=");
    return *this;
  } 

  
  for (int i=0; i <end; i++){
    (*succ_pred_pointer)[i]=(*(other.succ_pred_pointer))[i];
    for (int j=0; j <end; j++){
      (*matrix)[i][j]=(*(other.matrix))[i][j];
    }
  }
  return *this;
}      



bool Lisa_Graph::init_pred_pointer(int knot)
{
  if((knot<=0)||(knot>n)){return false;};
  
  (*succ_pred_pointer)[knot-1].y=knot;
  return true;
}

int Lisa_Graph::get_next_successor(int knot)
{

  int old_knot=(*succ_pred_pointer)[knot-1].x;
  int next_knot=(*matrix)[knot][old_knot].x;

  if(next_knot==end){
    //START OF AN EDGE LIST
    if(old_knot==knot){
	next_knot=(*matrix)[knot][0].x;
    }
    //END OF AN EDGE LIST
    else{
      if(old_knot==(*matrix)[knot][knot].y){
	next_knot=(*matrix)[knot][0].x; 
      }
    }
  };

  if(next_knot==end){
    init_succ_pointer(knot);
  }
  else{
    (*succ_pred_pointer)[knot-1].x=next_knot;
  };

  return next_knot;
	
}

int Lisa_Graph::get_next_predeccessor(int knot)
{
  int old_knot=(*succ_pred_pointer)[knot-1].y;
  int next_knot=abs((*matrix)[knot][old_knot].x);
  if(next_knot==end){
    //START OF AN EDGE LIST
    if(old_knot==knot){
      next_knot=abs((*matrix)[0][knot].x);
    }
    //END OF AN EDGE LIST
    else{
      if(old_knot==(*matrix)[knot][knot].y){
	next_knot=abs((*matrix)[0][knot].x);
      }
    }
  };

  if(next_knot==end){
    init_pred_pointer(knot);
  }
  else{
    (*succ_pred_pointer)[knot-1].y=next_knot;
  };

  return next_knot;
	
}

int Lisa_Graph::get_next_edge(int knot)
{
  int next_knot=get_next_successor(knot);
  if((next_knot!=end)&&(get_connection(knot,next_knot)!=EDGE)){  
    next_knot=end;
  };  
  return next_knot;
}


bool Lisa_Graph::no_edges()
{
  for(int i=1; i<end; i++){
    if(((*matrix)[i][i].x)!=end){
      return false;
    }
  };

  return true;
}

int Lisa_Graph::number_of_succ(int knot)
{
  int succ_count=0;

  init_succ_pointer(knot);
  while(get_next_successor(knot)<end){
    succ_count++;
  };
  
  return succ_count;
}

int Lisa_Graph::number_of_pred(int knot)
{
  int pred_count=0;

  init_pred_pointer(knot);
  while(get_next_predeccessor(knot)<end){
    pred_count++;
  };
  
  return pred_count;
}


bool Lisa_Graph::remove_all_con(int knot)
{
  //EDGES
  int next=(*matrix)[knot][knot].x;

  while(next<end){
    exclude_edge(knot,next);
    next=(*matrix)[knot][knot].x;
  };

  //ARCS
  next=(*matrix)[knot][0].x;
   
  while(next<end){
    exclude_arc(knot,next);
    next=(*matrix)[knot][0].x;
  }; 

  //CRA
  next=(*matrix)[0][knot].x;
   
  while(next<end){
    exclude_arc(next,knot);
    next=(*matrix)[0][knot].x;
  }; 
  
  return true;
};



bool Lisa_Graph::topsort(Lisa_Vector<int>* knot_sequence)
{ 
  knot_sequence->fill(0);
  if(no_edges()){
    //graph copy
    Lisa_Graph* top=new Lisa_Graph(this);
    //stores the number of successors of knot i in i-1
    Lisa_Vector<int>* succ=new Lisa_Vector<int>(n);
    Lisa_Vector<int>* pred=new Lisa_Vector<int>(n);
    int source=0;
    int v=0;
    int next=1;
  
    while(v<end){
      //new predeccessors
      for(v=1; v<end; v++){
	if((*knot_sequence)[v-1]==0){
	  (*pred)[v-1]=top->number_of_pred(v);
	}
      };
      v=1;
      source=0;
      //find first source
      while((v<(end))&&(source==0)){
	if(((*knot_sequence)[v-1]==0)&&((*pred)[v-1]==0)){
	  source=1;
	}
	else{
	  v++;
	}
      };
      
      //remove all connections of the first founded source 
      if(v<end){
	top->remove_all_con(v);
	(*knot_sequence)[v-1]=next;
	next++;
      };
    };
    
    delete succ;
    delete pred;
    delete top;
    return (next==end);
  }
  else{
    return false;
  }
}




void Lisa_Graph::read_ADJ(Lisa_Matrix<int>* adj)
{
  if((adj->get_n())==n)
    {
      for(int i=0; i<n; i++)
	{
	  for(int j=0; j<n; j++)
	    {
	      //Test ob Verbindung existiert
	      if((i!=j)&&((*adj)[i][j]==1)) 
		{
		  //Test ob Kante oder Bogen
		  if((*adj)[j][i]==1)
		    {
		      insert_edge(i+1,j+1);
		    }
		  else
		    {
		      insert_arc(i+1,j+1);
		    }
		}
	    }
	}
    }
}

bool Lisa_Graph::transitive_hull(Lisa_Graph* target)
{
  Lisa_Vector<int> queue(end);
  Lisa_Vector<bool> done(end);
  int qs,qe,curr;
  int succ;
  
  for (int i=1; i<end; i++){
     queue.fill(0);  
     done.fill(0);
     qs = qe = 0;
     
     init_succ_pointer(i);
     succ=get_next_successor(i);
     
     //alle Nachfolger des Knotens
     
     while(succ<end){
       queue[qe++] = succ;
       done[succ] = 1;
       succ=get_next_successor(i);
     }  
     
     while(qs<qe){
       curr = queue[qs++];
       target->insert_arc(i,curr);
       init_succ_pointer(curr);
       succ=get_next_successor(curr);
       while(succ<end){
	 if(!done[succ]){
	   done[succ]=1;
	   queue[qe++]=succ;
	 }
	 succ=get_next_successor(curr);
       }
     }
  }
  return true;
}


void Lisa_Graph::write(ostream & strm) const 
{
  Lisa_Matrix<int>* out=new Lisa_Matrix<int> (n,n);
  out=get_ADJ_Matrix(out);

  strm << endl << "<GRAPH>" << endl;
  strm << "n= "<< n << endl;
  strm << "ADJ= " << *out<< endl; 
  strm << "</GRAPH>" <<endl;

  delete out;
}

void Lisa_Graph::read(istream & strm)
{
  if (strm==NULL)
    {
      G_ExceptionList.lthrow("(in Lisa_Graph::read) no valid stream",ANY_ERROR);
      return;
    };

  string S;
  int number_of_knots;
  Lisa_Matrix<int>* adj_Matrix=0;
  for (;;)
    {
      S=""; 
      strm >>S;
      if (S=="") 
	{ 
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Graph.read",END_OF_FILE);
	  return;
	} 
      if (S=="<GRAPH>") break;
    }

  // Weiterlesen:
  
  for (;;)
    {
      S=""; 
      strm >>S;  
      if (S=="") 
	{
	  G_ExceptionList.lthrow("Unexpected End of File in Lisa_Graph.read",END_OF_FILE);
	  return;
	};
      
      if (S=="</GRAPH>") break; 
      // Weiterlesen:
      
      if (S=="n=") 
	{  
	  strm >> number_of_knots; 
	  init(number_of_knots);
	  adj_Matrix=new Lisa_Matrix<int> (number_of_knots, number_of_knots);
	} 
      if (S=="ADJ=")
	{  
	  strm >> *adj_Matrix;
	  read_ADJ(adj_Matrix);
	  delete adj_Matrix;
	}
    }

}











