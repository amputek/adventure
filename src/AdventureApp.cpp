#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "Math.hpp"
#include "Renderer.hpp"
#include "PathFinder.cpp"
#include "GameObject.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace math;

class Player : public GameObject{
  public:
    Player(){
        pos = vec2(170,260);
        currentRoute = new Route( vector<vec2>(0) );
    };
    Route * currentRoute;
    void update(){
        vec2 target = currentRoute->getTarget( pos );
        if( target == vec2(-1,-1) ){
            
        } else {
            float dir = atan2( target.y - pos.y, target.x - pos.x );
            vec2 beep = vec2( cos(dir), sin(dir) );
            pos += beep * 2.0f;
            direction = getDir( beep );
        }
    }
    void setRoute( vector<vec2> r, GameEvent * e ){
//        currentRoute->setup( r, event );
    }
    void draw( GameRenderer * renderer ){
        renderer->drawPlayer( pos, direction );
        renderer->drawRoute( currentRoute->getVec2s() );
    }
    string getDir( vec2 d ){
        float right = distance(vec2(1,0),d);
        float left = distance(vec2(-1,0),d);
        float up = distance(vec2(0, -1),d);
        float down = distance(vec2(0, 1),d);
        double smallest = min( min(up, down), min(left, right));
        if( smallest == right ) return "right";
        if( smallest == up ) return "up";
        if( smallest == left ) return "left";
        if( smallest == down ) return "down";
        return "down";
    }
    string direction = "down";
};

class AdventureApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
	void update() override;
	void draw() override;
    GameRenderer * renderer;
    
    Player player;
    PathFinder pathFinder;
    
    Interactable ropeItem;
    Interactable stairs;
    Interactable fingers;
    
    vector<GameObject*> drawers;
    vector<GameObject*> sortedDrawers;
    
};

void AdventureApp::setup(){
    setWindowSize( 800, 600 );
    
    gl::enable( GL_LINE_SMOOTH );
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    gl::enableAlphaBlending();
    
    renderer = new GameRenderer();
    player = * new Player();
    
    ropeItem = * new Interactable( "rope"      , vec2(142, 416), vec2(25,10), vec2(0,0), 50, 20 );
    stairs   = * new Interactable( "stairhead" , vec2(510, 335), vec2(40,220), vec2(-20,30), 120, 240 );
    fingers  = * new Interactable( "fingers"   , vec2(420, 329), vec2(40,90), vec2(0,0), 120, 240 );
    
    
    drawers.push_back( &ropeItem );
    drawers.push_back( &stairs );
    drawers.push_back( &player );
    drawers.push_back( &fingers );
    
    vector<NavPolygon> ps;
    
    vector<vec2> p;
    p.push_back( vec2(   92.000,  256.000 ) );
    p.push_back( vec2(  231.000,  237.000 ) );
    p.push_back( vec2(  328.000,  260.000 ) );
    p.push_back( vec2(  301.000,  281.000 ) );
    p.push_back( vec2(  237.000,  292.000 ) );
    p.push_back( vec2(  162.000,  287.000 ) );
    ps.push_back( *new NavPolygon( p ) );
    
    vector<vec2> p2;
    p2.push_back( vec2(  301.000,  281.000 ) );
    p2.push_back( vec2(  237.000,  292.000 ) );
    p2.push_back( vec2(  299.000,  346.000) );
    p2.push_back( vec2(  381.000,  331.000) );
    ps.push_back( *new NavPolygon( p2 ) );

    vector<vec2> p3;
    p3.push_back( vec2(  299.000,  346.000) );
    p3.push_back( vec2(  381.000,  331.000) );
    p3.push_back( vec2(  470.000,  312.000) ); //here
    p3.push_back( vec2(  481.000,  351.000) );
    p3.push_back( vec2(  473.000,  415.000) ); //here
    p3.push_back( vec2(  296.000,  374.000) );
    p3.push_back( vec2(  276.000,  356.000) );
    ps.push_back( *new NavPolygon( p3 ) );

    vector<vec2> p4;
    p4.push_back( vec2(  473.000,  415.000) );
    p4.push_back( vec2(  296.000,  374.000) );
    p4.push_back( vec2(  131.000,  383.000) );
    p4.push_back( vec2(   44.000,  433.000) );
    p4.push_back( vec2(  302.000,  457.000) );
    ps.push_back( *new NavPolygon( p4 ) );
    
    vector<vec2> p5;
    p5.push_back( vec2(  649.000,  380.000) );
    p5.push_back( vec2(  669.000,  344.000) );
    p5.push_back( vec2(  795.000,  341.000) );
    p5.push_back( vec2(  795.000,  369.000) );
    ps.push_back( *new NavPolygon( p5 ) );
    
    vector<vec2> p6;
    p6.push_back( vec2(  649.000,  380.000) );
    p6.push_back( vec2(  669.000,  344.000) );
    p6.push_back( vec2(  593.000,  324.000) );
    p6.push_back( vec2(  580.000,  352.000) );
    p6.push_back( vec2(  572.000,  398.000) );
    ps.push_back( *new NavPolygon( p6 ) );
    
    vector<vec2> p7;
    p7.push_back( vec2(  580.000,  352.000) );
    p7.push_back( vec2(  572.000,  398.000) );
    p7.push_back( vec2(  473.000,  415.000) ); //here
    p7.push_back( vec2(  481.000,  351.000) );
    
    ps.push_back( *new NavPolygon( p7 ) );
    
    NavMesh * navMesh = new NavMesh( ps );
    pathFinder = *new PathFinder( navMesh );
    
}

void AdventureApp::mouseMove( MouseEvent event ){
//    vec2 mousepos = vec2(event.getX(), event.getY() );
    
//    if( ropeItem.mouseOver( mousepos ) ){
//        
//    } else if( stairs.mouseOver( mousepos ) ){
//        
//    } else {
//        
//    }
}

void AdventureApp::mouseDown( MouseEvent event ){
    vec2 mousepos = vec2(event.getX(), event.getY() );
    cout << mousepos << "\n";
    
//    for( int i = 0; i < interactables.size(); i++ ){
//        Interactable * in = interactables[i];
//        pathFinder.setRoute( player.currentRoute, in->getWalkToPos(), player.getPos(), new EmptyEvent()  );
//    }
//
    if( stairs.mouseDown( mousepos ) ){
        pathFinder.setRoute( player.currentRoute, stairs.getWalkToPos(), player.getPos(), new EmptyEvent()  );
    } else if( ropeItem.mouseDown(mousepos) ){
        pathFinder.setRoute( player.currentRoute, ropeItem.getWalkToPos(), player.getPos(), new PickUpItemEvent( &ropeItem ) );
    } else {
        pathFinder.setRoute( player.currentRoute, mousepos, player.getPos(), new EmptyEvent() );
    }
}

void AdventureApp::update(){
    player.update();
    
    sortedDrawers.clear();
    sortedDrawers.empty();
    int drawn = 0;
    bool* d = new bool( drawers.size() );
    for (int i = 0; i < drawers.size(); i++) d[i] = false;
    while( drawn < drawers.size() ){
        int toDraw = 0;
        float maxY = 600;
        for (int i = 0; i < drawers.size(); i++) {
            if( !d[i] ){
                float y = dynamic_cast<GameObject*>(drawers[i])->getPos().y;
                if( y < maxY ){
                    maxY = y;
                    toDraw = i;
                }
            }
        }
        drawn++;
        d[toDraw] = true;
        sortedDrawers.push_back( drawers[toDraw] );
    }
}

void AdventureApp::draw(){
    renderer->drawBackdrop();
    
    for (int i = 0; i < sortedDrawers.size(); i++){
        sortedDrawers[i]->draw( renderer );
    }

    pathFinder.draw( renderer );
    
}

CINDER_APP( AdventureApp, RendererGl )
