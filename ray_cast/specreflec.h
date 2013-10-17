#ifndef SPEC_H
#define SPEC_H

#include "scene.h"

float *specShadeRay(Scene*,int,float*,float*,int);
float *reflecShadeRay(Scene*,int,float*,int);

#endif
