
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "scene.h"
#include "picture.h"



void print_usage() {
  std::cout <<  "This command takes only one input.\n";
  std::cout <<  "This input needs to be a file.\n";
}



int main (int argc, char *argv[]) {

  if (argc != 2) {
		print_usage();
    exit(0);
  }

  Scene *scene = new Scene();
  scene->createScene(argv[1]);
  

  Picture *picture = new Picture(scene);
  return 0;

}



  
