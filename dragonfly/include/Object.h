///
/// The base game world object
///

#ifndef __OBJECT_H__
#define __OBJECT_H__

// System includes.
#include <string>

// Engine includes.
#include "Box.h"              ///< Objects have a bounding box.
#include "Event.h"            ///< Objects can handle events.
#include "Position.h"         ///< Objects need a location.
#include "Sprite.h"           ///< Objects (often) have sprites.

namespace df {

/// Max number of events Object can be interested in.
const int MAX_OBJ_EVENTS = 100;

/// Types of solidness of Object.
enum Solidness {
  HARD,       ///< Object causes collisions and impedes.
  SOFT,       ///< Object causes collision, but doesn't impede.
  SPECTRAL,   ///< Object doesn't cause collisions.
};
 
/// Count of number of modified attribute categories.
const int ATTR_COUNT = 12;

/// Categories of attributes that indicate modification.
enum ObjectAttribute {
  ID, 
  ACTIVE,
  VISIBLE,            
  EVENTS,
  BOX,                   
  POS,               
  TYPE,                
  SPRITE,
  ALTITUDE,               
  SOLIDNESS,        
  NO_SOFT,               
  VELOCITY,           
};

class Object {

 private:
  int id;		      ///< Unique Object identifier.
  bool is_active;             ///< If false, Object not acted upon.
  bool is_visible;            ///< If true, Object gets drawn.
  Box box;                    ///< Box for sprite boundary & collisions.
  Position pos;               ///< Position in the game world.
  std::string type;	      ///< User-defined identification.
  std::string sprite_name;    ///< Sprite name in ResourceManager.
  Sprite *p_sprite;           ///< Sprite associated with Object.
  bool sprite_center;         ///< True if sprite centered on Object.
  char sprite_transparency;   ///< Sprite transparent character (0 if none).
  int sprite_index;           ///< Current index frame for sprite.
  int sprite_slowdown;        ///< Slowdown rate (1 = no slowdown, 0 = stop).
  int sprite_slowdown_count;  ///< Slowdown counter.
  int altitude;               ///< 0 to MAX_ALTITUDE (lower drawn first).
  Solidness solidness;        ///< Solidness state of Object.
  bool no_soft;               ///< True if won't move on soft Objects.
  float x_velocity;           ///< Horizontal speed in spaces per game step.
  float x_velocity_countdown; ///< Countdown to horizontal movement.
  float y_velocity;           ///< Vertical speed in spaces per game step.
  float y_velocity_countdown; ///< Countdown to vertical movement.
  int event_count;            ///< Number of events of interest.
  std::string event_name[MAX_OBJ_EVENTS]; ///< Event names.
  bool modified[ATTR_COUNT];  ///< Mod. attrib. since serialize().

 public:
  /// Construct Object. Set default parameters and
  /// add to game world (WorldManager).
  Object();

  /// Destroy Object. Unregister for any interested events.
  /// Remove from game world (WorldManager).
  virtual ~Object();

  /// Set Object id.
  void setId(int new_id);

  /// Get Object id.
  int getId() const;
  
  /// Set type identifier of Object.
  void setType(std::string new_type);

  /// Get type identifier of Object.
  std::string getType() const;

  /// Set Position of Object.
  void setPosition(Position new_pos);

  /// Get Position of Object.
  Position getPosition() const;

  /// Handle event (default is to ignore everything).
  /// Return 0 if ignored, else 1.
  virtual int eventHandler(const Event *p_event);

  ///< Return True if Object is HARD or SOFT, else false.
  bool isSolid() const;    

  /// Set solidness of Object, with checks for consistency.  
  /// Return 0 if ok, else -1.
  int setSolidness(Solidness new_solid);

  /// Return solidness of Object.
  Solidness getSolidness() const;

  /// Set "no soft" setting of Object (true - cannot move onto SOFT Objects).
  void setNoSoft(bool new_no_soft=true);

  /// Get "no soft" setting of Object (true - cannot move onto SOFT Objects).
  bool getNoSoft() const;

  /// Set altitude of Object, with checks for range [0, MAX_ALTITUDE].
  /// Return 0 if ok, else -1.
  int setAltitude(int new_altitude);
   
  /// Return altitude of Object.
  int getAltitude() const;
   
  /// Set x velocity of Object.
  void setXVelocity(float new_x_velocity);

  /// Get x velocity of Object.
  float getXVelocity() const;

  /// Set y velocity of Object.
  void setYVelocity(float new_y_velocity);

  /// Get y velocity of Object.
  float getYVelocity() const;

  /// Perform 1 step of velocity in horizontal direction.
  /// Return horizontal distance moved this step.
  int getXVelocityStep();

  /// Perform 1 step of velocity in vertical direction.
  /// Return vertical distance moved this step.
  int getYVelocityStep();

  /// Set visibility of Object.  Objects not visible are not drawn.
  /// Return 0 if ok, else -1.
  int setVisible(bool visible=true);

  /// Return visibility of Object.  Objects not visible are not drawn.
  bool isVisible() const;

  /// Set activeness of Object.  Objects not active are not acted upon
  /// by engine.
  /// Return 0 if ok, else -1.
  int setActive(bool active=true);

  /// Return activeness of Object.  Objects not active are not acted upon
  /// by engine.
  bool isActive() const;

  /// Set bounding box of Object.
  void setBox(Box new_box);

  /// Get bounding box of Object.
  Box getBox() const;

  /// Register for interest in event.  
  /// Keeps track of manager and event.  
  /// Return 0 if ok, else -1.
  int registerInterest(std::string event_type);

  /// Unregister for interest in event.  
  /// Return 0 if ok, else -1.
  int unregisterInterest(std::string event_type);

  /// Draw single Sprite Frame.
  /// Drawing accounts for centering & slowdown, and advances Sprite Frame.
  virtual void draw();

  /// Set Sprite name (in ResourceManager) for Object.
  void setSpriteName(std::string new_name);

  /// Get Sprite name (in ResourceManager) for Object.
  std::string getSpriteName() const;

  /// Set Object Sprite to new one.
  /// If set_box is true, set bounding box to size of Sprite.
  /// Set sprite index to 0 (first frame).
  void setSprite(Sprite *p_new_sprite, bool set_box=true);

  /// Return pointer to Sprite associated with this Object.
  Sprite *getSprite() const;

  /// Set Sprite to be centered at Object position (pos).
  void setCentered(bool centered=true);

  /// Indicates if Sprite is centered at Object position (pos).
  bool isCentered() const;

  /// Set Sprite transparency character (0 means none).
  void setTransparency(char transparent=' ');

  /// Get Sprite transparency character (0 means none).
  char getTransparency() const;

  /// Set index of current Sprite frame to be displayed.
  void setSpriteIndex(int new_sprite_index);

  /// Return index of current Sprite frame to be displayed.
  int getSpriteIndex() const;

  /// Slows down sprite animations.
  /// Slowdown value in multiples of GameManager frame time.
  void setSpriteSlowdown(int new_sprite_slowdown);
  int getSpriteSlowdown() const;
  void setSpriteSlowdownCount(int new_sprite_slowdown_count);
  int getSpriteSlowdownCount() const;
  
  /// Serialize Object attributes to single string.
  /// e.g., "id:110,is_active:true, ...
  /// Only modified attributes are serialized (unless all is true).
  /// Clear modified[] array.
  virtual std::string serialize(bool all=false);

  /// Deserialize string to become Object attributes.
  /// Return 0 if no errors, else -1.  
  virtual int deserialize(std::string s);

  /// Return true if attribute modified since last serialize.
  virtual bool isModified(enum ObjectAttribute attribute) const;
  
  /// Return true if any attribute modified since last serialize.
  virtual bool isModified() const;
};

} // end of namespace df
#endif // __OBJECT_H__
