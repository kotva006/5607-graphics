#ifndef PICTURE_H
#define PITCURE_H

#include "scene.h"

class Picture {
  public:
    Picture(){};
    ~Picture(){};
    Picture(Scene*);
    std::string data;
    
} ;

#endif
