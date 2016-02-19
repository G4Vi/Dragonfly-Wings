// Hero.h
#include "Object.h"
#include "EventKeyboard.h"
#include "EventMouse.h"
#include "Reticle.h"

class Hero : public df::Object {

    private:
        Reticle *p_reticle;
        void kbd(const df::EventKeyboard *p_keyboard_event);
        void move(int dy);
        void step();
        void fire(df::Position target);
        void mouse(const df::EventMouse *p_mouse_event);
        int move_slowdown;
        int move_countdown;
        int fire_slowdown;
        int fire_countdown;

    public:
        Hero();
        int eventHandler(const df::Event *p_e);
};
