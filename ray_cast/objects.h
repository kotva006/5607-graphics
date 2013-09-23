#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>

class Objects {
  public:
    Objects(){};
    ~Objects(){};
    float position[3];
    float materialcolor[3];
};

class Sphere: public Objects{
  public:
    Sphere(){};
    Sphere(float m1,float m2,float m3) {
      this->materialcolor[0] = m1;
      this->materialcolor[1] = m2;
      this->materialcolor[2] = m3;
    };
    float A(){return 1.0;};
    float B(float *o, float *d) {
      return (float) (2.0 * (d[0] * (o[0] - this->position[0]) +
                             d[1] * (o[1] - this->position[1]) +
                             d[2] * (o[2] - this->position[2])));
    };
    float C(float *o, float *d) {
      return (float) 
             pow((double) (o[0] - this->position[0]), 2.0) +
             pow((double) (o[1] - this->position[1]), 2.0) +
             pow((double) (o[2] - this->position[2]), 2.0) -
             pow((double) this->radius, 2.0);
    };
    ~Sphere(){};
    float radius; 
};

#endif
