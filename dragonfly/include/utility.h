//
// Utility functions to support Dragonfly and its games.
//
// Functions here do not use any attributes of any classes, so
// can stand alone.
//

#ifndef __UTILITY_H__
#define __UTILITY_H__

// Engine includes
#include "Box.h"
#include "Circle.h"
#include "Line.h"
#include "Object.h"
#include "Position.h"

namespace df {

// Function prototypes.

// Return true if value is between min and max (inclusive).
bool valueInRange(int value, int min, int max);

// Convert relative bounding Box for Object to absolute world Box.
Box getWorldBox(const Object *p_o);

// Convert relative bounding Box for Object to absolute world Box
// at Postion where.
Box getWorldBox(const Object *p_o, Position where);

// Return distance between any two Positions.
float distance(Position p1, Position p2);

// Return true of Position is within Box.
bool boxContainsPosition(Box b, Position p);

// Return true if Line segments intersect.
// (Parallel line segments don't intersect).
bool lineIntersectsLine(Line line1, Line line2);

// Return true if Line segment intersects Box.
bool lineIntersectsBox(Line line, Box b);

// Return true if Boxes intersect.
bool boxIntersectsBox(Box box1, Box box2);

// Return true if Circle intersects or contains Box.
bool circleIntersectsBox(Circle circle, Box b);

// Launch splash screen. Return 0 if ok, else -1.
int splash();

// Returns pretty-formatted time as char * string.
char *getTimeString();	    

// Convert world position to view position.
Position worldToView(Position world_pos);

// Convert view position to world position.
Position viewToWorld(Position view_pos);

// Convert integer to string, returning string.
std::string toString(int i);

// Convert float to string, returning string.
std::string toString(float f);

// Convert character to string, returning string.
std::string toString(char c);

// Convert boolean to string, returning string.
std::string toString(bool b);

// Match key:value pair in string in str, returning value.
// Pairs separated by commas (,).
// If str is empty, use previously parsed string str.
// Return empty string if no match.
std::string match(std::string str, std::string find);

} // end of namespace df
#endif // __UTILITY_H__
