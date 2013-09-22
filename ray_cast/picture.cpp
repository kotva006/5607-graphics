
#include <sstream>

#include "picture.h"
#include "scene.h"

using namespace std;

Picture::Picture(Scene *s) {
  
  stringstream ss (stringstream::in | stringstream::out); 
  ss << s->pixwidth << ' ' << s->pixheight;
  string width,height;
  ss >> width >> height;
  this->data = "P3\n" + width + "\n" + height + "\n255\n";

}

void Picture::append(string s) {
  this->data = this->data + s;
}
