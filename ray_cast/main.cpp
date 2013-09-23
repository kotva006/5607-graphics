
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

int main (int argc, char *argv[]) {

  if (argc != 2) {
		print_usage();
    exit(0);
  }

  Scene *scene = new Scene();
  scene->createScene(argv[1]);
  
  Window *w = new Window(scene);

  Picture *picture = new Picture(scene);

  int i,j;
  unsigned int k;
  int counter = 0;
  float *winpos, *raydir;
  string data = "";
  float mc = 255;

  cout << "starting loop\n";

  float test[3] = {1,0,0};
  float test2[3] = {0,1,0};
  float *res = vec::crossproduct(test,test2);
  cout << "C: " << res[0] << " " << res[1] << " " << res[2] << "\n";
  cout << "Color: " << scene->object[0]->materialcolor[0] << "\n";

  for ( i=0; i<scene->pixheight; i++) {
    for ( j=0; j<scene->pixwidth; j++) {
      winpos = vec::add(w->ul, vec::add(
               vec::mul(j,w->deltah), vec::mul(i,w->deltav)));
      raydir = vec::normalize(vec::sub(winpos,scene->eye)); 
      //cout << "H: " << w->deltah[1] << " W: " << w->deltav[1] << "\n";
      for (k=0; k<scene->object.size(); k++) {
        float d = (float)pow((double)scene->object[k]->B(scene->eye,raydir),2)-
                  (4*scene->object[k]->C(scene->eye,raydir));
        char color[13];
        //if (true) {cout << "D: " << (4*scene->object[k]->C(scene->eye,raydir)) << "\n";}
        if(d >= 0) {
          sprintf(color, "%d %d %d ", (int)(scene->object[k]->materialcolor[0] * mc),
                                      (int)(scene->object[k]->materialcolor[1] * mc),
                                      (int)(scene->object[k]->materialcolor[2] * mc));
          //cout << "Hit";
        } else {
          sprintf(color, "%d %d %d ", (int)scene->bkgcolor[0],
                                      (int)scene->bkgcolor[1],
                                      (int)scene->bkgcolor[2]);
        }
        counter++;
        if (counter > 4) {
          counter = 0;
          strcat(color, "\n");
        }
          
        data.assign(color);

        picture->append(data);
        data = "";
        
      }
    }
  }

  ofstream outFile;
  outFile.open("sample.ppm");
  outFile << picture->dump();
  outFile.close();

  return 0;

}



  
