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

		if(cls == '0'){
			Objects[id] = make_pair(time, make_pair(currX, currY));
		}
		else{
			pair<double, pair<int,int> > lastPos = Objects[id];
			double t0 = lastPos.first;
			int x0 = lastPos.second.first;
			int y0 = lastPos.second.second;
			tree->InsertTripSegment(id,x0,y0,currX,currY,t0,time);
			Objects[id] = make_pair(time,make_pair(currX,currY));
			
		}
	}

}

void readQueries(const char *inFilename, const char *outFilename, XTree* tree){
	vector<long>* resArray = new vector<long>;
	ifstream infile(inFilename);
	ofstream outfile(outFilename);
	string line;
	while(getline(infile,line)){
		istringstream iss(line);
		int x1, y1, x2, y2;
		double t1, t2;
		if(!(iss >> x1 >> y1 >> x2 >> y2 >> t1 >> t2)) break;
		tree->Search(x1, y1, x2, y2, t1, t2, resArray);

		sort(resArray->begin(),resArray->end());
		for(int i=0;i<resArray->size();i++) 
			outfile << (*resArray)[i] << " ";
		outfile << endl << endl;
	}
	outfile.close();
}

int main(){

	XTree kk = XTree();

	map<long, pair<int,int> > *Nodes = new map<long, pair<int,int> >();
	readNodes("data/Oldenburg/nodes.txt", Nodes);
	readEdges("data/Oldenburg/edges.txt", Nodes, &kk);

	readBrinkhoff("data/Oldenburg/trajectories.dat", &kk);
	kk.Build();

	readQueries("data/Oldenburg/queries.txt","data/X-OldenOut.txt", &kk);

	return 0;
}
