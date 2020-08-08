#include <iostream>
#include <vector>
#include <algorithm>
#include "Plane.h"
#include "Triangle.h"
#include "Vertex.h"

#ifndef __Model_h__
#define __Model_h__

typedef unsigned int uint;

class Model {
private:
  bool isHole;
  Vertex origin;
  std::vector<Vertex*> vertices;
  std::vector<Plane> px;
  std::vector<Plane> py;
  std::vector<Triangle> mesh;

public:
  // CONSTRUCTOR
  Model(Vertex Origin, bool Hole);
  // DESTRUCTORS
  void clear();
  ~Model() {clear();}
  // ASSIGNORS
  void addVertex(Vertex* Vertex) {vertices.push_back(Vertex);}
  void addXPlane(Plane Px) {px.push_back(Px);}
  void addYPlane(Plane Py) {py.push_back(Py);}
  void addTriangle(Triangle Tri) {mesh.push_back(Tri);}
  // ACCESSORS
  bool isAHole() {return isHole;}
  Vertex getOrigin() const {return origin;}
  Vertex* getVertices(uint num) {return vertices[num];}
  uint getNumVertices() {return vertices.size();}
  Plane getPlaneX(uint num) {return px[num];}
  uint getNumXPlanes() {return px.size();}
  Plane getPlaneY(uint num) {return py[num];}
  uint getNumYPlanes() {return py.size();}
  Triangle getMesh(uint num) {return mesh[num];}
  uint getMeshSize() {return mesh.size();}
  // SORTER
  void sortByX();
  void sortByY();
};

#endif
