#include "Vertex.h"

void Vertex::setName(int name) {
    this->name = name;
}

void Vertex::setxVal(int xVal) {
    this->xVal = xVal;
}

void Vertex::setyVal(int yVal) {
    this->yVal = yVal;
}

int Vertex::getName() {
    return this->name;
}

int Vertex::getxVal() {
    return xVal;
}

int Vertex::getyVal() {
    return yVal;
}

Vertex::Vertex(int name, int xVal, int yVal) {
    this->name = name;
    this->xVal = xVal;
    this->yVal = yVal;
    this->degree = 0;
}

void Vertex::incDegree() {
    degree++;
}

int Vertex::getDegree() {
    return degree;
}

