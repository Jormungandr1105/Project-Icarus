#include <cmath>
#include <ostream>
#include "Vertex.h"

Vertex::Vertex(double x, double y, double z, int number, Vertex origin) {
  x_coord = x;
  y_coord = y;
  z_coord = z;
  v_num = number;
  double rad = sqrt(pow(y_coord-origin.getY(),2) + pow(z_coord-origin.getZ(), 2));
  double cos, sin;
  cos = acos((y_coord-origin.getY())/rad);
  sin = asin((z_coord-origin.getZ())/rad);
  if (rad == 0) {
    angleTheta = 2*M_PI;
  } else if (cos > M_PI/2) {
    if (sin > 0) {
      angleTheta = cos;
    } else if (sin == 0) {
      angleTheta = M_PI;
    } else {
      angleTheta = 2*M_PI - cos;
    }
  } else if (cos == M_PI/2) {
    if (sin > 0) {
      angleTheta = M_PI/2;
    } else {
      angleTheta = (3*M_PI)/2;
    }
  } else {
    if (sin > 0) {
      angleTheta = cos;
    } else if (sin == 0) {
      angleTheta = 2*M_PI;
    } else {
      angleTheta = 2*M_PI - cos;
    }
  }
  rad = sqrt(pow(x_coord-origin.getX(),2) + pow(z_coord-origin.getZ(), 2));
  cos = acos((x_coord-origin.getX())/rad);
  sin = asin((z_coord-origin.getZ())/rad);
  if (rad == 0) {
    angleOmega = 2*M_PI;
  } else if (cos > M_PI/2) {
    if (sin > 0) {
      angleOmega = cos;
    } else if (sin == 0) {
      angleOmega = M_PI;
    } else {
      angleOmega = 2*M_PI - cos;
    }
  } else if (cos == M_PI/2) {
    if (sin > 0) {
      angleOmega = M_PI/2;
    } else {
      angleOmega = (3*M_PI)/2;
    }
  } else {
    if (sin > 0) {
      angleOmega = cos;
    } else if (sin == 0) {
      angleOmega = 2*M_PI;
    } else {
      angleOmega = 2*M_PI - cos;
    }
  }
}

// ACCESSORS
double Vertex::getX() const {return x_coord;}
double Vertex::getY() const {return y_coord;}
double Vertex::getZ() const {return z_coord;}
double Vertex::getTheta() const {return angleTheta;}
double Vertex::getOmega() const {return angleOmega;}
int Vertex::getNumber() const {return v_num;}



double Vertex::calcDist(Vertex* b) {
  double distance;
  double dx = pow(x_coord - b->getX(),2);
  double dy = pow(y_coord - b->getY(),2);
  double dz = pow(z_coord - b->getZ(),2);
  distance = sqrt(dx+dy+dz);
  return distance;
}

std::ostream& operator<<(std::ostream& os, const Vertex* Vt){
  os << Vt->x_coord << "," << Vt->y_coord << "," << Vt->z_coord <<
        ":" << Vt->angleTheta << ":" << Vt->angleOmega;
  return os;
}

// Non-Member Functions
// Sort via X Distance
bool sortX(const Vertex* a, const Vertex* b) {
  if (a->getX() > b->getX()) {
    return true;
  } else if (a->getX() == b->getX()){
    float aRad, bRad;
    aRad = sqrt(pow(a->getY(),2)+pow(a->getZ(),2));
    bRad = sqrt(pow(b->getY(),2)+pow(b->getZ(),2));

    if (a->getTheta() > b->getTheta()) {
      return true;
    } else if (a->getTheta() == b->getTheta() && aRad < bRad) {
      return true;
    }
  }
  return false;
}

//Sort via Y Distance
bool sortY(const Vertex* a, const Vertex* b) {
  if (a->getY() > b->getY()) {
    return true;
  } else if (a->getY() == b->getY()){
    float aRad, bRad;
    aRad = sqrt(pow(a->getX(),2)+pow(a->getZ(),2));
    bRad = sqrt(pow(b->getX(),2)+pow(b->getZ(),2));

    if (a->getOmega() > b->getOmega()) {
      return true;
    } else if (a->getOmega() == b->getOmega() && aRad < bRad) {
      return true;
    }
  }
  return false;
}

bool FindXPairs(const Vertex* a, const Vertex* b) {
  if (a->getX() > b->getX()){
    return true;
  }
  return false;
}

bool FindYPairs(const Vertex* a, const Vertex* b) {
  if (a->getY() > b->getY()){
    return true;
  }
  return false;
}
