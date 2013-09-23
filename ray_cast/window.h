#ifndef WINDOW_H
#define WINDOW_H

#include "scene.h"

using namespace std;

class Window {
  public:
    Window(){};
    ~Window(){};
    Window(Scene*);
    float h; //height of view window
    float w; //width of view window
    float *ul; //up left corner of window
    float *ur; //up right corner of window
    float *ll; //lower left corner of window
    float *lr; //lower right corner of window
    float *deltah;
    float *deltav;
    
} ;

#endif
