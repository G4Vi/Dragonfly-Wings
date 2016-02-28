//                                                                                 
// Sync                                                                          
//                                                                                 
//                 
//                  
//                                                                                 

#ifndef __SYNC_H__ 
#define __SYNC_H__

#include "Object.h"
#include <string>

class Sync{

 private:
 public:
 bool determineObChange(df::Object* anobject, char* messageStatus);
 int  sendObject(df::Object* tempOb, char* status);
 int  process(df::Object* tempOb, std::string data);
};

#endif // __SYNC_H__  
