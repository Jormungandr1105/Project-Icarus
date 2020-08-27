#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Triangle.h"
#include "Plane.h"
#include "Vertex.h"
#include "Model.h"
#include "Shapes.h"

typedef unsigned int uint;

//void cubeGen(Model &cube, double sidelength);
//void sphereGen(Model &sphere, double radius, double delta);
Model* ReadIn(std::string infile);
void slicer(Model* &model, bool sliceY);
void meshGen(Model* &model);
void stlWriter(std::vector<Model*> models, std::string fileName);
void printUpdate(int num, float den);


int main(int argc, char* argv[]) {
  // BASIC SHAPES GENERATOR
  /*
  Vertex originS = Vertex(0,0,0,0);
  Vertex originY = Vertex(10,10,0,1);
  Vertex originR = Vertex(-5,-5,0,2);
  double radius, delta;
  std::string fileName;
  std::string input = argv[4];
  radius = atof(argv[2]);
  delta = atof(argv[3]);
  fileName = argv[1];
  Model Sphere(originS, false);
  Model Cube(originY, false);
  Model Rectangle(originR, false);
  std::cout << "Generating Vertices...      " << std::endl;
  sphereGen(Sphere, radius, delta);
  cubeGen(Cube, 2*radius);
  rectGen(Rectangle, 20, 10, 5);
  std::cout << "Slicing...                " << std::endl;
  slicer(Sphere, false);
  slicer(Cube, true);
  slicer(Rectangle, true);
  std::cout << "Generating Mesh...        " << std::endl;
  meshGen(Sphere);
  meshGen(Cube);
  meshGen(Rectangle);
  std::cout << "Writing to File...        " << std::endl;
  std::vector<Model> Models;
  Models.push_back(Sphere);
  Models.push_back(Cube);
  Models.push_back(Rectangle);
  stlWriter(Models, fileName);
  std::cout << "Completed                 " << std::endl;
  */
  Model* first_model;
  std::vector<Model*> models;
  std::string fileName = argv[1];
  std::string input = argv[2];
  first_model = ReadIn(input);
  //std::cout << first_model->getMeshSize() << std::endl;
  models.push_back(first_model);
  while (first_model->next != NULL) {
    models.push_back(first_model->next);
    first_model = first_model->next;
  }
  stlWriter(models, fileName);
}

/*
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
}
*/
/*
void sphereGen(Model &sphere, double radius, double delta) {
  double num_slices = ((2*radius)/delta) + 1;
  double a, b, c, x, y, z;
  a = sphere.getOrigin().getX();
  b = sphere.getOrigin().getY();
  c = sphere.getOrigin().getZ();
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
        sphere.addVertex(current);
        if (z != 0) {
          current = new Vertex(x, y, -1*z, num);
          sphere.addVertex(current);
          num++;
        }
      }
      printUpdate(i*num_slices+j, pow(num_slices,2));
    }
  }
}
*/

Model* ReadIn(std::string infile) {
  double x, y, z, delta, radius, delx, dely, delz;
  bool hole;
  Model* first = NULL;
  Model* current = NULL;
  Model* last = NULL;
  Vertex origin;
  std::ifstream INFILE(infile);
  // Load in all the shapes from the file
  std::string name, hole_str;
  while (INFILE >> name && name != "END") {
    //INFILE >> name;
    std::cout << name << std::endl;
    if (name == "SPHERE") {
      INFILE >> x >> y >> z >> radius >> delta >> hole_str;
      std::cout << delta << std::endl;
      if (hole_str == "false" || hole_str == "FALSE") {hole = false;}
      else {hole = true;}
      origin = Vertex(x, y, z, 0);
      current = new Model(origin, hole);
      sphereGen(current, radius, delta);
      std::cout << current->getNumVertices() << std::endl;
      slicer(current, false);
      std::cout << current->getNumXPlanes() << std::endl;
      meshGen(current);
      std::cout << current->getMeshSize() << std::endl;
      if (first == NULL) {first = current;}
      if (last != NULL) {last->next = current;}
      last = current;
    } if (name == "CUBE") {
      INFILE >> x >> y >> z >> delx >> hole_str;
      if (hole_str == "false" || hole_str == "FALSE") {hole = false;}
      else {hole = true;}
      origin = Vertex(x, y, z, 0);
      current = new Model(origin, hole);
      cubeGen(current, delx);
      slicer(current, true);
      meshGen(current);
      if (first == NULL) {first = current;}
      if (last != NULL) {last->next = current;}
      last = current;
    }
  }
  return first;
}

void slicer(Model* &model, bool sliceY) {
  int first = 0;
  int last = 0;
  uint p = 0;
  // Slice X
  Plane cur_plane;
  model->sortByX();
  uint size = model->getNumVertices();
  double x = model->getVertices(0)->getX();

  for(uint i=1; i<=size; i++) {
    if (i == size) {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(model->getVertices(j));
      }
      cur_plane = Plane(current, p);
      model->addXPlane(cur_plane);
    } else if (model->getVertices(i)->getX() == x) {
      last = i;
    } else {
      std::vector<Vertex*> current;
      for (int j=first; j<=last; j++) {
        current.push_back(model->getVertices(j));
      }
      cur_plane = Plane(current, p);
      p++;
      model->addXPlane(cur_plane);
      first = i;
      last = i;
      x = model->getVertices(i)->getX();
    }
    printUpdate(i, size);
  }

  if (sliceY) {
    // Now Slice Y
    model->sortByY();
    double y = model->getVertices(0)->getY();
    first = 0;
    last = 0;

    for(uint i=1; i<=size; i++) {
      if (i == size) {
        std::vector<Vertex*> current;
        for (int j=first; j<=last; j++) {
          current.push_back(model->getVertices(j));
        }
        cur_plane = Plane(current, p);
        model->addYPlane(cur_plane);
      } else if (model->getVertices(i)->getY() == y) {
        last = i;
      } else {
        std::vector<Vertex*> current;
        for (int j=first; j<=last; j++) {
          current.push_back(model->getVertices(j));
        }
        cur_plane = Plane(current, p);
        p++;
        model->addYPlane(cur_plane);
        first = i;
        last = i;
        y = model->getVertices(i)->getY();
      }
    }
  }
}


void meshGen(Model* &model) {
  Triangle newT;
  Vertex *nextHigh, *nextLow;
  bool endLow, endHigh, nextIsLow;
  int num1, num2;
  int tot = 0;
  uint size = model->getNumXPlanes();
  for (uint i=1; i<size; i++) {
    Plane p1 = model->getPlaneX(i-1);
    Plane p2 = model->getPlaneX(i);
    endLow = false;
    endHigh = false;
    num1 = 1;
    num2 = 1;
    //std::cout << p1.getNumVert() << std::endl;
    if (p1.getNumVert() > 1) {nextLow = p1.getVertex(1);}
    else {endLow = true;}
    if (p2.getNumVert() > 1) {nextHigh = p2.getVertex(1);}
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
        if (!model->isAHole()) {
          newT = Triangle(nextLow, p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
        } else {
          newT = Triangle(nextLow, p2.getVertex(num1-1), p1.getVertex(num2-1), tot);
        }
        num1++;
      } else {
        if (!model->isAHole()) {
          newT = Triangle(nextHigh, p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
        } else {
          newT = Triangle(nextHigh, p2.getVertex(num1-1), p1.getVertex(num2-1), tot);
        }
        num2++;
      }
      model->addTriangle(newT);
      tot++;
      if (p1.getNumVert() > num1) {nextLow = p1.getVertex(num1);}
      else {endLow = true;}
      if (p2.getNumVert() > num2) {nextHigh = p2.getVertex(num2);}
      else {endHigh = true;}
    }
    // Clean Up Last 2 Triangles
    if (p1.getNumVert() > 1) {
      if (!model->isAHole()) {
        newT = Triangle(p1.getVertex(0), p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
      } else {
        newT = Triangle(p1.getVertex(0), p2.getVertex(num1-1), p1.getVertex(num2-1), tot);
      }
      tot++;
      num1 = 1;
      model->addTriangle(newT);
    }
    if (p2.getNumVert() > 1) {
      if (!model->isAHole()) {
        newT = Triangle(p2.getVertex(0), p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
      } else {
        newT = Triangle(p2.getVertex(0), p2.getVertex(num1-1), p1.getVertex(num2-1), tot);
      }
      tot++;
      num2 = 1;
      model->addTriangle(newT);
    }
    printUpdate(i, size);
    // Y Mesh Gen
    if (model->getNumYPlanes() != 0) {
      size = model->getNumYPlanes();
      for (uint i=1; i<size; i++) {
        Plane p1 = model->getPlaneY(i-1);
        Plane p2 = model->getPlaneY(i);
        endLow = false;
        endHigh = false;
        num1 = 1;
        num2 = 1;
        //std::cout << p1.getNumVert() << std::endl;
        if (p1.getNumVert() > 1) {nextLow = p1.getVertex(1);}
        else {endLow = true;}
        if (p2.getNumVert() > 1) {nextHigh = p2.getVertex(1);}
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
            newT = Triangle(nextLow, p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
            num1++;
          } else {
            newT = Triangle(nextHigh, p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
            num2++;
          }
          model->addTriangle(newT);
          tot++;
          if (p1.getNumVert() > num1) {nextLow = p1.getVertex(num1);}
          else {endLow = true;}
          if (p2.getNumVert() > num2) {nextHigh = p2.getVertex(num2);}
          else {endHigh = true;}
        }
        // Clean Up Last 2 Triangles
        if (p1.getNumVert() > 1) {
          newT = Triangle(p1.getVertex(0), p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
          tot++;
          num1 = 1;
          model->addTriangle(newT);
        }
        if (p2.getNumVert() > 1) {
          newT = Triangle(p2.getVertex(0), p1.getVertex(num1-1), p2.getVertex(num2-1), tot);
          tot++;
          num2 = 1;
          model->addTriangle(newT);
        }
      }
    }
  }
}


void stlWriter(std::vector<Model*> models, std::string fileName){
  std::ofstream STLFILE(fileName + ".stl");
  Triangle triangle;
  Vertex* vertex;
  uint size;
  uint total_size = 0;
  for (uint i=0; i<models.size(); i++) {
    total_size += models[0]->getMeshSize();
  }

  //STLFILE << "solid Sphere_Model\n";
  for (uint k=0; k<models.size(); k++) {
    size = models[k]->getMeshSize();
    STLFILE << "solid Sphere_Model\n";

    // Add all Triangles
    for(uint i=0; i<size; i++) {
      triangle = models[k]->getMesh(i);
      STLFILE << "\tfacet normal " << triangle.getNx() << " " << triangle.getNy() <<
                 " " << triangle.getNz() << "\n";
      STLFILE << "\t\touter loop \n";
      for (int j=0; j<3; j++) {
        vertex = models[k]->getMesh(i).get(j);
        STLFILE << "\t\t\tvertex " << vertex->getX() << " " <<
                   vertex->getY() << " " << vertex->getZ() << "\n";
      }
      STLFILE << "\t\tendloop\n";
      STLFILE << "\tendfacet\n";
      printUpdate(i, total_size);
    }
    STLFILE << "endsolid Sphere_Model";
  }
  STLFILE.close();
}
