///
/// A "mouse" event
///

#ifndef __EVENT_MOUSE_H__
#define __EVENT_MOUSE_H__

#include "Event.h"
#include "Position.h"

namespace df {

const std::string MOUSE_EVENT = "df::mouse";

/// Set of mouse actions recognized by Dragonfly.
enum EventMouseAction {
  UNDEFINED_MOUSE_ACTION = -1,
  CLICKED,
  PRESSED,
  MOVED,
};

/// Set of mouse buttons recognized by Dragonfly.
namespace Mouse {
enum EventMouseButton {
  UNDEFINED_MOUSE_BUTTON = -1,
  LEFT,
  RIGHT,
  MIDDLE,
};
} // end of namespace Mouse
  
class EventMouse : public Event {

 private:
  EventMouseAction mouse_action;         ///< Mouse action.
  Mouse::EventMouseButton mouse_button;  ///< Mouse button.
  Position mouse_xy;		         ///< Mouse (x,y) coordinates.

 public:
  EventMouse();

  /// Set mouse event's action.
  void setMouseAction(EventMouseAction new_mouse_action);

  /// Get mouse event's action.
  EventMouseAction getMouseAction() const;

  /// Set mouse event's button.
  void setMouseButton(Mouse::EventMouseButton new_mouse_button);

  /// Get mouse event's button.
  Mouse::EventMouseButton getMouseButton() const;

  /// Set mouse event's position.
  void setMousePosition(Position new_mouse_xy);

  /// Get mouse event's position.
  Position getMousePosition() const;
};

} // end of namespace df
#endif // __EVENT_MOUSE_H__
