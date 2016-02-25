//Saucer.cpp

#include "EventCollision.h"
#include "EventOut.h"
#include "Saucer.h"
#include "Explosion.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include <stdlib.h>

Saucer::Saucer()
{
    // Dragonfly managers needed for this method.
    df::LogManager &log_manager = df::LogManager::getInstance();
    df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
    df::WorldManager &world_manager = df::WorldManager::getInstance();

    // Setup "saucer" sprite.
    df::Sprite *p_temp_sprite = resource_manager.getSprite("saucer");
    if (!p_temp_sprite) {
        log_manager.writeLog("Saucer::Saucer(): Warning! Sprite '%s' not found", "saucer");
    } else {
        setSprite(p_temp_sprite);
        setSpriteSlowdown(4);		
    }

    // Set object type.
    setType("Saucer");

    // Set speed in horizontal direction.
    setXVelocity(-0.25); // 1 space left every 4 frames

    /*
    // Set starting location in the middle of window.
    int world_horiz = world_manager.getBoundary().getHorizontal();
    int world_vert = world_manager.getBoundary().getVertical();
    df::Position pos(world_horiz/2, world_vert/2);
    setPosition(pos);*/

    moveToStart();
}

int Saucer::eventHandler(const df::Event *p_e) {

    if (p_e->getType() == df::OUT_EVENT) {
        out();
        return 1;
    }

    if (p_e->getType() == df::COLLISION_EVENT) {
        const df::EventCollision *p_collision_event = dynamic_cast <df::EventCollision const *> (p_e);
        hit(p_collision_event);
        return 1;
    }

    return 0;
}

void Saucer::out() {
    if (getPosition().getX() >= 0)
        return;

    moveToStart();

}

void Saucer::moveToStart() {
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    df::Position temp_pos;

    int world_horiz = world_manager.getBoundary().getHorizontal();
    int world_vert = world_manager.getBoundary().getVertical();

    // x is off right side of window
    temp_pos.setX(world_horiz + rand()%world_horiz + 3);

    // y is in vertical range
    temp_pos.setY(rand()%(world_vert-1) + 1);

    world_manager.moveObject(this, temp_pos);
}

void Saucer::hit(const df::EventCollision *p_collision_event) {

    if ((p_collision_event -> getObject1() -> getType() == "Saucer") &&
            (p_collision_event -> getObject2() -> getType() == "Saucer"))
        return;


    if ((p_collision_event -> getObject1() -> getType() == "Bullet") ||
            (p_collision_event -> getObject2() -> getType() == "Bullet")) {


        Explosion *p_explosion = new Explosion;
        p_explosion -> setPosition(this -> getPosition());

        df::LogManager &log_manager = df::LogManager::getInstance();  
        log_manager.writeLog("here gh\n");
        new Saucer;
    }

}

