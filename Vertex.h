#include <string>
#pragma once
using namespace std;

class Vertex {

private:
    int name;
    float yVal;
    float xVal;
    int degree;

public:
    Vertex(int name, int xVal, int yVal);
    void setName(int name);
    void setxVal(int xVal);
    void setyVal(int yVal);
    void incDegree();

    int getName();
    int getxVal();
    int getyVal();
    int getDegree();

};

