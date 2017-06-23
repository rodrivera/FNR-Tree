#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

int main(int argc, char const *argv[])
{

	if (argc < 5){
		cout << "Usage: ./gen num_queries percX percY percT" << endl;
		cout << "percX, percY and percT must be float numbers between 0 and 100 (inclusive)." << endl;
		return -1;
	}

	int minX = 281, maxX = 2854; int minY = 3935, maxY = 30851; 					// Oldenburg
	//int minX = 56977411, maxX = 58790663; int minY = 51682794, maxY = 53102345; 	// San Francisco

	double minT = 1.0, maxT = 100.0;

	int dX = maxX-minX, dY = maxY-minY;
	double dT = maxT-minT;

	int num = atoi(argv[1]);
	float percX = atof(argv[2])/100, percY = atof(argv[3])/100, percT = atof(argv[4])/100;

	if ( percX < 0 || percX > 100 || percY < 0 || percY > 100 || percT < 0 || percT > 100){
		cout << "percX, percY and percT must be float numbers between 0 and 100 (inclusive)." << endl;
		return -1;
	}

	srand(time(NULL));

	int rX = round((1.0-percX)*dX), rY = round((1.0-percY)*dY);
	double rT = (1.0-percT)*dT;

	for (int i = 0; i < num; ++i){
		int qX1 = (rX != 0)? rand()%rX + minX : minX;
		int qX2 = qX1 + percX*dX;
		int qY1 = (rY != 0)? rand()%rY + minY : minY;
		int qY2 = qY1 + percY*dY;
		double qT1 = (rT != 0)? fmod(rand(),rT) + minT : minT;
		double qT2 = qT1 + percT*dT;

		cout << qX1 << " " << qY1 << " " << qX2 << " " << qY2 << " " << qT1 << " " << qT2 << endl;
	}

	return 0;
}
