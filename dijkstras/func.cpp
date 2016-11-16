/*************************************************************************************
 *  Name: Akardet Sam Tancharoensuksavai
 *  Course: 4110.002 - Algorithms
 *  Assignment: Homework 3
 *  Date: 09/29/16
 *  Description: Basic implementation of Dijkstra's algorithm as described in
 *               Section 24.3 of CLRS.
 *************************************************************************************/

#include "header.hpp"

/*
 openFile - opens file from command line and reads into string variable
 @params - int argc - number of arguments
           const char * argv[] - each argument string from commandline
 @return - string - read file
 */
string openFile(int argc, const char * argv[]) {
    string data;
    
    //check if user passed filename
    if (argc != 2)
        cout<<"usage: "<< argv[0] <<" <filename>\n";
    else {
        ifstream FILE (argv[1]);
        
        //check if file opened properly
        if (!FILE.is_open())
            cout << "Could not open file\n";
        //read each line into data variable
        else {
            string str;
            vector<string> parsedString;
            while(getline(FILE, str)) {
                data += str;
                data.push_back('\n');
            }
        }
        
        FILE.close();
    }
    
    return data;
}

/*
 splitString - split data string into individual string vectors at every newline
 @params - string line - text from file
 @return - vector<string> - each vector string is the source vertex and destination vertx and weights
 */
vector <string> splitString(string line) {
    vector<string> v ; // vector to add the strings
    
    size_t prev_pos = 0, pos;
    while ((pos = line.find_first_of("\n", prev_pos)) != string::npos)
    {
        if (pos > prev_pos)
            v.push_back(line.substr(prev_pos, pos-prev_pos));
        prev_pos= pos+1;
    }
    if (prev_pos < line.length())
        v.push_back(line.substr(prev_pos, string::npos));
    
    return v;
    
}

/*
 removeNonCharacters - parse vector string to just the source vertex, destination vertex, and weight 
                       with no special characters into vector string
 @param - string line - raw string of edge data
 @return - vector<string> - vector of individual vertices and edge weights
 */
vector <string> removeNonCharacters(string line) {
    vector<string> v ; //vector to add the characters
    
    size_t prev_pos = 0, pos;
    while ((pos = line.find_first_of(";(), ", prev_pos)) != string::npos)
    {
        if (pos > prev_pos)
            v.push_back(line.substr(prev_pos, pos-prev_pos));
        prev_pos= pos+1;
    }
    if (prev_pos < line.length())
        v.push_back(line.substr(prev_pos, string::npos));
    
    return v;
    
}

/*
 createGraph - allocate memory for size of graph
 @param - vector<int> graphData - vector holding total vertices and edges
 @return - struct Graph* - Graph allocated to appropriate size based on total vertices and edges
 */
struct Graph* createGraph(vector<int> graphData) {
    
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
    graph->vertex = graphData[0];
    graph->edges = graphData[1];
    
    return graph;
}

/*
 buildGraph - add edges to graph structure
 @params - struct Graph* graph - graph structure allocated with memory
           vector<string> graphString - separated strings of vertices and weights
 @return - struct Graph* - completed graph with appropriate edges and weights
 */
struct Graph* buildGraph(struct Graph* graph, vector<string> graphString) {
    int edgeCount = 0;
    graph->edge = new Edge[graph->edges]; //allocating space for appropriate number edge structures
    
    for(int i = 0; i <= graphString.size()-1; i++) {
        vector<string> parsedVertexString; //variable to hold vertices and weights
        string vertexString = graphString[i]; //string to pass to function to remove special characters
        
        parsedVertexString = removeNonCharacters(vertexString);
        string headVertex = parsedVertexString[0];
        
        for(int j = 1; j < parsedVertexString.size()-1; j+=2) {
            graph->edge[edgeCount].src = headVertex;
            graph->edge[edgeCount].dest = parsedVertexString[j];
            graph->edge[edgeCount].weight = stoi(parsedVertexString[j+1]);
            edgeCount++;
        }
    }
    
    return graph;
}

/*
 buildMatrix - build out N x N matrix with appropriate edge weights
 @params - struct Graph* graph - completed graph with appropriate edges and weights
           vector<string> vertexKey - vector of vertices
 @return - vector<vector<int>> - 2D matrix of graph
 */
vector<vector<int>> buildMatrix(struct Graph* graph, vector<string> vertexKey) {
    vector<vector<int>> edgeMatrix;
    vector<int> row;
    int count = graph->vertex; //variable for total number of vertices

    for(int i = 0; i < count; i++) {
        for(int j = 0; j < count; j++) {
            string src = vertexKey[i]; //set source vertex
            string dest = vertexKey[j]; //set destination vertex
            
            int weight = getEdgeWeight(graph,src,dest); //get the weight of appropriate edge
            row.push_back(weight); //add weight to row
        }
        edgeMatrix.push_back(row); //add row to matrix
        row.clear();
    }

    return edgeMatrix;
    
}

/*
 dijkstras - find minimum path from source vertex which is the first vertex in file
 @param - vector<vector<int>> graphMatrix - complete 2D matrix of graph
          vector<int> graphData - total number of vertices and edges
          vector<string> vertices - vector of vertices
 */
void dijkstras(vector<vector<int>> graphMatrix, vector<int> graphData, vector<string> vertices) {
    int V = graphData[0]; //total number of vertices
    int min;
    vector<int> distance; //hold the minimum distance from source to destination vertex
    vector<bool> visited; //check if vertex has been visited
    
    //populate distance and visited vectors with default value
    for(int i = 0; i < V; i++) {
        distance.push_back(MAX_INT);
        visited.push_back(false);
    }
    distance[0] = 0; //set source to 0
    
    int current = 0;
    while(visited[current] != true) {
        for(int j = 1; j < V; j++) {
            //check if edge exists
            if (graphMatrix[current][j] != -1) {
                int pathTotal = distance[current] + graphMatrix[current][j]; //get sum of current path distance weight and edge weight
                if(pathTotal < distance[j])
                    distance[j] = pathTotal;
            }
        }
        visited[current] = true;
        min = findMin(distance, visited); //find minimum weight in distance vector that has not been visited
        current = min;
        print(distance, vertices);
    }
    
    print(distance, vertices);
    
    
}

/*
 findMin - find the next vertex to analyze that has not been visited yet
 @params - vector<int> distVector - distance vector
           vector<bool> visitedVector - vector of vertices that have been visited
 @return - int - position of next minimum vertex
 */
int findMin(vector<int> distVector, vector<bool> vistedVector) {
    int min = MAX_INT, place = 0;
    
    for(int i = 1; i < distVector.size(); i++) {
        //check if vertex has been visited
        if(vistedVector[i] != true) {
            //check if weight is less than min
            if(distVector[i] < min) {
                min = distVector[i];
                place = i;
            }
        }
    }
    
    return place;
}

/*
 getEdgeWeight - returns the edge weight from the graph
 @params - struct Graph* graph - completed graph with vertices and edges
           string src - source vertex
           string dest - destination vertex
 @return - int - weight of the edge
 */
int getEdgeWeight(struct Graph* graph, string src, string dest) {
    int weight = -1;
    for(int i = 0; i < graph->edges; i++) {
        if(graph->edge[i].src == src && graph->edge[i].dest == dest) {
            return graph->edge[i].weight;
        }
    }

    return weight;
}

/*
 populateKey - create a key of the vertices
 @params - struct Graph* graph - completed graph with vertices and edges
 @return - vector<string> - vector of all the vertices
 */
vector<string> populateKey(struct Graph* graph) {
    int i = 0;
    int j = 0;
    vector<string> key;
    
    while (i < graph->edges)
    {

        if (key.empty())
        {
            key.push_back(graph->edge[i].src); //add the first vertex
        }
        else if (key[j] != graph->edge[i].src) //add vertex if not already in key
        {
            key.push_back(graph->edge[i].src);
            j++;
            i++;
        }
        else //vertex is already in key
        {
            i++;
        }
    }
    
    return key;
    
}

/*
 getVertexEdgeTotals - traverse through data and count total number of edges and vertices
 @params - string data - full text of data file
 @return - vector<int> - returns vector where total number of vertices is at position [0] and
                         total number of edges at position [1]
 */
vector<int> getVertexEdgeTotals(string data) {
    vector<int> graphTotals; //used to put graph totals
    size_t vertexTotal = count(data.begin(), data.end(), '\n'); //number of new lines indicate number of vertices
    size_t edgeTotal = count(data.begin(), data.end(), ';'); //number of ; indicate number of edges
    
    graphTotals.push_back(static_cast<int>(vertexTotal));
    graphTotals.push_back(static_cast<int>(edgeTotal));
    
    return graphTotals;
}

/*
 print - print the minimum distance from source to each corresponding vertex
 @params - vector<int> distances - vector of minimum distances
           vector<string> vertices - vector of vertices
*/
void print(vector<int> distances, vector<string> vertices) {
    cout << "------------------------------------------" << endl;
    for (int i = 0; i < vertices.size(); i++) {
        cout << vertices[i] << " | ";
        if(distances[i] == MAX_INT)
            cout << "INF" << endl;
        else
            cout << distances[i] << endl;
    }
    cout << "------------------------------------------" << endl;
}
