//
// Test.cpp
//
// This is a direct port of the C version of the RTree test program.
//

#include <iostream>
#include "RTree.h"
#include <vector>

using namespace std;

typedef string ValueType;

struct Rect
{
  Rect()  {}

  Rect(int a_minX, int a_minY, int a_maxX, int a_maxY)
  {
    min[0] = a_minX;
    min[1] = a_minY;

    max[0] = a_maxX;
    max[1] = a_maxY;
  }


  int min[2];
  int max[2];
};

struct Rect rects[] =
{
  Rect(0, 0, 2, 2), // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
  Rect(5, 5, 7, 7),
  Rect(8, 5, 9, 6),
  Rect(7, 1, 9, 2),
  Rect(0, 0, 10, 10)
};

int nrects = sizeof(rects) / sizeof(rects[0]);

//Rect search_rect(6, 4, 10, 6); // search will find above rects that this one overlaps
Rect search_rect(0 , 9, 5, 10); // search will find above rects that this one overlaps

bool MySearchCallback(ValueType id, void* arg)
{
  cout << "Hit data rect " << id << "\n";
  return true; // keep going
}


int main()
{
  typedef RTree<ValueType, int, 2, float> MyTree;
  MyTree tree;

  int i, nhits;
  cout << "nrects = " << nrects << "\n";

  vector<string> ss(5);
  ss[0] = "Cero"; ss[1] = "Uno"; ss[2] = "Dos"; ss[3] = "Tres"; ss[4] = "Cuatro";

  for(i=0; i<nrects; i++)
  {
    tree.Insert(rects[i].min, rects[i].max, ss[i]); // Note, all values including zero are fine in this version
  }

  nhits = tree.Search(search_rect.min, search_rect.max, MySearchCallback, NULL);

  cout << "Search resulted in " << nhits << " hits\n";

  // Iterator test
  int itIndex = 0;
  MyTree::Iterator it;
  for( tree.GetFirst(it);
       !tree.IsNull(it);
       tree.GetNext(it) )
  {
    string value = tree.GetAt(it);

    int boundsMin[2] = {0,0};
    int boundsMax[2] = {0,0};
    it.GetBounds(boundsMin, boundsMax);
    cout << "it[" << itIndex++ << "] " << value << " = (" << boundsMin[0] << "," << boundsMin[1] << "," << boundsMax[0] << "," << boundsMax[1] << ")\n";
  }

  // Iterator test, alternate syntax
  itIndex = 0;
  tree.GetFirst(it);
  while( !it.IsNull() )
  {
    string value = *it;
    ++it;
    cout << "it[" << itIndex++ << "] " << value << "\n";
  }

  return 0;

  // Output:
  //
  // nrects = 4
  // Hit data rect 1
  // Hit data rect 2
  // Search resulted in 2 hits
  // it[0] 0 = (0,0,2,2)
  // it[1] 1 = (5,5,7,7)
  // it[2] 2 = (8,5,9,6)
  // it[3] 3 = (7,1,9,2)
  // it[0] 0
  // it[1] 1
  // it[2] 2
  // it[3] 3
}

