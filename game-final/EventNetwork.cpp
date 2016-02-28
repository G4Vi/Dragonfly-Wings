// Event Network.cpp
//
//
#include "EventNetwork.h"
using namespace df;
EventNetwork::EventNetwork()
{
    setType(df::NETWORK_EVENT);
    bytes = 0;
}


int EventNetwork::getBytes() const
{
    return bytes;
}

