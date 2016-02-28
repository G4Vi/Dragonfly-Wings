//
// Reticle.h
//
#ifndef __RETICLE_H__ 
#define __RETICLE_H__
#include "Object.h"

#define RETICLE_CHAR '+'

class Reticle : public df::Object {

 public:
  Reticle();
  void draw(void);
  int eventHandler(const df::Event *p_e);
};

#endif // __RETICLE_H__  
