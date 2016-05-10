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
#include "GameObject.hpp"

class GameEvent{
  public:
    GameEvent(){ }
    virtual void trigger() = 0;
};

class EmptyEvent : public GameEvent{
    void trigger(){}
};

class PickUpItemEvent : public GameEvent{
  public:
    PickUpItemEvent( Interactable * i ){
        item = i;
    }
    void trigger(){
        std::cout << "picking up " << item->name << "\n";
    }
  private:
    Interactable * item;
};

#endif /* Event_hpp */
