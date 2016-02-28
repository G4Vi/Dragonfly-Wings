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
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    if(network_manager.isData())
    {
        std::cout << network_manager.isData() << std::endl;
        network_manager.onEvent(&EN);
    }
}                                                                                                            
                                                                                                             
                                                                                                                                         
