#include <iostream>
#include "XTree.h"

#include <set>
#include <map>
#include <fstream>
#include <sstream>

#include <chrono>
#include <iomanip>

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
	set<long>* resArray = new set<long>;
	ifstream infile(inFilename);
	ofstream outfile(outFilename);
	string line;
	int cont = 1;

	chrono::microseconds duration(0);

	while(getline(infile,line)){
		istringstream iss(line);
		int x1, y1, x2, y2;
		double t1, t2;
		if(!(iss >> x1 >> y1 >> x2 >> y2 >> t1 >> t2)) break;

		chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
		tree->Search(x1, y1, x2, y2, t1, t2, resArray);
		chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

		duration += chrono::duration_cast<chrono::microseconds>( end - start );	

		outfile << "Test #" << cont++ << endl;
		for (set<long>::iterator it=resArray->begin(); it!=resArray->end(); ++it){
			outfile << *it << " ";
		}
		outfile << endl << endl;
	}

	cout << "   > Queries time  = " << right << setw(10);
	cout << duration.count();
	cout << " microseconds" << endl;
	outfile.close();
}

int main(int argc, char const *argv[])
{

	if(argc != 6)
	{
		cout << "Usage: ./xtest [nodesFile] [edgesFile] [trajectoriesFile] [queriesFile] [outfile]" << endl;
		return -1;
	}

	const char* nodesFile 			= argv[1];
	const char* edgesFile 			= argv[2];
	const char* trajectoriesFile 	= argv[3];
	const char* queriesFile 		= argv[4];
	const char* outFile 			= argv[5];

	XTree kk = XTree();

	map<long, pair<int,int> > *Nodes = new map<long, pair<int,int> >();
	//chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
	readNodes(nodesFile, Nodes);
	readEdges(edgesFile, Nodes, &kk);

	chrono::high_resolution_clock::time_point start2 = chrono::high_resolution_clock::now();
	readBrinkhoff(trajectoriesFile, &kk);
	kk.Build();
	chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();

	cout << ">   X-Tree indicators:" << endl;
	cout << "   > MEMORY USAGE  = " << right << setw(10);
	cout << kk.size();
	cout << " Bytes" << endl;
	cout << "   > Building time = " << right << setw(10);
	cout << chrono::duration_cast<chrono::microseconds>( end - start2 ).count();
	cout << " microseconds" << endl;
	
/*	// SPATIAL SIZE VS TEMPORAL SIZE
	pair<size_t,size_t> dSize = kk.dSize();
	cout << "sSize: " << dSize.first << endl << "tSize: " << dSize.second << endl;
*/
/*	// INTERVAL TREE SIZES
	map<long,long> stats = kk.stats();
	map<long,long>::iterator it = stats.begin();
	while(it != stats.end()){
		cout << (*it).first << " " << (*it).second << endl;
		it++;
	}
*/

	readQueries(queriesFile, outFile, &kk);
	cout << endl;

	return 0;
}
