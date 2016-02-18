///
/// A 2-d bounding box
///

#ifndef __BOX_H__
#define __BOX_H__

#include "Position.h"

namespace df {

class Box {

 private:
  Position corner;         ///< Upper left corner of box.
  int horizontal;          ///< Horizontal dimension.
  int vertical;            ///< Vertical dimension.

 public:
  /// Create box with upper-left corner, horiz and vert sizes.
  Box(Position init_corner, int init_horizontal, int init_vertical);

  /// Create box with (0,0) for corner, and 0 for horiz and vert.
  Box();

  /// Set upper left corner of box.
  void setCorner(Position new_corner);

  /// Get upper left corner of box.
  Position getCorner() const;

  /// Set horizontal size of box.
  void setHorizontal(int new_horizontal);

  /// Get horizontal size of box.
  int getHorizontal() const;

  /// Set vertical size of box.
  void setVertical(int new_vertical);

  /// Get vertical size of box.
  int getVertical() const;

  bool operator==(const Box &other) const; ///< Compare boxes.
  bool operator!=(const Box &other) const; ///< Compare boxes.
};

} // end of namespace df
#endif //__BOX_H__
