// Sync.cpp                  
//                                    
//                                    
#include "Sync.h"
#include "NetworkManager.h"
#include "WorldManager.h"
                               
#include <iostream>
bool Sync::determineObChange(df::Object* anobject, std::string* messageStatus)
{
    if(anobject->getType() ==  "Hero")
    {
        if(anobject->isModified(df::ID))
        {
            *messageStatus = "NEW";
            return true;
        }
        else if(anobject->isModified(df::POS))
        {
            *messageStatus = "UPDATE";
            return true;            
        }
    }
    else if(anobject->getType() == "Saucer")
    {
        if(anobject->isModified(df::ID))
        {
            *messageStatus = "NEW";
            return true;
        }
    }
    return false;
}

int Sync::sendObject(df::Object* tempOb, std::string status)
{
    std::string thedata;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    //thedata  = status + tempOb->serialize();
    //otherstring = status;
    thedata = status;
    thedata += tempOb->serialize();

    //std::cout << "thedata is " << thedata << std::endl;
    network_manager.send2((void *)thedata.c_str(), thedata.length()+1);  
    return 0;
}                                                                                                           
                                                                                                             
                                                                                                                                         
int Sync::process(df::Object* tempOb, std::string data)
{
    /*df::WorldManager &world_manager = df::WorldManager::getInstance();
    if(tempOb->getType() == "RemoteShip")
    {
        std::string x = df::match(data.c_str(), "pos-x");
        std::string y = df::match(data.c_str(), "pos-y");
        df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
        world_manager.moveObject(tempOb, new_pos);
        return 0;
    }*/
    return tempOb->deserialize(data);
}
