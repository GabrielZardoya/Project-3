#include <unordered_map>
#include <vector>
#include "Vertex.h"

using namespace std;

class Graph
{
private:
    unordered_map<int, vector<pair<int, float>>> adjacencyList;
    int numOfVertices;
public:
    void insertEdge(int sourceVertex, int destinationVertex, float edgeWeight);
    bool isEdge(int sourceVertex, int destinationVertex);
    float getWeight(int sourceVertex, int destinationVertex);
    vector<int> getAdjacent(int vertex);
    void printGraph();
    vector<pair<pair<int, int>, float>> getAdjList();
    void setVertices(int vertices);
    int getVertices();
};
