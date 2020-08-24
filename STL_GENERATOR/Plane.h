#include <vector>
#include "Vertex.h"

#ifndef __Plane_h__
#define __Plane_h__

class Plane {
private:
  std::vector<Vertex*> vertices;
  int p_num;

public:
  // CONSTRUCTORS
  Plane(std::vector<Vertex*> Vertices, int number) {
    vertices = Vertices;
    p_num = number;
  }

  Plane() {}

  // DESTRUCTOR
  //~Plane() {clear();}
  void clear() {
    for (unsigned int i=0; i<vertices.size(); i++) {
      delete vertices[i];
    }
  }

  // ASSIGNORS
  void addVertToPlane(Vertex* vertex) {vertices.push_back(vertex);}

  // ACCESSORS
  Vertex* getVertex(int x) {return vertices[x];}
  int getNumVert() const {return vertices.size();}
  int getNumber() const {return p_num;}
};

#endif
