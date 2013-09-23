#ifndef PICTURE_H
#define PITCURE_H

#include "scene.h"

class Picture {
  public:
    Picture(){};
    ~Picture(){};
    Picture(Scene*);
    std::string data;
    void append(std::string);
    std::string dump(){return data;};
    
} ;

#endif
