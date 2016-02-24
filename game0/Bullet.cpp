//Bullet.cpp

#include "EventOut.h"
#include "Bullet.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include <stdlib.h>

Bullet::Bullet(df::Position hero_pos)
{
        // Dragonfly managers needed for this method.
        df::LogManager &log_manager = df::LogManager::getInstance();
        df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
        df::WorldManager &world_manager = df::WorldManager::getInstance();

        // Setup "saucer" sprite.
        df::Sprite *p_temp_sprite = resource_manager.getSprite("bullet");
        if (!p_temp_sprite) {
                log_manager.writeLog("Bullet::Bullet(): Warning! Sprite '%s' not found", "bullet");
        } else {
                setSprite(p_temp_sprite);
                setSpriteSlowdown(5);		
        }

        // Set object type.
        setType("Bullet");

        // Set speed in horizontal direction.
        setXVelocity(1); // 1 space left every 4 frames

        // Set starting location at hero
        df::Position pos(hero_pos.getX()+3, hero_pos.getY());
        setPosition(pos);

}

int Bullet::eventHandler(const df::Event *p_e) {

        if (p_e->getType() == df::OUT_EVENT) {
                out();
                return 1;
        }

        if (p_e->getType() == df::COLLISION_EVENT) {
                const df::EventCollision *p_collision_event = 
                        dynamic_cast <const df::EventCollision *> (p_e);
                hit(p_collision_event);
                return 1;
        }

        return 0;
}

void Bullet::out() {
        df::WorldManager &world_manager = df::WorldManager::getInstance();
        world_manager.markForDelete(this);
}

void Bullet::hit(const df::EventCollision *p_collision_event) {
        df::WorldManager &world_manager = df::WorldManager::getInstance();
        world_manager.markForDelete(p_collision_event->getObject1());
        world_manager.markForDelete(p_collision_event->getObject2());
}
