#include <iostream>
#include "includes/rtree/RTree.h"
#include "includes/intervaltree/IntervalTree.h"
using namespace std;

typedef int ValueType;

struct Rect
{
  Rect()  {}
  Rect(double a_minX, double a_maxX){
    min[0] = a_minX;
    max[0] = a_maxX;
  }
  double min[2];
  double max[2];
};

struct Rect rects[] ={
  Rect(40.925692162825015, 42.013711816306454),
  Rect(42.013711816306454, 42.38138742334501),
  Rect(42.38138742334501, 43.521932163546246),
  Rect(43.521932163546246, 44.09054342743819),
  Rect(44.09054342743819, 44.46090439287422)
};
int nrects = sizeof(rects) / sizeof(rects[0]);

struct Rect queries[] ={
	Rect(43.4765, 44.4665),
	Rect(4.99999,5)
};
int nqueries = sizeof(queries) / sizeof(queries[0]);

bool MySearchCallback(ValueType id, void* arg)
{
  cout << "F found " << id << "\n";
  return true;
}

int main(int argc, char const *argv[]){
	
	RTree<ValueType, double, 2, double> F;

	int i, nhits;
	vector<Interval<int, double> > intervals;

	for(i=0; i<nrects; i++){
		F.Insert(rects[i].min, rects[i].max, i);
		intervals.push_back(Interval<int, double>(rects[i].min[0], rects[i].max[0], i));
	}

	IntervalTree<int, double> X(intervals);

	for (int i = 0; i < nqueries; ++i)
	{
		vector<Interval<int, double> > auxRes;

	    X.findOverlapping(queries[i].min[0],queries[i].max[0],auxRes);
		nhits = F.Search(queries[i].min, queries[i].max, MySearchCallback, NULL);

		cout << "Search resulted in F: " << nhits << " and X: " << auxRes.size() <<" hits\n";

		for (int j = 0; j < auxRes.size(); ++j){
			cout << "X found " << auxRes[j].value << endl;
		}
		cout << "\n ------------------------------- \n" << endl;
	}

	return 0;
}