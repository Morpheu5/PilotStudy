#include "cinder/app/AppBasic.h"
#include "cinder/System.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "CellController.h"

using namespace ci;

CellController::CellController() {
	
}

CellController::~CellController() {
	
}

void CellController::init() {
	for(int i = 1; i <= 10; i++) {
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
				int tid = touchIt->id();

				_activeTouches[tid] = TouchPoint(*touchIt);
				_touchedCells[tid] = cellIt->second.id();
			}
		}
	}
}

void CellController::updateTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		if(_activeTouches.find(touchIt->id()) != _activeTouches.end()) {
			int tid = touchIt->id();
			int cid = _touchedCells[tid];
			ci::Vec2f lastMovement = touchIt->lastMovement();
			ci::Vec2f position = touchIt->position();

			_activeTouches[tid].addPoint(position);
			_cells[cid].moveBy(lastMovement);
		}
	}
}

void CellController::removeTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		int tid = touchIt->id();
		_activeTouches.erase(tid);
		_touchedCells.erase(tid);
	}
}

void CellController::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->second.update();
	}
}

void CellController::draw() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->second.draw();
	}
}
