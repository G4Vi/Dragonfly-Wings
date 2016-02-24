//Reticle.cpp
//
//
#include "Reticle.h"
#include "EventMouse.h"
#include "GraphicsManager.h"
#include "WorldManager.h"

Reticle::Reticle()
{
        setType("Reticle");
        setSolidness(df::SPECTRAL); //No collision
        setAltitude(df::MAX_ALTITUDE); //top view

        registerInterest(df::MOUSE_EVENT);


}

int Reticle::eventHandler(const df::Event *p_e) {

        if (p_e->getType() == df::MOUSE_EVENT) {
                const df::EventMouse *p_mouse_event = 
                        dynamic_cast <const df::EventMouse *> (p_e);
                if (p_mouse_event->getMouseAction() == df::MOVED) {
                        setPosition(p_mouse_event->getMousePosition());
                        return 1;
                }
        }

        // If get here, have ignored this event.
        return 0;
}

void Reticle::draw() {
        df::GraphicsManager &graphics_manager = df::GraphicsManager::getInstance();
        graphics_manager.drawCh(getPosition(), RETICLE_CHAR, df::RED); 
}

