
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

  cout << "starting loop\n";

  for ( i=0; i<scene->pixheight; i++) {
    for ( j=0; j<scene->pixheight; j++) {
      winpos = vec::add(w->ul, vec::add(
               vec::mul(i,w->deltah), vec::mul(j,w->deltav)));
      raydir = vec::normalize(vec::sub(winpos,scene->eye)); 
      for (k=0; k<scene->object.size(); k++) {
        float d = (float)pow((double)scene->object[k]->B(scene->eye,raydir),2)-
                  (4*scene->object[k]->C(scene->eye,raydir));
        char color[13];
        if(d >= 0) {
          sprintf(color, "%d %d %d ", (int)scene->object[k]->materialcolor[0],
                                      (int)scene->object[k]->materialcolor[1],
                                      (int)scene->object[k]->materialcolor[2]);
          cout << "Hit";
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
        
      }
    }
  }

  ofstream outFile;
  outFile.open("sample.ppm");
  outFile << picture->dump();
  outFile.close();

  return 0;

}



  
