#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>

#include "objects.h"

class Scene {
  public:
    Scene() { this->error = false;};
    ~Scene(){};
    float eye[3];
    float viewdir[3];
    float updir[3];
    float viewdist;
    float fovv;
    float aspect;
    int   pixheight;
    int   pixwidth;
    float bkgcolor[3];
    std::vector<Objects *> object;
    std::vector<Light *> lights;
    bool error;

    void createScene(char*);
    void findPixWidth();
} ;

#endif
