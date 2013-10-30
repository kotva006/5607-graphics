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
  unsigned int k,m,n = 0;
  int counter = 0;
  float *winpos, *raydir, *r, *bcc;//*shade, *r;
  float shade[3];
  string data = "";
  float mc = 255; // Max color value
  float t,s_t,p_t,s_t1,s_t2 = 0;

  cout << "Starting loop...\n";

  /*
   * This loop does the main work of processing each of the rays.
   * It sends a ray through every pixel of the picture and if it hits any
   * of the defined objects, color of the pixel gets set to the color of that
   * object, else it gets the background color.
   * Finally if the color is determined it gets added to the data of the
   * picture.
   */
  Polygon *poly = dynamic_cast<Polygon *>(scene->object[0]);
  
  //cout << "P: " << poly->vertex[0]->pos[0] << endl;
  //cout << "P: " << poly->vertex[1]->pos[0] << endl;
  //cout << "P: " << poly->vertex[2]->pos[0] << endl;
  //cout << "MC:" << scene->object[0]->mc[0] << endl;
  //cout << "MC:" << scene->object[1]->mc[0] << endl;
  //cout << "MC:" << scene->object[0]->mc[2] << endl;
  for ( i=0; i<scene->pixheight; i++) {
    for ( j=0; j<scene->pixwidth; j++) {

      winpos = vec::add(w->ul, vec::add(
               vec::mul(j,w->deltah), vec::mul(i,w->deltav)));
      raydir = vec::normalize(vec::sub(winpos,scene->eye)); 
       
      char color[13]; //Set the color to the default background
      color[12] = '\0';
      sprintf(color, "%d %d %d ", (int)(scene->bkgcolor[0] * mc),
                                  (int)(scene->bkgcolor[1] * mc),
                                  (int)(scene->bkgcolor[2] * mc));

      float s_lowest = 100000;
      float p_lowest = 100000;
      int s_k = -1, p_k = -1, x = -1;
      for (k=0; k < scene->object.size(); k++) {

        Sphere *s = dynamic_cast<Sphere *>(scene->object[k]);
        Polygon *p = dynamic_cast<Polygon *>(scene->object[k]);
        if ( s != NULL ) {

          float d = (float)pow((double)s->B(scene->eye,raydir),2)-
                    (4*s->C(scene->eye));

          if(d >= 0) {

            s_t1 = (-1 * s->B(scene->eye,raydir)-sqrt(d)) / 2;
            s_t2 = (-1 * s->B(scene->eye,raydir)+sqrt(d)) / 2;
            s_t = ((s_t1 < s_t2) ?  s_t1 : s_t2);
            if (s_t < s_lowest) {
              s_lowest = s_t;
              s_k = k;
            }
          }
        } else if ( p != NULL ) {
        
          float p0[3], p1[3], p2[3];
          bcc = NULL;
          //cout<<"MY P_K "<<p_k<<" "<<s_k<<endl;
          for (m=0; m < p->face.size(); m++) {

            string tag_1 = p->face[m]->data[0];
            string tag_2 = p->face[m]->data[1];
            string tag_3 = p->face[m]->data[2];


            if (tag_1.length() == 2) { //for regular verticies
              for(n=0; n < p->vertex.size(); n++) {
                if (tag_1.compare(p->vertex[n]->tag) == 0) {
                  p0[0] = p->vertex[n]->pos[0];
                  p0[1] = p->vertex[n]->pos[1];
                  p0[2] = p->vertex[n]->pos[2];
                }
                  
                if (tag_2.compare(p->vertex[n]->tag) == 0) {
                  p1[0] = p->vertex[n]->pos[0];
                  p1[1] = p->vertex[n]->pos[1];
                  p1[2] = p->vertex[n]->pos[2];
                }

                if (tag_3.compare(p->vertex[n]->tag) == 0) {
                  p2[0] = p->vertex[n]->pos[0];
                  p2[1] = p->vertex[n]->pos[1];
                  p2[2] = p->vertex[n]->pos[2];
                }
              }
              p->face[m]->D(p0,p1,p2);
              if (p->face[m]->isNotZero(raydir)) {
                p_t = p->face[m]->t(scene->eye,raydir);
                if(p_t > 0 && p_t < p_lowest) {
                  float *p_0 = vec::add(scene->eye,vec::mul(p_t,raydir));
                  bcc = p->face[m]->getBCC(p_0,p0,p1,p2);
                  if (bcc != NULL) {
                    //cout<<"P0 "<<p0[0]<<" "<<p0[1]<<" "<<p0[2]<<endl;
                    p_lowest = p_t;
                    p_k = k;
                  }
                }
              }
            } else if ((tag_1.substr(1,1)).compare("n") == 0) {
            } else if ((tag_1.substr(1,1)).compare("t") == 0) {
            }

          }

        } else {
          printf("Null s and p ");
        }
      }
      if (p_k != -1 || s_k != -1) {
        cout<<"P_k "<<p_k<<" S_k "<<s_k<<endl;
        t = (p_lowest<s_lowest) ? p_lowest : s_lowest;
        x = (p_lowest<s_lowest) ? p_k : s_k;
        if (x == -1) {
          cout<<"BAD:"<<p_k<<s_k<<p_lowest<<s_lowest<<endl;
        }
        r = vec::add(scene->eye,vec::mul(t,raydir));//intersection point
        //shade = shadeRay(scene, k, r, 0); //Shade ray and change color
        
        shade[0] = scene->object[x]->mc[0];
        shade[1] = scene->object[x]->mc[1];
        shade[2] = scene->object[x]->mc[2];
        sprintf(color, "%d %d %d ", (int) (shade[0] * mc),
                                    (int) (shade[1] * mc),
                                    (int) (shade[2] * mc));
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

  

  float *V = vec::normalize(vec::mul(-1,s->viewdir));
  float *N = V;//gets rid of warning N gets changed anyway
  Sphere *sp = dynamic_cast<Sphere *>(s->object[k]);
  Polygon *p = dynamic_cast<Polygon *>(s->object[k]);
  if (sp != NULL) {
    N = vec::div(vec::sub(r,sp->position),sp->radius);
  } else if(p != NULL) {
    N = p->face[0]->N;
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
  float temp_r1 = 0, temp_r2 = 0, temp_r3 = 0, d = 0, 
        t = 0, t1 = 0, t2 = 0;

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

    float p_lowest = 0,p_k = 0;

    for (w = 0; w < s->object.size(); w++) {

      Sphere *sphere = dynamic_cast<Sphere *>(s->object[w]);
      Polygon *poly  = dynamic_cast<Polygon *>(s->object[w]); 
  
      if ( sphere != NULL ) {

        d = (float)pow((double)sphere->B(r,vec::normalize(s->lights[m]->position)),2)-
                  (4*sphere->C(r));
      
        if (d >= 0) {
          t1 = (-1 * sphere->B(r,vec::normalize(s->lights[m]->position))-sqrt(d)) / 2;
          t2 = (-1 * sphere->B(r,vec::normalize(s->lights[m]->position))+sqrt(d)) / 2;
          t = ((t1 < t2) ?  t1 : t2);
       
          mod =  ((t > 0.1) ? 0 : al);
          w = s->object.size(); //break;
        }
      } else if (poly != NULL) {
        float p0[3], p1[3], p2[3];
        float p_t = -1;
        for (m=0; m < p->face.size(); m++) {

          string tag_1 = p->face[m]->data[0];
          string tag_2 = p->face[m]->data[1];
          string tag_3 = p->face[m]->data[2];


          if (tag_1.length() == 1) { //for regular verticies
            for(n=0; n < p->vertex.size(); n++) {
              if (tag_1.compare(p->vertex[n]->tag) == 0) {
                p0[0] = p->vertex[n]->pos[0];
                p0[1] = p->vertex[n]->pos[1];
                p0[2] = p->vertex[n]->pos[2];
              }
                  
              if (tag_2.compare(p->vertex[n]->tag) == 0) {
                p1[0] = p->vertex[n]->pos[0];
                p1[1] = p->vertex[n]->pos[1];
                p1[2] = p->vertex[n]->pos[2];
              }

              if (tag_3.compare(p->vertex[n]->tag) == 0) {
                p2[0] = p->vertex[n]->pos[0];
                p2[1] = p->vertex[n]->pos[1];
                p2[2] = p->vertex[n]->pos[2];
              }
            }
            p->face[m]->D(p0,p1,p2);
            if (p->face[m]->isNotZero(vec::normalize(s->lights[m]->position))) {
              p_t = p->face[m]->t(s->eye,s->lights[m]->position);
              if(p_t > 0 && fabs(p_t) < fabs(p_lowest)) {
                p_lowest = p_t;
                p_k = m;
              }
            }
          }
        }
      } else {
        printf("sphere Null");
      }
    }
    temp_r1 += mod * lr * ((kd*odr*NoL) + (ks*osr*pow(NoH,n)));
    temp_r2 += mod * lg * ((kd*odg*NoL) + (ks*osg*pow(NoH,n)));
    temp_r3 += mod * lb * ((kd*odb*NoL) + (ks*osb*pow(NoH,n)));
    
  }
  float *R = vec::sub(vec::mul(2 * vec::dot(N,I),N),I);
  float *T = vec::add(vec::mul(sqrt(1-(pow(ni/nt,2)*(1-pow(vec::dot(N,I),2)))),
                               vec::negate(N)), vec::mul((ni/nt),(vec::sub(vec::mul(
                                                         vec::dot(N,I),N),I))));
  //spec = specShadeRay(s,k,r,R,1); 
  if ( al != 1 && p == NULL ) {
    //refrac = refracRay(s,k,r,T,ni,1);
  } else {
    refrac[0] = 0;
    refrac[1] = 0;
    refrac[2] = 0;
  }

  ret[0] = ka*odr + temp_r1;// + Fr * spec[0] + (1-Fr) * (1 - al) * refrac[0];
  ret[1] = ka*odg + temp_r2;// + Fr * spec[1] + (1-Fr) * (1 - al) * refrac[1];
  ret[2] = ka*odb + temp_r3;// + Fr * spec[2] + (1-Fr) * (1 - al) * refrac[2];

  if (spec[0] * Fr != 0 || spec[1] *Fr != 0 || spec[2] * Fr != 0) {
    //printf("Spec %f %f %f ", spec[0] * Fr, spec[1] * Fr, spec[2] * Fr);
  }

  //printf("Set return: %f %f %f\n", ret[0], ret[1], ret[2]);

  if (ret[0] > 1.0) {ret[0] = 1;}
  if (ret[1] > 1.0) {ret[1] = 1;}
  if (ret[2] > 1.0) {ret[2] = 1;}
  if (ret[0] < 0.0) {ret[0] = 0.0;}
  if (ret[1] < 0.0) {ret[1] = 0.0;}
  if (ret[2] < 0.0) {ret[2] = 0.0;}

  return ret;

}
