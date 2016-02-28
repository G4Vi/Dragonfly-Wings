//                                                                                 
// Sync                                                                          
//                                                                                 
//                 
//                  
//                                                                                 

#ifndef __SYNC_H__ 
#define __SYNC_H__

#include "Object.h"

class Sync{

 private:
 public:
 bool determineObChange(df::Object* anobject, char* messageStatus);
 int  sendObject(df::Object* tempOb, char* status);
};

#endif // __SYNC_H__  
