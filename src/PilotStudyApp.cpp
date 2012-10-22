#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include <ctime>

#include "Cell.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class PilotStudyApp : public AppBasic {

	std::vector<Cell> _cells;

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
	gl::enableAlphaBlending(true);

	// SEED THE RANDOM

	for(int i = 0; i < 10; i++) {
		_cells.push_back(Cell((ci::Vec2f(25+ci::randFloat(getWindowWidth()-50), 25+ci::randFloat(getWindowHeight()-50)))));
	}
}

void PilotStudyApp::mouseDown( MouseEvent event ) {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		if(it->hit(event.getPos())) {
			console() << "Hit!" << endl;
		} else {
			console() << "Miss" << endl;
		}
	}
}

void PilotStudyApp::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->update();
	}
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->draw();
	}
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )
