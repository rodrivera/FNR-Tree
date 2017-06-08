#ifndef FNRTREE_H
#define FNRTREE_H

//#define DEBUG 1

#ifdef DEBUG
#define D(x) x
#else 
#define D(x)
#endif

#include <iostream>
#include <vector>
#include "includes/rtree/RTree.h"

using namespace std;

class FNRTree
{
public:
	class SpatialLeaf;
	class TemporalLeaf;

private:
	RTree<SpatialLeaf*, int, 2, float> *SpatialLevel;

public:
	~FNRTree() {};
	
	FNRTree()
	{
		SpatialLevel = new RTree<SpatialLeaf*, int, 2, float>;
	}

	class Line
	{
	public:
		int min[2];
		int max[2];

		Line();
		~Line() {};
		Line(int a_minX, int a_minY, int a_maxX, int a_maxY)
		{
			min[0] = a_minX; min[1] = a_minY;
			max[0] = a_maxX; max[1] = a_maxY;
		}
		bool equals(const Line& other)
		{
			return min[0]==other.min[0] && min[1]==other.min[1] && max[0]==other.max[0] && max[0]==other.max[0];
		}
	};

	class Interval
	{
	public:
		double timeIn[1];
		double timeOut[1];

		Interval();
		~Interval() {};
		Interval(double tIn, double tOut)
		{
			timeIn[0] = tIn;
			timeOut[0] = tOut;
		}
	};

	class TemporalLeaf
	{
	private:
		long objectId;
		bool movementDirection;
		Interval* interval;

	public:
		TemporalLeaf();
		~TemporalLeaf() {};
		TemporalLeaf(Interval* in, long id, bool dir)
		{
			interval = in;
			objectId = id;
			movementDirection = dir;
		}
		long getId()
		{
			return objectId;
		}
		Interval* getInterval()
		{
			return interval;
		}
	};

	class SpatialLeaf
	{
	private:
		bool orientation;
		RTree<TemporalLeaf*, double, 1, float>* temporalTree;
		Line* line;

	public:
		string nnn;

		SpatialLeaf();
		~SpatialLeaf() {};
		SpatialLeaf(Line* l, bool ori, string nn)
		{
			line = l;
			orientation = ori;
			nnn = nn;
			temporalTree = new RTree<TemporalLeaf*, double, 1, float>();
		}
		Line* getLine()
		{
			return line;
		}
		bool getOrientation()
		{
			return orientation;
		}
		RTree<TemporalLeaf*, double, 1, float>* getTemporalTree()
		{
			return temporalTree;
		}		
	};

	int networkSize()
	{
		return SpatialLevel->Count();
	}

	void InsertLine(int x1, int y1, int x2, int y2, string name)
	{
		D(cout << "> BEGIN InsertLine:" << endl);

		bool ori = !( (x2 - x1)*(y2 - y1) >= 0 ); // 0 -> / , 1 -> \ .
		Line* tmpLine = new Line(min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2));

		D(cout << "    > Inserting.. (" << tmpLine->min[0] << "," << tmpLine->min[1] << ")->(" << tmpLine->max[0] << "," << tmpLine->max[1] << ")" << endl);

		SpatialLeaf* tmpLeaf = new SpatialLeaf(tmpLine,ori,name);
		SpatialLevel->Insert(tmpLine->min, tmpLine->max, tmpLeaf);

		D(cout << "> END   InsertLine." << endl);
	}

	class InsertIntervalArgs
	{
	public:
		long objectId;
		Line* line;
		Interval* timeInterval;
		bool orientation;

		InsertIntervalArgs();
		~InsertIntervalArgs() {};
		InsertIntervalArgs(long id, Line* l, Interval* i, bool o){
			objectId = id; 
			line = l;
			timeInterval = i;
			orientation = o;
		};

	};

	static bool InsertTimeInterval(SpatialLeaf* id, void* arg)
	{
		D(cout << "    > TRYING InsertTimeInterval..." << endl);	

		InsertIntervalArgs* args = (InsertIntervalArgs*)arg;
		long objectId = args->objectId;
		Line* targetLine = args->line;
		Line* line = id->getLine();
		bool orientation = args->orientation;

		if(!targetLine->equals(*line)) return true;

		D(cout << "      > BEGIN InsertTimeInterval." << endl);	

		Interval* tmpInterval = args->timeInterval;

		D(string arrow = orientation? "<--" : "-->";)
		D(cout << "        > Inserting.. id: "<<objectId<<" interval["<<tmpInterval->timeIn[0]<<","<<tmpInterval->timeOut[0]<<"] "<<arrow<<" into "<<id->nnn<< endl;)

		TemporalLeaf* tmpLeaf = new TemporalLeaf(tmpInterval, objectId,orientation);
		id->getTemporalTree()->Insert(tmpInterval->timeIn,tmpInterval->timeOut,tmpLeaf);
		
		D(cout << "      > END   InsertTimeInterval." << endl);
		return false;
	}

	void InsertTripSegment(long objectId, int x1, int y1, int x2, int y2, double entranceTime, double exitTime)
	{
		D(cout << "> BEGIN InsertTripSegment." << endl;)

		Line* tmpLine = new Line(min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2));
		bool orientation = !(x1<x2);
		Interval *tmpInterval = new Interval(entranceTime,exitTime);
		InsertIntervalArgs* args = new InsertIntervalArgs(objectId, tmpLine, tmpInterval, orientation);

		SpatialLevel->Search(tmpLine->min, tmpLine->max, this->InsertTimeInterval, (void*)args);

		D(cout << "> END   InsertTripSegment." << endl;)
	}

	class searchArgs
	{
	public:
		Line* sWindow;
		Interval* tWindow;
		vector<long>* resultArray;
		SpatialLeaf* lf;

		searchArgs();
		~searchArgs() {};
		searchArgs(Line* l, Interval* i, vector<long>* r){
			sWindow = l;
			tWindow = i;
			resultArray = r;
		};

	};

	static bool SegmentIntersectRectangle(
        int rectangleMinX,int rectangleMinY,
        int rectangleMaxX,int rectangleMaxY,
        int p1X,int p1Y,int p2X,int p2Y)
    {
        int minX = p1X, maxX = p2X;
        if (p1X > p2X)
        {
            minX = p2X;
            maxX = p1X;
        }
        if (maxX > rectangleMaxX)
        {
            maxX = rectangleMaxX;
        }
        if (minX < rectangleMinX)
        {
            minX = rectangleMinX;
        }
        if (minX > maxX)
        {
            return false;
        }
        int minY = p1Y, maxY = p2Y;
        double dx = p2X - p1X;
        if (dx != 0)
        {
            double a = (p2Y - p1Y)/dx;
            double b = p1Y - a*p1X;
            minY = a*minX + b;
            maxY = a*maxX + b;
        }
        if (minY > maxY)
        {
            int tmp = maxY;
            maxY = minY;
            minY = tmp;
        }

        if (maxY > rectangleMaxY)
        {
            maxY = rectangleMaxY;
        }

        if (minY < rectangleMinY)
        {
            minY = rectangleMinY;
        }

        if (minY > maxY)
        {
            return false;
        }
        return true;
    }

	static bool auxTemporalSearch(TemporalLeaf* id, void* arg)
	{
		D(cout << "    > BEGIN auxTemporalSearch." << endl;)		
		D(cout << "      Found: " << id->getId() << " -> [" << id->getInterval()->timeIn[0] << ", " << id->getInterval()->timeOut[0] <<"]"<< endl;)

		searchArgs* args = (searchArgs*)arg;
		Line* sBox = args->sWindow;
		Line* lSeg = args->lf->getLine();
		bool ori = args->lf->getOrientation();

		int p1X,p1Y, p2X,p2Y;
		if(ori)
		{
			p1X = lSeg->min[0]; p1Y = lSeg->max[1];
			p2X = lSeg->max[0]; p2Y = lSeg->min[1];
		}
		else
		{
			p1X = lSeg->min[0]; p1Y = lSeg->min[1];
			p2X = lSeg->max[0]; p2Y = lSeg->max[1];	
		}

		if(SegmentIntersectRectangle(sBox->min[0],sBox->min[1],sBox->max[0],sBox->max[1],p1X, p1Y, p2X, p2Y))
		{
			vector<long>* resultArray = args->resultArray;
			resultArray->push_back(id->getId());
		}
		D(cout << "    > END   auxTemporalSearch." << endl;)
		return true;
	}

	static bool auxSpatialSearch(SpatialLeaf* id, void* arg)
	{

		D(cout << "  > BEGIN auxSpatialSearch." << endl;)
		D(cout << "    " << id->nnn;)
		searchArgs* args = (searchArgs*)arg;
		Interval* temporalWindow = args->tWindow;
		args->lf = id;

		D(cout << " -> interval = [" << temporalWindow->timeIn[0] << ", " << temporalWindow->timeOut[0] << "]" << endl;)

		id->getTemporalTree()->Search(temporalWindow->timeIn, temporalWindow->timeOut, auxTemporalSearch, arg);
		D(cout << "  > END   auxSpatialSearch." << endl;)
		return true;
	}

	int Search(int x1, int y1, int x2, int y2, double entranceTime, double exitTime, vector<long>* resultArray)
	{
		D(cout << "> BEGIN Search." << endl;)

		resultArray->clear();
		Line* spatialWindow = new Line(min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2));
		Interval* temporalWindow = new Interval(entranceTime,exitTime);
		searchArgs* args = new searchArgs(spatialWindow, temporalWindow, resultArray);

		SpatialLevel->Search(spatialWindow->min, spatialWindow->max, auxSpatialSearch, (void*)args);

		D(cout << "> END   Search." << endl;)
		return resultArray->size();
	}

}; 

#endif //FNRTREE_H
