/*
 * ************** MO.h *******************************
 * 
 * Classes to represent MOs and JOs as successor list (in memory) and
 * as rank matrix (on file)
 * @version 2.3pre3
 *
 * 27.8.1998
*/

#ifndef _mo_jo_h 
#define _mo_jo_h

#include <iostream>

#include "../basics/matrix.hpp"

//**************************************************************************

const int MO_SOURCE=-1;
const int NO_SUCCESSOR=-1;

class Lisa_MO;
class Lisa_JO;

//**************************************************************************

/// basic class representing one order:
class Lisa_OrderWithoutRepetition
   {
     private: 
        Lisa_Vector<int> * succv;
        int start;
        int n;
        
        /// default constructor
        Lisa_OrderWithoutRepetition();
        
        /// friends
        friend class Lisa_MO;
        friend class Lisa_JO;
     public: 
        /// construct only with given size:
        Lisa_OrderWithoutRepetition(const int n);
        /// what is the succ of i?
        int succ(int i) const;
        /// insert i after j:
        void insert(const int i, const int j);
        /// read/write in memory:
        void write_rank(Lisa_Vector<int>*) const;
        /// returns FALSE if given vector is not a permutation
        int read_rank(Lisa_Vector<int>*);
        ~Lisa_OrderWithoutRepetition();
   }; 

//**************************************************************************

/// Classes Lisa_MO and Lisa_JO are not to be confused with each other!
class Lisa_MO
   {
     private:
        Lisa_OrderWithoutRepetition * orders;
        int n,m;
     public: 
        int get_n() const {return n;}
        int get_m() const {return m;}
        /// construct only with given size:
        Lisa_MO(const int n, const int m);
        /// what machine is the succ of (i,j) in MO of job i?
        int succ(int i, int j) const;
        /// insert machine j after k in MO of job i:
        void insert(const int i, const int j, const int k);
        /// read/write in memory:
        void write_rank(Lisa_Matrix<int>*) const;
        /// returns FALSE if given vector is not a permutation
        int read_rank(Lisa_Matrix<int>*);
        void write (std::ostream& = std::cout) const;
        void read (std::istream& = std::cin);
        ~Lisa_MO();
   };

//**************************************************************************

class Lisa_JO
   {
     private:
        Lisa_OrderWithoutRepetition * orders;
        int n,m;
     public: 
        int get_n() const {return n;}
        int get_m() const {return m;}
        /// construct only with given size:
        Lisa_JO(const int n, const int m);
        /// what job is the succ of (i,j) in JO of machine k?
        int succ(int i, int j) const;
        /// insert job i after k in JO of machine j:
        void insert(const int i, const int j, const int k);
        /// read/write in memory:
        void write_rank(Lisa_Matrix<int>*) const;
        /// returns FALSE if given vector is not a permutation
        int read_rank(Lisa_Matrix<int>*);
        void write (std::ostream& = std::cout) const;
        void read (std::istream& = std::cin);
        ~Lisa_JO();
   };

//**************************************************************************

/// analogous classes with repetition allowed simply use lists:
class Lisa_MoWithRepetition
   {
     private: 
        int n, m;
     public:
        Lisa_List<int> * orders;
        int get_n() const {return n;}
        int get_m() const {return m;}
	Lisa_MoWithRepetition(int n_in, int m_in)
	   {
             n=n_in; m=m_in; 
             orders=new Lisa_List<int>[n];
           }
        ~Lisa_MoWithRepetition() { delete [] orders; }
     //   void write_rank(Lisa_MatrixOfLists<int> *);
     //        void read_rank(Lisa_MatrixOfLists<int> *);
        void write(std::ostream& = std::cout) const;
        void read(std::istream& = std::cin);
   };    

//**************************************************************************

class Lisa_JoWithRepetition
   {
     private: 
        int n, m;
     public:
        Lisa_List<int> * orders;
        int get_n() const {return n;}
        int get_m() const {return m;}
	Lisa_JoWithRepetition(int n_in, int m_in)
	   {
             n=n_in; m=m_in; 
             orders=new Lisa_List<int>[m];
           }
        ~Lisa_JoWithRepetition(){ delete [] orders; }
     //  void write_rank(Lisa_MatrixOfLists<int> *);
     //   void read_rank(Lisa_MatrixOfLists<int> *);
        void write (std::ostream& = std::cout) const;
        void read (std::istream& = std::cin);
   };   

//**************************************************************************

inline std::ostream& operator << (std::ostream& strm, const Lisa_MO & v){
     v.write(strm);
     return strm;
}

//**************************************************************************

inline std::istream& operator >> (std::istream& strm, Lisa_MO & v){
     v.read(strm);
     return strm;
}

//**************************************************************************

inline std::ostream& operator << (std::ostream& strm, const Lisa_JO& l){
     l.write(strm);
     return strm;
}

//**************************************************************************

inline std::istream& operator >> (std::istream& strm, Lisa_JO& l){
     l.read(strm);
     return strm;
}

#endif

//**************************************************************************

