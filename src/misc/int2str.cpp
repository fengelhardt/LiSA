/**
 * @author  Per Willenius
 * @version 2.3pre3
 */ 

#include <stdio.h>

#include "int2str.hpp"

using namespace std;

//**************************************************************************

bool inttst(const double wert){
  const long n = (long) wert;
  const double diff = wert - (double) n;
  
  if(diff<=0.001 && diff>=-0.001) return 1;
  else return 0;
}

//**************************************************************************

string ztos(const double wert){
  char umwd[100];
  
  if(inttst(wert)) sprintf(umwd,"%0.0f",wert);
  else sprintf(umwd,"%0.2f",wert);
 
  return (string) umwd;
}

//**************************************************************************

string ztofs(const double wert){
  char umwd[100];
  
  if(inttst(wert)) sprintf(umwd,"%4.0f",wert);
  else sprintf(umwd,"%4.2f",wert);
  
  return (string) umwd;
}

//**************************************************************************

int stoz(const string S){
    
  int i1=0,wert=0;
  if ((S=="empty")||(S==""))
    return 0;
  while (S[i1]!='\0'){
    wert*=10;
    wert+=(int) S[i1];
    wert-=48;
    i1++;
  } 
  
  return wert;
}

//**************************************************************************

