#ifndef VECTORS_H
#define VECTORS_H

namespace vec {

  float* add(float*, float*);
  float* sub(float*, float*);
  float* mul(float, float*);
  float* div(float*, float);

  float* negate(float*);

  float* crossproduct(float*, float*);
  float dot(float*,float*);

  float* normalize(float*);

}

#endif
