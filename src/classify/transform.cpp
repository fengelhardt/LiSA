#include "reduction.hpp"
#include "transform.hpp"
#include "../lisa/lvalues.hpp"



// NOTE: The current implementation should just give a hint of how to do the transform.
//       The actual transform is missing.
//       This example demonstrates how to use one Atomic_Transform for both directions.
//       The Identity transform is invoked for any pair (s,t) for every graph g.
//       The interesting case is when the sizes of the two involved problem instances differ.

bool 
Identity_AtomicTransform::transform(int g ,int s,int t,
																																				TransformObjects& to,
																																				TransformDir d)
{
		//check for correct transform
		if(s != t)
				return false;
		if(g == M_NUMBER && s == M_VAL) //process number of machines
				{
						//problems has fixed aize ?
						if( to.transf_problem->get_property(M_NUMBER) != M_VAL ||
										to.target_problem->get_property(M_NUMBER) != M_VAL)
								return false;
						//perform forward transform
						if(d == FORWARD)
								{
										//check special cases of problem sizes
										if(to.transf_problem->m_no > to.target_problem->m_no) //cannot decrease
												return false;
										if(to.transf_problem->m_no == to.target_problem->m_no) //nothing to do
												return true;
										//check validity of target problem
										if(!(to.values-> valid) || (((int)to.transf_problem->m_no) != to.values->get_m()))
												return false;
										//okay here comes the transform
										/* +++++++++++++++++++++++++++++
											*
											*  put transform code here
											*
											* +++++++++++++++++++++++++++++ 
											*/
										return true;
								}
						//backwards transform
						//check special cases of problem sizes
						if(to.transf_problem->m_no < to.target_problem->m_no)//cannot increase
								return false;
						if(to.transf_problem->m_no == to.target_problem->m_no) //nothing to do
								return true;
						//ckeck validity
						if(!(to.values-> valid) || (((int)to.transf_problem->m_no) != to.values->get_m()))
								return false;
						/* +++++++++++++++++++++++++++++
							*
							*  put transform code here
							*
							* +++++++++++++++++++++++++++++ 
							*/
						return true;
				}
		if(g == JOB_NR && s == J_VAL) //process number of jobs
				{
						/* +++++++++++++++++++++++++++++
							*
							*  put transform code here
							*
							* +++++++++++++++++++++++++++++ 
							*/
						return false;
				}
		return true;
}

//implementation of SumCi -> SumWiCi

bool SumWiCi_AtomicTransform::transform(int g ,int s,int t,
																																								TransformObjects& to,
																																								TransformDir dir){
		//check signature
		if( (g != OBJECTIVE) || (s != SUM_CI) || (t != SUM_WICI))
				return false;
		switch(dir)
				{
				case FORWARD: //forward transform
						to.values->make_WI(); //create weights
						to.values->WI->fill(1.0); //unit fill
						to.transf_problem->set_property(g,t); //set problem signature
						break;
				case BACKWARDS:
						to.values->no_WI(); // destroy weights
						to.transf_problem->set_property(g,s); //set problem signature
						break;
				}
		//that's it we were successful
		return true;
}



