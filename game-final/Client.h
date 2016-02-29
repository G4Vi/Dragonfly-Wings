//
// Client.h
//

#include "EventKeyboard.h"
#include "EventMouse.h"
#include "EventNetwork.h"
#include "Object.h"
#include "Reticle.h"

#include "Settings.h"
#include "Sync.h"
#include "RemoteShip.h"

#include "Saucer.h"
#include "Points.h"

#include <vector>

class Client : public df::Object {

 private:
  Reticle *p_reticle;
  Settings* theSet;
  Sync* syncHalp;
  RemoteShip* otherPlayer;
  std::vector<Saucer*> saucers;
  Points* thepoints;

  int fire_slowdown;
  int fire_countdown;
  int move_slowdown;
  int move_countdown;
  int nuke_count;
  void network(const df::EventNetwork *p_network_event);
  void mouse(const df::EventMouse *p_mouse_event);
  void kbd(const df::EventKeyboard *p_keyboard_event);
  void move(int dy);
  void fire(df::Position target);
  void step();
  void nuke();
  
 public:
  Client(Settings* info);
  ~Client();
  int eventHandler(const df::Event *p_e);
};

