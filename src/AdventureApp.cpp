#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class AdventureApp : public App {
  public:
	void setup() override;
	void mouseDown( MouseEvent event ) override;
	void update() override;
	void draw() override;
};

void AdventureApp::setup()
{
}

void AdventureApp::mouseDown( MouseEvent event )
{
}

void AdventureApp::update()
{
}

void AdventureApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( AdventureApp, RendererGl )
