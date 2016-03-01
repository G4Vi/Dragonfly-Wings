//
// Client.cpp
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
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "Client.h"
#include "Bullet.h"
#include "RemoteShip.h"

// Networking
#include "Sentry.h"
#include "NetworkManager.h"
#include <sstream>
#include <stdlib.h>		// for rand()
#include <iostream> //will remove
int sExplode;
std::vector<Bullet*> bullets2;

Client::Client(Settings* info) {

    // Link to "ship" sprite.
    df::ResourceManager &resource_manager = df::ResourceManager::getInstance();
    df::LogManager &log_manager = df::LogManager::getInstance();
    df::Sprite *p_temp_sprite;
    p_temp_sprite = resource_manager.getSprite("ship");
    if (!p_temp_sprite) {
        log_manager.writeLog("Client::Client(): Warning! Sprite '%s' not found",
                             "ship");
    } else {
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
    //setType("Hero");
    setType("Client");

    // Set starting location.
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    df::Position pos(7, (world_manager.getBoundary().getVertical()/2)+7);
    setPosition(pos);

    // Create reticle for firing bullets2.
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

    thepoints = new Points(1);

    //std::cout << "the points are " << df::toString(thepoints->getValue()) << std::endl;

}

Client::~Client() {
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    Object* temp2;

    network_manager.shutDown();
    if(sExplode == 1)
    {
        temp2 = otherPlayer;
    }
    else
    {
        temp2 = this;
    }
    otherPlayer->killowner = 0;

    world_manager.markForDelete(otherPlayer);
    world_manager.markForDelete(thepoints);

    // Create GameOver object.
    GameOver *p_go = new GameOver;

    //Explosion
    exp(temp2);

    // Mark Reticle for deletion.
    df::WorldManager::getInstance().markForDelete(p_reticle);
}

int Client::exp(Object* toexplode)
{
    for (int i=-8; i<=8; i+=5) {
        for (int j=-5; j<=5; j+=3) {
            df::Position temp_pos = toexplode->getPosition();
            temp_pos.setX(toexplode->getPosition().getX() + i);
            temp_pos.setY(toexplode->getPosition().getY() + j);
            Explosion *p_explosion = new Explosion;
            p_explosion -> setPosition(temp_pos);
        }
    }
}

// Handle event.
// Return 0 if ignored, else 1.
int Client::eventHandler(const df::Event *p_e) {
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
void Client::network(const df::EventNetwork *p_network_event) {
    std::string type, data;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    std::cout << p_network_event->line << std::endl;

    if(memcmp(p_network_event->line, "NEW", 3)==0)
    {
        data = (p_network_event->line+4);
        if(memcmp(p_network_event->line, "NEWH", 4) == 0)
        {
            otherPlayer = new RemoteShip(0);
            std::string x = df::match(data.c_str(), "x");
            std::string y = df::match(data.c_str(), "y");
            df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
            world_manager.moveObject(otherPlayer, new_pos);
        }
        else if(memcmp(p_network_event->line, "NEWS", 4) == 0)
        {
            Saucer* tempSaucer = new Saucer;
            tempSaucer->host = 0;
            //saucers.push_back(new Saucer);
            std::string id = df::match(data.c_str(), "id");
            //saucers.back()->setId(atoi(id.c_str()));
            tempSaucer->setId(atoi(id.c_str()));
            std::string x = df::match(data.c_str(), "x");
            std::string y = df::match(data.c_str(), "y");
            df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
            //world_manager.moveObject(saucers.back(), new_pos);
            world_manager.moveObject(tempSaucer, new_pos);
        }
        else if(memcmp(p_network_event->line, "NEWB", 4) == 0)
        {
            std::string x = df::match(data.c_str(), "x");
            std::string y = df::match(data.c_str(), "y");
            df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
            Bullet* ab = new Bullet(new_pos);
            std::string id = df::match(data.c_str(), "id");
            ab->setId(atoi(id.c_str()));
            ab->setType("ClientBullet");

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
        else if(memcmp(p_network_event->line, "UPDATEP", 7) == 0)
        {
            std::string p = df::match(data.c_str(), "p");
            thepoints->setValue(thepoints->getValue()+atoi(p.c_str()));
        }
    }
    else if(memcmp(p_network_event->line, "DELETE", 6)==0)
    {
        data = (p_network_event->line+7);
        if(memcmp(p_network_event->line, "DELETEH", 7) == 0)
        {
            sExplode = 0;
            world_manager.markForDelete(this);
        }
        else if(memcmp(p_network_event->line, "DELETER", 7) == 0)
        {
            sExplode = 1;
            world_manager.markForDelete(this);
        }
        else if(memcmp(p_network_event->line, "DELETEID", 8) == 0)
        {
            std::string id = df::match(data.c_str(), "id");
            df::ObjectList all_objects = world_manager.getAllObjects();
            df::ObjectListIterator i(&all_objects);
            for (i.first(); !i.isDone(); i.next()) {
                if(i.currentObject()->getId() == atoi(id.c_str()))
                {
                    world_manager.markForDelete(i.currentObject());
                    break;
                }
            }
        }

    }
}

// Take appropriate action according to mouse action.
void Client::mouse(const df::EventMouse *p_mouse_event) {

    // Pressed button?
    if ((p_mouse_event->getMouseAction() == df::CLICKED) &&
            (p_mouse_event->getMouseButton() == df::Mouse::LEFT))
        fire(p_mouse_event->getMousePosition());
}

// Take appropriate action according to key pressed.
void Client::kbd(const df::EventKeyboard *p_keyboard_event) {

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
void Client::move(int dy) {

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
void Client::fire(df::Position target) {

    // See if time to fire.
    if (fire_countdown > 0)
        return;
    fire_countdown = fire_slowdown;

    // Fire Bullet towards target.
    Bullet *p = new Bullet(getPosition());
    p->setYVelocity((float) (target.getY() - getPosition().getY()) /
                    (float) (target.getX() - getPosition().getX()));

    p->setType("ClientBullet");

    bullets2.push_back(p);

    // Play "fire" sound.
    df::Sound *p_sound = df::ResourceManager::getInstance().getSound("fire");
    p_sound->play();
}

// Decrease rate restriction counters.
void Client::step() {

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
        std::string herostr, bullets2tr, message, messageStatus;
        std::ostringstream os, bs, bbs;
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

        //Build Bullet string
        for (int i=0; i<bullets2.size(); i++)
        {
            std::ostringstream os3;
            if(syncHalp->determineObChange(bullets2[i], &messageStatus))
            {
                bullets2[i]->serialize();
                os3.seekp(0);
                os3 << messageStatus << ",id:" << bullets2[i]->getId() << ",x:"  << bullets2[i]->getPosition().getX() << ",y:" << bullets2[i]->getPosition().getY() << ",";
                std::cout << "os3 is " << os3.str() << std::endl;
                const std::string &temp = os3.str();
                msize = temp.length();
                os3.seekp(0);
                os3 << msize;
                os3 << temp;
                bbs << os3.str();
                linecnt++;
                //std::cout << "bullet: "<< os3.str() << std::endl;
            }
        }
        bullets2tr = bbs.str();
        bullets2.erase(bullets2.begin(), bullets2.end());

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
            message = linecnt2 + herostr + bullets2tr;
            //std::cout << message << std::endl;
            network_manager.send2((void *)message.c_str(), message.length());
        }

    }

}



// Send "nuke" event to all objects.
void Client::nuke() {

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

