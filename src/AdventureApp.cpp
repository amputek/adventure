#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "Math.hpp"
#include "Renderer.hpp"
#include "System.hpp"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace gmath;

class AdventureApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
    void mouseMove( MouseEvent event ) override;
	void update() override;
	void draw() override;
    
    RenderSystem * renderer;

    vector<GameObject*> gameObjects;
    GameObject * player;
    
    NavMesh * navMesh;
    
    
};


string HoverSystem::hovered;
vector<GameObject*> InventorySystem::inventoryItems;

void AdventureApp::setup(){
    setWindowSize( 800, 600 );
    setWindowPos( 600, 0 );
    
    gl::enable( GL_LINE_SMOOTH );
    glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
    gl::enableAlphaBlending();
    
    renderer = new RenderSystem();

//    Switcher * fingers  = new Switcher( "fingers"   , vec2(420, 329), vec2(40,90) , vec2(-20,20), 100, 107 );
    
    
    player = new GameObject("player");
    player->addAComponent( new PositionComponent( vec2(170,260) ) );
    player->addAComponent( new SpriteComponent("dude", vec2(31,90)) );
    player->addAComponent( new RouteFollowerComponent() );
    player->addAComponent( new DirectionComponent() );
    player->addAComponent( new PlayerComponent() );
    player->addAComponent( new HoverableComponent( "player" ) );
    player->addAComponent( new RectComponent(61,100) );
    
    GameObject * ropeItem = new GameObject("rope");
    ropeItem->addAComponent( new PositionComponent( vec2(142, 416) ) );
    ropeItem->addAComponent( new RectComponent(  50, 20 ) );
    ropeItem->addAComponent( new InteractableComponent( vec2(25,10) ) );
    ropeItem->addAComponent( new SpriteComponent( "rope", vec2(25,10) ) );
    ropeItem->addAComponent( new HoverableComponent( "pick up rope" ) );
    ropeItem->addAComponent( new ItemComponent() );
    
    GameObject * stairs = new GameObject("stairs");
    stairs->addAComponent( new PositionComponent( vec2(510, 335) ) );
    stairs->addAComponent( new InteractableComponent( vec2(-20,30) ) );
    stairs->addAComponent( new SpriteComponent( "stairhead", vec2(40,220) ) );
    stairs->addAComponent( new RectComponent( 120, 240 ) );
    stairs->addAComponent( new HoverableComponent( "talk to face" ) );


    cout << "pos: " << printBits( PositionComponent::bits() ) << "\n";
    cout << "spr: " << printBits( SpriteComponent::bits() ) << "\n";
    cout << "dir: " << printBits( DirectionComponent::bits() ) << "\n";
    cout << "rec: " << printBits( RectComponent::bits() ) << "\n";
    cout << "int: " << printBits( InteractableComponent::bits() ) << "\n";
    cout << "rfo: " << printBits( RouteFollowerComponent::bits() ) << "\n";
    cout << "pla: " << printBits( PlayerComponent::bits() ) << "\n";
    cout << "hov: " << printBits( HoverableComponent::bits() ) << "\n";
    cout << "-----" << "\n";
    cout << "PLA: " << printBits( player->componentBits ) << " " << player->noOfComponents() << "\n";
    cout << "STA: " << printBits( stairs->componentBits ) << "\n";
    cout << "ROP: " << printBits( ropeItem->componentBits ) << "\n";
    
    player->removeComponent<RectComponent>();
    
    cout << "PLA: " << printBits( player->componentBits ) << " " << player->noOfComponents() << "\n";
    
    gameObjects.push_back( stairs );
    gameObjects.push_back( ropeItem );
    gameObjects.push_back( player );

    
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
    
    navMesh = new NavMesh( ps );
    
}

void AdventureApp::mouseMove( MouseEvent event ){
    vec2 mousepos = event.getPos();
    HoverSystem::update( gameObjects, navMesh, mousepos );
}

void AdventureApp::mouseDown( MouseEvent event ){
    vec2 mousepos = vec2(event.getX(), event.getY() );
    cout << "Mouse Click: " << mousepos << "\n";
    
    PathFinderSystem::movePlayerTo( navMesh, player, gameObjects, mousepos );
}

void AdventureApp::update(){
    MovementSystem::update( player );
}

void AdventureApp::draw(){
    renderer->drawBackdrop();
    renderer->drawNavMesh( navMesh->shapes, navMesh->points );
    renderer->drawGameObjects( gameObjects );
    HoverSystem::draw();
    InventorySystem::draw();
}

CINDER_APP( AdventureApp, RendererGl )
