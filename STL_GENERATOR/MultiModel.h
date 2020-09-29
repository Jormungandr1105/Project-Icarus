#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cmath>
#include "Plane.h"
#include "Triangle.h"
#include "Vertex.h"
#include "Model.h"
#include "Shapes.h"


#ifndef __MultiModel_h__
#define __MultiModel_h__

typedef unsigned int uint;

class MultiModel {
private:
  std::vector<Vertex*> vertices;
  std::vector<Plane> px;
  std::vector<Plane> py;
  std::vector<Triangle> mesh;

public:
  Model* first,* last;
  std::vector<Vertex> origins;

  // CONSTRUCTORS
  MultiModel(Model* model);
  MultiModel(const MultiModel& that);
  MultiModel& operator=(const MultiModel& that);
  // DESTRUCTORS
  void clear();
  ~MultiModel() {clear();}
  // ASSIGNORS
  void addModel(Model* model);
  void addVertex(Vertex* Vertex) {vertices.push_back(Vertex);}
  void addXPlane(Plane Px) {px.push_back(Px);}
  void addYPlane(Plane Py) {py.push_back(Py);}
  void addTriangle(Triangle Tri) {mesh.push_back(Tri);}
  // ACCESSORS
  Vertex* getVertices(uint num) const {return vertices[num];}
  uint getNumVertices() const {return vertices.size();}
  Plane getPlaneX(uint num) const {return px[num];}
  uint getNumXPlanes() const {return px.size();}
  Plane getPlaneY(uint num) const {return py[num];}
  uint getNumYPlanes() const {return py.size();}
  Triangle getMesh(uint num) const {return mesh[num];}
  uint getMeshSize() const {return mesh.size();}
  // MODIFIERS
  void removeUnnecessary();
  void slicer();
  void meshGen();
  void stlWriter(std::string fileName);
  std::vector<Vertex*> removeExtras(std::vector<Vertex*> vec1, bool isX);
  void simplifyGeometry();
  // SORTERS
  void sortByX();
  void sortByY();
};

#endif
