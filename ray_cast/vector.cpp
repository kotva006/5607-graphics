
#include <iostream>
#include <cmath>

#include "vector.h"

//Vector addition
Vec Vec::operator+(const Vec& other) {
  return Vec(other.x+x,other.y+y,other.z+z);
}
//Vector subtraction
Vec Vec::operator-(const Vec& other) {
  return Vec(x-other.x,y-other.y,z-other.z);
}

/*Vec operator-(const Vec& v1, const Vec& v2) {
  return Vec(v1.x-v2.x,v1.y-v2.y,v1.z-v2.z);
}*/
//Vector negation
/*Vec Vec::operator-() {
  return Vec(-1*x,-1*y,-1*z);
}*/

/*
 *  --------------------------------------------------------------------------
 * These are special and must be done in the right order (see above class)
 */
Vec operator*(const float other, const Vec& v2) {
  return Vec(other*v2.x,other*v2.y,other*v2.z);
}
Vec operator*(const Vec& v1, const float other) {
  return Vec(other*v1.x,other*v1.y,other*v1.z);
}

Vec operator/(const Vec& v1, const float& other) {
  return Vec(v1.x/other,v1.y/other,v1.z/other);
}
/*
 *-------------------------- End Special-ness---------------------------------
 */

float dot(Vec v1, Vec v2) {
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vec cross(Vec v1, Vec v2) {
  return Vec( v1.y*v2.z - v1.z*v2.y,
              v1.z*v2.x - v1.x*v2.z,
              v1.x*v2.y - v1.y*v2.x );
}

float len(Vec v) {
  return sqrt(pow(v.x,2)+pow(v.y,2)+pow(v.z,2));
}

Vec norm(Vec v) {
  float l = len(v);
  return Vec( v.x/l, v.y/l, v.z/l );
}

