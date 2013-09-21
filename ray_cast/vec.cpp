
#include "vec.h"

vec* crossproduct(vec *v1, vec *v2) {
      vec *rvec = new vec();
      rvec->x = (v1->y * v2->z) - (v2->y * v1->z);
      rvec->y = (v1->x * v2->z) + (v2->x * v1->z);
      rvec->z = (v1->x * v2->y) - (v1->y * v2->x);
      return rvec;
    } ;
        
