
#include "include/index.h"
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include <vector>


using namespace std;

// to compile:           g++ -c test.cpp
// to compile and link:  g++ -o prog test.cpp libtree.a -lm
//                       g++ -std=c++11 -o prog test.cpp libtree.a -lm

struct Rect rects[] = {
	{9, 6, 9, 6}, // xmin, ymin, xmax, ymax (for 2 dimensional RTree)
	{8, 5, 8, 5},
	{7, 5, 7, 5},
	{7, 8, 7, 8},
};

int nrects = sizeof(rects) / sizeof(rects[0]);

struct Rect search_rect = {
	{6, 4, 10, 6}, // search will find above rects that this one overlaps
};

unordered_map<int, vector<int>* > hashMap;

int MySearchCallback(int id, void* arg) 
{
    // Note: -1 to make up for the +1 when data was inserted
    int *queryId = (int*)arg;
    int key = *queryId;
    int value = id-1;
	
    printf("Hit base layer input rect %d with query rectangle = %d \n", value, key );

    // check if key is not present in the hashMap
    if ( hashMap.find( key ) == hashMap.end() )
    {
       vector<int> *newArr = new vector<int>();
       newArr->push_back( value );

       hashMap[ key ] = newArr;
    }
    else  // when the key is present, retrieve the corresponding vector and add the new value there
    {
       vector<int> *existingArr = hashMap.at( key );
       existingArr->push_back( value );
    }

    return 1; // keep going
}


void displayMap()
{
     // Traversing unordered_map<int, vector<int>* > hashMap;
    
    for (auto x : hashMap)
    {
      cout << "key = "<<x.first << " values are "; 
  
      vector<int>* valueArr =  x.second;
  
      for( int val : *valueArr )
          cout<< val<<" , ";
      
      cout<<endl;    
    }
}

int main()
{
	struct Node* root = RTreeNewIndex();
	int i, nhits;
	printf("nrects = %d\n", nrects);
	/*
	 * Insert all the data rects.
	 * Notes about the arguments:
	 * parameter 1 is the rect being inserted,
	 * parameter 2 is its ID. NOTE: *** ID MUST NEVER BE ZERO ***, hence the +1,
	 * parameter 3 is the root of the tree. Note: its address is passed
	 * because it can change as a result of this call, therefore no other parts
	 * of this code should stash its address since it could change undernieth.
	 * parameter 4 is always zero which means to add from the root.
	 */
	for(i=0; i<nrects; i++)
		RTreeInsertRect(&rects[i], i+1, &root, 0); // i+1 is rect ID. Note: root can change
	
        int queryId = -1134;
        nhits = RTreeSearch(root, &search_rect, MySearchCallback, &queryId);
	printf("Search resulted in %d hits\n", nhits);

        displayMap();
      
	return 0;
}
