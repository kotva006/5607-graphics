#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "objects.h"

class Scene {
  public:
    Scene() { this->error = false;};
    ~Scene(){};
    Vec   eye;
    Vec   viewdir;
    Vec   updir;
    float viewdist;
    float fovv;
    float aspect;
    int   pixheight;
    int   pixwidth;
    Vec   bkgcolor;
    std::vector<Objects *> object;
    std::vector<Light *> lights;
    bool error;

    void createScene(char*);
    void findPixWidth();
} ;

#endif
