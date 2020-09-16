#include "Model.h"


Model::Model(Vertex Origin, bool Hole) {
  origin = Origin;
  isHole = Hole;
  original = true;
  next = NULL;
}

Model::Model(const Model& that) {
  origin = that.getOrigin();
  isHole = that.isAHole();
  original = false;
  next = that.next;
  uint numVert = that.getNumVertices();
  uint numXPlanes = that.getNumXPlanes();
  uint numYPlanes = that.getNumYPlanes();
  uint meshSize = that.getMeshSize();
  for(uint i=0; i<numVert; i++) {vertices.push_back(that.getVertices(i));}
  for(uint j=0; j<numXPlanes; j++) {px.push_back(that.getPlaneX(j));}
  for(uint k=0; k<numYPlanes; k++) {py.push_back(that.getPlaneY(k));}
  for(uint l=0; l<meshSize; l++) {mesh.push_back(that.getMesh(l));}
  x_min = that.x_min;
  x_max = that.x_max;
  y_min = that.y_min;
  y_max = that.y_max;
  z_min = that.z_min;
  z_max = that.z_max;
}

Model& Model::operator=(const Model& that){
  origin = that.getOrigin();
  isHole = that.isAHole();
  original = false;
  next = that.next;
  uint numVert = that.getNumVertices();
  uint numXPlanes = that.getNumXPlanes();
  uint numYPlanes = that.getNumYPlanes();
  uint meshSize = that.getMeshSize();
  for(uint i=0; i<numVert; i++) {vertices.push_back(that.getVertices(i));}
  for(uint j=0; j<numXPlanes; j++) {px.push_back(that.getPlaneX(j));}
  for(uint k=0; k<numYPlanes; k++) {py.push_back(that.getPlaneY(k));}
  for(uint l=0; l<meshSize; l++) {mesh.push_back(that.getMesh(l));}
  x_min = that.x_min;
  x_max = that.x_max;
  y_min = that.y_min;
  y_max = that.y_max;
  z_min = that.z_min;
  z_max = that.z_max;
  return *this;
}

void Model::clear() {
  if (original) {
    std::cout << "CLEARING MODEL" << std::endl;
    for (unsigned int i=0; i<px.size(); i++) {
      px[i].clear();
    }
  }
}

void Model::sortByX() {
  std::cout << "Sorting X             " << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortX);
}

void Model::sortByY() {
  std::cout << "Sorting Y             " << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortY);
}


bool Model::isWithin(Vertex* Vertex, double epsilon) {
  double x, y, z, z_prime, z_2;
  x = Vertex->getX();
  y = Vertex->getY();
  z = Vertex->getZ();
  if (x>x_min && x<x_max) {
    if (y>y_min && y<y_max) {
      if(z>z_min && z<z_max) {
        Plane x_plane;
        for(uint j=0; j<px.size(); j++) {
          x_plane = px[j];
          if (x_plane.getNumVert()!=0 && x_plane.getVertex(0)->getX() == x) {
            for(uint k=0; k<x_plane.getNumVert(); k++) {
              if (x_plane.getVertex(k)->getY() == y) {
                z_prime = origin.getZ();
                z_2 = x_plane.getVertex(k)->getZ();
                if (abs(z-z_prime)+epsilon<abs(z_2-z_prime)) {return true;}
                else {return false;}
              }
            }
            return false;
          }
        }
      }
    }
  }
  return false;
}


bool checkForCollision(Model* a, Model* b) {
  if (checkForBetween(a->x_min, a->x_max, b->x_min, b->x_max)) {
    if (checkForBetween(a->y_min, a->y_max, b->y_min, b->y_max)) {
      if (checkForBetween(a->z_min, a->z_max, b->z_min, b->z_max)) {
        std::cout << "Collision!                     " << std::endl;
        return true;
      }
    }
  }
  return false;
}

bool checkForBetween(float a_min, float a_max, float b_min, float b_max) {
  bool answer;
  if (a_min >= b_min && a_min <= b_max) {return true;}
  if (a_max >= b_min && a_min <= b_max) {return true;}
  if (b_min >= a_min && b_min <= a_max) {return true;}
  if (b_max >= a_min && b_max <= a_max) {return true;}
  return false;
}
