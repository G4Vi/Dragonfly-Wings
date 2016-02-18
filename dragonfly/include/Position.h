///
/// A 2d (x,y) position
///

#ifndef __POSITION_H__
#define __POSITION_H__

namespace df {

class Position {

 private:
  int x; 			///< Horizontal coordinate in 2d world.
  int y; 			///< Vertical coordinate in 2d world.

 public:

  /// Create object at 2d location (x,y).  
  Position(int init_x, int init_y);

  /// Default 2d (x,y) location is (0,0).
  Position();

  int getX() const;		    ///< Get horizontal coordinate.
  void setX(int new_x);		    ///< Set horizontal coordinate.
  int getY()const;		    ///< Get vertical coordinate.
  void setY(int new_y);		    ///< Set vertical coordinate.
  void setXY(int new_x, int new_y); ///< Set horizizontal & vertical coord.
  bool operator==(const Position &other) const; ///< Compare Positions.
  bool operator!=(const Position &other) const; ///< Compare Positions.
};

} // end of namespace df
#endif //__POSITION_H__
