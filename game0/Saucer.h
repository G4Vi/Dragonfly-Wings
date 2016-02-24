//Saucer.h

#include "EventCollision.h"
#include "Object.h"
  class Saucer : public df::Object {
   void moveToStart();
   void out();
   void hit(const df::EventCollision *p_collision_event);
   public:
    Saucer();
    int eventHandler(const df::Event *p_e);
  };

