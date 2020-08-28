#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Plane.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Model.h"

void cubeGen(Model* &cube, double sidelength);
void rectGen(Model* &rect, double xlen, double ylen, double zlen);
void sphereGen(Model* &sphere, double radius, double delta);


void printUpdate(int num, float den);
