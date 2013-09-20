
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "scene.h"



void print_usage() {
  char *usage_1 = "This command takes only one input.\n";
  char *usage_2 = "This input needs to be a file.\n";
  std::cout << strcat(usage_1, usage_2);
}



int main (int argc, char *argv[]) {

  if (argc != 2) {
		print_usage();
    exit(0);
  }

  Scene *scene = new Scene();
  scene->createScene(argv[1]);

  return 0;

}



  
