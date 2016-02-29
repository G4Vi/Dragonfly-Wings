//
// RemoteShip.cpp
//

// Engine includes.
#include "EventStep.h"
#include "EventView.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Bullet.h"
#include "Explosion.h"
#include "GameOver.h"
#include "RemoteShip.h"

RemoteShip::RemoteShip() {

  // Link to "ship" sprite.
  df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
  df::LogManager &log_manager = df::LogManager::getInstance();
  df::Sprite *p_temp_sprite;
  p_temp_sprite = resource_manager.getSprite("ship");
  if (!p_temp_sprite) {
    log_manager.writeLog("RemoteShip::RemoteShip(): Warning! Sprite '%s' not found", 
			 "ship");
  } else {
    //p_temp_sprite->setColor(df::RED);
    setSprite(p_temp_sprite);
    setSpriteSlowdown(3);  // 1/3 speed animation.
    setTransparency();	   // Transparent sprite.
  }

  // Player controls RemoteShip, so register for input events.
 
  // Need to update rate control each step.
  registerInterest(df::STEP_EVENT);

  // Set object type.
  setType("RemoteShip");

  // Set starting location.
  df::WorldManager &world_manager = df::WorldManager::getInstance();
  df::Position pos(7, world_manager.getBoundary().getVertical()/2);
  setPosition(pos);  

  // Set attributes that control actions.
  move_slowdown = 2;
  move_countdown = move_slowdown;
  fire_slowdown = 30;
  fire_countdown = fire_slowdown;
}
  
RemoteShip::~RemoteShip() {

    /*
  // Create GameOver object.
  GameOver *p_go = new GameOver;
  
  // Make big explosion.
  for (int i=-8; i<=8; i+=5) {
    for (int j=-5; j<=5; j+=3) {
      df::Position temp_pos = this->getPosition();
      temp_pos.setX(this->getPosition().getX() + i);
      temp_pos.setY(this->getPosition().getY() + j);
      Explosion *p_explosion = new Explosion;
      p_explosion -> setPosition(temp_pos);
    }
  }
  */
}
 
// Handle event.
// Return 0 if ignored, else 1.
int RemoteShip::eventHandler(const df::Event *p_e) {
  if (p_e->getType() == df::STEP_EVENT) {
    step();
    return 1;
  }
 
  // If get here, have ignored this event.
  return 0;
}

// Move up or down.
void RemoteShip::move(int dy) {

  // See if time to move.
  if (move_countdown > 0)
    return;
  move_countdown = move_slowdown;

  // If stays on window, allow move.
  df::Position new_pos(getPosition().getX(), getPosition().getY() + dy);
  df::WorldManager &world_manager = df::WorldManager::getInstance();
  if ((new_pos.getY() > 3) && 
      (new_pos.getY() < world_manager.getBoundary().getVertical()-1))
    world_manager.moveObject(this, new_pos);
}

// Fire bullet towards target.
void RemoteShip::fire(df::Position target) {

  // See if time to fire.
  if (fire_countdown > 0)
    return;
  fire_countdown = fire_slowdown;

  // Fire Bullet towards target.
  Bullet *p = new Bullet(getPosition());
  p->setYVelocity((float) (target.getY() - getPosition().getY()) /
		  (float) (target.getX() - getPosition().getX()));

  // Play "fire" sound.
  df::Sound *p_sound = df::ResourceManager::getInstance().getSound("fire");
  p_sound->play();
}

// Decrease rate restriction counters.
void RemoteShip::step() {

  // Move countdown.
  move_countdown--;
  if (move_countdown < 0)
    move_countdown = 0;

  // Fire countdown.
  fire_countdown--;
  if (fire_countdown < 0)
    fire_countdown = 0;
}


