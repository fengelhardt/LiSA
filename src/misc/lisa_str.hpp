/* LD */

/*
Author: Lars Dornheim
Description: string class
File: lisa_str.hpp
Date: 21.2.1999
*/


#ifndef lisa_str_hpp
#define lisa_str_hpp


#include <iostream.h>


bool IsWhiteSpace(const char TestChar);

void CopyMemory(char* const Destination,
                const char* const Source,
                const int Size);


class string

{
private:
     char* Data;
     int* TextLength;
     int* ReferenceCount;
     char* Text;

     void Allocate(const int Size);
     void AppendDigits(const int Number);
     void CheckLength(int& LengthToCheck,
                      const int StartPosition) const;
     bool CheckPosition(int& PositionToCheck,
                        const bool EndAllowed) const;
     void CloneData(void);
     void CopyAndAssignString(const string& AssignString);
     void Deallocate(char* DeallocateText) const;
     void TerminateText(void) const;

public:
     string(void);
     string(const string& InitString);
     string(const char InitChar);
     string(const char* const CString);
     string(const char* const CString,
            int StringLength);
     string(int InitNumber);

     ~string(void);

     void append(const string& AppendString);
     void assign(const string& AssignString);
     char at(int Position) const;
     char& at(int Position);
     const char* c_str(void) const;
     void clear(void);
     int compare(const string& CompareString) const;
     bool empty(void) const;
     void erase(int ErasePosition,
                int EraseLength);
     int find(const string& SearchString,
              int SearchStartPosition=0) const;
     void insert(int InsertPosition,
                 const string& InsertString);
     int length(void) const;
     void replace(int ReplacePosition,
                  int ReplaceLength,
                  const string& ReplaceString);
     string substr(int SubStringPosition,
                   int SubStringLength) const;
     void swap(string& SwapString);
     int toint(void) const;

     char operator [](int Position) const;
     char& operator [](int Position);
     string& operator =(const string& AssignString);
     string& operator +=(const string& AppendString);
};


string operator +(const string& String1,
                  const string& String2);

bool operator ==(const string& String1,
                 const string& String2);
bool operator !=(const string& String1,
                 const string& String2);
bool operator <(const string& String1,
                const string& String2);
bool operator >(const string& String1,
                const string& String2);
bool operator <=(const string& String1,
                 const string& String2);
bool operator >=(const string& String1,
                 const string& String2);

istream& operator >>(istream& InputStream,
                     string& InputString);
ostream& operator <<(ostream& OutputStream,
                     const string& OutputString);


#endif
