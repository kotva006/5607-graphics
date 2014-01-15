#ifndef _VECTOR_H
#define _VECTOR_H

#include <iostream>
#include <cmath>

class Vec{
  public:
    float x;
    float y;
    float z;
    Vec(){};
    ~Vec(){};
    Vec(float X, float Y, float Z)
      : x(X), y(Y), z(Z) {};
    Vec operator+(const Vec& other);
    Vec operator-(const Vec& other);
//    Vec operator-();
    void print() {
      std::cout<<x<<" "<<y<<" "<<z<<std::endl;
    };
} ;

/*
 *  --------------------------------------------------------------------------
 * These are special and must be done in the right order (see above class)
 */
Vec operator*(const float other, const Vec& v2);
Vec operator*(const Vec& v1, const float other);

Vec operator/(const Vec& v1, const float& other); 
/*
 *-------------------------- End Special-ness---------------------------------
 */

float dot(Vec v1, Vec v2); 

Vec cross(Vec v1, Vec v2);

float len(Vec v);

Vec norm(Vec v);

#endif
