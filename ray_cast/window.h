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
    Vec ul; //up left corner of window
    Vec ur; //up right corner of window
    Vec ll; //lower left corner of window
    Vec lr; //lower right corner of window
    Vec deltah;
    Vec deltav;
    
} ;

#endif
