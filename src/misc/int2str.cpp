/*
 * ************** int2str.cpp ******************************
 * 
 * description:      functions to convert from int to string 
 *
 * owner:            Per Willenius
 *
 * date:             15.9.1998
 *
 * version:          V.1.0
 *
*/ 

#include <stdio.h>

#include "int2str.hpp"

//******************************************************************************

bool inttst(const double wert){
  const long int n = (long int) wert;
  const double diff=wert-(double)n;
  if(diff<=0.001 && diff>=-0.001) return 1;
  else return 0;
}

//******************************************************************************

string ztos(const double wert){
  char umwd[100];
  
  if(inttst(wert)) sprintf(umwd,"%0.0f",wert);
  else sprintf(umwd,"%0.2f",wert);
  
  string S = umwd;
  return S;
}

//******************************************************************************

string ztofs(const double wert){
  char umwd[100];
  
  if(inttst(wert)) sprintf(umwd,"%4.0f",wert);
  else sprintf(umwd,"%4.2f",wert);
  
  string S = umwd;
  return S;
}

//******************************************************************************

int stoz(const string S){
  int i1=0,wert=0;
  
  if ((S=="empty")||(S=="")) return 0;
  
  while (S[i1]!='\0'){
    wert*=10;
    wert+=(int) S[i1];
    wert-=48;
    i1++;
  }
  
  return wert;
}

//******************************************************************************






