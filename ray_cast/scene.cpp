
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <math.h>

#include "scene.h"
#include "vec.h"

#define PI 3.14159265

using namespace std;

void Scene::createScene(char *name) {

  ifstream inFile;
  inFile.open(name);
  if (!inFile.is_open()) {
    return;
  }
  
  string line = ""; 
  string value = "";
  
  float mc1 = 0.0;
  float mc2 = 0.0;
  float mc3 = 0.0;
    
  while ( getline(inFile, line)) {
  
    istringstream iss(line);
    iss >> value;
    cout << value + "\n";

    if (value.compare("eye") == 0) {
        cout << "NOO";
        float temp = 0.0;
        iss >>  temp;
        (this->eye->x = temp;
        //cout << "stuff";
        //iss >> this->eye->y;
        //iss >> this->eye->z;
    } else if (value.compare("viewdir") == 0) {
        iss >> this->viewdir->x;
        iss >> this->viewdir->y;
				iss >> this->viewdir->z;
	  } else if (value.compare("updir") == 0) {
				iss >> this->updir->x;
				iss >> this->updir->y;
				iss >> this->updir->z;
	  } else if (value.compare("viewdist") == 0) {
				iss >> this->viewdist;
		} else if (value.compare("fovv") == 0) {
				iss >> this->fovv;
		} else if (value.compare("aspect") == 0) {
				iss >> this->aspect;
    } else if (value.compare("pixheight") == 0) {
        iss >> this->pixheight;
        //this->findPixWidth();
    } else if (value.compare("bkgcolor") == 0) {
				iss >> this->bkgcolor->x;
        iss >> this->bkgcolor->y;
				iss >> this->bkgcolor->z;
    } else if (value.compare("materialcolor") == 0) {
        // store color attributes
        iss >> mc1;
        iss >> mc2;
        iss >> mc3;
        // First gets the color of the object,
        // then gets the type of shape
        getline(inFile, line);
        istringstream sss(line);
        sss >> value;
        if (value.compare("sphere")) {
          // Creates a sphere with the input color extracted above
          // Then sets the position and the radius of the sphere
          Sphere *sphere = new Sphere(mc1,mc2,mc3);
          sss >> sphere->position->x;
          sss >> sphere->position->y;
          sss >> sphere->position->z;
          sss >> sphere->radius;
          this->object->push_back(dynamic_cast<Objects*>(sphere));
        }
    } else {
        cout << "Invalid option " + value + "\n";
    }
  }
}
