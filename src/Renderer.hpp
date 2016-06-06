#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"
#include "Math.hpp"
#include "GameObject.hpp"
#include "System.hpp"

using namespace std;
using namespace ci::app;
using namespace ci;


//this is a system I guess?
class RenderSystem : public ECSystem{
    
  public:
    
    RenderSystem() : ECSystem(){
        loadPngImage( "backdrop" );
        loadPngImage( "dude/down" );
        loadPngImage( "dude/up" );
        loadPngImage( "dude/left" );
        loadPngImage( "dude/right" );
        loadPngImage( "stairhead" );
        loadPngImage( "fingers" );
        loadPngImage( "rope" );
    }

    cinder::gl::TextureRef * get( string s ){ return &images[s]; };
    
    float getWidthOfTex( SpriteComponent * sprite ){
        return images[sprite->getFileName()]->getWidth();
    }
    float getHeightOfTex( SpriteComponent * sprite ){
        return images[sprite->getFileName()]->getHeight();
    }
    
    void drawGameObjects( vector< GameObject * > objs ){
        
        //    sortedDrawers.clear();
        //    sortedDrawers.empty();
        //    int drawn = 0;
        //    bool* d = new bool( drawers.size() );
        //    for (int i = 0; i < drawers.size(); i++) d[i] = false;
        //    while( drawn < drawers.size() ){
        //        int toDraw = 0;
        //        float maxY = 600;
        //        for (int i = 0; i < drawers.size(); i++) {
        //            if( !d[i] ){
        //                float y = dynamic_cast<GameObject*>(drawers[i])->getPos().y;
        //                if( y < maxY ){
        //                    maxY = y;
        //                    toDraw = i;
        //                }
        //            }
        //        }
        //        drawn++;
        //        d[toDraw] = true;
        //        sortedDrawers.push_back( drawers[toDraw] );
        //    }
        
        
        for (int i = 0; i < objs.size(); i++){
            GameObject * obj = objs[i];
            long itemMask = SpriteComponent::bits() + PositionComponent::bits() + InteractableComponent::bits();
            long playerMask = PlayerComponent::bits();
            long rectMask = SpriteComponent::bits() + PositionComponent::bits() + RectComponent::bits();
            
            bool isItem = (obj->componentBits & itemMask) == itemMask;
            bool isPlayer = (obj->componentBits & playerMask) == playerMask;
            if( isItem ){
                drawItem( obj );
            } else if( isPlayer ){
                drawPlayer( obj );
            }
            
            if( match( obj, rectMask ) ){
                drawRect( obj );
            }
            
            

        }
    }
    
    void drawRect( GameObject * obj ){
        
        SpriteComponent * sprite = obj->getComponent<SpriteComponent>();
        RectComponent * rect = obj->getComponent<RectComponent>();
        PositionComponent * position = obj->getComponent<PositionComponent>();
        
        vec2 pos = position->getPos();
        
        Rectf aRect = Rectf(pos.x - sprite->drawOffset.x,
                            pos.y - sprite->drawOffset.y,
                            pos.x - sprite->drawOffset.x + rect->width,
                            pos.y - sprite->drawOffset.y + rect->height);
        gl::color( ColorA( 1.0f, 1.0f, 1.0f, 0.2f ) );
        gl::drawStrokedRect( aRect );
        gl::drawSolidCircle( pos, 4 );
    }
    
    void drawItem( GameObject * obj ){
        
        SpriteComponent * sprite = obj->getComponent<SpriteComponent>();
        PositionComponent * position = obj->getComponent<PositionComponent>();
        
        vec2 pos = position->getPos();
        
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::TextureRef tex = images[sprite->getFileName()];
        gl::draw( tex, pos - sprite->drawOffset );
    }
    
    void drawPlayer( GameObject * obj ){

        SpriteComponent * sprite = obj->getComponent<SpriteComponent>();
        DirectionComponent * dir = obj->getComponent<DirectionComponent>();
        PositionComponent * position = obj->getComponent<PositionComponent>();
    
        string s = sprite->filename + "/" + dir->direction;
        
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::draw( images[s], position->getPos() - sprite->drawOffset );
    }
    
    void drawBackdrop(){
        gl::clear( Color( 0, 0, 0 ) );
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::draw( images["backdrop"], vec2(0,0) );
    }
    
    
    void drawRoute( vector<vec2> playerRoute ){
        if( debug::show() ){
            gl::color( Color( 1.0f, 1.0f, 1.0f ) );
            if(playerRoute.size() > 1 ){
                for(int i = 0; i < playerRoute.size() - 1; i++){
                    gl::drawLine( playerRoute[i], playerRoute[i+1] );
                }
            }
        }
    }
    
    
    void drawVec2( vec2 pos ){
        if( debug::show() ){
            gl::drawString( "" + to_string( int(pos.x) ) + "," + to_string( int(pos.y) ), pos );
        }
    }
    
    //slap this on a FBO
    void drawNavMesh( vector< vector<vec2> > shapes , vector<vec2> nodes ){
        for(int i = 0; i < shapes.size(); i++){
            drawNavPoly( shapes[i], false );
        }
        gl::color( ColorA( 1.0f, 0.1f, 0.0f, 0.5f ) );
        for(int i = 0; i < nodes.size(); i++){
            gl::drawSolidCircle( nodes[i], 5.0f );
        }
    }
    
//    void drawNavMesh( NavMesh * navMesh ){
//        
//        vector<NavPolygon> polys = navMesh->polys;
//        vector<Node*> nodes = navMesh->nodes;
//        
//        for(int i = 0; i < polys.size(); i++){
//            drawNavPoly( polys[i].points, false );
//        }
//        
//        for(int i = 0; i < nodes.size(); i++){
//            vector<vec2> neighbours;
//            for(int n = 0; n < nodes[i]->neighbours.size(); n++){
//                neighbours.push_back( nodes[i]->neighbours[n]->pos );
//            }
//            drawNode( nodes[i]->pos, nodes[i]->name, false, neighbours );
//        }
//    }
    
    void drawNavPoly( vector<vec2> points, bool selected ){
        if( debug::show() ){
            Shape2d shape;
            shape.moveTo( points[0] );
            if( selected ) drawVec2( points[0] );
            for(int i = 1; i < points.size(); i++){
                shape.lineTo( points[i] );
                if( selected ) drawVec2( points[i] );
            }
            shape.close();
            
            gl::color( ColorA( 1.0f, 0.0f, 0.0f, 0.05f ) );
            if( selected ) gl::color( ColorA( 1.0f, 0.0f, 0.0f, 0.35f ) );
            
            gl::drawSolid( shape );
            gl::color( ColorA( 1.0f, 0.0f, 0.0f, 0.5f ) );
            gl::draw( shape );
        }
//        if( selected ) gl::drawStrokedCircle( centerRef->pos, 15 );
 
    }
    
    void drawNode( vec2 pos, int name, bool selected, vector<vec2> neighbours ){
        if( debug::show() ){
            gl::color( ColorA( 1.0f, 0.1f, 0.0f, 0.5f ) );
            if( selected ) gl::color( Color( 1.0f, 0.6f, 0.4f ) );
            gl::drawSolidCircle( pos, 5.0f );
            gl::drawString( " " + to_string(name), pos );
            if( selected ){
                for (int i = 0; i < neighbours.size(); i++) {
                    gl::drawLine( neighbours[i], pos );
                }
            }
        }
    }
    
    
  private:
    
     void loadPngImage( string src ){
        string fullsrc = src + ".png";
        try {
            auto img = loadImage( loadAsset( fullsrc ) );
            images[src] = gl::Texture2d::create( img );
        } catch( Exception &exc ) {
            CI_LOG_EXCEPTION( "failed to load image.", exc );
        }
    }
    
    map<string, gl::TextureRef>	images;
    
};


#endif /* Renderer_hpp */
