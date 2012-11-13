#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Timeline.h"
#include "cinder/Xml.h"

#include "TuioClient.h"
#include "TuioCursor.h"
#include "OscSender.h"
#include "OscListener.h"

#include "Common.h"

#include "TouchPoint.h"
#include "CellController.h"
#include "Score.h"

#define FPS 60
#define LOOPS_LENGTH 2

using namespace ci;
using namespace ci::app;

class PilotStudyApp : public AppBasic {

	CellController _cellController;
	Score _score;

	tuio::Client _tuioClient;
	TouchMap _activePoints;

	int _currentBar;
	CueRef _cue;

	std::string _hostname;
	int _port;
	osc::Sender _sender;
	osc::Listener _listener;

public:
	void prepareSettings(Settings* settings);
	void setup();

	void keyDown(KeyEvent event);
	
	void mouseDown(MouseEvent event);
	void mouseDrag(MouseEvent event);
	void mouseUp(MouseEvent event);
	void mouseMove(MouseEvent event);

	void touchesBegan(TouchEvent event);
	void touchesMoved(TouchEvent event);
	void touchesEnded(TouchEvent event);

	void playTimerCallback();

	void update();
	void draw();
};

void PilotStudyApp::prepareSettings(Settings* settings) {
	settings->enableMultiTouch();
}

void PilotStudyApp::setup() {
	Rand::randomize();
	setFrameRate(FPS);
	//setFullScreen(true);
	setWindowSize(800, 800);
	gl::enableAlphaBlending(true);
	gl::enable(GL_TEXTURE_2D);

	_tuioClient.registerTouches(this);
	_tuioClient.connect(); // defaults to UDP 3333

	_hostname = "localhost";
	_port = 3000;
	_sender.setup(_hostname, _port);

	XmlTree tracksDoc(loadAsset("tracksconfig.xml"));
	
	_cellController.init();
	_currentBar = 0;
	_score.init();
	_score.position(getWindowSize()/2);

	// I'd rather use an OSC Listener but for the time being...
	_cue = timeline().add( std::bind(&PilotStudyApp::playTimerCallback, this), timeline().getCurrentTime() + LOOPS_LENGTH );
	_cue->setDuration(LOOPS_LENGTH);
	_cue->setAutoRemove(false);
	_cue->setLoop(true);
}

void PilotStudyApp::keyDown(KeyEvent event) {
	switch(event.getChar()) {
	case 'f':
		setFullScreen(!isFullScreen());
	case 'c':
		gl::enableAlphaBlending(true);
		_score.position(getWindowSize()/2);
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

void PilotStudyApp::mouseMove(MouseEvent event) {

}

void PilotStudyApp::touchesBegan(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		int tid = it->getId();
		Vec2f position = it->getPos();

		_activePoints[tid] = TouchPoint(tid, position);
		l.push_back(_activePoints[tid]);
	}
	_cellController.addTouches(l);
}

void PilotStudyApp::touchesMoved(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		int tid = it->getId();
		Vec2f position = it->getPos();

		_activePoints[tid].addPoint(position);
		l.push_back(_activePoints[tid]);
	}
	_cellController.updateTouches(l);
}

void PilotStudyApp::touchesEnded(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		int tid = it->getId();

		l.push_back(_activePoints[tid]);
		_activePoints.erase(tid);
	}
	_cellController.removeTouches(l);
}

void PilotStudyApp::playTimerCallback() {
	std::list<int> playingCells = _score.cellsInBar(_currentBar);
	
	osc::Message m;
	m.setAddress("/playclip");
	m.addStringArg("track1");
	m.addStringArg("track9");
	m.addStringArg("track17");
	m.addStringArg("track25");
	m.setRemoteEndpoint(_hostname, _port);
	_sender.sendMessage(m);

    _currentBar = (++_currentBar)%8;
    _score.cellsInBar(_currentBar);
}

void PilotStudyApp::update() {
	_cellController.update();

	_score.cells(_cellController.cells());
	_score.update();
}

void PilotStudyApp::draw() {
	gl::clear(ColorA8u(41, 51, 61));

	_score.draw();
	_cellController.draw();

	for(auto it = _activePoints.begin(); it != _activePoints.end(); ++it) {
		gl::drawSolidCircle(it->second.position(), 10);
	}
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )