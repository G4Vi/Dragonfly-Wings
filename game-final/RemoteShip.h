//
// RemoteShip.h
//
#ifndef __RemoteShip_H__ 
#define __RemoteShip_H__

#include "Object.h"
class RemoteShip : public df::Object {

 private:  
  int fire_slowdown;
  int fire_countdown;
  int move_slowdown;
  int move_countdown;  
  void move(int dy);
  void fire(df::Position target);
  void step(); 
  

 public:
  RemoteShip(int killowner);
  ~RemoteShip();
  Object* owner;
  int killowner; 
  int eventHandler(const df::Event *p_e);
};
#endif //RemoteShip_H
