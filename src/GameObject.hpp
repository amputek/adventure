
#ifndef GameObject_hpp
#define GameObject_hpp

#include <stdio.h>
#include "Math.hpp"
#include "Event.hpp"

using namespace gmath;
using namespace ci;
using namespace std;


class Component{
  public:
    static unsigned long bits(){ return 0x0; }
};

class GameObject{
    
public:
    GameObject(string a){ name = a; }
    unsigned long componentBits = 0x0;
    
    template<class T>
    void addAComponent( T c ){
        components[ &typeid(*c) ] = c;
        componentBits += c->bits();
    }
    
    template <typename T>
    T* getComponent(){
        // cout << "GETTING: " << &typeid(T) << "\n";
        if(components.count(&typeid(T)) != 0){
            return static_cast<T*>(components[&typeid(T)]);
        } else {
            // return NullComponent;
            cout << "component don't exist yo" << "\n";
            return nullptr;
        }
    }
    
    template <typename T>
    void removeComponent(){
        if(components.count(&typeid(T)) != 0){
            components.erase( &typeid(T) );
            componentBits -= T::bits();
        } else {
            cout << "no component to erase" << "\n";
        }
    }
    
    
    string getName(){ return name; }
    int noOfComponents(){ return components.size(); };
    
private:
    unordered_map< const type_info* , Component * > components;
    string name;
};

class ItemComponent : public Component{
public:
    static unsigned long bits(){ return 0x100; }
};

class HoverableComponent : public Component {
  public:
    HoverableComponent( string ht ){
        hoverText = ht;
    }
    string hoverText;
    static unsigned long bits(){ return 0x80; }
};

class PlayerComponent : public Component{
  public:
    static unsigned long bits(){ return 0x40; }
};

class RouteFollowerComponent : public Component{
  public:
    RouteFollowerComponent(){}
    void setRoute( vector<vec2> r, GameEvent * e ){
        route = r;
        routeIndex = 0;
        plannedEvent = e;
    }
    static unsigned long bits(){ return 0x20; }
//private:
    vector<vec2> route = vector<vec2>(0);
    GameEvent * plannedEvent;
    int routeIndex = 0;
};

class DirectionComponent : public Component{
  public:
    DirectionComponent(){
        direction = "down";
    }
    string direction = "down";
    static unsigned long bits(){ return 0x4; }
};

class SpriteComponent : public Component{
  public:
    SpriteComponent(){}
    SpriteComponent(string a, vec2 dof){
        filename = a;
        drawOffset = dof;
    }
    string filename;
    vec2 drawOffset;
    string getFileName(){ return filename; }
    static unsigned long bits(){ return 0x2; }
};


class PositionComponent : public Component{
  public:
    PositionComponent(){}
    PositionComponent(vec2 a){ pos = a; }
    vec2 getPos(){ return pos; };
    void setPos( vec2 a ){ pos = a; };
    static unsigned long bits(){ return 0x1; }
  private:
    vec2 pos;
};


class RectComponent : public Component{
public:
    RectComponent();
    RectComponent( int w, int h ){
        width = w;
        height = h;
    }
    int width, height;
    static unsigned long bits(){ return 0x8; }
};


class InteractableComponent : public Component{
  public:
    InteractableComponent();
    InteractableComponent( vec2 wtp ){
        walkToOffset = wtp;
    }
    vec2 walkToOffset = vec2(0,0);
    static unsigned long bits(){ return 0x10; }
};




#endif /* GameObject_hpp */
