#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>

#include "scene.h"
#include "picture.h"
#include "vectors.h"
#include "window.h"
#include "specreflec.h"

using namespace std;

void print_usage() {
  cout <<  "This command takes only one input.\n";
  cout <<  "This input needs to be a file.\n";
}

float *shadeRay(Scene*, int, float*,int);
float *willHitWhere2(Scene*,float*,float*);

int main (int argc, char *argv[]) {


  /**
   * This checks for the correct type of inputs
   */
  if (argc != 2) {
		print_usage();
    exit(0);
  }

  Scene *scene = new Scene();
  scene->createScene(argv[1]);

  if (scene->error) {
    cout << "Invalid file name: " << argv[1] << "\n";
    exit(1);
  }

  Window *w = new Window(scene);

  Picture *picture = new Picture(scene);

  int i,j;
  //unsigned int k;
  int counter = 0;
  float *winpos, *raydir, *shade, *r;
  string data = "";
  float mc = 255; // Max color value
  //float t,t1,t2 = 0;

  cout << "Starting loop...\n";
  cout << "sphere count " << scene->object.size()<< endl;

  /*
   * This loop does the main work of processing each of the rays.
   * It sends a ray through every pixel of the picture and if it hits any
   * of the defined objects, color of the pixel gets set to the color of that
   * object, else it gets the background color.
   * Finally if the color is determined it gets added to the data of the
   * picture.
   */
  for ( i=0; i<scene->pixheight; i++) {
    for ( j=0; j<scene->pixwidth; j++) {

      winpos = vec::add(w->ul, vec::add(
               vec::mul(j,w->deltah), vec::mul(i,w->deltav)));
      raydir = vec::normalize(vec::sub(winpos,scene->eye)); 
       
      char color[13]; //Set the color to the default background
      sprintf(color, "%d %d %d ", (int)(scene->bkgcolor[0] * mc),
                                  (int)(scene->bkgcolor[1] * mc),
                                  (int)(scene->bkgcolor[2] * mc));

      //float lowest = -1000;
      //float hk = -1;
      float *hit = willHitWhere2(scene, raydir, scene->eye);

      if (hit[0] != -100000) {
        shade = shadeRay(scene, (int) hit[3], hit, 0);
        sprintf(color, "%d %d %d ", (int) (shade[0] * mc),
                                    (int) (shade[1] * mc),
                                    (int) (shade[2] * mc));
      }
      /*for (k=0; k<scene->object.size(); k++) {

        Sphere *s = dynamic_cast<Sphere *>(scene->object[k]);
        if ( s != NULL ) {

          float d = (float)pow((double)s->B(scene->eye,raydir),2)-
                    (4*s->C(scene->eye));

          if(d >= 0 && s->position[2] > lowest) {

            lowest = s->position[2];
            t1 = (-1 * s->B(scene->eye,raydir)-sqrt(d)) / 2;
            t2 = (-1 * s->B(scene->eye,raydir)+sqrt(d)) / 2;
            t = ((t1 < t2) ?  t1 : t2);
            r = vec::add(scene->eye,vec::mul(t,raydir));//intersection point
            hk = k;
          
          }
        } else {
          printf("Null s");
        }
      }
      if (hk >= 0) {
        shade = shadeRay(scene, hk, r, 0); //Shade ray and change color
        sprintf(color, "%d %d %d ", (int) (shade[0] * mc),
                                    (int) (shade[1] * mc),
                                    (int) (shade[2] * mc));
      }*/
      // This assures we don't go over the 70 char per line limit of ppm
      counter++;
      if (counter > 4) {
        counter = 0;
        strcat(color, "\n");
      }
      // Adding data to picture
      data.assign(color);
      picture->append(data);
      data = "";

    }
  }
  printf("Loop Finished\n");
  //creates the output file of the scene
  ofstream outFile;
  char fileName[100] = "\0";
  strcat(fileName,argv[1]);
  strcat(fileName,".ppm");
  outFile.open(fileName);
  outFile << picture->dump();
  outFile.close();

  return 0;

}

float *willHitWhere2(Scene *s,float *r_dir, float *o) {
  
  float *r = vec::normalize(r_dir);
  
  float *ret = (float*) malloc(sizeof(float) * 4);
  ret[0] = -100000;
  float t1, t2, t = 0;
  float t_temp = 100000;
  //float low = -1000;
  unsigned int i = 0;

  for (i=0; i < s->object.size(); i++) {

    Sphere *sphere = dynamic_cast<Sphere *>(s->object[i]);

    if ( sphere != NULL ) {
      
      float d = pow(sphere->B(o,r),2) - (4 * sphere->C(o));

      if ( d >= 0 ) {// && sphere->position[2] > low) {
        //low = sphere->position[2]; 
        t1 = (-1 * sphere->B(o,r) - sqrt(d)) / 2;
        t2 = (-1 * sphere->B(o,r) + sqrt(d)) / 2;
        t  = ((t1 < t2) ? t1 : t2);

        if (fabs(t) < fabs(t_temp)) {
          t_temp = t;
          ret = vec::add(o,vec::mul(t,r));
          ret[3] = (float) i;
        }
      }
    }
  }
  return ret;
}
    

float * shadeRay(Scene *s, int k, float *r, int c) {

  //Getting the values from the material for readbility
  float odr = s->object[k]->mc[0];
  float odg = s->object[k]->mc[1];
  float odb = s->object[k]->mc[2];
  float osr = s->object[k]->mc[3];
  float osg = s->object[k]->mc[4];
  float osb = s->object[k]->mc[5];
  float ka  = s->object[k]->mc[6];
  float kd  = s->object[k]->mc[7];
  float ks  = s->object[k]->mc[8];
  float n   = s->object[k]->mc[9];
  float al  = s->object[k]->mc[10];
  float nt  = s->object[k]->mc[11];


  //printf("mc: %f %f %f \n", odr, odg, odb);
  //cout << "K: " << k << " ";

  

  float *V = vec::normalize(vec::mul(-1,s->viewdir));
  float *N = V;//gets rid of error
  Sphere *sp = dynamic_cast<Sphere *>(s->object[k]);
  if (sp != NULL) {
    N = vec::div(vec::sub(r,sp->position),sp->radius);
  } else {
    N[0] = 0; N[1] = 1; N[2] = 0;
    printf("Set normal weird\n");
  }
  float ni = 1.0;
  float Frn  = pow((nt-ni)/(nt+ni),2);
  float *I = vec::negate(s->viewdir);
  float Fr = Frn + (1 - Frn) * pow((1 - vec::dot(I,N)),5);


  float *ret = (float *) malloc(sizeof(float) * 3);
  float *spec = (float *) malloc(sizeof(float) * 3); 
  float *refrac = (float *) malloc(sizeof(float) * 3); 
  unsigned int m,w;
  float temp_r1, temp_r2, temp_r3, d, t, t1, t2 = 0;

  //The loop that sums all the light sources
  for (m = 0; m < s->lights.size(); m++) {

    float lr  = s->lights[m]->color[0];
    float lg  = s->lights[m]->color[1];
    float lb  = s->lights[m]->color[2];

    float *L;
    if (s->lights[m]->w == 0) {
      L = vec::normalize(vec::mul(-1, s->lights[m]->position));
    } else {
      L =  vec::normalize(vec::sub(s->lights[m]->position, r));
    }

    float *H  = vec::normalize(vec::add(L,V));
    float NoL = fmax(0.0,vec::dot(N,L));
    float NoH = fmax(0.0,vec::dot(N,H));
    float mod = 1.0;
    //printf("H: %f %f %f NoL %f Noh %f\n", H[0], H[1], H[2], NoL, NoH);
    //printf("L: %f %f %f", L[0], L[1], L[2]);

    //Checking if a light ray hits another object on way back to source

    for (w = 0; w < s->object.size(); w++) {

      Sphere *sphere = dynamic_cast<Sphere *>(s->object[w]);
  
      if ( sphere != NULL ) {

        d = (float)pow((double)sphere->B(r,vec::normalize(s->lights[m]->position)),2)-
                  (4*sphere->C(r));
      
        if (d >= 0) {
          t1 = (-1 * sphere->B(r,vec::normalize(s->lights[m]->position))-sqrt(d)) / 2;
          t2 = (-1 * sphere->B(r,vec::normalize(s->lights[m]->position))+sqrt(d)) / 2;
          t = ((t1 < t2) ?  t1 : t2);
       
          mod =  ((t > 0.1) ? 0 : 1.0);
          w = s->object.size(); //break;
        }
      } else {
        printf("sphere Null");
      }
    }
    temp_r1 += mod * lr * ((kd*odr*NoL) + (ks*osr*pow(NoH,n)));
    temp_r2 += mod * lg * ((kd*odg*NoL) + (ks*osg*pow(NoH,n)));
    temp_r3 += mod * lb * ((kd*odb*NoL) + (ks*osb*pow(NoH,n)));
    
  }
  //float *R = vec::sub(vec::mul(2 * vec::dot(N,I),N),I);
  //float *T = vec::add(vec::mul(sqrt(1-(pow(ni/nt,2)*(1-pow(vec::dot(N,I),2)))),
  //                             vec::negate(N)), vec::mul((ni/nt),(vec::sub(vec::mul(
  //                                                       vec::dot(N,I),N),I))));
  //spec = specShadeRay(s,k,r,R,1); 
  if (al != 1) {
    //refrac = refracRay(s,k,r,T,ni,1);
  } else {
    refrac[0] = 0;
    refrac[1] = 0;
    refrac[2] = 0;
  }

  ret[0] = ka*odr + temp_r1;// + Fr * spec[0];// + (1-Fr) * (1 - al) * refrac[0];
  ret[1] = ka*odg + temp_r2;// + Fr * spec[1];// + (1-Fr) * (1 - al) * refrac[1];
  ret[2] = ka*odb + temp_r3;// + Fr * spec[2];// + (1-Fr) * (1 - al) * refrac[2];

  //printf("Set return: %f %f %f\n", ret[0], ret[1], ret[2]);

  if (ret[0] > 1.0) {ret[0] = 1;}
  if (ret[1] > 1.0) {ret[1] = 1;}
  if (ret[2] > 1.0) {ret[2] = 1;}
  if (ret[0] < 0.0) {ret[0] = 0.0;}
  if (ret[1] < 0.0) {ret[1] = 0.0;}
  if (ret[2] < 0.0) {ret[2] = 0.0;}

  return ret;

}
