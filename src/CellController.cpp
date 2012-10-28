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
		_cells[i] = c;
	}
}

void CellController::addTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		for(auto cellIt = _cells.begin(); cellIt != _cells.end(); ++cellIt) {
			if(cellIt->second.hit(touchIt->position())) {
				_activeTouches[touchIt->id()] = TouchPoint(*touchIt);
				_touchedCells[touchIt->id()] = cellIt->second.id();
			}
		}
	}
}

void CellController::updateTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		if(_activeTouches.find(touchIt->id()) != _activeTouches.end()) {
			_activeTouches[touchIt->id()].addPoint(touchIt->position());
			ci::Vec2f v = touchIt->lastMovement();
			int cid = _touchedCells[touchIt->id()];
			_cells[cid].moveBy(v);
		}
	}
}

void CellController::removeTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		_activeTouches.erase(touchIt->id());
		_touchedCells.erase(touchIt->id());
	}
}

void CellController::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		/*
		auto jt = it;
		for(++jt; jt != _cells.end(); ++jt) {
			ci::Vec2f direction = it->second.position() - jt->second.position();
			float distance = direction.length();
			if(distance < 75.0f) {
				float F = 10.0f / distance;
				direction.normalize();
				it->second.moveBy(F*direction);
				jt->second.moveBy(-F*direction);
			}
		}
		*/
	}
}

void CellController::draw() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->second.draw();
	}
}
