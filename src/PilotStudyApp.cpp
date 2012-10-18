#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"

#include "TuneBlock.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PilotStudyApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void PilotStudyApp::setup() {
	setWindowSize(600, 600);
	setFrameRate(30);
	//setFullScreen(true);
}

void PilotStudyApp::mouseDown( MouseEvent event ) {
}

void PilotStudyApp::update() {
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	TuneBlock t(ci::Vec2f(0, 0));
	t.draw();
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )
