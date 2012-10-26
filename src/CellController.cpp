#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "CellController.h"

using namespace ci;

CellController::CellController() {

}

CellController::~CellController() {

}

void CellController::init() {
	for(int i = 0; i < 10; i++) {
		ci::Vec2f p = Vec2f(randFloat(app::getWindowWidth()), randFloat(app::getWindowHeight()));
		Cell c(p);
		c.id(i);
		_cells.push_back(c);
	}
}

void CellController::addTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		for(auto cellIt = _cells.begin(); cellIt != _cells.end(); ++cellIt) {
			if(cellIt->hit(touchIt->position())) {
				_activeTouches[touchIt->id()] = TouchPoint(*touchIt);
			}
		}
	}
}

void CellController::updateTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		if(_activeTouches.find(touchIt->id()) != _activeTouches.end()) {
			_activeTouches[touchIt->id()].addPoint(touchIt->position());
		}
	}
}

void CellController::removeTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		_activeTouches.erase(touchIt->id());
	}
}

void CellController::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->update();
		for(auto touchIt = _activeTouches.begin(); touchIt != _activeTouches.end(); ++touchIt) {
			app::console() << touchIt->second.id() << std::endl;
		}
	}
}

void CellController::draw() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->draw();
	}
}
