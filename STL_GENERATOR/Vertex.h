#ifndef __Vertex_h__
#define __Vertex_h__

class Vertex {
private:
  double x_coord;
  double y_coord;
  double z_coord;
  int v_num;
  double angleTheta, angleOmega;


public:
  Vertex* next_;
  Vertex* prev_;

  // CONSTRUCTOR
  Vertex() {}
  Vertex(double x, double y, double z, int number);

  // ACCESSORS
  double getX() const;
  double getY() const;
  double getZ() const;
  double getTheta() const;
  double getOmega() const;
  int getNumber() const;

  // Distance Finder
  double calcDist(Vertex* b);

};

bool sortX(const Vertex* a, const Vertex* b);
bool sortY(const Vertex* a, const Vertex* b);

#endif
