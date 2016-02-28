// Sync.cpp                  
//                                    
//                                    
#include "Sync.h"
#include "NetworkManager.h"

                               
                                   
bool Sync::determineObChange(df::Object* anobject, char* messageStatus)
{
    if(anobject->getType() ==  "Hero")
    {
        if(anobject->isModified(df::ID))
        {
            messageStatus = "NEW";
            return true;
        }
        else if(anobject->isModified(df::POS))
        {
            messageStatus = "UPDATE";
            return true;            
        }
    }
    return false;
}

int Sync::sendObject(df::Object* tempOb, char* status)
{
    std::string thedata;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    //thedata  = status + tempOb->serialize();
    thedata = tempOb->serialize();
    network_manager.send2((void *)thedata.c_str(), thedata.length()+1);  
    return 0;
}                                                                                                           
                                                                                                             
                                                                                                                                         
int Sync::process(df::Object* tempOb, std::string data)
{
    return tempOb->deserialize(data);
}
