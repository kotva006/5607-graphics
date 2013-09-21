#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "objects.h"
#include "vec.h"

class Scene {
  public:
    Scene() {};
    ~Scene(){};
    vec *eye;
    vec *viewdir;
    vec *updir;
    float viewdist;
    float fovv;
    float aspect;
    float pixheight;
    float pixwidth;
    vec *bkgcolor;
    std::vector<Objects*> *object;

    void createScene(char*);
    void findPixWidth();
} ;

#endif
