#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "TuioClient.h"
#include "TuioCursor.h"
#include "OscMessage.h"

#include "TouchPoint.h"
#include "CellController.h"

#define FPS 60

using namespace ci;
using namespace ci::app;

class PilotStudyApp : public AppBasic {

	CellController _cellController;
	tuio::Client _tuioClient;
	std::map<uint32_t, TouchPoint> _activePoints;

public:
	void prepareSettings(Settings* settings);
	void setup();

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
	setWindowSize(600, 600);
	setFrameRate(FPS);
	//setFullScreen(true);
	gl::enableAlphaBlending(true);

	_tuioClient.registerTouches(this);
	_tuioClient.connect(); // defaults to UDP 3333

	_cellController.init();
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
	_activePoints.insert(std::make_pair(0, TouchPoint(0, event.getPos())));
}

void PilotStudyApp::mouseDrag(MouseEvent event) {
/*	for(auto it = _cells.rbegin(); it != _cells.rend(); ++it) {
		if(it->hit(event.getPos())) {
			it->position(event.getPos());
			break;
		}
	} */
	_activePoints[0].addPoint(event.getPos());
}

void PilotStudyApp::mouseUp(MouseEvent event) {
	_activePoints.erase(0);
}

void PilotStudyApp::touchesBegan(TouchEvent event) {
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		_activePoints.insert(std::make_pair(it->getId(), TouchPoint(it->getId(), it->getPos())));
	}
}

void PilotStudyApp::touchesMoved(TouchEvent event) {
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		_activePoints[it->getId()].addPoint(it->getPos());
	}
}

void PilotStudyApp::touchesEnded(TouchEvent event) {
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		_activePoints.erase(it->getId());
	}
}

void PilotStudyApp::update() {
	_cellController.update();
	for(auto it = _activePoints.begin(); it != _activePoints.end(); ++it) {
		console() << it->first << " :: " << it->second.path().getPoints().back() << std::endl;
	}
}

void PilotStudyApp::draw() {
	gl::clear(Color(0, 0, 0));

	_cellController.draw();
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )