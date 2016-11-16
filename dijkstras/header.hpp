/*************************************************************************************
 *  Name: Akardet Sam Tancharoensuksavai
 *  Course: 4110.002 - Algorithms
 *  Assignment: Homework 3
 *  Date: 09/29/16
 *  Description: Basic implementation of Dijkstra's algorithm as described in
 *               Section 24.3 of CLRS.
 *************************************************************************************/

#ifndef main_h
#define main_h

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <algorithm>

using namespace std;

struct Edge
{
    string src, dest;
    int weight;
};

struct Graph
{
    int vertex, edges;
    
    struct Edge* edge;
};

const int MAX_INT = numeric_limits<int>::max();

string openFile(int argc, const char * argv[]);
vector <string> splitString(string line);
vector <string> removeNonCharacters(string line);
struct Graph* createGraph(vector<int> graphData);
struct Graph* buildGraph(struct Graph* graph, vector<string> graphString);
vector<vector<int>> buildMatrix(struct Graph* graph, vector<string> vertexKey);
int getEdgeWeight(struct Graph* graph, string src, string dest);
vector<int> getVertexEdgeTotals(string data);
int findMin(vector<int> distVector, vector<bool> visitedVector);
vector<string> populateKey(struct Graph* graph);
void dijkstras(vector<vector<int>> graphMatrix, vector<int> graphData, vector<string> verticies);
void print(vector<int> distances, vector<string> vertices);



#endif /* main_h */
