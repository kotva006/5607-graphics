
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

#include "scene.h"

Scene* Scene::createScene(char *name) {

  std::ifstream inFile;
  inFile.open(name);
  if (!inFile.is_open()) {
    return NULL;
  }

  Scene *file = new Scene();
  
  std::string line = ""; 
  std::string value = "";
  
  float mc1 = 0.0;
  float mc2 = 0.0;
  float mc3 = 0.0;
    
  while ( inFile.getline(inFile, line)) {
  
    std::istringstream iss(line);
    iss >> value;

    switch(value) {
      case "eye":
        iss >> file->eye[0];
        iss >> file->eye[1];
        iss >> file->eye[2];
        break;
      case "viewdir":
        iss >> file->viewdir[0];
        iss >> file->viewdir[1];
				iss >> file->viewdir[2];
				break;
			case "updir":
				iss >> file->updir[0];
				iss >> file->updir[1];
				iss >> file->updir[2];
				break;
			case "viewdist":
				iss >> file->viewdist;
				break;
			case "fovv":
				iss >> file->fovv;
				break;
			case "aspect":
				iss >> file->aspect;
        break;
      case "pixheight":
        iss >> file->pixheight;
        file->findPixHeight();
        break;
      case "bkgcolor":
				iss >> file->bkgcolor[0];
        iss >> file->bkgcolor[1];
				iss >> file->bkgcolor[2];
        break;
      case "materialcolor":
        iss >> mc1;
        iss >> mc2;
        iss >> mc3;
        break;
      default:
        std::cout << "Invalid option " + value;
    }

  }

  return file;

}

   

  
