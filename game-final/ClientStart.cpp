//
// ClientStart.cpp
//

// Engine includes.
#include "Color.h"
#include "EventKeyboard.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "ClientStart.h"
#include "Client.h"
#include "Music.h"
#include "Points.h"
#include "Saucer.h"

ClientStart::ClientStart() {
  setType("ClientStart");

  // Link to "message" sprite.
  df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
  df::Sprite *p_temp_sprite = resource_manager.getSprite("clientstart");
  if (!p_temp_sprite) {
    df::LogManager &log_manager = df::LogManager::getInstance();
    log_manager.writeLog("ClientStart::ClientStart(): Warning! Sprite 'clientstart' not found");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(15);		  
  }

  // Put in center of screen.
  setLocation(df::CENTER_CENTER);

  // Register for "keyboard" event.
  registerInterest(df::KEYBOARD_EVENT);

  // Play start music.
  p_music = df::ResourceManager::getInstance().getMusic("start music");
  playMusic();
}

// Play start music.
void ClientStart::playMusic() {
  p_music->play();
}

// Handle event.
// Return 0 if ignored, else 1.
int ClientStart::eventHandler(const df::Event *p_e) {
  df::GameManager &game_manager = df::GameManager::getInstance();

  if (p_e->getType() == df::KEYBOARD_EVENT) {
    df::EventKeyboard *p_keyboard_event = (df::EventKeyboard *) p_e;
    switch (p_keyboard_event->getKey()) {
    case df::Keyboard::P: 			// play
      start();
      break;
    case df::Keyboard::Q:			// quit
      game_manager.setGameOver();
      break;
    default:
      break;
    }
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}

void ClientStart::start() {

  // Create hero.
  new Client;

  // Spawn some saucers to shoot.
  for (int i=0; i<16; i++)
    new Saucer;

  // Setup heads-up display.
  new Points;		                     // Points display.
  df::ViewObject *p_vo = new df::ViewObject; // Count of nukes.
  p_vo->setLocation(df::TOP_LEFT);
  p_vo->setViewString("Nukes");
  p_vo->setValue(1);
  p_vo->setColor(df::YELLOW);

  // Pause start music.
  p_music->pause();

  // When game starts, become inactive.
  setActive(false);
}

// Override default draw so as not to display "value".
void ClientStart::draw() {
  df::Object::draw();
}
