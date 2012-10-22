#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include <ctime>

#include "Cell.h"

#define FPS 60

using namespace ci;
using namespace ci::app;
using namespace std;

class PilotStudyApp : public AppBasic {

	std::list<Cell> _cells;

public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void PilotStudyApp::setup() {
	setWindowSize(600, 600);
	setFrameRate(FPS);
	//setFullScreen(true);
	gl::enableAlphaBlending(true);

	// SEED THE RANDOM

	for(int i = 0; i < 10; i++) {
		ci::Vec2f p = ci::Vec2f(randFloat(getWindowWidth()), randFloat(getWindowHeight()));
		Cell c(p);
		c.id(i);
		_cells.push_back(c);
	}
}

void PilotStudyApp::mouseDown( MouseEvent event ) {
	for(auto it = _cells.end(); it != _cells.begin(); --it) {
		if(it->hit(event.getPos())) {
			Cell c(*it);
			_cells.erase(it);
			_cells.push_front(c);
			console() << "Hit! (" << c.id() << ")" << endl;
			break;
		}
	}
}

void PilotStudyApp::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		//it->update();
	}
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->draw();
	}
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )
