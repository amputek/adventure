//
//  System.hpp
//  Adventure
//
//  Created by Rob Dawson on 24/05/2016.
//
//

#ifndef System_hpp
#define System_hpp

#include "NavMesh.cpp"
#include "GameObject.hpp"
#include "Event.hpp"

#include <stdio.h>

class ECSystem{
  public:
    static bool match( GameObject * obj, long mask ){
        return (obj->componentBits & mask) == mask;
    }
};

class RectSystem : public ECSystem{
public:
    static bool inRect( GameObject * obj, vec2 mousepos ){
        vec2 pos = obj->getComponent<PositionComponent>()->getPos();
        vec2 dof = obj->getComponent<SpriteComponent>()->drawOffset;
        float w = obj->getComponent<RectComponent>()->width;
        float h = obj->getComponent<RectComponent>()->height;
        Rectf rect = Rectf( pos.x - dof.x, pos.y - dof.y, pos.x - dof.x + w, pos.y - dof.y + h );
        return mousepos.x > rect.x1 && mousepos.y > rect.y1 && mousepos.x < rect.x2 && mousepos.y < rect.y2;
    }
};

class HoverSystem : public ECSystem{
public:
    static void update( vector<GameObject*> objs, NavMesh * navMesh, vec2 mousepos ){
        
        hovered = "";
        
        if( navMesh->pointInsideNavMesh( mousepos ) ){
            hovered = "move";
        }
        
        long requiredBitMask = HoverableComponent::bits() + RectComponent::bits();
        for(vector<GameObject*>::iterator it = objs.begin(); it != objs.end(); ++it) {
            if( match( *it, requiredBitMask ) ){
                if( RectSystem::inRect( *it, mousepos ) ){
                    hovered = (*it)->getComponent<HoverableComponent>()->hoverText;
                    break;
                }
            }
        }
    }
    static void draw(){
        gl::drawString( hovered, vec2(40,40) );
    }
private:
    static string hovered;
};


class InventorySystem : public ECSystem {
public:
    static vector<GameObject*> inventoryItems;
    static void addItem( GameObject * item ){
        inventoryItems.push_back( item );
        item->removeComponent<PositionComponent>();
        item->removeComponent<RectComponent>();
        item->removeComponent<ItemComponent>();
        item->removeComponent<PositionComponent>();
        item->removeComponent<InteractableComponent>();
        item->removeComponent<HoverableComponent>();

        cout << "removing " << item->getName() << "\n";
    }
    void removeItem( GameObject * item ){
        
    }
    static void draw(){
        int i = 40;
        for(vector<GameObject*>::iterator it = inventoryItems.begin(); it != inventoryItems.end(); ++it) {
            gl::drawString( (*it)->getName() , vec2(i, 140) );
            i += 30;
        }
    }
};


class MovementSystem : public ECSystem{
  public:
    static void update( GameObject * player ){
        RouteFollowerComponent * pRoute = player->getComponent<RouteFollowerComponent>();
        PositionComponent * pPos = player->getComponent<PositionComponent>();
        DirectionComponent * pDir = player->getComponent<DirectionComponent>();
        
        vec2 target = getTarget( pPos, pRoute );
        
        if( target != vec2(-1,-1) ){
            vec2 facingAngle = getFacingAngle( pPos->getPos(), target );
            
            vec2 newPosition = pPos->getPos() + facingAngle * 2.0f;
            pPos->setPos( newPosition );
            
            setDir( facingAngle, pDir );
        }
    }
    static void setDir( vec2 d, DirectionComponent * pDir ){
        float right = distance(vec2(1,0),d);
        float left = distance(vec2(-1,0),d);
        float up = distance(vec2(0, -1),d);
        float down = distance(vec2(0, 1),d);
        double smallest = min( min(up, down), min(left, right));
        if( smallest == right ) pDir->direction = "right";
        if( smallest == up ) pDir->direction = "up";
        if( smallest == left ) pDir->direction = "left";
        if( smallest == down ) pDir->direction = "down";
    }
    static vec2 getFacingAngle( vec2 pos, vec2 target ){
        
        if( target != vec2(-1,-1) ){
            float dir = atan2( target.y - pos.y, target.x - pos.x );
            vec2 beep = vec2( cos(dir), sin(dir) );
            return beep;
        }
        return vec2(-1,-1);
    }
    static vec2 getTarget( PositionComponent * p, RouteFollowerComponent * rfc ){
        vec2 currentPos = p->getPos();
        if( rfc->route.size() > 1 ){
            if( distance( currentPos, rfc->route[rfc->routeIndex] ) < 5.0f ){
                if( rfc->routeIndex < rfc->route.size() - 1){
                    rfc->routeIndex++;
                    return rfc->route[rfc->routeIndex];
                } else {
                    rfc->route.clear();
//                    rfc->plannedEvent->trigger();
                    cout << rfc->plannedEvent->type() << "\n";
                    if( rfc->plannedEvent->type() == "item" ){
                        PickUpItemEvent * event = static_cast<PickUpItemEvent*>(rfc->plannedEvent);
                        GameObject * item = event->item;
                        InventorySystem::addItem( item );
                        cout << "ok" << "\n";
                    }
                    
                    return vec2(-1,-1);
                }
            }
            return rfc->route[rfc->routeIndex];
        }
        return vec2(-1,-1);
    }
};


//this class is kinda pointless... make it part of navmesh?
//this is a system!
class PathFinderSystem : public ECSystem{
public:
    
    static void movePlayerTo( NavMesh * navMesh, GameObject * player, vector<GameObject*> objs, vec2 mousepos ){
        
        generateRoute( navMesh, player, mousepos );
        
        for(vector<GameObject*>::iterator it = objs.begin(); it != objs.end(); ++it) {
            
            long interactableMask = InteractableComponent::bits() + RectComponent::bits();
            
            if( match( *it , interactableMask ) ){
                if( RectSystem::inRect( *it, mousepos ) ){
                    generateRoute( navMesh, player, *it );
                }
            }
        }
    }
    
private:
    static void generateRoute( NavMesh * navMesh, GameObject * player, GameObject * target ){
        cout << "Moving To: " << target->getName() << "\n";
        vec2 targetPosition = target->getComponent<PositionComponent>()->getPos() + target->getComponent<InteractableComponent>()->walkToOffset;
        vector<vec2> posList = navMesh->generateRoute( targetPosition, player->getComponent<PositionComponent>()->getPos() );
        if( posList.size() > 1 ){
            if( match( target , ItemComponent::bits() ) ){
                player->getComponent<RouteFollowerComponent>()->setRoute( posList, new PickUpItemEvent( target ) );
                cout << "ITEM YO" << "\n";
            } else {
                player->getComponent<RouteFollowerComponent>()->setRoute( posList, new EmptyEvent() );
            }
        }
    }
    static void generateRoute( NavMesh * navMesh, GameObject * player, vec2 target ){
        vector<vec2> posList = navMesh->generateRoute( target, player->getComponent<PositionComponent>()->getPos() );
        if( posList.size() > 1 ) player->getComponent<RouteFollowerComponent>()->setRoute( posList, new EmptyEvent() );
    }
};






#endif /* System_hpp */
