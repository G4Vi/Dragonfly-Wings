//
// game.cpp
// 

// Engine includes.
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "Saucer.h"

void loadResources(void);
void populateWorld(void);

void populateWorld(void)
{
    new Saucer;
}

void loadResources()
{
    df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
    resource_manager.loadSprite("sprites/saucer-spr.txt", "saucer");
}
int main(int argc, char *argv[]) {
  df::LogManager &log_manager = df::LogManager::getInstance();
  
  // Start up game manager.
  df::GameManager &game_manager = df::GameManager::getInstance();
  if (game_manager.startUp())  {
    log_manager.writeLog("Error starting game manager!");
    game_manager.shutDown();
    return 0;
  }

  // Set flush of logfile during development (when done, make false).
  log_manager.setFlush(true);

  // Load game resources.
  loadResources();

  // Populate game world with some objects.
  populateWorld();

  // Run the game.
  game_manager.run();

  // Show splash screen.
  //df::splash();

  // Shut everything down.
  game_manager.shutDown();
}

