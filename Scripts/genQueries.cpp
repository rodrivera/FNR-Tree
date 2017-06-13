#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

int main(int argc, char const *argv[])
{
	
	//int minX = 281, maxX = 2854;
	int minX = 1, maxX = 100;
	//int minY = 3935, maxY = 30851;
	int minY = 1, maxY = 100;

	double minT = 1, maxT = 100; 

	int dX = maxX-minX+1, dY = maxY-minY+1;
	double dT = maxT-minT+1;

	float percX = 0.5, percY = 0.5, percT = 0.5;

	srand(time(NULL));

	int 	qX1 = rand()%(int)round((1.0-percX)*dX) + minX, 	qX2 = qX1 + percX*dX;
	int 	qY1 = rand()%(int)round((1.0-percY)*dY) + minY, 	qY2 = qY1 + percY*dY;
	double 	qT1 = fmod(rand(),(1.0-percT)*dT) + minT, 	qT2 = qT1 + percT*dT;

	cout << qX1 << " " << qX2 << " " << qY1 << " " << qY2 << " " << qT1 << " " << qT2 << endl;

	return 0;
}