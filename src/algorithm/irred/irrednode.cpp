/**
 * @author  Marc Moerig
 * @version 2.3pre3
 */

#include "irrednode.hpp"

using namespace std;

//**************************************************************************
  
Lisa_IrredNode::Lisa_IrredNode(Lisa_Graph* plangraph_in, 
                               Lisa_Graph* compgraph_in, const int status_in){
                                 
  plangraph = new Lisa_Graph(*plangraph_in);
  compgraph = new Lisa_Graph(*compgraph_in);
  status = status_in;
}

//**************************************************************************

Lisa_IrredNode::~Lisa_IrredNode(){
  if (plangraph) delete plangraph;
  if (compgraph) delete compgraph;
}

//**************************************************************************

void Lisa_IrredNode::write(ostream& strm) const{
  strm << "<IrredNode>" << endl;
  strm << *plangraph << *compgraph;
  strm << "</IrredNode>" << endl;
}

//**************************************************************************

Lisa_IrredResult::Lisa_IrredResult(const int ins_method_in):
                                                  ins_method(ins_method_in){
  results = new Lisa_List<Lisa_IrredNode*>();
}

//**************************************************************************

Lisa_IrredResult::~Lisa_IrredResult(){
  if(!results->empty()){
    Lisa_IrredNode* curr;
    results->reset();
    do{
      curr = results->exclude();
      delete curr;
    }while(!results->empty());
  }
  
  delete results;
}

//**************************************************************************

void Lisa_IrredResult::insert(Lisa_Graph* plangraph_in,
                              Lisa_Graph* compgraph_in){

  static int lastsize = 0;	
  //static int countall=0;countall++;cout<<"inserted:"<<results->length()
  //                                     <<"/"<<countall<<endl;

  if(ins_method==NO_FILTER){
    Lisa_IrredNode* nd = new Lisa_IrredNode(plangraph_in,compgraph_in,Lisa_IrredNode::UNKNOWN);
    results->append(nd);
    return;
  }else if(ins_method==POTENTIALLY_IRREDUCIBLE){
    if (!results->empty()){
      results->reset();
      do{
	if (Lisa_GraphAlgorithms::smaller(results->get()->compgraph,compgraph_in)) return;
	if (Lisa_GraphAlgorithms::equal(results->get()->compgraph,compgraph_in)) return;
      }while(results->next());
    }
    
    Lisa_IrredNode* nd = new Lisa_IrredNode(plangraph_in,compgraph_in,Lisa_IrredNode::UNKNOWN);
    results->append(nd);
    
    
    if (results->length() > lastsize+1000){
      compare_all();
      delete_reducible();
      lastsize = results->length();
    }
  }else{ // ins_method==ALL
    if (!results->empty()){
      results->reset();
      do{
	if (Lisa_GraphAlgorithms::equal(results->get()->compgraph,compgraph_in)) return;
      }while(results->next());
    }
    
    Lisa_IrredNode* nd = new Lisa_IrredNode(plangraph_in,compgraph_in,Lisa_IrredNode::UNKNOWN);
    results->append(nd);
  }

}

//**************************************************************************

void Lisa_IrredResult::compare_all(){
  if(!results->empty()){
    
    Lisa_IrredNode* curr;
    unsigned int position;
    results->reset();
    
    do{
      curr = results->get();
      if (curr->status == Lisa_IrredNode::UNKNOWN){
	position = results->get_index();
	//cout << "checking: " << position << endl;

	results->reset();
	
	do{
	  if (Lisa_GraphAlgorithms::smaller(results->get()->compgraph,curr->compgraph)){
	    curr->status = Lisa_IrredNode::NOT_IRREDUCIBLE;
	  }
	}while(results->next());
	
	results->locate(position);
      }
    }while(results->next());
  }
}

//**************************************************************************

void Lisa_IrredResult::delete_reducible(){
  if(!results->empty()){

    results->reset();
    do{
      if(results->get()->status == Lisa_IrredNode::NOT_IRREDUCIBLE){
	delete results->exclude();
	results->previous();
      }
    }while(results->next());
  }
}

//**************************************************************************

void Lisa_IrredResult::write(ostream& strm)const{
  strm << "<IrredResult>" << endl;
  if(!results->empty()){
    Lisa_IrredNode* curr;
    results->reset();
    do{
      curr = results->get();
      strm << *curr;
    }while(results->next());
  }
  strm << "</IrredResult>" << endl;
}

//**************************************************************************

