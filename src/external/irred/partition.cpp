/*
 * ************** partition.cpp *******************************
 *
 * Owner: Marc Moerig
 *
 * 25.02.01
 * last changed 26.02.01
 */

#include "partition.hpp"
  
Lisa_Partition::Lisa_Partition(const int in){
  part = new Lisa_Vector<int>(in);
  pstart = part->get_first();
  pstop = part->get_last();
  max = in-1;
  int i = 0;
  for (int* ppart=pstart;ppart!=pstop;){
    *(ppart++)=i++;
  }
}

//**************************************************************
  
Lisa_Partition::~Lisa_Partition(){
  delete part;
}

//**************************************************************
  
void Lisa_Partition::join(int a,int b){
  a = (*part)[a];
  b = (*part)[b];
  
  if(a!=b){
    for (int* ppart = pstart;ppart!=pstop;ppart++){
      if(*ppart==a){ 
	*ppart = b;
      }
      if(*ppart==max){
	*ppart = a;
      }
    }
    max--;
  }
}

//**************************************************************

void Lisa_Partition::get_partition(const int num,Lisa_Vector<int>* vec){
  int count = 1;
  int i = 0;
  for (int* ppart=pstart;ppart!=pstop;ppart++){
    if(*ppart==num){
      (*vec)[count++] = i;
    }
    i++;
  }
  (*vec)[0] = count-1;
}

//**************************************************************

int Lisa_Partition::get_num_of_partitions(){
  return max+1;
}

//**************************************************************

int Lisa_Partition::get_partition_size(const int num){
  int count = 0;
  for (int* ppart=pstart;ppart!=pstop;ppart++)
    if (*ppart==num) count++;
  return count;
}

//**************************************************************
  
void Lisa_Partition::write(ostream& strm = cout) const{
  strm << "<PARTITION>" << endl;
  strm << *part;
  strm << "</PARTITION>" << endl;
}
