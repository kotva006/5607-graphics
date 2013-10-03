#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <sstream>

class Objects {
  public:
    Objects(){};
    ~Objects(){};
    float position[3];
    float mc[10];
};

class Light: public Objects{
  public:
    Light(){};
    Light(std::istringstream& iss) {
      iss >> this->position[0];
      iss >> this->position[1];
      iss >> this->position[2];
      iss >> this->w;
      iss >> this->color[0];
      iss >> this->color[1];
      iss >> this->color[2];
    };
    float w; 
    float color[3];
    ~Light(){};
} ;

class Sphere: public Objects{
  public:
    Sphere(){};
    Sphere(float m1,float m2,float m3) {
      this->mc[0] = m1;
      this->mc[1] = m2;
      this->mc[2] = m3;
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
