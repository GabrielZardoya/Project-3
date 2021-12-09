#include <iostream>
#include <algorithm>
#include <set>
#include "Graph.h"

void Graph::insertEdge(int sourceVertex, int destinationVertex, float edgeWeight)
{
    adjacencyList[sourceVertex].push_back(make_pair(destinationVertex, edgeWeight));
}

bool Graph::isEdge(int sourceVertex, int destinationVertex)
{
    for(auto iter = adjacencyList[sourceVertex].begin(); iter != adjacencyList[sourceVertex].end(); ++iter)
    {
        if(iter->first == destinationVertex)
        {
            return true;
        }
    }
    return false;
}

float Graph::getWeight(int sourceVertex, int destinationVertex)
{
    for(auto iter = adjacencyList[sourceVertex].begin(); iter != adjacencyList[sourceVertex].end(); ++iter)
    {
        if(iter->first == destinationVertex)
        {
            return iter->second;
        }
    }
    return 0.0;
}

vector<int> Graph::getAdjacent(int vertex)
{
    vector<int> adjacentVertices;
    for(auto iter = adjacencyList[vertex].begin(); iter != adjacencyList[vertex].end(); ++iter)
    {
        adjacentVertices.push_back(iter->first);
    }
    return adjacentVertices;
}

void Graph::printGraph()
{
    for(auto mapIter = adjacencyList.begin(); mapIter != adjacencyList.end(); ++mapIter)
    {
        for(auto vectorIter = mapIter->second.begin(); vectorIter != mapIter->second.end(); ++vectorIter)
        {
            cout << mapIter->first << " " << vectorIter->first << " " << vectorIter->second << endl;
        }
    }
}

vector<pair<pair<int, int>, float>> Graph::getAdjList() {

    vector<pair<pair<int, int>, float>> edges;
    set<pair<int, int>> visited;

    for(auto mapIter = adjacencyList.begin(); mapIter != adjacencyList.end(); ++mapIter)
    {
        for(auto vectorIter = mapIter->second.begin(); vectorIter != mapIter->second.end(); ++vectorIter)
        {
            int source = mapIter->first;
            int dest = vectorIter->first;
            pair<int, int> e (dest, source);

            if (visited.count(e) != 0) {
                continue;
            }
            else {
                pair<int, int> e1 (source, dest);
                visited.insert(e1);
            }

            pair<int, int> connectedVertices (source, dest);
            pair<pair<int, int>, float> connectedWeight (connectedVertices, vectorIter->second);
            edges.push_back(connectedWeight);
        }
    }

    return edges;
}

int Graph::getVertices() {
    return numOfVertices;
}

void Graph::setVertices(int vertices) {
    this->numOfVertices = vertices;
}
