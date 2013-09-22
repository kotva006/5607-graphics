#ifndef OBJECTS_H
#define OBJECTS_H


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
    ~Sphere(){};
    float radius; 
};

#endif
