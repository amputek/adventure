//
//  Event.hpp
//  Adventure
//
//  Created by Rob Dawson on 06/05/2016.
//
//

#ifndef Event_hpp
#define Event_hpp

#include <stdio.h>
//#include "System.hpp"

class GameObject;

class GameEvent{
  public:
    GameEvent(){ }
    virtual void trigger() = 0;
    virtual std::string type() = 0;
};

class EmptyEvent : public GameEvent{
    void trigger(){}
    std::string type(){ return "empty"; }
};

class PickUpItemEvent : public GameEvent{
  public:
    PickUpItemEvent( GameObject * i ){
        item = i;
    }
    void trigger(){
        std::cout << "trigger" << "\n";
//        InventorySystem::addItem( item );
//        std::cout << "picking up " << item->getName() << "\n";
    }
    std::string type(){ return "item"; }
    GameObject * item;
};

//class InteractEvent : public GameEvent{
//  public:
//    InteractEvent( Interactable * i ){
//        doodad = i;
//    }
//    void trigger(){
//        std::cout << "interactig with up " << doodad->name << "\n";
//    }
//  private:
//    Interactable * doodad;
//};


#endif /* Event_hpp */
