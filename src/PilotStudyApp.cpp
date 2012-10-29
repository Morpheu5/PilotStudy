#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "TuioClient.h"
#include "TuioCursor.h"
#include "OscMessage.h"

#include "Common.h"

#include "TouchPoint.h"
#include "CellController.h"
#include "Score.h"

#define FPS 60

using namespace ci;
using namespace ci::app;

class PilotStudyApp : public AppBasic {

	CellController _cellController;
	Score _score;

	tuio::Client _tuioClient;
	TouchMap _activePoints;

public:
	void prepareSettings(Settings* settings);
	void setup();

	void keyDown(KeyEvent event);
	
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseUp(MouseEvent event);

	void touchesBegan(TouchEvent event);
	void touchesMoved(TouchEvent event);
	void touchesEnded(TouchEvent event);

	void update();
	void draw();
};

void PilotStudyApp::prepareSettings(Settings* settings) {
	settings->enableMultiTouch();
}

void PilotStudyApp::setup() {
	Rand::randomize();
	setFrameRate(FPS);
	setFullScreen(true);
	gl::enableAlphaBlending(true);

	_tuioClient.registerTouches(this);
	_tuioClient.connect(); // defaults to UDP 3333

	_cellController.init();
	_score.init();
	_score.position(getWindowSize()/2);
}

void PilotStudyApp::keyDown(KeyEvent event) {
	switch(event.getChar()) {
	case 'f':
		setFullScreen(!isFullScreen());
		gl::enableAlphaBlending(true);
		break;
	case 'q':
		exit(0);
		break;
	}
}

void PilotStudyApp::mouseDown(MouseEvent event) {
	_activePoints[0] = TouchPoint(0, event.getPos());
	std::list<TouchPoint> l;
	l.push_back(_activePoints[0]);
	_cellController.addTouches(l);
}

void PilotStudyApp::mouseDrag(MouseEvent event) {
	_activePoints[0].addPoint(event.getPos());
	std::list<TouchPoint> l;
	l.push_back(_activePoints[0]);
	_cellController.updateTouches(l);
}

void PilotStudyApp::mouseUp(MouseEvent event) {
	std::list<TouchPoint> l;
	l.push_back(_activePoints[0]);
	_activePoints.erase(0);
	_cellController.removeTouches(l);
}

void PilotStudyApp::touchesBegan(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		_activePoints[it->getId()] = TouchPoint(it->getId(), it->getPos());
		l.push_back(_activePoints[it->getId()]);
	}
	_cellController.addTouches(l);
}

void PilotStudyApp::touchesMoved(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		_activePoints[it->getId()].addPoint(it->getPos());
		l.push_back(_activePoints[it->getId()]);
	}
	_cellController.updateTouches(l);
}

void PilotStudyApp::touchesEnded(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		l.push_back(_activePoints[it->getId()]);
		_activePoints.erase(it->getId());
	}
	_cellController.removeTouches(l);
}

void PilotStudyApp::update() {
	_cellController.update();
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	_score.draw();
	_cellController.draw();

	for(auto it = _activePoints.begin(); it != _activePoints.end(); ++it) {
		gl::drawSolidCircle(it->second.position(), 10);
	}
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )