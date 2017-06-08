#include <iostream>
#include "FNRTree.h"
#include "XTree.h"

#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

#define runo 0,0,10,10
#define rdos 10,10,20,0

#define t1 2,6,4,8,0,10
#define t2 0,9,6,10,0,10

#define t3 12,6,14,8,0,10
#define t4 13,9,16,10,0,10

int main(){
	FNRTree ftree = FNRTree();
	XTree xtree = XTree();

	ftree.InsertLine(runo, "Ruta 1");
	ftree.InsertTripSegment(1,runo,0,5);
	xtree.InsertLine(runo, "Ruta 1");
	xtree.InsertTripSegment(1,runo,0,5);
	cout << "" << endl;
	ftree.InsertLine(rdos, "Ruta 2");
	ftree.InsertTripSegment(2,rdos,5,10);
	xtree.InsertLine(rdos, "Ruta 2");
	xtree.InsertTripSegment(2,rdos,5,10);

	xtree.Build();

	vector<long>* fresArray = new vector<long>;
	vector<long>* xresArray = new vector<long>;
	
	ftree.Search(t1,fresArray); xtree.Search(t1,xresArray);
	cout << fresArray->size() << " " << xresArray->size() << endl;

	fresArray->clear(); xresArray->clear();
	ftree.Search(t2,fresArray); xtree.Search(t2,xresArray);
	cout << fresArray->size() << " " << xresArray->size() << endl;

	fresArray->clear(); xresArray->clear();
	ftree.Search(t3,fresArray); xtree.Search(t3,xresArray);
	cout << fresArray->size() << " " << xresArray->size() << endl;

	fresArray->clear(); xresArray->clear();
	ftree.Search(t4,fresArray); xtree.Search(t4,xresArray);
	cout << fresArray->size() << " " << xresArray->size() << endl;

}