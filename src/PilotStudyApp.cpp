#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"
#include "cinder/Timeline.h"
#include "cinder/Xml.h"
#include "cinder/Text.h"
#include "cinder/Font.h"

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
#define LOOP_SELECTION_CIRCLE_RADIUS 350.0f

using namespace ci;
using namespace ci::app;

struct Tag {
private:
	float _leading;

public:
	std::string name;
	Vec2f position;
	gl::Texture texture;
	bool selected;

	void draw() {
		TextLayout l;
		Font f("Arial Bold", 36);
		_leading = f.getLeading();
		l.setFont(f);
		if(selected) {
			l.setColor(ColorA(1, 1, 1, 1));
		} else {
			l.setColor(ColorA(1, 1, 1, 0.25f));
		}
		l.addLine(name);
		texture = gl::Texture(l.render(true, true));
		texture.enableAndBind();
		/*
		Area a = texture.getBounds();
		a.expand(20, 10);
		texture.disable();
		gl::color(ColorA8u(82, 102, 122));
		gl::drawStrokedRect(a + position - a.getSize()/2 + Vec2f(20, 10 + f.getLeading()));
		texture.enableAndBind();
		*/
		gl::color(1,1,1);
		gl::draw(texture, position - texture.getSize()/2);
		texture.disable();
	}

	bool hit(Vec2f p) {
		Area a = texture.getBounds();
		a.expand(20, 10);
		a += position - a.getSize()/2 + Vec2f(20, 10 + _leading);
		return a.contains(p);
	}
};

class PilotStudyApp : public AppBasic {

	CellController _cellController;
	Score _score;
	enum Scene {
		TagSelectionScene, LoopSelectionScene, ActionScene
	} _scene;
	std::vector<Tag> _tags;
	std::map<int, Cell> _tempCells;

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
	void prepareNextScene();
	void setupForScene(Scene s);

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
	//settings->setFullScreen(true);
	settings->setWindowSize(1920, 1080);
	settings->setFrameRate(FPS);
}

void PilotStudyApp::setup() {
	Rand::randomize();
	gl::enableAlphaBlending(true);
	gl::enable(GL_TEXTURE_2D);

	_tuioClient.registerTouches(this);
	_tuioClient.connect(); // defaults to UDP 3333

	_hostname = "localhost";
	_port = 3000;
	_sender.setup(_hostname, _port);

	setupForScene(TagSelectionScene);
}

void PilotStudyApp::prepareNextScene() {
	switch(_scene) {
		case TagSelectionScene: {
			XmlTree loopsDoc("root", "");
			XmlTree loops("loops", "");
			XmlTree tagsDoc(loadAsset("tags.xml"));
			XmlTree selTagsDoc("root", "");
			XmlTree selTags("tags", "");

			std::set<std::string> loopSet;
			for(auto tagIt = _tags.begin(); tagIt != _tags.end(); ++tagIt) {
				tagIt->texture.disable();
				if(tagIt->selected) {
					XmlTree selTag("tag", "");
					selTag.setAttribute("name", tagIt->name);
					selTags.push_back(selTag);
					for(auto xTagIt = tagsDoc.begin("root/tags/tag"); xTagIt != tagsDoc.end(); ++xTagIt) {
						if(xTagIt->getAttributeValue<std::string>("name") == tagIt->name) {
							for(auto loopIt = xTagIt->begin("loop"); loopIt != xTagIt->end(); ++loopIt) {
								loopSet.insert(loopIt->getAttributeValue<std::string>("name"));
							}
						}
					}
				}
			}
			selTagsDoc.push_back(selTags);
			selTagsDoc.write(writeFile(getAssetPath("")/"selected_tags.xml"));
			for(auto loopIt = loopSet.begin(); loopIt != loopSet.end(); ++loopIt) {
				XmlTree loop("loop", "");
				loop.setAttribute("name", *loopIt);
				loops.push_back(loop);
			}
			loopsDoc.push_back(loops);
			loopsDoc.write(writeFile(getAssetPath("")/"temp_loops.xml"));
			break;
		}
		case LoopSelectionScene: {
			XmlTree loopsDoc("root", "");
			XmlTree loops("loops", "");
			std::map<int, Cell> cells = _cellController.cells();
			for(auto it = cells.begin(); it != cells.end(); ++it) {
				if(it->second.position().distance(getWindowCenter()) < LOOP_SELECTION_CIRCLE_RADIUS) {
					XmlTree loop("loop", "");
					loop.setAttribute("name", it->second.loopName());
					loops.push_back(loop);
				}
			}
			loopsDoc.push_back(loops);
			loopsDoc.write(writeFile(getAssetPath("")/"selected_loops.xml"));
			break;
		}
		case ActionScene: {
			break;
		}
	}
}

void PilotStudyApp::setupForScene(Scene s) {
	_scene = s;

	switch(_scene) {
		case TagSelectionScene: {
			XmlTree tagsDoc(loadAsset("tags.xml"));
			XmlTree tags = tagsDoc.getChild("root/tags");
			int tagCount = tags.getChildren().size();
			int tagCounter = 0;
			for(XmlTree::Iter tag = tags.begin(); tag != tags.end(); ++tag) {
				Tag t;
				t.name = tag->getAttributeValue<std::string>("name");
				Vec2f p = Vec2f(0.0f, 350.0f);
				p.rotate(((((tagCount%2)*0.5)+tagCounter)*2*M_PI)/tagCount);
				t.position = p + getWindowCenter();
				t.selected = true;
				
				_tags.push_back(t);
				tagCounter++;
			}
			break;
		}
		case LoopSelectionScene: {
			XmlTree loopsDoc(loadAsset("temp_loops.xml"));
			std::vector<std::string> loops;
			for(auto it = loopsDoc.begin("root/loops/loop"); it != loopsDoc.end(); ++it) {
				loops.push_back(it->getAttributeValue<std::string>("name"));
			}
			_cellController.init(0, loops);
			break;
		}
		case ActionScene: {
			XmlTree loopsDoc(loadAsset("selected_loops.xml"));
			// iterators iterate over children
			XmlTree loops = loopsDoc.getChild("root/loops");
			std::vector<std::string> loopNames;
			for(XmlTree::Iter loop = loops.begin(); loop != loops.end(); ++loop) {
				loopNames.push_back(loop->getAttributeValue<std::string>("name"));
			}

			_cellController.init(4, loopNames);
			_currentBar = 0;
			_score.init();
			_score.position(getWindowSize()/2);

			// I'd rather use an OSC Listener but for the time being...
			_cue = timeline().add( std::bind(&PilotStudyApp::playTimerCallback, this), timeline().getCurrentTime() + LOOPS_LENGTH );
			_cue->setDuration(LOOPS_LENGTH);
			_cue->setAutoRemove(false);
			_cue->setLoop(true);
			break;
		}
	}
}

void PilotStudyApp::keyDown(KeyEvent event) {
	switch(event.getChar()) {
		case 'f': {
			setFullScreen(!isFullScreen());
		}
		case 'c': {
			gl::enableAlphaBlending(true);
			switch(_scene) {
				case TagSelectionScene: {
					for(int i = 0; i < _tags.size(); i++) {
						Vec2f p = Vec2f(0.0f, LOOP_SELECTION_CIRCLE_RADIUS);
						p.rotate(((((_tags.size()%2)*0.5)+i)*2*M_PI) / _tags.size());
						_tags[i].position = p + getWindowCenter();
					}
					break;
				}
				case LoopSelectionScene: {
					
					break;
				}
				case ActionScene: {
					_score.position(getWindowSize()/2);
					break;
				}
			}
			break;
		}
		case 'n': {
			switch(_scene) {
				case TagSelectionScene: {
					prepareNextScene();
					setupForScene(LoopSelectionScene);
					break;
				}
				case LoopSelectionScene: {
					prepareNextScene();
					setupForScene(ActionScene);
					break;
				}
				case ActionScene: {
					// nop
					break;
				}
			}
			break;
		}
		case 'q': {
			exit(0);
			break;
		}
	}
}

void PilotStudyApp::mouseDown(MouseEvent event) {
	_activePoints[0] = TouchPoint(0, event.getPos());
	std::list<TouchPoint> l;
	l.push_back(_activePoints[0]);
	switch(_scene) {
		case TagSelectionScene: {
			for(int i = 0; i < _tags.size(); i++) {
				if(_tags[i].hit(event.getPos())) {
					_tags[i].selected = !_tags[i].selected;
				}
			}
			break;
		}
		case LoopSelectionScene:
		case ActionScene: {
			_cellController.addTouches(l);
			break;
		}
	}
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

	switch(_scene) {
		case TagSelectionScene: {
            for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
                for(int i = 0; i < _tags.size(); i++) {
                    if(_tags[i].hit(it->getPos())) {
                        _tags[i].selected = !_tags[i].selected;
                    }
                }
                break;
            }
		}
		case LoopSelectionScene:
		case ActionScene: {
			_cellController.addTouches(l);
			break;
		}
	}
}

void PilotStudyApp::touchesMoved(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		int tid = it->getId();
		Vec2f position = it->getPos();

		_activePoints[tid].addPoint(position);
		l.push_back(_activePoints[tid]);
	}
	
	switch(_scene) {
		case TagSelectionScene: {
			break;
		}
		case LoopSelectionScene:
		case ActionScene: {
			_cellController.updateTouches(l);
			break;
		}
	}
}

void PilotStudyApp::touchesEnded(TouchEvent event) {
	std::list<TouchPoint> l;
	for(auto it = event.getTouches().begin(); it != event.getTouches().end(); ++it) {
		int tid = it->getId();

		l.push_back(_activePoints[tid]);
		_activePoints.erase(tid);
	}

	switch(_scene) {
		case TagSelectionScene: {
			break;
		}
		case LoopSelectionScene:
		case ActionScene: {
			_cellController.removeTouches(l);
			break;
		}
	}
}

void PilotStudyApp::playTimerCallback() {
    _currentBar = (++_currentBar)%8;
	std::list<Cell> playingCells = _score.cellsInBar(_currentBar);
	
	osc::Message m;
	m.setAddress("/playclip");
	for(auto it = playingCells.begin(); it != playingCells.end(); ++it) {
		m.addStringArg(it->loopName());
	}
	m.setRemoteEndpoint(_hostname, _port);
	_sender.sendMessage(m);
}

void PilotStudyApp::update() {
	switch(_scene) {
		case TagSelectionScene: {
			break;
		}
		case LoopSelectionScene: {
			_cellController.update();
			break;
		}
		case ActionScene: {
			_cellController.update();
			_score.cells(_cellController.cells());
			_score.update();
			break;
		}
	}
}

void PilotStudyApp::draw() {
	gl::clear(ColorA8u(41, 51, 61));

	switch (_scene) {
		case TagSelectionScene: {
			for(auto tagIt = _tags.begin(); tagIt != _tags.end(); ++tagIt) {
				tagIt->draw();
			}
			break;
		}
		case LoopSelectionScene: {
			gl::color(ColorA8u(82, 102, 122));
			glLineWidth(10.0f);

			gl::drawSolidCircle(getWindowCenter(), LOOP_SELECTION_CIRCLE_RADIUS);

			glLineWidth(1.0f);
			gl::color(ColorA8u(255, 255, 255));

			_cellController.draw();
			
			break;
		}
		case ActionScene: {
			_score.draw();
			_cellController.draw();
			break;
		}
	}
}

CINDER_APP_BASIC( PilotStudyApp, RendererGl )