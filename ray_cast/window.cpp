#include <math.h>
#include <iostream>

#include "window.h"
#include "scene.h"
#include "vectors.h"

#define PI 3.14159265

using namespace vec;

Window::Window(Scene *s) {

  float *u = normalize(crossproduct(s->viewdir, s->updir));
  float *v = normalize(crossproduct(u, s->viewdir));
  float *n = normalize(s->viewdir);
  
  std::cout << "U: " << u[0] << " " << u[1] << " " << u[2] << "\n";
  std::cout << "V: " << v[0] << " " << v[1] << " " << v[2] << "\n";
  std::cout << "N: " << n[0] << " " << n[1] << " " << n[2] << "\n";

  this->h  = 2.0 * s->viewdist * (float) tan (((double) s->fovv * PI / 180.0) / 2);
  this->w  = this->h * s->aspect;

  std::cout << "H: " << this->h << "\n";

  this->ul = add(s->eye, add(mul(s->viewdist,n), 
                 sub(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->ur = add(s->eye, add(mul(s->viewdist,n), 
                 add(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->ll = add(s->eye, sub(mul(s->viewdist,n), 
                 sub(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->lr = add(s->eye, sub(mul(s->viewdist,n), 
                 add(mul((this->h/2.0),v), mul((this->w/2.0),u))));
  
  std::cout << "UL: " << this->ul[0] << " " << this->ul[1] << " " << this->ul[2] << "\n";
  std::cout << "UR: " << this->ur[0] << " " << this->ur[1] << " " << this->ur[2] << "\n";
  std::cout << "LL: " << this->ll[0] << " " << this->ll[1] << " " << this->ll[2] << "\n";
  std::cout << "LR: " << this->lr[0] << " " << this->lr[1] << " " << this->lr[2] << "\n";
  this->deltah = div(sub(this->ur, this->ul), (float)(s->pixwidth - 1));
  this->deltav = div(sub(this->ll, this->ul), (float)(s->pixheight - 1));

}

