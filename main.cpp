#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <unordered_set>
#include <cmath>
#include <chrono>
#include <queue>
#include <algorithm>
#include <set>
#include <random>
#include "Graph.h"
#pragma once

using namespace std;
typedef chrono::high_resolution_clock Clock;

bool sortByEdgeWeight(pair<pair<int, int>, float> a, pair<pair<int, int>, float> b) {
    // ***** HELPER FUNCTION TO SORT THE PAIR BY THE VALUE ***** //
    return (a.second < b.second);
}

vector<pair<pair<int, int>, float>> sortEdges(vector<pair<pair<int, int>, float>> edges) {

    // ***** SORT THE EDGES BY WEIGHT IN ASCENDING ORDER ***** //
    sort(edges.begin(), edges.end(), sortByEdgeWeight);

    return edges;
}

float getDistance(float x1, float x2, float y1, float y2) {

    // ***** GET THE DISTANCE BETWEEN TWO VERTICES USING THE DISTANCE FORMULA ***** //
    float distance = pow(pow(x2 - x1, 2) + pow(y2 - y1, 2), 0.5);
    return distance;
}

Graph dataFromCSV() {

    Graph myGraph;
    cout << "Enter file path: " << endl;
    string filePath;
    cin >> filePath;

    // ****** THIS IS THE TEST CSV FILE I CREATED ***** //
    ifstream csvFile(filePath);
    vector<Vertex> vertices;
    set<int> visited;
    int count = 0;

    cout << "Distance Threshold: " << endl;
    int dist;
    cin >> dist;

    // ***** READ ALL THE DATA FROM THE CSV AND CREATE VERTICES FOR EACH LINE IN THE FILE ***** //
    if (csvFile.is_open()) {
        string data;
        string name;
        string xVal;
        string yVal;

        while(getline(csvFile, data)) {
            istringstream stream(data);
            getline(stream, name, ',');
            getline(stream, xVal, ',');
            getline(stream, yVal, ',');
            Vertex v(stoi(name), stoi(xVal), stoi(yVal));
            vertices.push_back(v);

            // ***** CHECK TO SEE WHICH VERTICES SHOULD BE CONNECTED BASED ON PROXIMITY *****//
            for (int i = 0; i < vertices.size(); i++) {

                if (abs(getDistance(v.getxVal(), vertices.at(i).getxVal(), v.getyVal(), vertices.at(i).getyVal())) <= dist && v.getName() != vertices.at(i).getName()) {
                    myGraph.insertEdge(v.getName(), vertices.at(i).getName(), abs(getDistance(v.getxVal(), vertices.at(i).getxVal(), v.getyVal(), vertices.at(i).getyVal())));
                    v.incDegree();

                    count++;
                    // ***** THE VISITED SET IT TO KEEP TRACK OF HOW MANY VERTICES HAVE AT LEAST ONE EDGE ***** //
                    if (visited.count(v.getName()) == 0) {
                        visited.insert(v.getName());
                    }

                    // ***** THE GRAPH IS UNDIRECTED, THEREFORE THIS IS TO CHECK TO MAKE SURE THE ADJACENCY LIST IS DOES NOT UNDER-COUNT ***** //
                    if (!myGraph.isEdge(vertices.at(i).getName(), v.getName())) {
                        if (visited.count(vertices.at(i).getName()) == 0) {
                            visited.insert(vertices.at(i).getName());
                        }
                        vertices.at(i).incDegree();
                        myGraph.insertEdge(vertices.at(i).getName(), v.getName(), abs(getDistance(v.getxVal(), vertices.at(i).getxVal(), v.getyVal(), vertices.at(i).getyVal())));
                    }
                }
            }
        }
    }

    myGraph.printGraph();
    cout << "Number of Vertices: " << visited.size() << endl;
    cout << "Number of Edges: " << count << endl;
    myGraph.setVertices(visited.size());
    cout << endl;
    return myGraph;
}

int findParent(int v, vector<int> &parent) {

    // ***** FIND THE PARENT OF EACH VERTEX ***** //
    if (parent.at(v) == -1) {
        return v;
    }

    parent.at(v) = findParent(parent.at(v), parent);

    return parent.at(v);
}

void unionEdges(int v1, int v2, vector<int> &parent, vector<int> &rank) {

    // ***** CONNECT TWO VERTICES FOR THE MST THAT DO NOT CREATE A CYCLE ***** //

    if (findParent(v1, parent) != findParent(v2, parent)) {

        if (rank.at(findParent(v1, parent)) < rank.at(findParent(v2, parent))) {
            parent.at(findParent(v1, parent)) = findParent(v2, parent);
            rank.at(findParent(v2, parent)) += rank.at(findParent(v1, parent));
        }
        else {
            parent.at(findParent(v2, parent)) = findParent(v1, parent);
            rank.at(findParent(v1, parent)) += rank.at(findParent(v2, parent));
        }
    }
}

void kruskals(Graph myGraph, int num) {

    vector<int> parent;
    vector<int> rank;

    parent.resize(num, -1);
    rank.resize(num, -1);

    auto t1 = Clock::now();
    float mst = 0;

    // ***** SORT ALL THE EDGE WEIGHTS IN ASCENDING ORDER ***** //
    vector<pair<pair<int, int>, float>> edges = myGraph.getAdjList();
    edges = sortEdges(edges);

    for (int i = 0; i < edges.size(); i++) {

        // ***** TAKE THE EDGE WITH THE LOWEST WEIGHT AND TEST IF IT WILL CREATE A CYCLE
        float weight = edges.at(i).second;
        int source = edges.at(i).first.first;
        int dest = edges.at(i).first.second;

        // ***** IF THE EDGE DOES NOT CREATE A CYCLE CREATE A UNION BETWEEN THE TWO EDGES AND ADD THE WEIGHT TO THE MST
        if (findParent(source, parent) != findParent(dest, parent)) {
            unionEdges(source, dest, parent, rank);
            mst += weight;
        }
    }
    auto t2 = Clock::now();
    cout << "KRUSKAL MST: " << mst << " UNITS" << endl;
    cout << "TIME TO RUN WITH " << num << " VERTICES: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " NANOSECONDS" << endl;
    cout << endl;

}

Graph randomValues(int num) {

    auto t1 = Clock::now();

    Graph myGraph;
    vector<Vertex> vertices;
    Vertex rightVertex(0, 0, 0);

    float rightmost = 0.0;
    int runs = 0;
    int upper =  0;
    int lower = 50;

    // ***** PARTITION NUM INTO RUNS OF 10 VERTICES PER "POD" ***** //
    while (runs != num / 10) {

        vertices.clear();

        int curr = (runs*10);

        // ***** FOR LOOP WILL RUN TO CREATE 10 VERTICES AND CONNECT ALL VERTICES TO EACH OTHER ***** //
        for (int i = curr; i < curr + 10; i++) {
            float xVal = rand() % 15 + (runs*15);
            float yVal = rand() % 15 + (runs*15);

            cout << i << ": (" << xVal << ", " << yVal << ")" << endl;

            Vertex v(i, xVal, yVal);
            vertices.push_back(v);

            if (runs != 0 && i % 10 == 0) {
                myGraph.insertEdge(v.getName(), rightVertex.getName(), abs(getDistance(v.getxVal(), rightVertex.getxVal(), v.getyVal(), rightVertex.getyVal())));
                myGraph.insertEdge(rightVertex.getName(), v.getName(), abs(getDistance(v.getxVal(), rightVertex.getxVal(), v.getyVal(), rightVertex.getyVal())));
            }

            // ***** FIND THE RIGHMOST VERTEX IN THIS POD TO CONNECT TO THE NEXT POD ***** //
            if (xVal > rightmost) {
                rightmost = xVal;
                rightVertex = v;
            }

            // ***** CONNECT ALL VERTICES IN THE SAME POD *****//
            for (int i = 0; i < vertices.size(); i++) {

                if (v.getName() != vertices.at(i).getName()) {
                    myGraph.insertEdge(v.getName(), vertices.at(i).getName(), abs(getDistance(v.getxVal(), vertices.at(i).getxVal(), v.getyVal(), vertices.at(i).getyVal())));

                    // ***** THE GRAPH IS UNDIRECTED, THEREFORE THIS IS TO CHECK TO MAKE SURE THE ADJACENCY LIST IS DOES NOT UNDER-COUNT ***** //
                    if (!myGraph.isEdge(vertices.at(i).getName(), v.getName())) {

                        myGraph.insertEdge(vertices.at(i).getName(), v.getName(), abs(getDistance(v.getxVal(), vertices.at(i).getxVal(), v.getyVal(), vertices.at(i).getyVal())));
                    }
                }
            }
        }
        runs++;
        upper += 50;
        lower += 50;
    }
    auto t2 = Clock::now();
    myGraph.printGraph();
    cout << "TIME TO GENERATE A GRAPH WITH " << num << " VERTICES: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " NANOSECONDS" << endl;
    cout << endl;

    return myGraph;
}

void prims(Graph myGraph, int num) {
    auto t1 = Clock::now();
    random_device rd;
    default_random_engine eng(rd());
    uniform_int_distribution<int> vertexdistr(0, (num - 1));
    int currentVertex = vertexdistr(eng);

    float mst = 0;
    bool foundSmallestEdge = false;
    unordered_set<int> currentMinimumSpanningTree;
    currentMinimumSpanningTree.insert(currentVertex);
    priority_queue<pair<float, int>, vector<pair<float, int>>, greater<>> smallestEdgeQueue;
    vector<int> currentAdjacentVertices = myGraph.getAdjacent(currentVertex);

    for(int i = 0; i < currentAdjacentVertices.size(); i++)
    {
        if(currentMinimumSpanningTree.count(currentAdjacentVertices[i]) == 0)
        {
            smallestEdgeQueue.push(make_pair(myGraph.getWeight(currentVertex, currentAdjacentVertices[i]), currentAdjacentVertices[i]));
        }
    }

    for(int i = 0; i < (num - 1); i++)
    {
        foundSmallestEdge = false;
        while(!foundSmallestEdge)
        {
            if(currentMinimumSpanningTree.count(smallestEdgeQueue.top().second) == 0)
            {
                mst += smallestEdgeQueue.top().first;
                currentVertex = smallestEdgeQueue.top().second;
                currentMinimumSpanningTree.insert(currentVertex);
                currentAdjacentVertices = myGraph.getAdjacent(currentVertex);
                smallestEdgeQueue.pop();
                for(int j = 0; j < currentAdjacentVertices.size(); j++)
                {
                    if(currentMinimumSpanningTree.count(currentAdjacentVertices[j]) == 0)
                    {
                        smallestEdgeQueue.push(make_pair(myGraph.getWeight(currentVertex, currentAdjacentVertices[j]), currentAdjacentVertices[j]));
                    }
                }
                foundSmallestEdge = true;
            }
            else
            {
                smallestEdgeQueue.pop();
            }
        }
    }

    auto t2 = Clock::now();
    cout << "PRIMS MST: " << mst << " UNITS" << endl;
    cout << "TIME TO RUN WITH " << num << " VERTICES: " << chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " NANOSECONDS" << endl;
    cout << endl;
}

int main() {

    bool exit = false;
    Graph myGraph;
    int num;

    while (!exit) {

        // ***** DISPLAY THE MENU ***** //
        cout << "***** MINIMUM SPAN DISTANCE SIMULATOR *****" << endl;
        cout << "1. Randomize values" << endl;
        cout << "2. Import from CSV" << endl;
        cout << "3. Prim's Algorithm" << endl;
        cout << "4. Kruskal's Algorithm" << endl;
        cout << "5. Compare Performance" << endl;
        cout << "6. Exit" << endl;
        cout << "*******************************************" << endl;

        // ***** TAKE INPUT FROM THE USER ***** //
        int choice;
        cin >> choice;

        // ***** GENERATE RANDOM DATA ***** //
        if (choice == 1) {
            cout << "Enter Number of Vertices: " << endl;
            cin >> num;
            myGraph = randomValues(num);
        }

        // ***** READ DATA FROM A CSV FILE AND CREATE A GRAPH ***** //
        else if (choice == 2) {
            myGraph = dataFromCSV();
            num = myGraph.getVertices();
        }

        // ***** CONDUCT PRIMS ALGORITHM AND OUTPUT THE WEIGHT OF THE MST ***** //
        else if (choice == 3) {
            prims(myGraph, num);
        }

        // ***** CONDUCT KRUSKALS ALGORITHM AND OUTPUT THE WEIGHT OF THE MST ***** //
        else if (choice == 4) {
            kruskals(myGraph, num);
        }

        // ***** CONDUCT KRUSKALS ALGORITHM AND PRIMS ALGORITHM AND COMPARE THE TIME TO EXECUTE ***** //
        else if (choice == 5) {
            prims(myGraph, num);
            kruskals(myGraph, num);
        }

        // ***** EXIT THE PROGRAM ***** //
        if (choice == 6) {
            exit = true;
        }
    }
}