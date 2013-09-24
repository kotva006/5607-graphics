#include <cstdlib>
#include <cmath>

/*
 * This file defines the functions for the float vectors
 * In here are the cross-product, normalization, add, sub, mul, and div
 */

namespace vec {

  float* add(float *v1, float *v2) {
  
    float *r = (float*) malloc(sizeof(float) *3);
    r[0] = v1[0] + v2[0];
    r[1] = v1[1] + v2[1];
    r[2] = v1[2] + v2[2];
    return r;

  }  

  float* sub(float *v1, float *v2) {
	
	  float *r = (float*) malloc(sizeof(float) *3);
	  r[0] = v1[0] - v2[0];
	  r[1] = v1[1] - v2[1];
	  r[2] = v1[2] - v2[2];
	  return r;

  }

  float* mul(float c, float *v) {

    float *r = (float*) malloc(sizeof(float) * 3);
    r[0] = v[0] * c;
    r[1] = v[1] * c;
    r[2] = v[2] * c;
    return r;

  } 

  float* div(float *v, float c) {

    float *r = (float*) malloc(sizeof(float) * 3);
    r[0] = v[0] / c;
    r[1] = v[1] / c;
    r[2] = v[2] / c;
    return r;

  }

  float* crossproduct(float* v1,float* v2) {
    float *rvec = (float*) malloc(sizeof(float) * 3);
    rvec[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    rvec[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    rvec[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    return rvec;
  } 

  float* normalize(float* v) {
  
    float length = (float) sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));
    float *rv = (float*) malloc(sizeof(float) * 3);
    rv[0] = v[0] / length;
    rv[1] = v[1] / length;
    rv[2] = v[2] / length;
    return rv;

  }

}

