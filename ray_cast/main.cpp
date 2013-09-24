
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


  /**
   * This checks for the correct type of inputs
   */
  if (argc != 2) {
		print_usage();
    exit(0);
  }


  //cout << "Creating Scene...\n";
  Scene *scene = new Scene();
  scene->createScene(argv[1]);

  if (scene->error) {
    cout << "Invalid file name: " << argv[1] << "\n";
    exit(1);
  }

  //cout << "Defining window...\n"; 
  Window *w = new Window(scene);

  //cout << "Initializing picture...\n";
  Picture *picture = new Picture(scene);

  int i,j;
  unsigned int k;
  int counter = 0;
  float *winpos, *raydir;
  string data = "";
  float mc = 255; // Max color value

  //cout << "Starting loop...\n";

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
       
      char color[13];
      sprintf(color, "%d %d %d ", (int)(scene->bkgcolor[0] * mc),
                                  (int)(scene->bkgcolor[1] * mc),
                                  (int)(scene->bkgcolor[2] * mc));
      for (k=0; k<scene->object.size(); k++) {
        float d = (float)pow((double)scene->object[k]->B(scene->eye,raydir),2)-
                  (4*scene->object[k]->C(scene->eye,raydir));

        if(d >= 0) {
          sprintf(color, "%d %d %d ", (int)(scene->object[k]->materialcolor[0] * mc),
                                      (int)(scene->object[k]->materialcolor[1] * mc),
                                      (int)(scene->object[k]->materialcolor[2] * mc));
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
