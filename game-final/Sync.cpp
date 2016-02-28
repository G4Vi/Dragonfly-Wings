// Sync.cpp                  
//                                    
//                                    
#include "Sync.h"
#include "NetworkManager.h"

                               
                                   
bool Sync::determineObChange(df::Object* anobject, char* messageStatus)
{
    if(anobject->getType() ==  "Hero")
    {
        if(anobject->isModified(df::POS))                                          
        {
            messageStatus = "UPDATE";
            return true;            
        }
        else
        {
            messageStatus = "NEW";
            return true;
        }
    }
    return false;
}

int Sync::sendObject(df::Object* tempOb, char* status)
{
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    std::string thedata =  tempOb->serialize();                                        
    network_manager.send2((void *)thedata.c_str(), thedata.length()+1);  
    return 0;
}                                                                                                           
                                                                                                             
                                                                                                                                         
int Sync::process(df::Object* tempOb, std::string data)
{
    return tempOb->deserialize(data);
}
