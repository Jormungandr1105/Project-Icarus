#include "Shapes.h"

void cubeGen(Model* &cube, double sidelength, double delta) {
  double x,y,z;
  uint m = 0;
  double a = cube->getOrigin().getX();
  double b = cube->getOrigin().getY();
  double c = cube->getOrigin().getZ();
  uint num_slices = (sidelength/delta)+1;
  Vertex* current;
  for (uint i=0; i<num_slices; i++) {
    for (uint j=0; j<num_slices; j++) {
      x = i*delta - a;
      y = j*delta - b;
      z = c-(sidelength/2.0);
      current = new Vertex(x, y, z, m, cube->getOrigin());
      cube->addVertex(current);
      z = c+(sidelength/2.0);
      m++;
      current = new Vertex(x, y, z, m, cube->getOrigin());
      cube->addVertex(current);
      m++;
    }
  }
  cube->x_min = a-(sidelength/2.0);
  cube->x_max = a+(sidelength/2.0);
  cube->y_min = b-(sidelength/2.0);
  cube->y_max = b+(sidelength/2.0);
  cube->z_min = c-(sidelength/2.0);
  cube->z_max = c+(sidelength/2.0);
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


void rectGen(Model* &rect, double xlen, double ylen, double zlen, double delta) {
  double x_side, y_side, z_side, x,y,z;
  uint num_slices_x, num_slices_y;
  uint m = 0;
  x_side = xlen/2.0;
  y_side = ylen/2.0;
  z_side = zlen/2.0;
  num_slices_x = (xlen/delta) + 1;
  num_slices_y = (ylen/delta) + 1;
  double a = rect->getOrigin().getX();
  double b = rect->getOrigin().getY();
  double c = rect->getOrigin().getZ();
  Vertex* current;
  for (uint i=0; i<num_slices_x; i++) {
    for (uint j=0; j<num_slices_y; j++) {
      x = i*delta - x_side + a;
      y = j*delta - y_side + b;
      z = c-z_side;
      current = new Vertex(x, y, z, m, rect->getOrigin());
      rect->addVertex(current);
      z = c+z_side;
      m++;
      current = new Vertex(x, y, z, m, rect->getOrigin());
      rect->addVertex(current);
      m++;
    }
  }
  rect->x_min = a-x_side;
  rect->x_max = a+x_side;
  rect->y_min = b-y_side;
  rect->y_max = b+y_side;
  rect->z_min = c-z_side;
  rect->z_max = c+z_side;
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
