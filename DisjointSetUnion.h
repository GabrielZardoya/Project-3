#pragma once
#include <vector>
using namespace std;

class DisjointSetUnion {

    vector<int> parent;
    vector<int> rank;

public:
    DisjointSetUnion(int n);
    int findParent(int v);
    void unionEdges(int v1, int v2);
};

