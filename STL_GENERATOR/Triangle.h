#include "Vertex.h"
#include "Plane.h"

#ifndef __Triangle_h__
#define __Triangle_h__

class Triangle {
private:
  Vertex vert1;
  Vertex vert2;
  Vertex vert3;
  double nx, ny, nz;
  int t_num;

public:
  // CONSTRUCTORS
  Triangle(Vertex v1, Vertex v2, Vertex v3, int number);

  Triangle() {
  }


  // ACCESSORS
  Vertex get(int number);
  int getNumber() {return t_num;}
  double getNx() {return nx;}
  double getNy() {return ny;}
  double getNz() {return nz;}

  //bool checkIfExists(Vertex a, Vertex b, Vertex c);
};

#endif
