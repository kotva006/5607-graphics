#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <string>

#include "vector.h"

class Objects {
  public:
    Objects(){};
    virtual ~Objects(){};
    Vec pos;
    float mc[12];
} ;

class VerBase {
  public:
    VerBase(){};
    virtual ~VerBase() {};
    Vec pos;
    std::string tag;
} ;

class Vertex : public VerBase {
  public:
    Vertex(){};
    ~Vertex(){};
    Vertex(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos.x;
      iss >> this->pos.y;
      iss >> this->pos.z;
    }
} ;

class TexCor : public VerBase {
  public:
    TexCor(){};
    ~TexCor(){};
    Vec pos;
    
    TexCor(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos.x;
      iss >> this->pos.y;
      iss >> this->pos.z;
    }
} ;

class NorDir : public VerBase {
  public:
    NorDir(){};
    ~NorDir(){};
    Vec pos;
    NorDir(std::istringstream& iss, int count) {
      std::ostringstream ss;
      ss << "v" << count;
      this->tag = ss.str();
      iss >> this->pos.x;
      iss >> this->pos.y;
      iss >> this->pos.z;
    }
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
    Vec   N;
    float alpha;
    float beta;
    float gamma;
    float Area;
    void D(Vec p0, Vec p1, Vec p2) {
      Vec e1 = p1-p0;
      Vec e2 = p2-p0;
      Vec n  = cross(e1,e2);
      this->a = n.x;
      this->b = n.y;
      this->c = n.z;
      this->d = 0 - n.x*p0.x - n.y*p0.y - n.z*p0.z;
      this->N = norm(n);
      this->Area = 0.5 * len(n);
    };
    Vec *getBCC(Vec p, Vec p0, Vec p1, Vec p2) {
      //float *ret = (float*) malloc(sizeof(float) * 3);
      Vec e1  = p1-p0;
      Vec e2  = p2-p0;
      Vec e3  = p-p1;
      Vec e4  = p-p2;
      float a_area = 0.5 * len(cross(e3,e4));
      float b_area = 0.5 * len(cross(e4,e2));
      float c_area = 0.5 * len(cross(e1,e3));
      this->alpha = a_area / this->Area;
      this->beta  = b_area / this->Area;
      this->gamma = c_area / this->Area;
      if (!(this->alpha >= 0 && this->alpha <= 1 && 
            this->beta >= 0 && this->beta <= 1 &&
            this->gamma >= 0 && this->gamma <= 1)) { return NULL;}
      if (!(this->alpha + this->beta + this->gamma  -1 > -0.0001)) {return NULL;}
      return new Vec(this->alpha,this->beta,this->gamma);
    } ;
      
    bool isNotZero(Vec r) {
      if ((this->a*r.x + this->b*r.y + this->c*r.z) == 0){
        return false;}
      return true;
    } ;
    float t(Vec o, Vec r) { //takes in orgin and dir
      return -1.0 * (this->a*o.x + this->b*o.y + this->c*o.z + this->d) /
                    (this->a*r.x + this->b*r.y + this->c*r.z);
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
      iss >> this->pos.x;
      iss >> this->pos.y;
      iss >> this->pos.z;
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
    float B(Vec o, Vec d) {
      return (float) (2.0 * (d.x[0] * (o.x - this->pos.x) +
                             d.y[1] * (o.y - this->pos.y) +
                             d.z[2] * (o.z - this->pos.z)));
    };
    //Takes in the origin of the ray
    float C(Vec o) {
      return (float) 
             pow((double) (o.x - this->pos.x), 2.0) +
             pow((double) (o.y - this->pos.y), 2.0) +
             pow((double) (o.z - this->pos.z), 2.0) -
             pow((double) this->radius, 2.0);
    };
    ~Sphere(){};
    float radius; 
};

#endif
