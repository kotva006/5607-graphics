
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

using namespace std;

void print_usage() {
  cout <<  "This command takes only one input.\n";
  cout <<  "This input needs to be a file.\n";
}

float *shadeRay(Scene*, int, float*);

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
  unsigned int k;
  int counter = 0;
  float *winpos, *raydir, *shade, *r;
  string data = "";
  float mc = 255; // Max color value
  float t,t1,t2 = 0;

  cout << "Starting loop...\n";

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

      float lowest = -1000;
      for (k=0; k<scene->object.size(); k++) {

        float d = (float)pow((double)scene->object[k]->B(scene->eye,raydir),2)-
                  (4*scene->object[k]->C(scene->eye));

        if(d >= 0 && scene->object[k]->position[2] > lowest) {

          lowest = scene->object[k]->position[2];
          t1 = (-1 * scene->object[k]->B(scene->eye,raydir)-sqrt(d)) / 2;
          t2 = (-1 * scene->object[k]->B(scene->eye,raydir)+sqrt(d)) / 2;
          t = ((t1 < t2) ?  t1 : t2);
          r = vec::add(scene->eye,vec::mul(t,raydir));//intersection point
          
          shade = shadeRay(scene, k, r); //Shade ray and change color
          sprintf(color, "%d %d %d ", (int) (shade[0] * mc),
                                      (int) (shade[1] * mc),
                                      (int) (shade[2] * mc));
        }
      }
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

float * shadeRay(Scene *s, int k, float *r) {

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


  //printf("R: %f %f %f \n", r[0], r[1], r[2]);

  float *V = vec::normalize(vec::mul(-1,s->viewdir));
  float *N = vec::div(vec::sub(r,s->object[k]->position),s->object[k]->radius);
  //printf("N: %f %f %f \n", N[0], N[1], N[2]);

  //printf("Passed Values %f %f %f %f %f %f %f %f %f %f\n", odr, odg, odb,
  //                     osr, osg, osb, ka, kd, ks, n);


  float *ret = (float *) malloc(sizeof(float) * 3);
  unsigned int m,w;
  float temp_r1, temp_r2, temp_r3, d, t, t1, t2 = 0;

  //The loop that sums all the light sources
  for (m = 0; m < s->lights.size(); m++) {

    float lr  = s->lights[m]->color[0];
    float lg  = s->lights[m]->color[1];
    float lb  = s->lights[m]->color[2];

    float *L; // TO DO figure out what w does... 0 = dir 1 = point
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

    // READ THE DOCUMENTATION TO DEBUG BIOTCH
    for (w = 0; w < s->object.size(); w++) {

      d = (float)pow((double)s->object[w]->B(r,vec::normalize(s->lights[m]->position)),2)-
                (4*s->object[w]->C(r));
      
      if (d >= 0) {
        t1 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))-sqrt(d)) / 2;
        t2 = (-1 * s->object[w]->B(r,vec::normalize(s->lights[m]->position))+sqrt(d)) / 2;
        t = ((t1 < t2) ?  t1 : t2);
       
        mod =  ((t > 0.1) ? 0 : 1.0);
        w = s->object.size(); //break;
      }
    }
    temp_r1 += mod * lr * ((kd*odr*NoL) + (ks*osr*pow(NoH,n)));
    temp_r2 += mod * lg * ((kd*odg*NoL) + (ks*osg*pow(NoH,n)));
    temp_r3 += mod * lb * ((kd*odb*NoL) + (ks*osb*pow(NoH,n)));
    
  }

  ret[0] = ka*odr + temp_r1;
  ret[1] = ka*odg + temp_r2;
  ret[2] = ka*odb + temp_r3;

  //printf("Set return: %f %f %f\n", ret[0], ret[1], ret[2]);

  if (ret[0] > 1.0) {ret[0] = 1;}
  if (ret[1] > 1.0) {ret[1] = 1;}
  if (ret[2] > 1.0) {ret[2] = 1;}
  if (ret[0] < 0.0) {ret[0] = 0.0;}
  if (ret[1] < 0.0) {ret[1] = 0.0;}
  if (ret[2] < 0.0) {ret[2] = 0.0;}

  return ret;

}
  
