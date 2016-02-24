//Bullet.h
//
//
#include "Object.h"
#include "EventCollision.h"

class Bullet : public df::Object {

    private:
        void out();
        void hit(const df::EventCollision *p_collision_event);

    public:
         Bullet(df::Position hero_pos);
         int eventHandler(const df::Event *p_e);
};

