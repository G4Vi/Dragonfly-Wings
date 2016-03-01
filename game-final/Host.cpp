//
// Host.cpp
//

// Engine includes.
#include "EventMouse.h"
#include "EventStep.h"
#include "EventView.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

// Game includes.
#include "Bullet.h"
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "Host.h"
//#include "Points.h"


// Networking
#include "Sentry.h"
#include "NetworkManager.h"
#include <sstream>
#include <cstdlib>
#include <iostream> //will remove
char packet[4096];
std::vector<Bullet*> bullets;
Host::Host(Settings* info) {

    // Link to "ship" sprite.
    df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
    df::LogManager &log_manager = df::LogManager::getInstance();
    df::Sprite *p_temp_sprite;
    p_temp_sprite = resource_manager.getSprite("ship");    
    if (!p_temp_sprite) {
        log_manager.writeLog("Host::Host(): Warning! Sprite '%s' not found", 
                "ship");
        } else {
        p_temp_sprite->setColor(df::RED);
        setSprite(p_temp_sprite);        
        setSpriteSlowdown(3);  // 1/3 speed animation.
        setTransparency();	   // Transparent sprite.
    }

    // Player controls hero, so register for input events.
    registerInterest(df::KEYBOARD_EVENT);
    registerInterest(df::MOUSE_EVENT);

    // Need to update rate control each step.
    registerInterest(df::STEP_EVENT);

    

    // Set object type.
    setType("Hero");

    // Set starting location.
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    df::Position pos(7, (world_manager.getBoundary().getVertical()/2) - 7);
    setPosition(pos);

    // Create reticle for firing bullets.
    p_reticle = new Reticle();
    p_reticle->draw();

    // Set attributes that control actions.
    move_slowdown = 2;
    move_countdown = move_slowdown;
    fire_slowdown = 30;
    fire_countdown = fire_slowdown;
    nuke_count = 1;

    //Create a Network Manager and a Sentry to create network events
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    network_manager.registerInterest(this, df::NETWORK_EVENT);
    new df::Sentry;    
    syncHalp = new Sync;
    network_manager.startUp(info);

    // Spawn some saucers to shoot.
    for (int i=0; i<16; i++)
    {
        saucers.push_back(new Saucer);
    }

     thepoints = new Points(0);		                     // Points display.



}

Host::~Host() {

    std::string tempMessage, message;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    df::WorldManager &world_manager = df::WorldManager::getInstance();

    //Send out ship distruction message
    tempMessage = "DELETER,";
    message = "010" + df::toString((int)tempMessage.length()) + tempMessage;
    network_manager.send2((void *)message.c_str(), message.length());
    network_manager.shutDown();

    world_manager.markForDelete(otherPlayer);
    world_manager.markForDelete(thepoints);


    // Create GameOver object.
    GameOver *p_go = new GameOver;

    // Make big explosion.
    for (int i=-8; i<=8; i+=5) {
        for (int j=-5; j<=5; j+=3) {
            df::Position temp_pos = this->getPosition();
            temp_pos.setX(this->getPosition().getX() + i);
            temp_pos.setY(this->getPosition().getY() + j);
            Explosion *p_explosion = new Explosion;
            p_explosion -> setPosition(temp_pos);
        }
    }

    // Mark Reticle for deletion.
    df::WorldManager::getInstance().markForDelete(p_reticle);
}

// Handle event.
// Return 0 if ignored, else 1.
int Host::eventHandler(const df::Event *p_e) {
    if (p_e->getType() == df::KEYBOARD_EVENT) {
        const df::EventKeyboard *p_keyboard_event = dynamic_cast <const df::EventKeyboard *> (p_e);
        kbd(p_keyboard_event);
        return 1;
    }

    if (p_e->getType() == df::MOUSE_EVENT) {
        const df::EventMouse *p_mouse_event = dynamic_cast <const df::EventMouse *> (p_e);
        mouse(p_mouse_event);
        return 1;
    }

    if (p_e->getType() == df::STEP_EVENT) {
        step();
        return 1;
    }

    if (p_e->getType() == df::NETWORK_EVENT) { 
        const df::EventNetwork *p_network_event = dynamic_cast <const df::EventNetwork *> (p_e);
        network(p_network_event);
        return 1;                             
    }

    // If get here, have ignored this event.
    return 0;
}

// Network
void Host::network(const df::EventNetwork *p_network_event) {
    std::string type, data, sprite_name;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
     df::WorldManager &world_manager = df::WorldManager::getInstance();

     std::cout << "line is " << p_network_event->line << std::endl;

     if(memcmp(p_network_event->line, "NEW", 3)==0)
     {
         data = (p_network_event->line+4);
         if(memcmp(p_network_event->line, "NEWH", 4) == 0)
         {
             otherPlayer = new RemoteShip;
             std::string x = df::match(data.c_str(), "x");
             std::string y = df::match(data.c_str(), "y");
             df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
             world_manager.moveObject(otherPlayer, new_pos);
         }
         else if(memcmp(p_network_event->line, "NEWB", 4) == 0)
         {
             std::string x = df::match(data.c_str(), "x");
             std::string y = df::match(data.c_str(), "y");
             df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
             Bullet* ab = new Bullet(new_pos);
             std::string id = df::match(data.c_str(), "id");
             ab->setId(atoi(id.c_str()));
         }
     }     
     else if(memcmp(p_network_event->line, "UPDATE", 6)==0)
     {
         data = (p_network_event->line+7);
         if(memcmp(p_network_event->line, "UPDATEH", 7) == 0)
         {
             std::string x = df::match(data.c_str(), "x");
             std::string y = df::match(data.c_str(), "y");
             df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
             world_manager.moveObject(otherPlayer, new_pos);
         }
     }
}

// Take appropriate action according to mouse action.
void Host::mouse(const df::EventMouse *p_mouse_event) {

    // Pressed button?ssw
    if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
            (p_mouse_event->getMouseButton() == df::Mouse::LEFT))
        fire(p_mouse_event->getMousePosition());
}

// Take appropriate action according to key pressed.
void Host::kbd(const df::EventKeyboard *p_keyboard_event) {

    switch(p_keyboard_event->getKey()) {
        case df::Keyboard::W:       // up
            if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
                move(-1);
            break;
        case df::Keyboard::S:       // down
            if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN)
                move(+1);
            break;
        case df::Keyboard::SPACE:   // nuke!
            if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED)
                nuke();
            break;
        case df::Keyboard::Q:        // quit
            if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED) {
                df::WorldManager &world_manager = df::WorldManager::getInstance();
                world_manager.markForDelete(this);
            }
            break;
    };

    return;
}

// Move up or down.
void Host::move(int dy) {

    // See if time to move.
    if (move_countdown > 0)
        return;
    move_countdown = move_slowdown;

    // If stays on window, allow move.
    df::Position new_pos(getPosition().getX(), getPosition().getY() + dy);
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    if ((new_pos.getY() > 3) && 
            (new_pos.getY() < world_manager.getBoundary().getVertical()-1))
        world_manager.moveObject(this, new_pos);
}

// Fire bullet towards target.
void Host::fire(df::Position target) {

    // See if time to fire.
    if (fire_countdown > 0)
        return;
    fire_countdown = fire_slowdown;

    // Fire Bullet towards target.
    Bullet *p = new Bullet(getPosition());
    p->setYVelocity((float) (target.getY() - getPosition().getY()) /
            (float) (target.getX() - getPosition().getX()));

    bullets.push_back(p);



    // Play "fire" sound.
    df::Sound *p_sound = df::ResourceManager::getInstance().getSound("fire");
    p_sound->play();
}

// Decrease rate restriction counters.
void Host::step() {

    // Move countdown.
    move_countdown--;
    if (move_countdown < 0)
        move_countdown = 0;

    // Fire countdown.
    fire_countdown--;
    if (fire_countdown < 0)
        fire_countdown = 0;

    //Send out data by network every step
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();

    if(network_manager.isConnected())
    {
         std::string herostr, saucerstr, bulletstr, pointsstr, message, messageStatus;
         std::ostringstream os, ss, bs, bbs;
         int msize, linecnt;
         linecnt = 0;

        //build local Hero string
        if(syncHalp->determineObChange(this, &messageStatus))
        {
            this->serialize();
            os << messageStatus << ",x:" << getPosition().getX() << ",y:"  << getPosition().getY() << ",";
            const std::string &temp = os.str();
            msize = temp.length();
            os.seekp(0);
            if(msize < 10)
            {
                os << "0" << msize;
            }
            else
            {
                os << msize;
            }
            os << temp;
            herostr = os.str();
            linecnt++;           
           // std::cout << herostr << std::endl;            
        }

        //Build Saucer string
        for (int i=0; i<saucers.size(); i++)
        {
            std::ostringstream os2;
            if(syncHalp->determineObChange(saucers[i], &messageStatus))
            {
                saucers[i]->serialize();
                os2.seekp(0);
                os2 << messageStatus << ",id:" << saucers[i]->getId() << ",x:"  << saucers[i]->getPosition().getX() << ",y:" << saucers[i]->getPosition().getY() << ",";               
                const std::string &temp = os2.str();
                msize = temp.length();
                os2.seekp(0);
                os2 << msize;
                os2 << temp;
                ss << os2.str();
                linecnt++;
                //std::cout << "saucer: "<< os2.str() << std::endl;
            }
        }
        saucerstr = ss.str();
        saucers.erase(saucers.begin(), saucers.end());

        //Build Bullet string
        for (int i=0; i<bullets.size(); i++)
        {
            std::ostringstream os3;
            if(syncHalp->determineObChange(bullets[i], &messageStatus))
            {
                bullets[i]->serialize();
                os3.seekp(0);
                os3 << messageStatus << ",id:" << bullets[i]->getId() << ",x:"  << bullets[i]->getPosition().getX() << ",y:" << bullets[i]->getPosition().getY() << ",";
                const std::string &temp = os3.str();
                msize = temp.length();
                os3.seekp(0);
                os3 << msize;
                os3 << temp;
                bbs << os3.str();
                linecnt++;
                //std::cout << "bullet: "<< os2.str() << std::endl;
            }
        }
        bulletstr = bbs.str();
        bullets.erase(bullets.begin(), bullets.end());

        //Build score string
        if(thepoints->scoreUpdate > 0)
        {
            pointsstr = "UPDATEP,p:"+ df::toString(thepoints->scoreUpdate) + ",";
            pointsstr = df::toString((int)pointsstr.length()) + pointsstr;
            thepoints->scoreUpdate = 0;
            linecnt++;
           // std::cout << "points: "<< pointsstr << std::endl;
        }

        //send out everything
        if(linecnt > 0)
        {
            std::string linecnt2;
            bs.seekp(0);
            if(linecnt < 10)
                bs << "0" << linecnt;
            else
                bs << linecnt;
            linecnt2 = bs.str();
            message = linecnt2 + herostr + saucerstr + bulletstr + pointsstr;
            //std::cout << message << std::endl;
            network_manager.send2((void *)message.c_str(), message.length());
        }


    }

}



// Send "nuke" event to all objects.
void Host::nuke() {

    // Check if nukes left.
    if (!nuke_count) 
        return;
    nuke_count--;

    // Create "nuke" event and send to interested Objects.
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    EventNuke nuke;
    world_manager.onEvent(&nuke);

    // Send "view" event do decrease number of nukes to interested ViewObjects.
    df::EventView ev("Nukes", -1, true);
    world_manager.onEvent(&ev);

    // Play "nuke" sound.
    df::Sound *p_sound = df::ResourceManager::getInstance().getSound("nuke");
    p_sound->play();
}

