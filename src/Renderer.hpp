#ifndef Renderer_hpp
#define Renderer_hpp

#include <stdio.h>
#include "cinder/ImageIo.h"
#include "cinder/gl/Texture.h"
#include "cinder/Log.h"
#include "Math.hpp"


using namespace std;
using namespace ci::app;
using namespace ci;

class GameRenderer{
    
  public:
    
    GameRenderer(){
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
    
    void drawBackdrop(){
        gl::clear( Color( 0, 0, 0 ) );
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::draw( images["backdrop"], vec2(0,0) );
    }
    
    void drawItem( vec2 pos, vec2 dof, vec2 wtp, string item ){
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::TextureRef tex = images[item];
        
        float w = tex->getWidth();
        float h = tex->getHeight();
        gl::draw( tex, pos - dof );
        
        if( debug::show() ){
            gl::drawStrokedRect( Rectf( pos.x - dof.x, pos.y - dof.y, pos.x - dof.x + w, pos.y - dof.y + h ) );
            gl::drawSolidCircle( pos, 4 );
            gl::drawSolidCircle( pos + wtp, 3 );
        }
    }
    
    void drawPlayer( vec2 pos, string direction ){
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        string s = "dude/" + direction;
        gl::draw( images[s], pos - vec2(31,90) );
        if( debug::show() ){
            gl::color( Color( 0.2f, 0.8f, 1.0f ) );
            gl::drawSolidCircle( pos, 2 );
        }
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
    
    void drawDoodad( vec2 pos, string name ){
        gl::color( Color( 1.0f, 1.0f, 1.0f ) );
        gl::draw( images[name], pos );
    }
    
    void drawVec2( vec2 pos ){
        if( debug::show() ){
            gl::drawString( "" + to_string( int(pos.x) ) + "," + to_string( int(pos.y) ), pos );
        }
    }
    
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
