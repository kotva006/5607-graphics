#include <math.h>
#include <iostream>

#include "window.h"
#include "scene.h"

#define PI 3.14159265


/*
 * This is all the information regarding the window into the scene.
 * It defines the frame and how much the ray moves through the frame.
 */

Window::Window(Scene *s) {
  
  Vec u = norm(cross(s->viewdir, s->updir));
  Vec v = norm(cross(u, s->viewdir));
  Vec n = norm(s->viewdir);
  
  //std::cout << "U: " << u[0] << " " << u[1] << " " << u[2] << "\n";
  //std::cout << "V: " << v[0] << " " << v[1] << " " << v[2] << "\n";
  //std::cout << "N: " << n[0] << " " << n[1] << " " << n[2] << "\n";

  this->h  = 2.0 * s->viewdist * (float) tan (((double) s->fovv * PI / 180.0) / 2);
  this->w  = this->h * s->aspect;

  //std::cout << "H: " << this->h << "\n";

  this->ul = s->eye + (s->viewdist*n) + (this->h/2.0)*v - (this->w/2.0)*u;
  this->ur = s->eye + (s->viewdist*n) + (this->h/2.0)*v + (this->w/2.0)*u;
  this->ll = s->eye + (s->viewdist*n) - (this->h/2.0)*v - (this->w/2.0)*u;
  this->lr = s->eye + (s->viewdist*n) - (this->h/2.0)*v + (this->w/2.0)*u;

  std::cout << "UL: " << this->ul.x << " " << this->ul.y << " " << this->ul.z << "\n";
  std::cout << "UR: " << this->ur.x << " " << this->ur.y << " " << this->ur.z << "\n";
  std::cout << "LL: " << this->ll.x << " " << this->ll.y << " " << this->ll.z << "\n";
  std::cout << "LR: " << this->lr.x << " " << this->lr.y << " " << this->lr.z << "\n";

  this->deltah = (this->ur - this->ul) / (s->pixwidth - 1);
  this->deltav = (this->ll - this->ul) / (s->pixheight - 1);

}

