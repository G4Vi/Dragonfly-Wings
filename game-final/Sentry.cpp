// Sentry.cpp                  
//                                    
//                                    
#include "Sentry.h"
#include "EventStep.h"
#include "EventNetwork.h"
#include "NetworkManager.h"
#include <iostream> //to remove

using namespace df;                                      
Sentry::Sentry()          
{                                     
    registerInterest(df::STEP_EVENT); 
    setType("Sentry");
    setSolidness(df::SPECTRAL);
    setVisible(false);
}                                     
                                      
int Sentry::eventHandler(const df::Event *p_e) {                                                                                              
                                                                                                             
  if (p_e->getType() == df::STEP_EVENT) {                                                                    
    doStep();                                                                                                  
    return 1;                                                                                                
  }                                                                                                          
                                                                                                             
  // If get here, have ignored this event.                                                                   
  return 0;                                                                                                  
}

void Sentry::doStep()
{
    EventNetwork EN;
    int cnt, i, length;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    if(network_manager.isData()> 0)
    {
        memset(EN.line, 0, 30);
        cnt = network_manager.getMessageCount();
        std::cout<< "Message count is " << cnt << std::endl;
        for(i = 0; i < cnt; i ++)
        {
            length = network_manager.getMessageLength();
            network_manager.receive(EN.line, length, false);
            EN.line[length] = '\0';
            EN.line[29] = '\0';
            network_manager.onEvent(&EN);
        }
    }
}                                                                                                            
                                                                                                             
                                                                                                                                         
