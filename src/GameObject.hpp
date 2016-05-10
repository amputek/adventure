//
//  GameObject.hpp
//  Adventure
//
//  Created by Rob Dawson on 06/05/2016.
//
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Renderer.hpp"

using namespace ci;
using namespace std;


class GameObject{
  public:
    vec2 getPos(){ return pos; }
    virtual void draw( GameRenderer * renderer ) = 0;
  protected:
    vec2 pos;
};

class Interactable : public GameObject {
  public:
    Interactable(){};
    Interactable( string n, vec2 p, vec2 dof, vec2 wtp, int w, int h ){
        pos = p;
        name = n;
        width = w;
        height = h;
        drawOffset = dof;
        walkToOffset = wtp;
    }
    int width;
    int height;
    vec2 drawOffset = vec2(0,0);
    vec2 walkToOffset = vec2(0,0);
    string name;
    vec2 getWalkToPos(){
        return pos + walkToOffset;
    }
    bool inRect( vec2 mousepos ){
        return mousepos.x > pos.x - width/2 && mousepos.x < pos.x + width/2 && mousepos.y > pos.y - height/2 && mousepos.y < pos.y + height/2;
    }
    bool mouseOver( vec2 mousepos ){
        return inRect( mousepos );
    }
    bool mouseDown( vec2 mousepos ){
        return inRect( mousepos );
    }
    void draw( GameRenderer * renderer ){
        renderer->drawItem( pos, drawOffset, walkToOffset, name );
    }
};

class Item : public Interactable {
    
};


#endif /* GameObject_hpp */
