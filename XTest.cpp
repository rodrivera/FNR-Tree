#include <iostream>
#include "XTree.h"

#include <vector>
#include <map>
#include <fstream>
#include <sstream>

using namespace std;

void readNodes(const char *filename, map<long, pair<int,int> > *m){
	ifstream infile(filename);
	string line;
	while(getline(infile,line)){
		istringstream iss(line);
		long id; int x, y;
		if(!(iss >> id >> x >> y)) break;
		m->insert(make_pair(id,make_pair(x,y)));
	}
}

void readEdges(const char *filename, map<long, pair<int,int> > *nodes, XTree* tree){
	ifstream infile(filename);
	string line;
	while(getline(infile,line)){
		istringstream iss(line);
		long id, A, B; string name;
		if(!(iss >> id >> A >> B)) break;

		pair<int, int> Acoord, Bcoord;
		Acoord = nodes->find(A)->second;
		Bcoord = nodes->find(B)->second;
		tree->InsertLine(Acoord.first,Acoord.second,Bcoord.first,Bcoord.second, name);
	}
}

void readBrinkhoff(const char *filename, XTree* tree){
	map<long, pair<double, pair<int,int> > >Objects; // id -> (time, (x,y) )

	ifstream infile(filename);
	string line;
	while(getline(infile,line)){
		istringstream iss(line);
		char cls; long id; double time, speed;
		int cID, currX, currY, nextX, nextY;
		if(!(iss >> cls >> id >> cID >> 
			time  >> currX >> currY >> 
			speed >> nextX >> nextY)) break;

		if(cls == '0'){ // NEW OBJECT
//			cout << "NEW " << id <<" "<< time  <<" ("<< currX <<", "<< currY <<") "<< endl;
			Objects[id] = make_pair(time, make_pair(currX, currY));
		}
		else{			// OLD or FINISH
//			cout << "OLD " << id <<" "<< time  <<" ("<< currX <<", "<< currY <<") "<< endl;
			pair<double, pair<int,int> > lastPos = Objects[id];
			double t0 = lastPos.first;
			int x0 = lastPos.second.first;
			int y0 = lastPos.second.second;
			tree->InsertTripSegment(id,x0,y0,currX,currY,t0,time);
			Objects[id] = make_pair(time,make_pair(currX,currY));
			
		}
	}

}

int main(){

	XTree kk = XTree();

	map<long, pair<int,int> > *Nodes = new map<long, pair<int,int> >();
	readNodes("Oldenburg/nodes.txt", Nodes);
	readEdges("Oldenburg/edges.txt", Nodes, &kk);

	readBrinkhoff("Oldenburg/node_oldenburg.dat", &kk);

	//                 	ID	(x1,y1)	(x2,y2)	[t1,t2]
	//kk.InsertTripSegment(1,	9690,7634,	9992,7896,	0,10);

	vector<long>* resArray = new vector<long>;
	kk.Search(9000, 7500, 10000, 8000, 5, 18, resArray);
	cout << "N hits = "<< resArray->size() << endl; 
	for(int i=0;i<resArray->size();i++) 
		cout << (*resArray)[i] << " "; 
	cout << endl << endl;

	resArray->clear();
	kk.Search(4500, 3500, 12000, 8400, 2, 19, resArray);
	cout << "N hits = "<< resArray->size() << endl; 
	for(int i=0;i<resArray->size();i++) 
		cout << (*resArray)[i] << " "; 
	cout << endl << endl;

	resArray->clear();
	kk.Search(300, 3500, 9900, 7400, 0, 8, resArray);
	cout << "N hits = "<< resArray->size() << endl; 
	for(int i=0;i<resArray->size();i++) 
		cout << (*resArray)[i] << " "; 
	cout << endl << endl;

	resArray->clear();
	kk.Search(0, 0, 20000, 20000, 0, 20, resArray);
	cout << "N hits = "<< resArray->size() << endl; 
	for(int i=0;i<resArray->size();i++) 
		cout << (*resArray)[i] << " "; 
	cout << endl << endl;

	return 0;
}