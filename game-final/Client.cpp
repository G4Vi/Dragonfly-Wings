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
#include "Bullet.h"
#include "EventNuke.h"
#include "Explosion.h"
#include "GameOver.h"
#include "Client.h"
#include "RemoteShip.h"

// Networking
#include "Sentry.h"
#include "NetworkManager.h"
#include <iostream> //will remove
char cpacket[4096];

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
    setType("Hero");

    // Set starting location.
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    df::Position pos(7, (world_manager.getBoundary().getVertical()/2)+7);
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

}

Client::~Client() {

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
    std::string type, data, sprite_name;
    df::NetworkManager &network_manager = df::NetworkManager::getInstance();
    df::WorldManager &world_manager = df::WorldManager::getInstance();
    std::cout << p_network_event->line << std::endl;

    if(memcmp(p_network_event->line, "NEW", 3)==0)
    {
        data = (p_network_event->line+3);
        if(memcmp(p_network_event->line, "NEWH", 4) == 0)
        {
            otherPlayer = new RemoteShip;
            std::string x = df::match(data.c_str(), "pos-x");
            std::string y = df::match(data.c_str(), "pos-y");
            df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
            world_manager.moveObject(otherPlayer, new_pos);
        }
    }


    /*memset(cpacket, 0, 4096);
    int i = network_manager.receive(cpacket, 4096, false);
    if(i > 0)
    {       
        cpacket[4095] = '\0';
        std::cout<< cpacket << std::endl;
        if(memcmp(cpacket, "NEW", 3)==0)
        {
            data = (cpacket+3);
            type = df::match(cpacket, "type");
            if(type == "Hero")
            {
                otherPlayer = new RemoteShip;

                if(otherPlayer->getType() == "RemoteShip")
                {
                    std::string x = df::match(data.c_str(), "pos-x");
                    std::string y = df::match(data.c_str(), "pos-y");
                    df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
                    world_manager.moveObject(otherPlayer, new_pos);
                }
            }
            if(type == "Saucer")
            {
                std::cout << "suacerpos\n";
                saucers.push_back(new Saucer);
                std::string x = df::match(data.c_str(), "pos-x");
                std::string y = df::match(data.c_str(), "pos-y");
                df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
                world_manager.moveObject(saucers.back(), new_pos);
                //syncHalp->process(sauce, data);
            }
        }
        else if(memcmp(cpacket, "UPDATE", 6)==0)
        {
            sprite_name = df::match(cpacket, "sprite_name");
            if(sprite_name == "ship")
            {
                data = (cpacket+6);
                if(otherPlayer->getType() == "RemoteShip")
                {
                    std::string x = df::match(data.c_str(), "pos-x");
                    std::string y = df::match(data.c_str(), "pos-y");
                    df::Position new_pos(atoi(x.c_str()), atoi(y.c_str()));
                    world_manager.moveObject(otherPlayer, new_pos);
                }
            }
        }
        else
        {
           data = cpacket;
        }
        std::cout<< data << std::endl;
    }*/
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
        std::string messageStatus;
        //Local ship
        if(syncHalp->determineObChange(this, &messageStatus))
        {            
            syncHalp->sendObject(this, messageStatus);
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

