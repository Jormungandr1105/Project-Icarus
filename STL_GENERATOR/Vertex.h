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

  // CONSTRUCTOR
  Vertex() {}
  Vertex(double x, double y, double z) {x_coord=x; y_coord=y; z_coord=z;}
  Vertex(double x, double y, double z, int number, Vertex origin);

  // ACCESSORS
  double getX() const;
  double getY() const;
  double getZ() const;
  double getTheta() const;
  double getOmega() const;
  int getNumber() const;

  void setAngle(Vertex origin);

  // Distance Finder
  double calcDist(Vertex* b);

  // Print Override
  friend std::ostream& operator<<(std::ostream& os, const Vertex* Vt);
};

bool sortX(const Vertex* a, const Vertex* b);
bool sortY(const Vertex* a, const Vertex* b);
bool FindXPairs(const Vertex* a, const Vertex* b);
bool FindYPairs(const Vertex* a, const Vertex* b);

#endif
