#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Plane.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Model.h"

void cubeGen(Model* &cube, double sidelength, double delta);
void rectGen(Model* &rect, double xlen, double ylen, double zlen, double delta);
void sphereGen(Model* &sphere, double radius, double delta);


void printUpdate(int num, float den);
