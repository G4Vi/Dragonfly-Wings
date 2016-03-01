//
// Saucer.h
//
#ifndef __SAUCER_H__ 
#define __SAUCER_H__

#include "Object.h"
#include "EventCollision.h"
 
class Saucer : public df::Object {
 private:
  void moveToStart();
  void out();
  void hit(const df::EventCollision *p_collision_event);
 
 public:
  Saucer();
  ~Saucer();
  Object* owner;
  int host;
 int eventHandler(const df::Event *p_e);
};
#endif //SAUCER_H
