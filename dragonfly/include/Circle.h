///
/// A 2d circle
///

#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include "Position.h"

namespace df {

class Circle {

 private:
  Position center;        ///< Circle middle.
  float radius;           ///< Circle raduis.

 public:

  /// Create circle at center with radius r.
  Circle(Position init_center, float init_radius);

  /// Default circle segment is at (0,0) with radius 0.
  Circle();

  void setCenter(Position new_center); ///< Set center.
  Position getCenter() const;          ///< Get center.
  void setRadius(float new_radius);    ///< Set radius.
  float getRadius() const;             ///< Get radius.
};

} // end of namespace df
#endif //__CIRCLE_H__
