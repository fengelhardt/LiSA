/* LD */

/*
Author: Lars Dornheim
Description: string class
File: lisa_str.cpp
Date: 21.2.1999
*/


#include "lisa_str.hpp"


bool IsWhiteSpace(const char TestChar)

{
return (TestChar=='\t') || (TestChar=='\n') || (TestChar=='\v') || (TestChar=='\f') || (TestChar=='\r') || (TestChar==' ');
}


void CopyMemory(char* const Destination,
                const char* const Source,
                const int Size)

{
for (int I=0; I<=Size-1; ++I)
     Destination[I]=Source[I];
}


void string::Allocate(const int Size)

{
Data=new char[2*sizeof(int)+Size+1];
ReferenceCount=(int*)&Data[0];
*ReferenceCount=1;
TextLength=(int*)&Data[sizeof(int)];
*TextLength=Size;
Text=&Data[2*sizeof(int)];
TerminateText();
}


void string::AppendDigits(const int Number)

{
if (Number!=0)
     {
     int DigitValue=Number%10;
     AppendDigits((Number-DigitValue)/10);
     append((char)((int)'0'+DigitValue));
     }
}


void string::CheckLength(int& LengthToCheck,
                         const int StartPosition) const

{
if (StartPosition+LengthToCheck>*TextLength)
     LengthToCheck=*TextLength-StartPosition;
}


bool string::CheckPosition(int& PositionToCheck,
                           const bool EndAllowed) const

{
bool Result=true;
if (PositionToCheck<0)
     PositionToCheck=0;
if (EndAllowed)
     {
     if (PositionToCheck>*TextLength)
          PositionToCheck=*TextLength;
     }
else
     {
     if (PositionToCheck>*TextLength-1)
          PositionToCheck=*TextLength-1;
     if (*TextLength==0)
          Result=false;
     }
return Result;
}


void string::CloneData(void)

{
if (*ReferenceCount>1)
     {
     --*ReferenceCount;
     char* OldText=Text;
     Allocate(*TextLength);
     CopyMemory(&Text[0], &OldText[0], *TextLength);
     }
}


void string::CopyAndAssignString(const string& AssignString)

{
Data=AssignString.Data;
TextLength=AssignString.TextLength;
ReferenceCount=AssignString.ReferenceCount;
Text=AssignString.Text;
++*ReferenceCount;
}


void string::Deallocate(char* DeallocateText) const

{
int* DeallocateReferenceCount=(int*)&DeallocateText[-2*sizeof(int)];
--*DeallocateReferenceCount;
if (*DeallocateReferenceCount==0)
     {
     char* DeallocateData=&DeallocateText[-2*sizeof(int)];
     delete[] DeallocateData;
     }
}


void string::TerminateText(void) const

{
char* Terminator=&Data[2*sizeof(int)+*TextLength];
*Terminator='\0';
}


string::string(void)

{
Allocate(0);
}


string::string(const string& InitString)

{
CopyAndAssignString(InitString);
}


string::string(const char InitChar)

{
Allocate(1);
Text[0]=InitChar;
}


string::string(const char* const CString)

{
int StringLength=0;
if (CString!=NULL)
     while (CString[StringLength]!=0)
          ++StringLength;
Allocate(StringLength);
CopyMemory(&Text[0], &CString[0], StringLength);
}


string::string(const char* const CString,
               int StringLength)

{
if (StringLength<0)
     StringLength=0;
Allocate(StringLength);
CopyMemory(&Text[0], &CString[0], StringLength);
}


string::ztos(int InitNumber)

{
Allocate(0);
if (InitNumber==0)
     append('0');
else
     {
     if (InitNumber<0)
          {
          append('-');
          InitNumber=-InitNumber;
          }
     AppendDigits(InitNumber);
     }
}


string::~string(void)

{
Deallocate(Text);
}


void string::append(const string& AppendString)

{
int AppendStringLength=AppendString.length();
if (AppendStringLength>0)
     {
     char* OldText=Text;
     Allocate(*TextLength+AppendStringLength);
     CopyMemory(&Text[0], &OldText[0], *TextLength-AppendStringLength);
     CopyMemory(&Text[*TextLength-AppendStringLength], &AppendString.c_str()[0], AppendStringLength);
     Deallocate(OldText);
     }
}


void string::assign(const string& AssignString)

{
if (&AssignString!=this)
     {
     Deallocate(Text);
     CopyAndAssignString(AssignString);
     }
}


char string::at(int Position) const

{
char Result;
if (CheckPosition(Position, false))
     Result=Text[Position];
else
     Result='\0';
return Result;
}


char& string::at(int Position)

{
if (! CheckPosition(Position, false))
     Position=0;
CloneData();
return Text[Position];
}


const char* string::c_str(void) const

{
TerminateText();
return Text;
}


void string::clear(void)

{
if (*TextLength>0)
     {
     Deallocate(Text);
     Allocate(0);
     }
}


int string::compare(const string& CompareString) const

{
int I=0;
while ((I<*TextLength) && (I<CompareString.length()) && (Text[I]==CompareString.c_str()[I]))
     ++I;
int Result;
if ((I==*TextLength) || (I==CompareString.length()))
     if ((I==*TextLength) && (I==CompareString.length()))
          Result=0;
     else
          if (I==*TextLength)
               Result=-1;
          else
               Result=1;
else
     if (Text[I]<CompareString.c_str()[I])
          Result=-1;
     else
          Result=1;
return Result;
}


bool string::empty(void) const

{
return *TextLength==0;
}


void string::erase(int ErasePosition,
                   int EraseLength)

{
CheckPosition(ErasePosition, true);
CheckLength(EraseLength, ErasePosition);
if (EraseLength>0)
     {
     char* OldText=Text;
     Allocate(*TextLength-EraseLength);
     CopyMemory(&Text[0], &OldText[0], ErasePosition);
     CopyMemory(&Text[ErasePosition], &OldText[ErasePosition+EraseLength], *TextLength-ErasePosition);
     Deallocate(OldText);
     }
}


int string::find(const string& SearchString,
                 int StartPosition) const

{
CheckPosition(StartPosition, true);
int Result=-1;
int I=StartPosition;
while ((Result==-1) && (I<=*TextLength-SearchString.length()))
     {
     int J=0;
     while ((J<SearchString.length()) && (Text[I+J]==SearchString.c_str()[J]))
          ++J;
     if (J==SearchString.length())
          Result=I;
     else
          ++I;
     }
return Result;
}


void string::insert(int InsertPosition,
                    const string& InsertString)

{
if (&InsertString!=this)
     {
     CheckPosition(InsertPosition, true);
     if (InsertString.length()>0)
          {
          char* OldText=Text;
          Allocate(*TextLength+InsertString.length());
          CopyMemory(&Text[0], &OldText[0], InsertPosition);
          CopyMemory(&Text[InsertPosition], &InsertString.c_str()[0], InsertString.length());
          CopyMemory(&Text[InsertPosition+InsertString.length()], &OldText[InsertPosition], *TextLength-(InsertPosition+InsertString.length()));
          Deallocate(OldText);
          }
     }
}


int string::length(void) const

{
return *TextLength;
}


void string::replace(int ReplacePosition,
                     int ReplaceLength,
                     const string& ReplaceString)

{
if (&ReplaceString!=this)
     {
     CheckPosition(ReplacePosition, true);
     CheckLength(ReplaceLength, ReplacePosition);
     if ((ReplaceLength>0) || (ReplaceString.length()>0))
          {
          char* OldText=Text;
          Allocate(*TextLength-ReplaceLength+ReplaceString.length());
          CopyMemory(&Text[0], &OldText[0], ReplacePosition);
          CopyMemory(&Text[ReplacePosition], &ReplaceString.c_str()[0], ReplaceString.length());
          CopyMemory(&Text[ReplacePosition+ReplaceString.length()], &OldText[ReplacePosition+ReplaceLength], *TextLength-(ReplacePosition+ReplaceString.length()));
          Deallocate(OldText);
          }
     }
}


string string::substr(int SubStringPosition,
                      int SubStringLength) const

{
CheckPosition(SubStringPosition, true);
CheckLength(SubStringLength, SubStringPosition);
return string(&Text[SubStringPosition], SubStringLength);
}


void string::swap(string& SwapString)

{
char* TempData=Data;
int* TempTextLength=TextLength;
int* TempReferenceCount=ReferenceCount;
char* TempText=Text;
Data=SwapString.Data;
TextLength=SwapString.TextLength;
ReferenceCount=SwapString.ReferenceCount;
Text=SwapString.Text;
SwapString.Data=TempData;
SwapString.TextLength=TempTextLength;
SwapString.ReferenceCount=TempReferenceCount;
SwapString.Text=TempText;
}


int string::toint(void) const

{
bool ParseError=*TextLength==0;
bool Positive=true;
int StartPosition=0;
if (! ParseError)
     if ((Text[StartPosition]=='+') || (Text[StartPosition]=='-'))
          {
          if (Text[StartPosition]=='-')
               Positive=false;
          ++StartPosition;
          if (*TextLength<=StartPosition)
               ParseError=true;
          }
int Result=0;
if (! ParseError)
     {
     int Base=1;
     int I=*TextLength-1;
     while ((I>=StartPosition) && (! ParseError))
          {
          int Digit=(int)Text[I]-(int)'0';
          if ((Digit>=0) && (Digit<=9))
               Result+=Digit*Base;
          else
               ParseError=true;
          --I;
          Base*=10;
          }
     }
if (ParseError)
     Result=0;
else
     if (! Positive)
          Result*=-1;
return Result;
}


char string::operator [](int Position) const

{
return at(Position);
}


char& string::operator [](int Position)

{
return at(Position);
}


string& string::operator =(const string& AssignString)

{
assign(AssignString);
return *this;
}


string& string::operator +=(const string& AppendString)

{
append(AppendString);
return *this;
}


string operator +(const string& String1,
                  const string& String2)

{
string Result(String1);
Result.append(String2);
return Result;
}


bool operator ==(const string& String1,
                 const string& String2)

{
return String1.compare(String2)==0;
}


bool operator !=(const string& String1,
                 const string& String2)

{
return String1.compare(String2)!=0;
}


bool operator <(const string& String1,
                const string& String2)

{
return String1.compare(String2)==-1;
}


bool operator >(const string& String1,
                const string& String2)

{
return String1.compare(String2)==1;
}


bool operator <=(const string& String1,
                 const string& String2)

{
return String1.compare(String2)!=1;
}


bool operator >=(const string& String1,
                 const string& String2)

{
return String1.compare(String2)!=-1;
}


istream& operator >>(istream& InputStream,
                     string& InputString)

{
int InputStreamWidth=InputStream.width(0);
if (InputStream.ipfx0())
     {
     streambuf *InputStreamBuffer=InputStream.rdbuf();
     InputString.clear();
     while (1)
          {
          int InputChar=InputStreamBuffer->sbumpc();
          if (InputChar==EOF)
               {
               InputStream.setstate(ios::eofbit);
               break;
               }
          else
               if (IsWhiteSpace((char)InputChar))
                    {
                    InputStreamBuffer->sungetc();
                    break;
                    }
          InputString.append((char)InputChar);
          --InputStreamWidth;
          if (InputStreamWidth==1)
               break;
          }
     }
InputStream.isfx();
if (InputString.length()==0)
     InputStream.setstate(ios::failbit);
return InputStream;
}


ostream& operator <<(ostream& OutputStream,
                     const string& OutputString)

{
return OutputStream.write(OutputString.c_str(), OutputString.length());
}
