#include <cmath>
#include "Vertex.h"

Vertex::Vertex(double x, double y, double z, int number) {
  x_coord = x;
  y_coord = y;
  z_coord = z;
  v_num = number;
  next_ = NULL;
  prev_ = NULL;
  double rad = sqrt(pow(y_coord,2) + pow(z_coord, 2));
  double cos, sin;
  cos = acos(y_coord/rad);
  sin = asin(z_coord/rad);
  if (rad == 0) {
    angle = 2*M_PI;
  } else if (cos > M_PI/2) {
    if (sin > 0) {
      angle = cos;
    } else if (sin == 0) {
      angle = M_PI;
    } else {
      angle = 2*M_PI - cos;
    }
  } else if (cos == M_PI/2) {
    if (sin > 0) {
      angle = M_PI/2;
    } else {
      angle = (3*M_PI)/2;
    }
  } else {
    if (sin > 0) {
      angle = cos;
    } else if (sin == 0) {
      angle = 2*M_PI;
    } else {
      angle = 2*M_PI - cos;
    }
  }
}

// ACCESSORS
double Vertex::getX() const {return x_coord;}
double Vertex::getY() const {return y_coord;}
double Vertex::getZ() const {return z_coord;}
double Vertex::getTheta() const {return angle;}
int Vertex::getNumber() const {return v_num;}



double Vertex::calcDist(Vertex b) {
  double distance;
  double dx = pow(x_coord - b.getX(),2);
  double dy = pow(y_coord - b.getY(),2);
  double dz = pow(z_coord - b.getZ(),2);
  distance = sqrt(dx+dy+dz);
  return distance;
}

// Non-Member Functions
// Sort via X Distance
bool sortX(const Vertex &a, const Vertex &b) {
  if (a.getX() > b.getX()) {
    return true;
  } else if (a.getX() == b.getX()){
    float angleA, angleB, aRad, bRad;
    aRad = sqrt(pow(a.getY(),2)+pow(a.getZ(),2));
    bRad = sqrt(pow(b.getY(),2)+pow(b.getZ(),2));

    if (a.getTheta() > b.getTheta()) {
      return true;
    } else if (a.getTheta() == b.getTheta() && aRad < bRad) {
      return true;
    }
  }
    /*
  } else if (a.getX() == b.getX()) {
    if (a.getY() > b.getY()) {
      return true;
    } else if (a.getY() == b.getY()) {
      if (a.getZ() > b.getZ()) {
        return true;
      }
    }
  }
*/
  return false;
}

//Sort via Y Distance
bool sortY(const Vertex &a, const Vertex &b) {
  if (a.getY() > b.getY()) {
    return true;
  } else if (a.getY() == b.getY()) {
    if (a.getX() > b.getX()) {
      return true;
    } else if (a.getX() == b.getX()) {
      if (a.getZ() > b.getZ()) {
        return true;
      }
    }
  }

  return false;
}
