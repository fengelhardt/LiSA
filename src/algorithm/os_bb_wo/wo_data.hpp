/* ************************************************************************* */
/*                           Filename: Data.h                                */
/*                                                                           */
/*    Export: Globale Konstanten und Datenstrukturen                         */
/* ************************************************************************* */

#ifndef _DATA_H
#define _DATA_H

#define MaxNumOfMachines     30
#define MaxNumOfJobs         30
#define MaxNumOfOperations   MaxNumOfMachines*MaxNumOfJobs
#define MaxOpProM_And_J      30
#define NIL                   0
#define MaxInt            32000

typedef int boolean;        


struct List  {
                int          number;
                struct List  *next;
             };


struct Operation  {
		     int process_time,
			 machine_nr,
			 job_nr;
                  };


struct ArcList  {
                  struct List  *pred[MaxNumOfOperations+1],
                               *succ[MaxNumOfOperations+1];
                };


struct BlockList  {
                    struct List       *elements;
                    struct BlockList  *next;
                    int               last_of_block;
		    char              kind;
                  };


struct BranchList  {
                     int                branch_op;
		     char               before_or_after;
		     char               kind_of_block;
                     struct BranchList  *next;
                   };

struct NodeType  {
		    int 	      num_of_disj_succ[MaxNumOfOperations+1],
				      num_of_conj_succ[MaxNumOfOperations+1],
				      num_of_disj_pred[MaxNumOfOperations+1],
				      num_of_conj_pred[MaxNumOfOperations+1];
		    int		      lower_bound;
		    struct BlockList  *blocks;
                    struct BranchList  *order;
                 };


struct StackElement  {
                       struct NodeType       *node;
                       struct StackElement   *next;
                     };


#endif
