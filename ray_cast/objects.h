#ifndef OBJECTS_H
#define OBJECTS_H

#include <cmath>
#include <sstream>
#include <vector>
#include <string>

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
    Vertex(std::istringstream& iss) {
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    float pos[3];
} ;

class TexCor {
  public:
    TexCor(){};
    ~TexCor(){};
    TexCor(std::istringstream& iss) {
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    float pos[3];
} ;

class NorDir {
  public:
    NorDir(){};
    ~NorDir(){};
    NorDir(std::istringstream& iss) {
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    float pos[3];
} ;

class Face {
  public:
    Face(){};
    ~Face(){};
    Face(std::istringstream& iss) {
      iss >> this->pos[0];
      iss >> this->pos[1];
      iss >> this->pos[2];
    }
    float pos[3];
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
