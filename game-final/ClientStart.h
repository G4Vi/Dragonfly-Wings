//
// ClientStart.h
//

#include "Music.h"
#include "ViewObject.h"

class ClientStart : public df::ViewObject {

 private:
  df::Music *p_music;
  void start();

 public:
  ClientStart();
  int eventHandler(const df::Event *p_e);
  void draw();
  void playMusic();
};
