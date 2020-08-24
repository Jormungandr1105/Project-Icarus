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
  bool original;
  
public:
  double x_min, x_max, y_min, y_max, z_min, z_max;
  Model *next;

  // CONSTRUCTORS
  Model(Vertex Origin, bool Hole);
  Model(const Model& that);
  Model& operator=(const Model& that);
  // DESTRUCTORS
  void clear();
  ~Model() {clear();}
  // ASSIGNORS
  void addVertex(Vertex* Vertex) {vertices.push_back(Vertex);}
  void addXPlane(Plane Px) {px.push_back(Px);}
  void addYPlane(Plane Py) {py.push_back(Py);}
  void addTriangle(Triangle Tri) {mesh.push_back(Tri);}
  // ACCESSORS
  bool isAHole() const {return isHole;}
  Vertex getOrigin() const {return origin;}
  Vertex* getVertices(uint num) const {return vertices[num];}
  uint getNumVertices() const {return vertices.size();}
  Plane getPlaneX(uint num) const {return px[num];}
  uint getNumXPlanes() const {return px.size();}
  Plane getPlaneY(uint num) const {return py[num];}
  uint getNumYPlanes() const {return py.size();}
  Triangle getMesh(uint num) const {return mesh[num];}
  uint getMeshSize() const {return mesh.size();}
  // SORTER
  void sortByX();
  void sortByY();
};

#endif
