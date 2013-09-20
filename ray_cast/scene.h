#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "objects.h"

class Scene {
  public:
    Scene() {};
    ~Scene(){};
    float eye[3];
    float viewdir[3];
    float updir[3];
    float viewdist;
    float fovv;
    float aspect;
    float pixheight;
    float pixwidth;
    float bkgcolor[3];
    std::vector<Objects*> *object;

    void createScene(char*);
    void findPixHeight(){};
} ;

#endif
