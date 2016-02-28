//                                                                                 
// Sentry                                                                          
//                                                                                 
// Poll NetworkManager for incoming messages, generating network                   
// events (onEvent()) when there are complete messages available.                   
//                                                                                 

#ifndef __SENTRY_H__ 
#define __SENTRY_H__

#include "Object.h"

namespace df {

class Sentry : public Object {

 private:
  void doStep();

 public:
  Sentry();
  int eventHandler(const Event *p_e);
};

} // end of namespace df                                                           

#endif // __SENTRY_H__  