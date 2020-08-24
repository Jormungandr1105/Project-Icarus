#include "Shapes.h"

void cubeGen(Model &cube, double sidelength) {
  double i=1;
  double j=1;
  double k=1;
  double delta = sidelength/2.0;
  double a = cube.getOrigin().getX();
  double b = cube.getOrigin().getY();
  double c = cube.getOrigin().getZ();
  Vertex* current;
  for (int m=0; m<8; m++) {
    current = new Vertex(i*delta + a, j*delta + b, k*delta + c, m);
    i = -1*i;
    if (i==1) {j = -1*j;}
    if (m>=3) {k=-1;}
    cube.addVertex(current);
  }
  cube.x_min = a-delta;
  cube.x_max = a+delta;
  cube.y_min = b-delta;
  cube.y_max = b+delta;
  cube.z_min = c-delta;
  cube.z_max = c+delta;
}


void sphereGen(Model* &sphere, double radius, double delta) {
  double num_slices = ((2*radius)/delta) + 1;
  double a, b, c, x, y, z;
  a = sphere->getOrigin().getX();
  b = sphere->getOrigin().getY();
  c = sphere->getOrigin().getZ();
  Vertex* current;
  int num = 0;
  for (uint i=0; i<num_slices; i++) {
    for (uint j=0; j<num_slices; j++) {
      x = delta*i - radius;
      y = delta*j - radius;
      z = sqrt(pow(radius,2) - pow(x-a,2) - pow(y-b,2)) + c;
      if (z <= radius) {
        current = new Vertex(x, y, z, num);
        num++;
        sphere->addVertex(current);
        if (z != 0) {
          current = new Vertex(x, y, -1*z, num);
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


void ellipseGen(Model &ellipse, double x, double y, double z, double delta) {
  double numSlicesX, numSlicesY;
  double a, b, c;
}


void rectGen(Model &rect, double xlen, double ylen, double zlen) {
  double i=1;
  double j=1;
  double k=1;
  double x_side, y_side, z_side;
  x_side = xlen/2.0;
  y_side = ylen/2.0;
  z_side = zlen/2.0;
  double a = rect.getOrigin().getX();
  double b = rect.getOrigin().getY();
  double c = rect.getOrigin().getZ();
  Vertex* current;
  for (int m=0; m<8; m++) {
    current = new Vertex(i*x_side + a, j*y_side + b, k*z_side + c, m);
    i = -1*i;
    if (i==1) {j = -1*j;}
    if (m>=3) {k=-1;}
    rect.addVertex(current);
  }
  rect.x_min = a-xlen;
  rect.x_max = a+xlen;
  rect.y_min = b-ylen;
  rect.y_max = b+ylen;
  rect.z_min = c-zlen;
  rect.z_max = c+zlen;
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
