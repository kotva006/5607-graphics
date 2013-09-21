#ifndef OBJECTS_H
#define OBJECTS_H

#include "vec.h"

class Objects {
  public:
    Objects(){};
    ~Objects(){};
    vec *position;
    vec *materialcolor;
};

class Sphere: public Objects{
  public:
    Sphere(){};
    Sphere(float m1,float m2,float m3) {
      this->materialcolor->x = m1;
      this->materialcolor->y = m2;
      this->materialcolor->z = m3;
    };
    ~Sphere(){};
    float radius; 
};

#endif
