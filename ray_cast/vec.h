#ifndef VEC_H
#define VEC_H

class vec {
public:
  float x;
  float y;
  float z;
  vec()
    : x(0.0), y(0.0), z(0.0) {};
  ~vec(){};
  vec(float r, float b, float g)
    : x(r), y(b), z(g) {}; 
} ;

  vec* crossproduct(vec *, vec *);

#endif
