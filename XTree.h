#ifndef XTREE_H
#define XTREE_H

//#define DEBUG 1

#ifdef DEBUG
#define D(x) x
#else 
#define D(x)
#endif

#include <iostream>
#include <vector>
#include "includes/rtree/RTree.h"
#include "includes/intervaltree/IntervalTree.h"

using namespace std;

class XTree
{
public:
	class SpatialLeaf;

private:
	RTree<SpatialLeaf*, int, 2, float> *SpatialLevel;

public:
	~XTree() {};
	
	XTree()
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

	class SpatialLeaf
	{
	private:
		bool orientation;
		bool built;
		IntervalTree<pair<long, bool>, double > *temporalTree;
		vector<Interval<pair<long, bool>, double> > intervals;
		Line* line;

	public:
		string nnn;

		SpatialLeaf();
		~SpatialLeaf() {};
		SpatialLeaf(Line* l, bool ori, string nn)
		{
			built = false;
			line = l;
			orientation = ori;
			nnn = nn;
		}
		Line* getLine()
		{
			return line;
		}
		IntervalTree<pair<long, bool>, double>* getTemporalTree()
		{
			return temporalTree;
		}
		void insertInterval(Interval<pair<long, bool>, double> *i)
		{
			intervals.push_back(*i);
		}
		void build()
		{
			if(!built)
				temporalTree = new IntervalTree<pair<long, bool>, double>(intervals);
			built = true;
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
		Line* line;
		Interval<pair<long, bool>, double>* timeInterval;

		InsertIntervalArgs();
		~InsertIntervalArgs() {};
		InsertIntervalArgs(Line* l, Interval<pair<long, bool>, double>* i){
			line = l;
			timeInterval = i;
		};
		long objectId()
		{
			return timeInterval->value.first;
		}
		bool orientation()
		{
			return timeInterval->value.second;
		}

	};

	static bool InsertTimeInterval(SpatialLeaf* id, void* arg)
	{
		D(cout << "    > TRYING InsertTimeInterval..." << endl);	

		InsertIntervalArgs* args = (InsertIntervalArgs*)arg;
		int objectId = args->objectId();
		Line* targetLine = args->line;
		Line* line = id->getLine();
		bool orientation = args->orientation();

		if(!targetLine->equals(*line)) return true;

		D(cout << "      > BEGIN InsertTimeInterval." << endl);	

		Interval<pair<long, bool>, double>* tmpInterval = args->timeInterval;

		D(string arrow = orientation? "<--" : "-->";)
		D(cout << "        > Inserting.. id: "<<objectId<<" interval["<<tmpInterval->start<<","<<tmpInterval->stop<<"] "<<arrow<<" into "<<id->nnn<< endl;)

		//id->getTemporalTree()->Insert(tmpInterval->start,tmpInterval->stop,make_pair(objectId,orientation));
		id->insertInterval(tmpInterval);
		
		D(cout << "      > END   InsertTimeInterval." << endl);
		return false;
	}

	void InsertTripSegment(long objectId, int x1, int y1, int x2, int y2, double entranceTime, double exitTime)
	{
		D(cout << "> BEGIN InsertTripSegment." << endl;)

		Line* tmpLine = new Line(min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2));
		bool orientation = !(x1<x2);
		Interval<pair<long, bool>, double> *tmpInterval = new Interval<pair<long, bool>, double>(entranceTime, exitTime, make_pair(objectId, orientation));
		InsertIntervalArgs* args = new InsertIntervalArgs(tmpLine, tmpInterval);

		SpatialLevel->Search(tmpLine->min, tmpLine->max, this->InsertTimeInterval, (void*)args);

		D(cout << "> END   InsertTripSegment." << endl;)
	}

	class searchArgs
	{
	public:
		Line* sWindow;
		Interval<bool>* tWindow;
		vector<long>* resultArray;

		searchArgs();
		~searchArgs() {};
		searchArgs(Line* l, Interval<bool>* i, vector<long>* r){
			sWindow = l;
			tWindow = i;
			resultArray = r;
		};

	};

	static bool auxSpatialSearch(SpatialLeaf* id, void* arg)
	{

		D(cout << "  > BEGIN auxSpatialSearch." << endl;)
		D(cout << "    " << id->nnn;)
		searchArgs* args = (searchArgs*)arg;
		Interval<bool>* temporalWindow = args->tWindow;
		vector<long>* resultArray = args->resultArray;

		D(cout << " -> interval = [" << temporalWindow->start << ", " << temporalWindow->stop << "]" << endl;)

		vector<Interval<pair<long, bool>, double> > auxRes;
		id->build();
		id->getTemporalTree()->findOverlapping(temporalWindow->start,temporalWindow->stop,auxRes);
		for(int i=0;i<auxRes.size();i++)
		{
			resultArray->push_back(auxRes[i].value.first);
		}

		D(cout << "  > END   auxSpatialSearch." << endl;)
		return true;
	}

	int Search(int x1, int y1, int x2, int y2, double entranceTime, double exitTime, vector<long>* resultArray)
	{
		D(cout << "> BEGIN Search." << endl;)

		resultArray->clear();
		Line* spatialWindow = new Line(min(x1,x2), min(y1,y2), max(x1,x2), max(y1,y2));
		Interval<bool>* temporalWindow = new Interval<bool>(entranceTime,exitTime,false);
		searchArgs* args = new searchArgs(spatialWindow, temporalWindow, resultArray);

		SpatialLevel->Search(spatialWindow->min, spatialWindow->max, auxSpatialSearch, (void*)args);

		D(cout << "> END   Search." << endl;)
		return resultArray->size();
	}

}; 

#endif //XTREE_H