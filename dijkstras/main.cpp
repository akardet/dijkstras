/*************************************************************************************
 *  Name: Akardet Sam Tancharoensuksavai
 *  Course: 4110.002 - Algorithms
 *  Assignment: Homework 3
 *  Date: 09/29/16
 *  Description: Basic implementation of Dijkstra's algorithm as described in 
 *               Section 24.3 of CLRS.
 *************************************************************************************/

#include "header.hpp"

int main(int argc, const char * argv[]) {
    
    string file_content; //whole input file read into a string
    vector<string> splitData; //parsed out newline into separate string vectors
    vector<int> graphTotals; //verticies and edge totals
    vector<vector<int>> edgeMatrix; //matrix to hold the edge weights of each edge
    vector<string> vertexKey; //holds the verticies in order
    
    //read file into a string
    file_content = openFile(argc, argv);

    //split string into separate string vectors
    splitData = splitString(file_content);
    
    //determine number of total vertices and edges
    graphTotals = getVertexEdgeTotals(file_content);
    
    //create graph to appropriate size
    struct Graph* graph = createGraph(graphTotals);
    
    //add edges to graph structure
    graph = buildGraph(graph, splitData);
    
    //populate verticies
    vertexKey = populateKey(graph);
    
    //populate edge matrix with edge weights
    edgeMatrix = buildMatrix(graph, vertexKey);
    
    dijkstras(edgeMatrix, graphTotals, vertexKey);
    
    
    free(graph);
    
    return 0;
}
