#include "DisjointSetUnion.h"

// ***** CONSTRUCTOR ***** //
DisjointSetUnion::DisjointSetUnion(int n) {
    parent.resize(n, -1);
    rank.resize(n, 1);
}

// ***** FIND THE PARENT OF A VERTEX *****//
int DisjointSetUnion::findParent(int v) {

    if (parent.at(v) == -1) {
        return v;
    }

    parent.at(v) = findParent(parent.at(v));

   return parent.at(v);
}

// ***** IF AN EDGE DOES NOT CREATE A CYCLE, UNION THE TWO VERTICES ***** //
void DisjointSetUnion::unionEdges(int v1, int v2) {

    if (findParent(v1) != findParent(v2)) {

        if (rank.at(findParent(v1)) < rank.at(findParent(v2))) {
            parent.at(findParent(v1)) = findParent(v2);
            rank.at(findParent(v2)) += rank.at(findParent(v1));
        }
        else {
            parent.at(findParent(v2)) = findParent(v1);
            rank.at(findParent(v1)) += rank.at(findParent(v2));
        }
    }
}
