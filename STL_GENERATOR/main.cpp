#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include "Triangle.h"
#include "Plane.h"
#include "Vertex.h"
#include "Model.h"
#include "MultiModel.h"
#include "Shapes.h"


typedef unsigned int uint;


Model* ReadIn(std::string infile);
void slicer(Model* &model, bool sliceY);
void meshGen(Model* &model);
void stlWriter(Model* first_model, std::string fileName);
void printUpdate(int num, float den);


int main(int argc, char* argv[]) {
  // BASIC SHAPES GENERATOR
  Model* first_model;
  bool answer;
  uint n, m;
  n=1;
  m=2;
  std::vector<Model*> models;
  std::string fileName = argv[1];
  std::string input = argv[2];
  first_model = ReadIn(input);
  Model* main_model = first_model;
  Model* current_model = main_model->next;
  while(main_model->next != NULL) {
    while (current_model != NULL) {
      answer = checkForCollision(main_model, current_model);
      if (answer) {std::cout << n << "," << m << std::endl;}
      current_model = current_model->next;
      m++;
    }
    main_model = main_model->next;
    current_model = main_model->next;
    n++;
    m = n+1;
  }
  // MULTIMODEL ATTEMPT
  MultiModel* mmodel = new MultiModel(first_model);
  mmodel->removeUnnecessary();
  mmodel->slicer();
  mmodel->simplifyGeometry();
  mmodel->meshGen();
  mmodel->stlWriter(fileName);

  std::cout << "==================        " << std::endl;
  std::cout << "Writing to File...        " << std::endl;
  stlWriter(first_model, fileName);
  std::cout << "Completed                 " << std::endl;
  main_model = first_model;
  while(main_model->next != NULL) {
    current_model = main_model;
    main_model = main_model->next;
    delete current_model;
  }
  delete main_model;
}


Model* ReadIn(std::string infile) {
  double x, y, z, delta, radius, delx, dely, delz;
  bool hole;
  Model* first = NULL;
  Model* current = NULL;
  Model* last = NULL;
  Vertex origin;
  std::string job_num;
  std::ifstream INFILE("../Input_files/" + infile);
  INFILE >> job_num; INFILE >> job_num;
  std::cout << "======================" << std::endl;
  std::cout << "/// PROJECT_ICARUS \\\\\\" << std::endl;
  std::cout << "======================" << std::endl;
  std::cout << "=      JOB_" << job_num << "      =" << std::endl;
  std::cout << "======================" << std::endl;
  // Load in all the shapes from the file
  std::string name, hole_str;
  while (INFILE >> name && name != "END") {
    // SPHERE CREATION
    if (name == "SPHERE") {
      std::cout << name << "                  \n===========" << std::endl;
      INFILE >> x >> y >> z >> radius >> delta >> hole_str;
      if (hole_str == "false" || hole_str == "FALSE") {hole = false;}
      else {hole = true;}
      origin = Vertex(x, y, z);
      current = new Model(origin, hole);
      std::cout << "Generating Vertices...      " << std::endl;
      sphereGen(current, radius, delta);
      std::cout << "Slicing...                  " << std::endl;
      slicer(current, false);
      std::cout << "Generating Mesh...          " << std::endl;
      meshGen(current);
      if (first == NULL) {first = current;}
      if (last != NULL) {last->next = current;}
      last = current;
    // CUBE CREATION
    } if (name == "CUBE") {
      std::cout << name << "                  \n===========" << std::endl;
      INFILE >> x >> y >> z >> delx >> delta >> hole_str;
      if (hole_str == "false" || hole_str == "FALSE") {hole = false;}
      else {hole = true;}
      origin = Vertex(x, y, z);
      current = new Model(origin, hole);
      std::cout << "Generating Vertices...      " << std::endl;
      cubeGen(current, delx, delta);
      std::cout << "Slicing...                  " << std::endl;
      slicer(current, true);
      std::cout << "Generating Mesh...          " << std::endl;
      meshGen(current);
      if (first == NULL) {first = current;}
      if (last != NULL) {last->next = current;}
      last = current;
    // RECTANGLE CREATION
    } if (name == "RECT") {
      std::cout << name << "                  \n===========" << std::endl;
      INFILE >> x >> y >> z >> delx >> dely >> delz >> delta >> hole_str;
      if (hole_str == "false" || hole_str == "FALSE") {hole = false;}
      else {hole = true;}
      origin = Vertex(x, y, z);
      current = new Model(origin, hole);
      std::cout << "Generating Vertices...      " << std::endl;
      rectGen(current, delx, dely, delz, delta);
      std::cout << "Slicing...                  " << std::endl;
      slicer(current, true);
      std::cout << "Generating Mesh...          " << std::endl;
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
  }
  // Y Mesh Gen
  if (model->getNumYPlanes() != 0) {
    size = model->getNumYPlanes();
    for (uint i=1; i<size; i++) {
      Plane p1 = model->getPlaneY(i-1);
      Plane p2 = model->getPlaneY(i);
      //std::cout << p2.getVertex(0)->getY() << std::endl;
      endLow = false;
      endHigh = false;
      num1 = 1;
      num2 = 1;
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


void stlWriter(Model* first_model, std::string fileName){
  std::ofstream STLFILE("../STLs/" + fileName + ".stl");
  Model* current_model = first_model;
  Triangle triangle;
  Vertex* vertex;
  uint size;
  bool end = false;
  uint total_size = 0;
  uint m = 0;
  if (current_model == NULL) {end=true;}
  while (!end) {
    total_size += current_model->getMeshSize();
    if (current_model->next == NULL) {end=true;}
    current_model = current_model->next;
  }
  std::cout << total_size << std::endl;
  current_model = first_model;
  end = false;
  if (current_model == NULL) {end=true;}
  //STLFILE << "solid Sphere_Model\n";
  while (!end) {
    STLFILE << "solid Sphere_Model\n";
    size = current_model->getMeshSize();
    // Add all Triangles
    for(uint i=0; i<size; i++,m++) {
      triangle = current_model->getMesh(i);
      STLFILE << "\tfacet normal " << triangle.getNx() << " " <<
                 triangle.getNy() << " " << triangle.getNz() << "\n";
      STLFILE << "\t\touter loop \n";
      for (int j=0; j<3; j++) {
        vertex = current_model->getMesh(i).get(j);
        STLFILE << "\t\t\tvertex " << vertex->getX() << " " <<
                   vertex->getY() << " " << vertex->getZ() << "\n";
      }
      STLFILE << "\t\tendloop\n";
      STLFILE << "\tendfacet\n";
      printUpdate(m, total_size);
    }
    STLFILE << "endsolid Sphere_Model\n";
    if (current_model->next == NULL) {end=true;}
    current_model = current_model->next;
  }
  STLFILE.close();
}
