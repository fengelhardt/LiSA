/*
 * ************** int2str.C ******************************
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

//  ********************** System Includes ***************************
#include <stdio.h>


// ************************ Local Includes ********************
#include "int2str.hpp"


// *********************** functions *************************


bool inttst(double wert){
  double diff;
  long int n;
  n=(long int) wert;
  diff=wert-(double)n;
  if(diff<=0.001 && diff>=-0.001) return 1;
  else return 0;
}

string ztos(double wert){
  string S;
  char umwd[100];
  if(inttst(wert)) sprintf(umwd,"%0.0f",wert);
  else sprintf(umwd,"%0.2f",wert);
  S=umwd;
  return S;
}

string ztofs(double wert){
  string S;
  char umwd[100];
  if(inttst(wert)) sprintf(umwd,"%4.0f",wert);
  else sprintf(umwd,"%4.2f",wert);
  S=umwd;
  return S;
}

int stoz(string S){
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








