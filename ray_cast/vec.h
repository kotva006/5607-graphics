#ifndef VEC_H
#define VEC_H

class vec {
public:
  vec(){};
  ~vec(){};
  float x;
  float y;
  float z;
  vec(float r, float b, float g)
    : x(r), y(b), z(g) {}; 
} ;

  vec* crossproduct(vec *, vec *);

#endif
