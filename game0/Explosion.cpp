//Explosion.cpp

#include "Explosion.h"
#include "EventStep.h"
#include "GameManager.h"
#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"

Explosion::Explosion()
{
        // Dragonfly managers needed for this method.
        df::LogManager &log_manager = df::LogManager::getInstance();
        df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
        df::WorldManager &world_manager = df::WorldManager::getInstance();

        df::Sprite *p_temp_sprite = resource_manager.getSprite("explosion");
        if (!p_temp_sprite) {
                log_manager.writeLog("Explosion::Explosion(): Warning! Sprite '%s' not found", "explosion");
        } else {
                setSprite(p_temp_sprite);
                setSpriteSlowdown(0);		
        }

        // Set object type.
        setType("Explosion");
        registerInterest(df::STEP_EVENT);
        time_to_live = getSprite()->getFrameCount();
        setSolidness(df::SPECTRAL);

}

int Explosion::eventHandler(const df::Event *p_e) {

        if (p_e->getType() == df::STEP_EVENT) {
                step();
                return 1;
        }

        return 0;
}

void Explosion::step() {
        time_to_live--;
        if (time_to_live <= 0){
                df::WorldManager &world_manager = df::WorldManager::getInstance();
                world_manager.markForDelete(this);
        }
}

