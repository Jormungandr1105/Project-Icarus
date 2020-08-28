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
  std::cout << "Sorting X" << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortX);
}

void Model::sortByY() {
  std::cout << "Sorting Y" << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortY);
}
