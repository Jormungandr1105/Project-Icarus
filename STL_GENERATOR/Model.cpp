#include "Model.h"


Model::Model(Vertex Origin, bool Hole) {
  origin = Origin;
  isHole = Hole;
}


void Model::clear() {
  for (unsigned int i=0; i<px.size(); i++) {
    px[i].clear();
  }
}

void Model::sortByX() {
  //std::cout << "Starting Sort" << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortX);
  //std::cout << "Sorted" << std::endl;
}

void Model::sortByY() {
  std::sort(vertices.begin(), vertices.end(), sortY);
}
