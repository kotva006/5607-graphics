#include <math.h>

#include "window.h"
#include "scene.h"
#include "vectors.h"

#define PI 3.14159265

using namespace vec;

Window::Window(Scene *s) {

  float *u = normalize(crossproduct(s->viewdir, s->updir));
  float *v = normalize(crossproduct(u, s->viewdir));
  float *n = normalize(s->viewdir);

  this->h  = 2.0 * s->viewdist * (float) tan ((double) s->fovv * PI / 180.0);
  this->w  = this->h * s->aspect;

  this->ul = add(s->eye, add(mul(s->viewdist,n), 
                 sub(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->ur = add(s->eye, add(mul(s->viewdist,n), 
                 add(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->ll = add(s->eye, sub(mul(s->viewdist,n), 
                 sub(mul((this->h/2.0),v), mul((this->w/2.0),u))));

  this->ul = add(s->eye, sub(mul(s->viewdist,n), 
                 add(mul((this->h/2.0),v), mul((this->w/2.0),u))));

}

