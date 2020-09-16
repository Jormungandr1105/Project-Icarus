#include "MultiModel.h"

MultiModel::MultiModel(Model* model) {
  uint num_verts;
  first = model;
  last = model;
  num_verts = last->getNumVertices();
  for (int i=0; i<num_verts; i++) {
    vertices.push_back(last->getVertices(i));
  }
  while (last->next != NULL) {
    last = last->next;
    num_verts = last->getNumVertices();
    for (int i=0; i<num_verts; i++) {
      vertices.push_back(last->getVertices(i));
    }
  }
}

MultiModel::MultiModel(const MultiModel& that) {
  first = that.first;
  last = that.last;
  vertices = that.vertices;
}

MultiModel& MultiModel::operator=(const MultiModel& that){
  first = that.first;
  last = that.last;
  vertices = that.vertices;
  return *this;
}

/*
void MultiModel::addModel(Model* model) {
    if (first == NULL) {
      first = model;
      last = model;
    } else {
      last->next = model;
      last = model;
    }
}
*/

void MultiModel::removeUnnecessary() {
  double epsilon = .0005;
  bool within;
  Model* current_model;
  std::vector<Vertex*> new_vertices;
  std::cout << "REMOVING EXTRA POINTS" << std::endl;
  std::cout << vertices.size() << std::endl;
  for (uint i=0; i<vertices.size(); i++) {
    current_model = first;
    within = false;
    while (!within && current_model!=NULL) {
      within = current_model->isWithin(vertices[i], epsilon);
      current_model = current_model->next;
    }
    //std::cout << "END WHILE" << std::endl;
    if (!within) {new_vertices.push_back(vertices[i]);}
  }
  std::cout << new_vertices.size() << std::endl;
  vertices = new_vertices;
}

void MultiModel::simplifyGeometry() {
  for(uint i=1; i<px.size(); i++) {
    Plane p1 = px[i];
    Plane p0 = px[i-1];
    if (p0.getNumVert() == p1.getNumVert()) {
      bool failed = false;
      for (uint j=0; j<p0.getNumVert(); j++) {
        if (p0.getVertex(j)->getY() != p1.getVertex(j)->getY() ||
            p0.getVertex(j)->getZ() != p1.getVertex(j)->getZ()) {
              failed = true;
            }
      }
      if (!failed) {
        px.erase(px.begin()+i);
      }
    }
  }
  for(uint i=1; i<py.size(); i++) {
    Plane p1 = py[i];
    Plane p0 = py[i-1];
    if (p0.getNumVert() == p1.getNumVert()) {
      bool failed = false;
      for (uint j=0; j<p0.getNumVert(); j++) {
        if (p0.getVertex(j)->getX() != p1.getVertex(j)->getX() ||
            p0.getVertex(j)->getZ() != p1.getVertex(j)->getZ()) {
              failed = true;
            }
      }
      if (!failed) {
        py.erase(py.begin()+i);
      }
    }
  }
  Vertex* v0, *v1, *v2;
  for(uint i=0; i<px.size(); i++) {
    for (uint j=1; j<px[i].getNumVert()-1; j++) {
      v0 = px[i].getVertex(j-1);
      v1 = px[i].getVertex(j);
      v2 = px[i].getVertex(j+1);
      if (v0->getY() == v1->getY() && v1->getY() == v2->getY()){
        px[i].remove(j);
      } else if (v0->getZ() == v1->getZ() && v1->getZ() == v2->getZ()) {
        px[i].remove(j);
      }
    }
  }
  for(uint i=0; i<py.size(); i++) {
    for (uint j=1; j<py[i].getNumVert()-1; j++) {
      v0 = py[i].getVertex(j-1);
      v1 = py[i].getVertex(j);
      v2 = py[i].getVertex(j+1);
      if (v0->getX() == v1->getX() && v1->getX() == v2->getX()){
        py[i].remove(j);
      } else if (v0->getZ() == v1->getZ() && v1->getZ() == v2->getZ()) {
        py[i].remove(j);
      }
      //std::cout << py[i].getNumVert() << std::endl;
    }
  }
}


void MultiModel::slicer() {
  int first = 0;
  int last = 0;
  uint p = 0;
  // Slice X
  Plane cur_plane;
  this->sortByX();
  uint size = vertices.size();
  double x = vertices[0]->getX();
  for(uint i=1; i<=size; i++) {
    if (i == size) {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(vertices[j]);
      }
      cur_plane = Plane(current, p);
      px.push_back(cur_plane);
    } else if (vertices[i]->getX() == x) {
      last = i;
    } else {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(vertices[j]);
      }
      cur_plane = Plane(current, p);
      p++;
      px.push_back(cur_plane);
      first = i;
      last = i;
      x = vertices[i]->getX();
    }
  }
  // Now Slice Y
  this->sortByY();
  double y = vertices[0]->getY();
  first = 0;
  last = 0;
  for(uint i=1; i<=size; i++) {
    if (i == size) {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(vertices[j]);
      }
      cur_plane = Plane(current, p);
      py.push_back(cur_plane);
    } else if (vertices[i]->getY() == y) {
      last = i;
    } else {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(vertices[j]);
      }
      cur_plane = Plane(current, p);
      p++;
      py.push_back(cur_plane);
      first = i;
      last = i;
      y = vertices[i]->getY();
    }
  }
}


void MultiModel::meshGen() {
  Triangle newT;
  Vertex *nextHigh, *nextLow;
  bool endLow, endHigh, nextIsLow;
  int num1, num2;
  int tot = 0;
  Model* current_model = first;
  Plane cur_plane, p1, p2;
  double x_min, x_max, y_min, y_max, z_min, z_max, x, y, z;
  double delta = 1;
  uint num_planes;
  while(current_model!=NULL) {
    // ESTABLISHES BOUNDARIES FOR MESH GEN
    x_min = current_model->x_min;
    x_max = current_model->x_max;
    y_min = current_model->y_min;
    y_max = current_model->y_max;
    z_min = current_model->z_min;
    z_max = current_model->z_max;

    // X PLANES ===============================================================
    // FINDS PLANE ON EDGE OF BOUNDARY
    num_planes = px.size();
    int i=num_planes-1;
    cur_plane = px[i];
    //std::cout << "CHECK 0" << std::endl;
    //std::cout << x_min << std::endl;
    //std::cout << x_max << std::endl;
    //std::cout << cur_plane.getVertex(0)->getX() << std::endl;
    while(cur_plane.getVertex(0)->getX() > x_max) {
      i--;
      std::cout << cur_plane.getVertex(0)->getX() << std::endl;
      cur_plane = px[i];
    }
    i--;
    num_planes = px.size();
    std::cout << num_planes << std::endl;
    // STARTS MESH MAKING
    std::cout << "START X MESH" << std::endl;
    while(i>=0) {
      if (px[i].getNumVert() > 0 && px[i+1].getNumVert()>0) {
        if (px[i+1].getVertex(0)->getX() <= x_max &&
                    px[i].getVertex(0)->getX() >= x_min) {
          p1 = px[i+1];
          p2 = px[i];
          std::vector<Vertex*> vec1;
          std::vector<Vertex*> vec2;
          endLow = false;
          endHigh = false;
          num1 = 1;
          num2 = 1;

          for (uint a=0; a<p1.getNumVert(); a++) {
            x = p1.getVertex(a)->getX();
            y = p1.getVertex(a)->getY();
            z = p1.getVertex(a)->getZ();
            // I know its gross to look at, but its just checking if the point is
            // within the boundaries
            if (x>=x_min && x<=x_max && y>=y_min && y<=y_max && z>=z_min && z<=z_max) {
              vec1.push_back(p1.getVertex(a));
            }
          }
          for (uint b=0; b<p2.getNumVert(); b++) {
            x = p2.getVertex(b)->getX();
            y = p2.getVertex(b)->getY();
            z = p2.getVertex(b)->getZ();
            // See Above
            if (x>=x_min && x<=x_max && y>=y_min && y<=y_max && z>=z_min && z<=z_max) {
              vec2.push_back(p2.getVertex(b));
            }
          }

          vec1 = removeExtras(vec1, true);
          vec2 = removeExtras(vec2, true);
          //std::cout << "PASSED REMOVES" << std::endl;
          if (vec1.size() > 0 && vec2.size() > 0) {
            if (vec1.size() > 1) {nextLow = vec1[1];}
            else {endLow = true;}
            if (vec2.size() > 1) {nextHigh = vec2[1];}
            else {endHigh = true;}

            // Choose next Vertex for Triangle
            while (!endLow || !endHigh) {
              if (!endLow && !endHigh) {
                if (nextLow->getTheta() >= nextHigh->getTheta()) {
                  nextIsLow=true;
                } else {
                  nextIsLow=false;
                }
              } else if (!endLow) {
                nextIsLow=true;
              } else {
                nextIsLow=false;
              }
              // Create New Triangle
              if (nextIsLow) {
                if (!current_model->isAHole()) {
                  newT = Triangle(nextLow, vec1[num1-1], vec2[num2-1], tot);
                } else {
                  newT = Triangle(nextLow, vec2[num2-1], vec1[num1-1], tot);
                }
                num1++;
              } else {
                if (!current_model->isAHole()) {
                  newT = Triangle(nextHigh, vec1[num1-1], vec2[num2-1], tot);
                } else {
                  newT = Triangle(nextHigh, vec2[num2-1], vec1[num1-1], tot);
                }
                num2++;
              }
              mesh.push_back(newT);
              tot++;
              if (vec1.size() > num1) {nextLow = vec1[num1];}
              else {endLow = true;}
              if (vec2.size() > num2) {nextHigh = vec2[num2];}
              else {endHigh = true;}
            }
            // Clean Up Last 2 Triangles
            if (vec1.size() > 1) {
              if (!current_model->isAHole()) {
                newT = Triangle(vec1[0], vec1[num1-1], vec2[num2-1], tot);
              } else {
                newT = Triangle(vec1[0], vec2[num2-1], vec1[num1-1], tot);
              }
              tot++;
              num1 = 1;
              mesh.push_back(newT);
            }
            if (vec2.size() > 1) {
              if (!current_model->isAHole()) {
                newT = Triangle(vec2[0], vec1[num1-1], vec2[num2-1], tot);
              } else {
                newT = Triangle(vec2[0], vec2[num2-1], vec1[num1-1], tot);
              }
              tot++;
              num2 = 1;
              mesh.push_back(newT);
            }
          }
          //i--;
        }
      }
      i--;
    }

    // Y PLANES ===============================================================
    // FINDS PLANE ON EDGE OF BOUNDARY

    num_planes = py.size();
    uint j=0;
    cur_plane = py[j];
    //std::cout << y_min << std::endl;
    //std::cout << y_max << std::endl;
    //std::cout << cur_plane.getVertex(0)->getY() << std::endl;
    while(cur_plane.getVertex(0)->getY() <= y_min) {
      j++;
      cur_plane = py[j];
    }
    j++;
    std::cout << num_planes << std::endl;
    // STARTS MESH MAKING
    std::cout << "START Y MESH" << std::endl;
    //std::cout << j-1 << std::endl;
    while(j<num_planes) {
      if (py[j].getNumVert() > 0 && py[j-1].getNumVert()>0) {
        if (py[j].getVertex(0)->getY() <= y_max &&
              py[j-1].getVertex(0)->getY() >= y_min) {
            //std::cout << py[j].getVertex(0)->getY() << std::endl;
            //std::cout << y_max << std::endl;
            //std::cout << j-1 << std::endl;
            p1 = py[j-1];
            p2 = py[j];
            std::vector<Vertex*> vec1;
            std::vector<Vertex*> vec2;
            endLow = false;
            endHigh = false;
            num1 = 1;
            num2 = 1;

            for (uint a=0; a<p1.getNumVert(); a++) {
              x = p1.getVertex(a)->getX();
              y = p1.getVertex(a)->getY();
              z = p1.getVertex(a)->getZ();
              // I know its gross to look at, but its just checking if the point is
              // within the boundaries
              if (x>=x_min && x<=x_max && y>=y_min && y<=y_max && z>=z_min && z<=z_max) {
                vec1.push_back(p1.getVertex(a));
              }
            }
            for (uint b=0; b<p2.getNumVert(); b++) {
              x = p2.getVertex(b)->getX();
              y = p2.getVertex(b)->getY();
              z = p2.getVertex(b)->getZ();
              // See Above
              if (x>=x_min && x<=x_max && y>=y_min && y<=y_max && z>=z_min && z<=z_max) {
                vec2.push_back(p2.getVertex(b));
              }
            }

            vec1 = removeExtras(vec1, false);
            vec2 = removeExtras(vec2, false);
            //std::cout << "PASSED REMOVES" << std::endl;
            //std::cout << vec1.size() << std::endl;
            //for (int i=0; i<vec1.size(); i++){std::cout << vec1[i] << std::endl;}
            std::cout << "VEC2\n";
            for (int i=0; i<vec2.size(); i++){std::cout << vec2[i] << std::endl;}
            if (vec1.size()>0 && vec2.size()>0) {
              //std::cout << "PAST VERTEX" <<std::endl;
              if (vec1.size() > 1) {nextLow = vec1[1];}
              else {endLow = true;}
              if (vec2.size() > 1) {nextHigh = vec2[1];}
              else {endHigh = true;}

              // Choose next Vertex for Triangle
              while (!endLow || !endHigh) {
                if (!endLow && !endHigh) {
                  if (nextLow->getOmega() >= nextHigh->getOmega()) {
                    nextIsLow=true;
                  } else {
                    nextIsLow=false;
                  }
                } else if (!endLow) {
                  nextIsLow=true;
                } else {
                  nextIsLow=false;
                }
                // Create New Triangle
                if (nextIsLow) {
                  if (!current_model->isAHole()) {
                    newT = Triangle(nextLow, vec1[num1-1], vec2[num2-1], tot);
                  } else {
                    newT = Triangle(nextLow, vec2[num2-1], vec1[num1-1], tot);
                  }
                  num1++;
                } else {
                  if (!current_model->isAHole()) {
                    newT = Triangle(nextHigh, vec1[num1-1], vec2[num2-1], tot);
                  } else {
                    newT = Triangle(nextHigh, vec2[num2-1], vec1[num1-1], tot);
                  }
                  num2++;
                }
                mesh.push_back(newT);
                tot++;
                if (vec1.size() > num1) {nextLow = vec1[num1];}
                else {endLow = true;}
                if (vec2.size() > num2) {nextHigh = vec2[num2];}
                else {endHigh = true;}
              }
              // Clean Up Last 2 Triangles
              if (vec1.size() > 1) {
                if (!current_model->isAHole()) {
                  newT = Triangle(vec1[0], vec1[num1-1], vec2[num2-1], tot);
                } else {
                  newT = Triangle(vec1[0], vec2[num2-1], vec1[num1-1], tot);
                }
                tot++;
                num1 = 1;
                mesh.push_back(newT);
              }
              if (vec2.size() > 1) {
                if (!current_model->isAHole()) {
                  newT = Triangle(vec2[0], vec1[num1-1], vec2[num2-1], tot);
                } else {
                  newT = Triangle(vec2[0], vec2[num2-1], vec1[num1-1], tot);
                }
                tot++;
                num2 = 1;
                mesh.push_back(newT);
              }
            }
          }
          //j++;
      }
      j++;
    }
    std::cout << "CLEARED Y" << std::endl;
    current_model = current_model->next;
    //std::cout << mesh.size() << std::endl;
    //std::cout << "END" << std::endl;
  }
}


void MultiModel::stlWriter(std::string fileName) {
  std::ofstream STLFILE("../STLs/" + fileName + "2.stl");
  Triangle triangle;
  Vertex* vertex;
  uint size = mesh.size();
  bool end = false;
  uint m = 0;
  std::cout << size << std::endl;
  //STLFILE << "solid Sphere_Model\n";
  while (!end) {
    STLFILE << "solid Sphere_Model\n";
    // Add all Triangles
    for(uint i=0; i<size; i++,m++) {
      triangle = mesh[i];
      STLFILE << "\tfacet normal " << triangle.getNx() << " " <<
                 triangle.getNy() << " " << triangle.getNz() << "\n";
      STLFILE << "\t\touter loop \n";
      for (int j=0; j<3; j++) {
        vertex = mesh[i].get(j);
        STLFILE << "\t\t\tvertex " << vertex->getX() << " " <<
                   vertex->getY() << " " << vertex->getZ() << "\n";
      }
      STLFILE << "\t\tendloop\n";
      STLFILE << "\tendfacet\n";
      printUpdate(m, size);
    }
    STLFILE << "endsolid Sphere_Model\n";
    end = true;
  }
  STLFILE.close();
}


std::vector<Vertex*> MultiModel::removeExtras(std::vector<Vertex*> vec1, bool isX) {
  //std::cout << "STARTED\n";
  if (vec1.size() > 2) {
    std::vector<Vertex*> vec2;
    uint first = 0;
    uint last = 0;
    Vertex *a, *b;
    if (!isX) {
      //std::cout << "!IS X\n";
      std::sort(vec1.begin(), vec1.end(), FindXPairs);
      //for(uint j=0; j<vec1.size(); j++) {std::cout << vec1[j] << std::endl;}
      double x = vec1[0]->getX();
      for(uint i=1; i<=vec1.size(); i++) {
        if (i == vec1.size()) {
          for (int j=first; j<=last; j++) {
            if (j==first){a=vec1[j]; b=a;}
            else {
              if (vec1[j]->getZ() < a->getZ()) {a = vec1[j];}
              if (vec1[j]->getZ() > b->getZ()) {b = vec1[j];}
            }
          }
          vec2.push_back(a);
          vec2.push_back(b);
          //std::cout << "FINISHED CHECK\n";
        } else if (vec1[i]->getX() == x) {
          last = i;
          //std::cout << i << std::endl;
        } else {
          for (int j=first; j<=last; j++) {
            if (j==first){a=vec1[j]; b=a;}
            else {
              if (vec1[j]->getZ() < a->getZ()) {a = vec1[j];}
              if (vec1[j]->getZ() > b->getZ()) {b = vec1[j];}
            }
          }
          //std::cout << last << std::endl;
          vec2.push_back(a);
          vec2.push_back(b);
          first = i;
          last = i;
          x = vec1[i]->getX();
        }
      }
      //for(uint j=0; j<vec2.size(); j++) {std::cout << vec2[j] << std::endl;}
      std::sort(vec2.begin(), vec2.end(), sortY);
    } else {
      std::sort(vec1.begin(), vec1.end(), FindYPairs);
      double y = vec1[0]->getY();
      for(uint i=1; i<=vec1.size(); i++) {
        if (i == vec1.size()) {
          for (int j=first; j<=last; j++) {
            if (j==first){a=vec1[j]; b=a;}
            else {
              if (vec1[j]->getZ() < a->getZ()) {a = vec1[j];}
              if (vec1[j]->getZ() > b->getZ()) {b = vec1[j];}
            }
          }
          vec2.push_back(a);
          vec2.push_back(b);
        } else if (vec1[i]->getY() == y) {
          last = i;
        } else {
          for (int j=first; j<=last; j++) {
            if (j==first){a=vec1[j]; b=a;}
            else {
              if (vec1[j]->getZ() < a->getZ()) {a = vec1[j];}
              if (vec1[j]->getZ() > b->getZ()) {b = vec1[j];}
            }
          }
          vec2.push_back(a);
          if (b!=a) {vec2.push_back(b);}
          first = i;
          last = i;
          y = vec1[i]->getY();
        }
      }
      std::sort(vec2.begin(), vec2.end(), sortX);
    }
    return vec2;
  }
  return vec1;
}


void MultiModel::sortByX() {
  std::cout << "Sorting X             " << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortX);
}


void MultiModel::sortByY() {
  std::cout << "Sorting Y             " << std::endl;
  std::sort(vertices.begin(), vertices.end(), sortY);
}
