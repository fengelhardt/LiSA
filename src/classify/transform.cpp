#include "reduction.hpp"
#include "transform.hpp"
#include "../lisa/lvalues.hpp"



// NOTE: The current implementation should just give a hint of how to do the transform.
//       It is neither complete nor claimed to be correct.
bool 
Identity_AtomicTransform::transform(int g ,int s,int t,
																																				TransformObjects& to,
																																				TransformDir d)
{
		if(s != t)
				return false;
		if(g == M_NUMBER && s == M_VAL) //pumping machines
				{
						if( to.transf_problem->get_property(M_NUMBER) != M_VAL ||
										to.target_problem->get_property(M_NUMBER) != M_VAL)
								return false;
						//forward transform
						if(d == FORWARD)
								{
										if(to.transf_problem->m_no > to.target_problem->m_no)
												return false;
										if(to.transf_problem->m_no == to.target_problem->m_no)
												return true;
										if(!(to.values-> valid) || (((int)to.transf_problem->m_no) != to.values->get_m()))
												return false;
										// we have a real pump-up to perform
										//alter values, do not touch schedule
										//processing times
										unsigned m_new = to.target_problem->m_no;
										unsigned m_old =  to.values->get_m();
										unsigned n =  to.values->get_n();
										//i hope init does the the job ???? - MO may be destroyed ???
										to.values->init(n,m_new);
										if(to.values->PT)
												{
														for(unsigned i = 0; i < n; i++)
																for(unsigned j = m_old ; j < m_new; j++)
																		to.values->PT[i][j] = 0;
												}
										to.values->PT_to_SIJ();
										return true;
								}
						//backwards transform
						if(to.transf_problem->m_no < to.target_problem->m_no)
								return false;
						if(to.transf_problem->m_no == to.target_problem->m_no)
								return true;	
						if(!(to.values-> valid) || (((int)to.transf_problem->m_no) != to.values->get_m()))
								return false;
						// reinit should remove all machines from members
						to.values->init(to.values->get_n(),to.target_problem->m_no);
						// we have a real pump-down to perform
						// alter objects
						to.transf_problem->m_no = to.target_problem->m_no;
						// todo : alter schedule
						return true;
				}
		if(g == JOB_NR && s == J_VAL) //pumping jobs
				{
						//not implemented - the above is just an example of how to transform
						return false;
				}
		return true;
}




