#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include <ctime>

#include "CellController.h"

#define FPS 60

using namespace ci;
using namespace ci::app;

class PilotStudyApp : public AppBasic {

	CellController _cellController;

public:
	void setup();
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void update();
	void draw();
};

void PilotStudyApp::setup() {
	setWindowSize(600, 600);
	setFrameRate(FPS);
	//setFullScreen(true);
	gl::enableAlphaBlending(true);
}

void PilotStudyApp::mouseDown(MouseEvent event) {
/*	for(auto it = _cells.rbegin(); it != _cells.rend(); ++it) {
		if(it->hit(event.getPos())) {
			Cell c = *it;
			auto jt = (++it).base();
			_cells.erase(jt);
			_cells.push_back(c);
			console() << "Hit! (" << c.id() << ")" << endl;
			break;
		}
	} */
}

void PilotStudyApp::mouseDrag(MouseEvent event) {
/*	for(auto it = _cells.rbegin(); it != _cells.rend(); ++it) {
		if(it->hit(event.getPos())) {
			it->position(event.getPos());
			break;
		}
	} */
}

void PilotStudyApp::update() {
	_cellController.update();
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	_cellController.draw();
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )