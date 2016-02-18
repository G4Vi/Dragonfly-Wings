///
/// Fraps (FRAmes Per Second) recorder
/// 

#ifndef __FRAPS_H__
#define __FRAPS_H__

#include <stdio.h>

#include "Clock.h"    
#include "Event.h"    
#include "ViewObject.h"

namespace df {

const std::string FRAPS_STRING = "fps";
const std::string FRAPS_FILENAME = "fraps";

class Fraps : public ViewObject {

 private:
  bool do_record;         ///< True if recording.
  Clock clock;            ///< For computing frame rate.
  FILE *screen_fp;        ///< Output file handle.

 public:
  /// Frames per second meter (with option to record screen).
  /// defaults: TOP_RIGHT, green, don't record.
  Fraps(bool do_record=false);
  ~Fraps();

  /// Handle event each step, f9 to hide, f12 to toggle recording.
  int eventHandler(const Event *p_e);

  /// Set whether record or not, opens new file if turn on.
  void setRecord(bool new_record);
  bool getRecord() const;
};

} // end of namespace df
#endif
