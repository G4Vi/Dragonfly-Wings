//
// Points.cpp
//

// Engine includes
#include "Event.h"
#include "EventStep.h"
#include "GraphicsManager.h"
#include "LogManager.h"

// Game includes.
#include "Points.h"

Points::Points(int type) {
  setLocation(df::TOP_RIGHT);
  setViewString(POINTS_STRING);
  setColor(df::YELLOW);
  // Need to update score each second, so count "step" events.
  registerInterest(df::STEP_EVENT);
  scoreUpdate = 0;
  setType("Points");
  this->type = type;
}

// Handle event.
// Return 0 if ignored, else 1.
int Points::eventHandler(const df::Event *p_e) {
int first = getValue();

if(type == 0) //only host updates based on bullets
{
    // If step, increment score every second (30 steps).
    if (p_e->getType() == df::STEP_EVENT) {
      if (dynamic_cast <const df::EventStep *> (p_e) -> getStepCount() % 30 == 0)
        setValue(getValue() + 1);
      return 1;
    }

  // Parent handles event if score update.
  if (df::ViewObject::eventHandler(p_e))
  {
      scoreUpdate += 10;
      return 1;
  }

}
else
{
    // If step, increment score every second (30 steps).
    if (p_e->getType() == df::STEP_EVENT) {
      if (dynamic_cast <const df::EventStep *> (p_e) -> getStepCount() % 30 == 0)
        setValue(getValue() + 1);
      return 1;
    }
}



  // If get here, have ignored this event.
  return 0;
}
