#include <cmath>
#include <iostream>
#include "Triangle.h"

Triangle::Triangle(Vertex v1, Vertex v2, Vertex v3, int number) {
  double intNx, intNy, intNz, Vx, Vy, Vz, Wx, Wy, Wz, den;
  vert1 = v1;
  vert2 = v2;
  vert3 = v3;
  t_num = number;
  // V = v2 - v1
  Vx = v2.getX() - v1.getX();
  Vy = v2.getY() - v1.getY();
  Vz = v2.getZ() - v1.getZ();
  // W = v3 - v1
  Wx = v3.getX() - v1.getX();
  Wy = v3.getY() - v1.getY();
  Wz = v3.getZ() - v1.getZ();
  // Calculate Intermediate Normals
  intNx = (Vy*Wz) - (Vz*Wy);
  intNy = (Vz*Wx) - (Vx*Wz);
  intNz = (Vx*Wy) - (Vy*Wx);
  // Calculate Final Normals;
  //den = pow(intNx,2) + pow(intNy,2) + pow(intNz,2);
  //std::cout << den << std::endl;
  if (den == 0) {
    nx = 0;
    ny = 0;
    nz = 0;
  } else {
    nx = intNx;
    ny = intNy;
    nz = intNz;
  }
}

Vertex Triangle::get(int number) {
  if (number == 0) {
    return vert1;
  } else if (number == 1) {
    return vert2;
  } else {
    return vert3;
  }
}
