#include "Shapes.h"

void cubeGen(Model* &cube, double sidelength) {
  double i=1;
  double j=1;
  double k=1;
  double delta = sidelength/2.0;
  double a = cube->getOrigin().getX();
  double b = cube->getOrigin().getY();
  double c = cube->getOrigin().getZ();
  Vertex* current;
  for (int m=0; m<8; m++) {
    current = new Vertex(i*delta + a, j*delta + b, k*delta + c, m, cube->getOrigin());
    i = -1*i;
    if (i==1) {j = -1*j;}
    if (m>=3) {k=-1;}
    cube->addVertex(current);
  }
  cube->x_min = a-delta;
  cube->x_max = a+delta;
  cube->y_min = b-delta;
  cube->y_max = b+delta;
  cube->z_min = c-delta;
  cube->z_max = c+delta;
}


void sphereGen(Model* &sphere, double radius, double delta) {
  uint num_slices = ((2*radius)/delta) + 1;
  double a, b, c, x, y, z, epsilon;
  epsilon = radius*.0001;
  a = sphere->getOrigin().getX();
  b = sphere->getOrigin().getY();
  c = sphere->getOrigin().getZ();
  Vertex* current;
  int num = 0;
  for (uint i=0; i<num_slices; i++) {
    for (uint j=0; j<num_slices; j++) {
      x = delta*i - radius + a;
      y = delta*j - radius + b;
      z = sqrt(pow(radius,2) - pow(x-a,2) - pow(y-b,2));
      if (z < radius+epsilon) {
        current = new Vertex(x, y, c+z, num, sphere->getOrigin());
        num++;
        sphere->addVertex(current);
        if (abs(z) > epsilon) {
          current = new Vertex(x, y, c-z, num, sphere->getOrigin());
          sphere->addVertex(current);
          num++;
        }
      }
      printUpdate(i*num_slices+j, pow(num_slices,2));
    }
  }
  sphere->x_min = a-radius;
  sphere->x_max = a+radius;
  sphere->y_min = b-radius;
  sphere->y_max = b+radius;
  sphere->z_min = c-radius;
  sphere->z_max = c+radius;
}


void rectGen(Model* &rect, double xlen, double ylen, double zlen) {
  double i=1;
  double j=1;
  double k=1;
  double x_side, y_side, z_side;
  x_side = xlen/2.0;
  y_side = ylen/2.0;
  z_side = zlen/2.0;
  double a = rect->getOrigin().getX();
  double b = rect->getOrigin().getY();
  double c = rect->getOrigin().getZ();
  Vertex* current;
  for (int m=0; m<8; m++) {
    current = new Vertex(i*x_side + a, j*y_side + b, k*z_side + c, m, rect->getOrigin());
    i = -1*i;
    if (i==1) {j = -1*j;}
    if (m>=3) {k=-1;}
    rect->addVertex(current);
  }
  rect->x_min = a-xlen;
  rect->x_max = a+xlen;
  rect->y_min = b-ylen;
  rect->y_max = b+ylen;
  rect->z_min = c-zlen;
  rect->z_max = c+zlen;
}


void printUpdate(int num, float den) {
  uint bar_size = 20;
  float percent = (num/den)*bar_size;
  std::cout << "[";
  for (uint i=0; i<bar_size; i++) {
    if (i<percent) {std::cout << "*";}
    else {std::cout << "_";}
  }
  std::cout << "]\r";
}
