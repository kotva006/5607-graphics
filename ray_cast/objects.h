#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

#include "vectors.h"

class Objects {
  public:
    Objects(){};
    virtual ~Objects(){};
    float position[3];
    float mc[12];
} ;

class Vertex {
  public:
    Vertex(){};
    ~Vertex(){};
    Vertex(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    std::string tag;
    float pos[3];
} ;

class TexCor {
  public:
    TexCor(){};
    ~TexCor(){};
    TexCor(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    std::string tag;
    float pos[3];
} ;

class NorDir {
  public:
    NorDir(){};
    ~NorDir(){};
    NorDir(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    std::string tag;
    float pos[3];
} ;

class Face {
  public:
    Face(){};
    ~Face(){};
    Face(std::istringstream& iss) {
      iss >> this->data[0];
      iss >> this->data[1];
      iss >> this->data[2];
    }
    std::string data[3];
    float a;
    float b;
    float c;
    float d;
    float *N;
    float alpha;
    float beta;
    float gamma;
    float Area;
    void D(float *p0, float *p1, float *p2) {
      float *e1 = vec::sub(p1,p0);
      float *e2 = vec::sub(p2,p0);
      float *n  = vec::crossproduct(e1,e2);
      this->a = n[0];
      this->b = n[1];
      this->c = n[2];
      this->d = 0 - n[0]*p0[0] - n[1]*p0[1] - n[2]*p0[2];
      this->N = vec::normalize(n);
      this->Area = 0.5 * vec::len(n);
    };
    float *getBCC(float *p, float *p0, float *p1, float *p2) {
      float *ret = (float*) malloc(sizeof(float) * 3);
      float *e1  = vec::sub(p1,p0);
      float *e2  = vec::sub(p2,p0);
      float *e3  = vec::sub(p,p1);
      float *e4  = vec::sub(p,p2);
      float a_area = 0.5 * vec::len(vec::crossproduct(e3,e4));
      float b_area = 0.5 * vec::len(vec::crossproduct(e4,e2));
      float c_area = 0.5 * vec::len(vec::crossproduct(e1,e3));
      this->alpha = a_area / this->Area;
      this->beta  = b_area / this->Area;
      this->gamma = c_area / this->Area;
      if (!(this->alpha >= 0 && this->alpha <= 1 && 
            this->beta >= 0 && this->beta <= 1 &&
            this->gamma >= 0 && this->gamma <= 1)) { return NULL;}
      if (!(this->alpha + this->beta + this->gamma  -1 > -0.0001)) {return NULL;}
      ret[0] = this->alpha;
      ret[1] = this->beta;
      ret[2] = this->gamma;
      return ret;
    } ;
      
    bool isNotZero(float *r) {
      if ((this->a*r[0] + this->b*r[1] + this->c*r[2]) == 0){
        return false;}
      return true;
    } ;
    float t(float *o, float *r) { //takes in orgin and dir
      return -1.0 * (this->a*o[0] + this->b*o[1] + this->c*o[2] + this->d) /
                    (this->a*r[0] + this->b*r[1] + this->c*r[2]);
    };
} ;

class Polygon: public Objects{
  public:
    Polygon(){};
    ~Polygon(){};
    std::vector<Vertex *> vertex;
    std::vector<TexCor *> texcor;
    std::vector<NorDir *> nordir;
    std::vector<Face *> face;
    std::string texture;

} ;
      

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
    float A(){return 1.0;};
    float B(float *o, float *d) {
      return (float) (2.0 * (d[0] * (o[0] - this->position[0]) +
                             d[1] * (o[1] - this->position[1]) +
                             d[2] * (o[2] - this->position[2])));
    };
    //Takes in the origin of the ray
    float C(float *o) {
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
