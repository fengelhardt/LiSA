#include "beam_orders.hpp"

//define modulo to always be a positive integer even if a is negative 
#define MOD(a,b) (((a)%(b))+(b))%(b)

// this is not working due to the wierd sorting of Lisa_Order (what is a bug in my eyes)
// it should now -marc-
static const double DOUBLE_INF = std::numeric_limits<double>::infinity();
//static const double DOUBLE_INF = 10000000.;

Lisa_Order* makeOrder(InsertionOrder iord, int& ops, Lisa_OsProblem *os_problem){
  if(iord == ect)
    return makeECT(ops,os_problem);
  if(iord == queens)
    return makeQueenSweep(ops,os_problem);
  Lisa_Order *order = new Lisa_Order(os_problem->n,os_problem->m);
  int l = os_problem->n * os_problem->m;
  //use system time for independent random numbers
  //this is not reversable
  long seed = (long)time(NULL);
  int max_mn = MAX(os_problem->n,os_problem->m);
  int min_mn = MIN(os_problem->n,os_problem->m);
  int squares = max_mn/min_mn;
  if( (max_mn % min_mn) != 0)
    squares++;

  //create the insertion order
  for (int i=0; i < os_problem->n; i++)
    for (int j=0; j < os_problem->m; j++) {
      //do nothing for non-existing operations
      if (! (*os_problem->sij)[i+1][j+1]) 
	{
	  //order sorts non-decreasing
	  //non-existing ops get maximum key : +infinity;
	  l--;
	  order->read_one_key( i, j, DOUBLE_INF);
	  continue;
	}
      if (iord == lpt) 
	order->read_one_key( i, j, -(*(os_problem->time))[i+1][j+1]);
      else if (iord == spt) 
	order->read_one_key( i, j, (*(os_problem->time))[i+1][j+1]);
      else if (iord == diagonal){
	order->read_one_key(i,j, 
			    (MOD(j-i,min_mn))*min_mn*min_mn*squares + 
			    i*min_mn + (j/min_mn)*min_mn*min_mn + j%min_mn);
      }
      else if (iord == line_by_line)
	order->read_one_key( i, j, j * os_problem->n + i);
      else 
	order->read_one_key( i, j, lisa_random(1, 24213, &seed));
    }
  order->sort();
  ops = l;
  return order;
}


//############## eaeliest completion time order #########################


Lisa_Order* makeECT(int& ops, Lisa_OsProblem *os_problem){
  int n = os_problem->n, m = os_problem->m;
  int l = n*m;
  Lisa_Order *order = new Lisa_Order(n,m);
  Lisa_Order *ECT = new Lisa_Order(n,m);
  double *compl_times = new double[l];
  //initialize order and times
  for(int i = 0; i< n; i++)
    for(int j = 0; j < m; j++)
      {
	if (! (*os_problem->sij)[i+1][j+1])
	  {
	    l--;
	    compl_times[i + j*m] = DOUBLE_INF;
	    order->read_one_key(i,j,DOUBLE_INF);
	  }
	else{
	  if(os_problem->ri)
	    compl_times[i + j*m] = (*os_problem->ri)[i+1] + (*os_problem->time)[i+1][j+1];
	  else
	    compl_times[i + j*m] = (*os_problem->time)[i+1][j+1];
	}
	ECT->read_one_key(i,j,compl_times[i + j*m]);
      }
  ECT->sort();
  for(int op = 0; op < l; op++)
    {
      //next operation found - insert in final order
      int i = ECT->row(0), j = ECT->col(0);
      order->read_one_key(i,j,op);
      //mark current op as processed
      compl_times[i + j*m] = -1.0;
      //update completion times on machine j
      for(int t = 0; t < n; t++)
	{
	  //put processed to the end
	  if(compl_times[t + j*m] < 0.0)
	    ECT->read_one_key(t,j,DOUBLE_INF);
	  //existing ops on machine j finish later now
	  else if ((*os_problem->sij)[t+1][j+1]){
	    compl_times[t + j*m] += (*os_problem->time)[i+1][j+1];
	    ECT->read_one_key(t,j,compl_times[t + j*m]);
	  }
	}
      //update completion times for task i
      for(int k = 0; k < m; k++)
	{
	  //put processed to the end
	  if(compl_times[i + k*m] < 0.0)
	    ECT->read_one_key(i,k,DOUBLE_INF);
	  else if ((*os_problem->sij)[i+1][k+1]){
	    compl_times[i + k*m] += (*os_problem->time)[i+1][j+1];
	    ECT->read_one_key(i,k,compl_times[i + k*m]);
	  }
	}
      ECT->sort();
    }
  delete[] compl_times;
  delete ECT;
  ops = l;
  order->sort();
  return order;
}



//############## Queen sweep order #########################

#define QUEEN_POS_EVEN(M,I) (((M)%6!=2)?((I<((M)/2))?(2*(I)+1):((2*(I))%(M))):((I<((M)/2))?(((M)/2+2*(I)-1)%(M)):((2*(I)+(M)/2+2)%(M))))

#define QUEEN_POS(M,I) (((M)%2==0)?QUEEN_POS_EVEN(M,I):((I)<((M)-1))?(QUEEN_POS_EVEN(((M)-1),I)):((M)-1))

Lisa_Order* makeQueenSweep(int& ops, Lisa_OsProblem *os_problem){
  int n = os_problem->n, m = os_problem->m;
  int l = n*m;
  Lisa_Order *order = new Lisa_Order(n,m);
  int pos = 0;
  //horizontal sweep
  if(m >= n){
    for(int sweep = 0; sweep < m; sweep++){
      for(int i = 0; i < n; i++){
	pos = (QUEEN_POS(n,i) + sweep) % m;
	if (! (*os_problem->sij)[i+1][pos+1])
	  {
	    l--;
	    order->read_one_key(i,pos,DOUBLE_INF);
	    continue;
	  }
	order->read_one_key(i, pos, i + sweep*n);
      }
    }
  }
  //vertical sweep
  else{
    //out << "off we go " << endl;
    int row = 0;
    for(int sweep = 0; sweep < n; sweep++){
      for(int i = 0; i < m; i++){
	row = (i + sweep)%n;
	pos = QUEEN_POS(m,i); //column
	if (! (*os_problem->sij)[row+1][pos+1])
	  {
	    l--;
	    order->read_one_key(row,pos,DOUBLE_INF);
	    continue;
	  }
	order->read_one_key(row, pos, i + sweep*m);
      }	
    }
  }
  ops = l;
  //out << "done ... sorting" << endl;
  order->sort();
  return order;
}
