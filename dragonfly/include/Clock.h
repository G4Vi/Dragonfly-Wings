///
/// The clock, for timing
///

#ifndef __CLOCK_H__
#define __CLOCK_H__

namespace df {

class Clock {

 private:
  long int previous_time;  /// Previous time delta() called (in microseconds).

 public:
  /// Sets previous_time to current time.
  Clock();

  /// Return time elapsed since delta() was called, -1 if error.
  /// Units are microseconds.
  long int delta();

  /// Return time elapsed since delta() was called, -1 if error.
  /// Units are microseconds.
  /// Return -1 if error.
  long int split() const;
};

} // end of namespace df
#endif // __CLOCK_H__
