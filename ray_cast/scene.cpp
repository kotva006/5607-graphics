
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>

#include "scene.h"

void Scene::createScene(char *name) {

  std::ifstream inFile;
  inFile.open(name);
  if (!inFile.is_open()) {
    return;
  }
  
  std::string line = ""; 
  std::string value = "";
  
  float mc1 = 0.0;
  float mc2 = 0.0;
  float mc3 = 0.0;
    
  while ( getline(inFile, line)) {
  
    std::istringstream iss(line);
    iss >> value;

    if (value.compare("eye") == 0) {
        iss >> this->eye[0];
        iss >> this->eye[1];
        iss >> this->eye[2];
    } else if (value.compare("viewdir") == 0) {
        iss >> this->viewdir[0];
        iss >> this->viewdir[1];
				iss >> this->viewdir[2];
	  } else if (value.compare("updir") == 0) {
				iss >> this->updir[0];
				iss >> this->updir[1];
				iss >> this->updir[2];
	  } else if (value.compare("viewdist") == 0) {
				iss >> this->viewdist;
		} else if (value.compare("fovv") == 0) {
				iss >> this->fovv;
		} else if (value.compare("aspect") == 0) {
				iss >> this->aspect;
    } else if (value.compare("pixheight") == 0) {
        iss >> this->pixheight;
        this->findPixHeight();
    } else if (value.compare("bkgcolor") == 0) {
				iss >> this->bkgcolor[0];
        iss >> this->bkgcolor[1];
				iss >> this->bkgcolor[2];
    } else if (value.compare("materialcolor") == 0) {
        // store color attributes
        iss >> mc1;
        iss >> mc2;
        iss >> mc3;
        // First gets the color of the object,
        // then gets the type of shape
        getline(inFile, line);
        std::istringstream sss(line);
        sss >> value;
        if (value.compare("sphere")) {
          // Creates a sphere with the input color extracted above
          // Then sets the position and the radius of the sphere
          Sphere *sphere = new Sphere(mc1,mc2,mc3);
          sss >> sphere->position[0];
          sss >> sphere->position[1];
          sss >> sphere->position[2];
          sss >> sphere->radius;
          this->object->push_back(dynamic_cast<Objects*>(sphere));
        }
    } else {
        std::cout << "Invalid option " + value + "\n";
    }
  }
}

   

  
