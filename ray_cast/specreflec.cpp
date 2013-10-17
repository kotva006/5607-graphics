#include <cstdlib>
#include <cstdio>

#include "scene.h"
#include "vectors.h"
#include "specreflec.h"

float *willHitWhere(Scene *s, float *r_dir, float *o) {

  float *r = vec::normalize(r_dir);

  float *ret = (float*) malloc(sizeof(float) * 4); //Where is hit and what object
  ret[0] = -100000;
  float t1, t2, t = 0;
  float t_temp = 100000;
  unsigned int k = 0;

  for ( k = 0; k < s->object.size(); k++) {

    float d = pow(s->object[k]->B(o,r),2) - (4*s->object[k]->C(o));

    if (d >= 0 )  {

      t1 = (-1 * s->object[k]->B(o,r) - sqrt(d)) / 2;
      t2 = (-1 * s->object[k]->B(o,r) + sqrt(d)) / 2;
      t  = ((t1 < t2) ? t1 : t2);

      if ( t < t_temp) {

        t_temp = t;
        ret = vec::add(o,vec::mul(t,r));
        ret[3] = (float) k;

      }
    }
  }
  return ret;
}          


// o is the position that ray hits
// k is what it hits
float *specShadeRay(Scene *s, int k, float *o, float *R, int c) {

  float *hitStruct = willHitWhere(s, R, o);
  if (hitStruct[0] == -100000) {
    return s->bkgcolor;
  }

  int nk = hitStruct[3];
  float *r = hitStruct;
  float odr = s->object[nk]->mc[0]; 
  float odg = s->object[nk]->mc[1]; 
  float odb = s->object[nk]->mc[2]; 
  float osr = s->object[nk]->mc[3]; 
  float osg = s->object[nk]->mc[4]; 
  float osb = s->object[nk]->mc[5]; 
  float ka  = s->object[nk]->mc[6]; 
  float kd  = s->object[nk]->mc[7]; 
  float ks  = s->object[nk]->mc[8]; 
  float n   = s->object[nk]->mc[9]; 
  //float al  = s->object[nk]->mc[10];
  //float ni = s->object[nk]->mc[11];

  float Fnot = s->object[k]->mc[8];

  float *V = vec::normalize(vec::sub(s->eye,r));
  float *N = vec::div(vec::sub(r,s->object[nk]->position),s->object[nk]->radius);
  float *I = vec::negate(R);
  float Fr = Fnot + (1 - Fnot) * pow((1 - vec::dot(I,N)), 5);


  unsigned int w,m;
  float *ret = (float *) malloc(sizeof(float) * 3);
  float *spec = (float *) malloc(sizeof(float) * 3);
  float *nr   = (float *) malloc(sizeof(float) * 3);
  float temp_r1, temp_r2, temp_r3, d, t, t1, t2 = 0;

  nr[0] = hitStruct[0];
  nr[1] = hitStruct[1];
  nr[2] = hitStruct[2];

  for (m = 0; m < s->lights.size(); m++) {
  
    float lr = s->lights[m]->color[0];
    float lg = s->lights[m]->color[1];
    float lb = s->lights[m]->color[2];

    float *L;
    if (s->lights[m]->w == 0) {
      L = vec::normalize(vec::mul(-1, s->lights[m]->position));
    } else {
      L = vec::normalize(vec::sub(s->lights[m]->position, r));
    }


    float *H = vec::normalize(vec::add(L,V));
    float NoL = fmax(0.0,vec::dot(N,L));
    float NoH = fmax(0.0,vec::dot(N,H));
    float mod = 1.0;

    for (w = 0; w < s->object.size(); w++) {
    
      d = (float) pow(s->object[w]->B(r,vec::normalize(s->lights[m]->position)),2)-
                  (4*s->object[w]->C(r));

      if (d >= 0) {
        t1 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))-sqrt(d)) / 2;
        t2 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))+sqrt(d)) / 2;
        t  = ((t1 < t2) ? t1 : t2);

        mod = ((t > 0.1) ? 0 : 1.0);
        w = s->object.size();
      }
    }

    temp_r1 += mod * lr * ((kd*odr*NoL) + (ks*osr*pow(NoH,n)));
    temp_r2 += mod * lg * ((kd*odg*NoL) + (ks*osg*pow(NoH,n)));
    temp_r3 += mod * lb * ((kd*odb*NoL) + (ks*osb*pow(NoH,n)));

  }
  
  if ( c > 4) {
    spec[0] = 0;
    spec[1] = 0;
    spec[2] = 0;
  } else {
    c++;
    float *nR = vec::sub(vec::mul(2 * vec::dot(N,I),N),I);
    spec = specShadeRay(s,nk,nr,nR,c);
  }

  ret[0] = ka*odr + temp_r1 + Fr*spec[0];
  ret[1] = ka*odg + temp_r2 + Fr*spec[1];
  ret[2] = ka*odb + temp_r3 + Fr*spec[2];

  if (ret[0] > 1.0) {ret[0] = 1.0;}
  if (ret[1] > 1.0) {ret[1] = 1.0;}
  if (ret[2] > 1.0) {ret[2] = 1.0;}

  return ret;
}

float *refracRay(Scene* s,int k, float* o, float* T, float iior, int c) {
  
  float *hitStruct = willHitWhere(s, T, o);
  if (hitStruct[0] == -100000) {
    return s->bkgcolor;
  }

  int nk = hitStruct[3];
  float *r = hitStruct;
  float odr = s->object[nk]->mc[0]; 
  float odg = s->object[nk]->mc[1]; 
  float odb = s->object[nk]->mc[2]; 
  float osr = s->object[nk]->mc[3]; 
  float osg = s->object[nk]->mc[4]; 
  float osb = s->object[nk]->mc[5]; 
  float ka  = s->object[nk]->mc[6]; 
  float kd  = s->object[nk]->mc[7]; 
  float ks  = s->object[nk]->mc[8]; 
  float n   = s->object[nk]->mc[9]; 
  float al  = s->object[nk]->mc[10];
  float ior = s->object[nk]->mc[11];

  float ni,nt = 0;

  if (iior == ior) {
    ni = ior;
    nt = 1.0;
  } else {
    ni = 1.0;
    nt = ior;
  }

  //float Fnot = s->object[k]->mc[8];
  float Frn  = pow((nt-ni)/(nt+ni),2);

  float *V = vec::normalize(vec::sub(s->eye,r));
  float *N = vec::div(vec::sub(r,s->object[nk]->position),s->object[nk]->radius);
  float *I = vec::negate(R);
  //float Fr = Fnot + (1 - Fnot) * pow((1 - vec::dot(I,N)), 5);
  float Fr = Frn + (1 - Frn) * pow((1 - vec::dot(I,N)),5);

  unsigned int w,m;
  float *ret = (float *) malloc(sizeof(float) * 3);
  float *spec = (float *) malloc(sizeof(float) * 3);
  float *refrac = (float *) malloc(sizeof(float) * 3);

  float *nr   = (float *) malloc(sizeof(float) * 3);
  float temp_r1, temp_r2, temp_r3, d, t, t1, t2 = 0;

  nr[0] = hitStruct[0];
  nr[1] = hitStruct[1];
  nr[2] = hitStruct[2];

  for (m = 0; m < s->lights.size(); m++) {
  
    float lr = s->lights[m]->color[0];
    float lg = s->lights[m]->color[1];
    float lb = s->lights[m]->color[2];

    float *L;
    if (s->lights[m]->w == 0) {
      L = vec::normalize(vec::mul(-1, s->lights[m]->position));
    } else {
      L = vec::normalize(vec::sub(s->lights[m]->position, r));
    }


    float *H = vec::normalize(vec::add(L,V));
    float NoL = fmax(0.0,vec::dot(N,L));
    float NoH = fmax(0.0,vec::dot(N,H));
    float mod = 1.0;

    for (w = 0; w < s->object.size(); w++) {
    
      d = (float) pow(s->object[w]->B(r,vec::normalize(s->lights[m]->position)),2)-
                  (4*s->object[w]->C(r));

      if (d >= 0) {
        t1 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))-sqrt(d)) / 2;
        t2 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))+sqrt(d)) / 2;
        t  = ((t1 < t2) ? t1 : t2);

        mod = ((t > 0.1) ? 0 : 1.0);
        w = s->object.size();
      }
    }

    temp_r1 += mod * lr * ((kd*odr*NoL) + (ks*osr*pow(NoH,n)));
    temp_r2 += mod * lg * ((kd*odg*NoL) + (ks*osg*pow(NoH,n)));
    temp_r3 += mod * lb * ((kd*odb*NoL) + (ks*osb*pow(NoH,n)));

  }
  
  if ( c > 4) {
    spec[0] = 0;
    spec[1] = 0;
    spec[2] = 0;
    refrac[0] = 0;
    refrac[1] = 0;
    refrac[2] = 0;
  } else {
    c++;
    float *nR = vec::sub(vec::mul(2 * vec::dot(N,I),N),I);
    float *nT = vec::add(vec::mul(sqrt(1-(pow(ni/nt,2)*(1-pow(vec::dot(N,I),2)))),
                                 vec::negate(N)), vec::mul((ni/nt),(vec::sub(vec::mul(
                                                           vec::dot(N,I),N),I))));
    spec = specShadeRay(s,nk,nr,nR,c);
    if (ior != 1) {
      refrac = refracRay(s,nk,nr,nT,ni,c);
    } else {
      refrac[0] = 0;
      refrac[1] = 0;
      refrac[2] = 0;
    }

  }

  ret[0] = ka*odr + temp_r1 + Fr * spec[0] + (1-Fr) * (1 - al) * refrac[0];
  ret[1] = ka*odg + temp_r2 + Fr * spec[1] + (1-Fr) * (1 - al) * refrac[1];
  ret[2] = ka*odb + temp_r3 + Fr * spec[2] + (1-Fr) * (1 - al) * refrac[2];

  if (ret[0] > 1.0) {ret[0] = 1.0;}
  if (ret[1] > 1.0) {ret[1] = 1.0;}
  if (ret[2] > 1.0) {ret[2] = 1.0;}

  return ret;
}
