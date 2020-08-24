#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Plane.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Model.h"
//#include "main.cpp"

void cubeGen(Model &cube, double sidelength);
void rectGen(Model &rect, double xlen, double ylen, double zlen);
void sphereGen(Model* &sphere, double radius, double delta);
void ellipseGen(Model &ellipse, double x, double y, double z, double delta);


void printUpdate(int num, float den);
