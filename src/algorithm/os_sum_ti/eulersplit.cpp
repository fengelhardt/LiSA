/**
 * @author  Mathias Zimmermann
 * @version 2.3final
 */

#include "eulersplit.hpp"

using namespace std;

    Eulersplit::Eulersplit(Lisa_Matrix<TIMETYP> *matrix) {

      adjMatrix = new Lisa_Matrix<TIMETYP>(*matrix);
      int n = matrix->get_n();
      int m = matrix->get_m();
      g1 = new Lisa_Matrix<TIMETYP>(n, m);
      g2 = new Lisa_Matrix<TIMETYP>(n, m);
      (*g1).fill(Lisa_Matching::REMOVED);
      (*g2).fill(Lisa_Matching::REMOVED);
      left = new Lisa_VectorOfLists<int>(n);
      right = new Lisa_VectorOfLists<int>(m);
      side = -1;
      insert = 1;
      for (int i = 0; i < n; i++) {
	for (int j = 0; j < m; j++) {
	  if ((*matrix)[i][j] > 0) {
	    (*left)[i].append(j);
	    (*right)[j].append(i);
	  }
	}
      }
    }

    Eulersplit::~Eulersplit() {

      delete adjMatrix;
      delete g1;
      delete g2;
      delete left;
      delete right;
    }

    void Eulersplit::splitGraph() {

      int run = 1;
      while(run > 0) {
	run = splitSubGraph();
      }
    }

    Lisa_Matrix<TIMETYP>* Eulersplit::get_G1() {

      return g1;
    }

    Lisa_Matrix<TIMETYP>* Eulersplit::get_G2() {

      return g2;
    }

    int Eulersplit::splitSubGraph() {

      int start = 0;
      while ((*left)[start].empty()) {
	start++;
	if (start > left->get_m() -1) {
	  return 0;
	}
      }
      int current = start;
      int from = current;
      int count = 0;
      while (current > -1) {
	from = current;
	current = searchNeighbour(current);
	if (current > -1) {
	  insertEdge(from, current);
	  count++;
	}
      }

      if ((start != from) && (count % 2 == 0)) {
	insert = (insert == 1 ? 2 : 1);
      }

      side = -1;

      return 1;
    }

    int Eulersplit::searchNeighbour(int v) {
      if (side == -1) {
	if(!(*left)[v].empty()) {
	  return getRightNeighbour(v);
	}
      }
      else {
	if(!(*right)[v].empty()) {
	  return getLeftNeighbour(v);
	}
      }
      return -1;
    }

    int Eulersplit::getRightNeighbour(int lv) {
      side = 1;
      (*right)[(*left)[lv].first()].search_first(lv);
      (*right)[(*left)[lv].first()].exclude();
      (*left)[lv].reset();
      return (*left)[lv].exclude();
    }

    int Eulersplit::getLeftNeighbour(int rv) {
      side = -1;
      (*left)[(*right)[rv].first()].search_first(rv);
      (*left)[(*right)[rv].first()].exclude();
      (*right)[rv].reset();
      return (*right)[rv].exclude();
    }

    void Eulersplit::insertEdge(int u, int v) {
      if (insert == 1) {
	insert = 2;
	if (side == 1) {
	  (*g1)[u][v] = (*adjMatrix)[u][v];
	}
	else {
	  (*g1)[v][u] = (*adjMatrix)[v][u];
	}
      }
      else {
	insert = 1;
	if (side == 1) {
	  (*g2)[u][v] = (*adjMatrix)[u][v];
	}
	else {
	  (*g2)[v][u] = (*adjMatrix)[v][u];
	}
      }
    }