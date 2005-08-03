#include "Lisa_NativeDataHandler.hpp"

#include "../basics/graph.hpp"
#include "../lisa/lvalues.hpp"
#include "../lisa/ptype.hpp"
#include "../lisa/ctrlpara.hpp"
#include "../scheduling/schedule.hpp"
#include "../main/global.hpp"

bool Lisa_NativeDataHandler::write_to_stream(const Lisa_Schedule& S, std::string tag, std::ostream &out)
{
		if(tag == "plan" && (S.LR))
				{
						out << (*S.LR);
						return true;
				}
		else if(tag ==  "machine_sequences" && (S.NMO))
				{
						out <<  (*S.NMO);
						return true;
				}
		else if(tag ==  "job_sequences" && (S.NJO))
				{
						out <<  (*S.NJO);
						return true;
				}
		else if(tag == "completion_times" && (S.CIJ))
				{
						out <<  (*S.CIJ);
						return true;
				}
		return false;
}

bool Lisa_NativeDataHandler::write_to_stream(const Lisa_Values& V,   std::string tag, std::ostream &out)
{
		if(tag == "processing_times" && (V.PT))
				{
						out << *V.PT;
						return true;
				}
		else if(tag == "machine_order" && (V.MO))
				{
						out << *V.MO;
						return true;
				}
		else if(tag == "operation_set"  && (V.SIJ))
				{
						out << *V.SIJ;
						return true;
				}
		else if(tag == "release_times"  && (V.RD))
				{
						out << *V.RD;
						return true;
				}
		else if(tag ==  "due_dates" && (V.DD))
				{
						out << *V.DD;
						return true;
				}
		else if(tag ==  "weights" && (V.WI))
				{
						out << *V.WI;
						return true;
				}
			else if(tag ==  "weights2" && (V.WI2))
				{
						out << *V.WI2;
						return true;
				}
		else if(tag ==  "extra" && (V.EXTRA))
				{
						out << *V.EXTRA;
						return true;
				}
	return false;
}

bool Lisa_NativeDataHandler::write_to_stream(const Lisa_Graph& G,    std::string tag, std::ostream &out)
{
		Lisa_Matrix<int>* outM=new Lisa_Matrix<int> (G.get_vertices(),G.get_vertices());
		G.get_adjacency_matrix(outM);
		out << *outM;
		delete outM;
		return true;
}




bool Lisa_NativeDataHandler::read_from_stream(Lisa_Schedule& S,  std::string tag, std::istream &in)
{
		if(tag == "plan")
				{
						if (!S.LR) S.make_LR();
						in >> *S.LR;
						return true;
				}
		else if(tag == "machine_sequences")
				{
						if (!S.NMO) S.make_NMO();
						in >> *S.NMO;
						return true;
				}
			else if(tag == "job_sequences")
				{
						if (!S.NJO) S.make_NJO();
						in >> *S.NJO;
						return true;
				}	
		else if(tag == "completion_times")
				{
						if (!S.CIJ) S.make_CIJ();
						in >> *S.CIJ;
						return true;
				}
		return false;
}

bool Lisa_NativeDataHandler::read_from_stream(Lisa_Values& V,    std::string tag, std::istream &in)
{
		if(tag ==  "processing_times")
				{
						if (!V.PT) V.make_PT(); 
						in >> *V.PT;
						return true;
				}
		else if(tag == "machine_order")
								{
										if (!V.MO) V.make_MO(); 
										in >> *V.MO;
										return true;
								}
						else if(tag == "operation_set")
								{
										if (!V.SIJ) V.make_SIJ(); 
										in >> *V.SIJ;
										return true;
								}
						else if(tag == "release_times")
								{
										if (!V.RD) V.make_RD(); 
										in >> *V.RD;
										return true;
								}
						else if(tag == "due_dates")
								{
										if (!V.DD) V.make_DD(); 
										in >> *V.DD;
										return true;
								}
						else if(tag == "weights")
								{
										if (!V.WI) V.make_WI(); 
										in >> *V.WI;
										return true;
								}
						else if(tag == "weights_2")
								{
										if (!V.WI2) V.make_WI2(); 
										in >> *V.WI2;
										return true;
								}
						else if(tag == "extra")
								{
										if (!V.EXTRA) V.make_EXTRA(); 
										in >> *V.EXTRA;
										return true;
								}
		return false;
}

bool Lisa_NativeDataHandler::read_from_stream(Lisa_Graph& G,     std::string tag, std::istream &in)
{
		Lisa_Matrix<int>* adj_Matrix = new Lisa_Matrix<int> (G.get_vertices(),G.get_vertices());
		in >> adj_Matrix;
		G.set_adjacency_matrix(adj_Matrix);
		delete adj_Matrix;
		return true;
}
