//                                                                                 
// NetworkManager.h                                                                
//                                                                                 
// Manage network connections to/from engine.                                      
//                                                                                 
 
#ifndef __NETWORK_MANAGER_H__ 
#define __NETWORK_MANAGER_H__ 
 
// System includes.                                                                
#include <string.h> 
 
// Engine includes.                                                                
#include "Manager.h" 
 
#include "Settings.h"

#define DRAGONFLY_PORT "6666"   // Default port.                                   
 
namespace df { 
 
class NetworkManager : public df::Manager { 

 private:
  NetworkManager();                       // Private since a singleton.            
  NetworkManager(NetworkManager const&);  // Don't allow copy.                     
  void operator=(NetworkManager const&);  // Don't allow assignment.               
  int sock, betterSock;                               // Connected network socket.
  Settings* theSet;
 public:

  // Get the one and only instance of the NetworkManager.                          
  static NetworkManager &getInstance();
  ~NetworkManager();

  // Start up NetworkManager.                                                      
  int startUp(Settings* info);

  // Shut down NetworkManager.                                                     
  void shutDown();

  // Accept only network events.                                                   
  // Returns false for other engine events.                                        
  bool isValid(std::string event_type) const;

  // Block, waiting to accept network connection.                                  
  int accept2();

  // Make network connection.                                                      
  // Return 0 if success, else -1.                                                 
  int connect2(void* connStuff);

  // Close network connection.                                                     
  // Return 0 if success, else -1.                                                 
  // int close();

  // Return true if network connected, else false.                                 
  bool isConnected() const;

  // Return socket.                                                                
  // int getSocket() const;
  int getClientSock(void** connStuff);
  int getServSock();

  // Send buffer to connected network.                                             
  // Return 0 if success, else -1.                                                 
  int send2(void *buffer, int bytes);

  // Receive from connected network (no more than nbytes).                         
  // If peek is true, leave data in socket else remove.                            
  // Return number of bytes received, else -1 if error.                            
  int receive(void *buffer, int nbytes, bool peek = false);

  // Check if network data.                                                        
  // Return amount of data (0 if no data), -1 if not connected or error.           
  int isData();   
};

} // end of namespace df                                                           

#endif // __NETWORK_MANAGER_H__      
