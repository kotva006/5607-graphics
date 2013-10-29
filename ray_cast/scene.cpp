#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "scene.h"
#include "vectors.h"
#include "objects.h"

using namespace std;

void setmc(string line, float *array) {
  istringstream iss(line);
  int i = 0;
  iss >> line;
  for (i = 0; i < 12; i++) {
    iss >> array[i];
    //cout << "Object color: " << array[i] << "\n";
  }
}

void setTmc(float *array) {
  int i = 0;
  for (i = 0; i < 12; i++) {
    array[i] = -1;
  }
}

void Scene::createScene(char *name) {

  ifstream inFile;
  inFile.open(name);
  if (!inFile.is_open()) {
    this->error = true;
    return;
  }
  
  string line = ""; 
  string value = "";
  
  //storage for the material colors 
  float mc[12];
  int i = 0;
  int v_count  = 0;
  int vn_count = 0;
  int vt_count = 0;
  
  // Gets each line of the input file, and then adds the data to the scene

  Polygon *poly = new Polygon();
  
  while ( getline(inFile, line)) {
  
    istringstream iss(line);
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
        this->findPixWidth();
    } else if (value.compare("bkgcolor") == 0) {
				iss >> this->bkgcolor[0];
        iss >> this->bkgcolor[1];
        iss >> this->bkgcolor[2];
    } else if (value.compare("light") == 0) {
        Light *light = new Light(iss);
        this->lights.push_back(light); 
    } else if (value.compare("materialcolor")==0){
        // store color attributes
        if (poly->vertex.size() > 0) {
          poly->setMC(mc);
          this->object.push_back(poly);
          poly = new Polygon();
          v_count  = 0;
          vt_count = 0;
          vn_count = 0;
        }
        setmc(line,mc);
    } else if (value.compare("texture") == 0) {
        if (poly->vertex.size() > 0) {
          poly->setMC(mc);
          this->object.push_back(poly);
          poly = new Polygon();
          v_count  = 0;
          vt_count = 0;
          vn_count = 0;
        }
        setTmc(mc);
        iss >> poly->texture;
    } else if (value.compare("sphere") == 0) {
        // Creates a sphere with the input color extracted
        // Then sets the position and the radius of the sphere
        Sphere *sphere = new Sphere();
        iss >> sphere->position[0];
        iss >> sphere->position[1];
        iss >> sphere->position[2];
        iss >> sphere->radius;
        for (i=0; i < 12; i++) {
          sphere->mc[i] = mc[i];
        }
        this->object.push_back(sphere);
    } else if (value.compare("v") == 0) {
        Vertex *vec = new Vertex(iss, v_count);
        v_count++;
        poly->vertex.push_back(vec);
    } else if (value.compare("vt") == 0) {
        TexCor *tex = new TexCor(iss, vt_count);
        vt_count++;
        poly->texcor.push_back(tex);
    } else if (value.compare("vn") == 0) {
        NorDir *dir = new NorDir(iss, vn_count);
        vn_count++;
        poly->nordir.push_back(dir);
    } else if (value.compare("f") == 0) {
        Face *face = new Face(iss);
        poly->face.push_back(face);
    } else if (value.compare("") == 0) {
        //Blank line do nothing
        if (poly->vertex.size() > 0) {
          poly->setMC(mc);
          this->object.push_back(poly);
          poly = new Polygon();
          v_count  = 0;
          vt_count = 0;
          vn_count = 0;
        }
    } else {
        cout << "Invalid option " + value + ".\n";
        value = "";
    }
    value = "";
  }

  if (poly->vertex.size() > 0) {
    poly->setMC(mc);
    this->object.push_back(poly);
  }
  inFile.close();

  float *ret = vec::normalize(this->viewdir);
  this->viewdir[0] = ret[0];
  this->viewdir[1] = ret[1];
  this->viewdir[2] = ret[2];

}

void Scene::findPixWidth() {
  this->pixwidth = (int) (((float) this->pixheight) * this->aspect);
}
