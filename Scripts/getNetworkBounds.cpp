#include <bits/stdc++.h>
using namespace std;


int main(int argc, char const *argv[])
{
	ifstream infile(argv[1]);
	string line;
	int minX = INT_MAX, maxX = INT_MIN, minY = INT_MAX, maxY = INT_MIN;
	while(getline(infile,line)){
		istringstream iss(line);
		long id; int x, y;
		if(!(iss >> id >> x >> y)) break;
		maxX = max(maxX,x);
		minX = min(minX,x);

		maxY = max(maxY,y);
		minY = min(minY,y);
	}
	cout << "Bound for: " << argv[1] << endl;
	cout << "X = [ " << minX << ", " << maxX << "]" << endl;
	cout << "Y = [ " << minY << ", " << maxY << "]" << endl;

	return 0;
}