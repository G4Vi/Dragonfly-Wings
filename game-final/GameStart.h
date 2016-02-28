//
// GameStart.h
//

#include "Music.h"
#include "ViewObject.h"
#include "Settings.h"

class GameStart : public df::ViewObject {

 private:
  df::Music *p_music;
  Settings* theSet;
  void start();

 public:
  GameStart(Settings* info);
  int eventHandler(const df::Event *p_e);
  void draw();
  void playMusic();
};

