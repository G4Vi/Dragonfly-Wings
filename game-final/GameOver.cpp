//
// GameOver.cpp
//

// Engine includes.
#include "EventStep.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "GameOver.h"
#include "GameStart.h"

GameOver::GameOver() {

  setType("GameOver");

  // Link to "message" sprite.
  df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
  df::Sprite *p_temp_sprite = resource_manager.getSprite("gameover");
  if (!p_temp_sprite) {
    df::LogManager &log_manager = df::LogManager::getInstance();
    log_manager.writeLog("GameOver::GameOver(): Warning! Sprite 'gameover' not found");
  } else {
    setSprite(p_temp_sprite);
    setSpriteSlowdown(15);		  
    setTransparency('#');    // Transparent character.
    time_to_live = p_temp_sprite->getFrameCount() * 15;
  }

  // Put in center of window.
  setLocation(df::CENTER_CENTER);
  
  // Register for step event.
  registerInterest(df::STEP_EVENT);

  // Play "game over" sound.
  df::Sound *p_sound = df::ResourceManager::getInstance().getSound("game over");
  p_sound->play();
}

// When done, game over so reset things for GameStart.
GameOver::~GameOver() {
  df::WorldManager &world_manager = df::WorldManager::getInstance();

  // Remove Saucers and ViewObjects, re-activate GameStart.
  df::ObjectList object_list = world_manager.getAllObjects(true);
  df::ObjectListIterator i(&object_list);
  for (i.first(); !i.isDone(); i.next()) {
    df::Object *p_o = i.currentObject();
    if (p_o -> getType() == "Saucer" || p_o -> getType() == "ViewObject")
      world_manager.markForDelete(p_o);
    if (p_o -> getType() == "GameStart") {
      p_o -> setActive(true);
      dynamic_cast <GameStart *> (p_o) -> playMusic();	// Resume start music.
    }
  }
}

// Handle event.
// Return 0 if ignored, else 1.
int GameOver::eventHandler(const df::Event *p_e) {

  if (p_e->getType() == df::STEP_EVENT) {
    step();
    return 1;
  }

  // If get here, have ignored this event.
  return 0;
}

// Count down to end of message.
void GameOver::step() {
  time_to_live--;
  if (time_to_live <= 0) { 
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    world_manager.markForDelete(this);
  }
}

// Override default draw so as not to display "value".
void GameOver::draw() {
  df::Object::draw();
}
