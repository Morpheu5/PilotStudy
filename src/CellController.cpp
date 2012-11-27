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

void CellController::init(int players, std::vector<std::string>& loops) {
	Cell c;
	ci::Vec2f p;
	int i, j, k, n, id = 1;

	// Try to keep pools down to two rows tops
	int cols = std::max(8, (int)ceil(loops.size()/2.0));
	int borderPadding = (45 + 60 * ((loops.size()-1) / cols));
	int hDisplacement = floor(60 * (std::min(cols, (int)loops.size()) - 1) * 0.5);

	_cells.clear();

	switch(players) {
		case 4: {
			i = 0;
			j = 0;
			for(k = 0; k < loops.size(); k++) {
				p = Vec2f(app::getWindowWidth() - borderPadding + (60*j), app::getWindowHeight()/2 + hDisplacement - (60*(i % cols)));

				for(n = 0; n < cols; n++) {
					c = Cell(p);
					c.id(id);
					c.loopName(loops[k]);
					_cells[id] = c;
					id++;
				}

				if(++i % cols == 0) {
					j++;
				}
			}
		}
		case 3: {
			i = 0;
			j = 0;
			for(k = 0; k < loops.size(); k++) {
				p = Vec2f(borderPadding - (60*j), app::getWindowHeight()/2 - hDisplacement + (60*(i % cols)));
				
				for(n = 0; n < cols; n++) {
					c = Cell(p);
					c.id(id);
					c.loopName(loops[k]);
					_cells[id] = c;
					id++;
				}

				if(++i % cols == 0) {
					j++;
				}
			}
		}
		case 2: {
			i = 0;
			j = 0;
			for(k = 0; k < loops.size(); k++) {
				p = Vec2f(app::getWindowWidth()/2 + hDisplacement - (60 * (i % cols)), borderPadding - (60*j));
				
				for(n = 0; n < cols; n++) {
					c = Cell(p);
					c.id(id);
					c.loopName(loops[k]);
					_cells[id] = c;
					id++;
				}

				if(++i % cols == 0) {
					j++;
				}
			}
		}
		case 1: {
			i = 0;
			j = 0;
			for(k = 0; k < loops.size(); k++) {
				p = Vec2f(app::getWindowWidth()/2 - hDisplacement + (60 * (i % cols)), app::getWindowHeight() - borderPadding + (60*j));
				
				for(n = 0; n < cols; n++) {
					c = Cell(p);
					c.id(id);
					c.loopName(loops[k]);
					_cells[id] = c;
					id++;
				}

				if(++i % cols == 0) {
					j++;
				}
			}
			break;
		}
		case 0: { // This is a special one for the LoopSelectionScene
			int id = 1;
			for(k = 0; k < loops.size(); k++) {
				float radius = randFloat(300.0f);
				float angle = randFloat(2*M_PI);
				Vec2f p = Vec2f(radius, 0.0f);
				p.rotate(angle);
				p += app::getWindowCenter();
				c = Cell(p);
				c.id(id);
				c.loopName(loops[k]);
				_cells[id] = c;
				id++;
			}
			break;
		}
		default:
			break;
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
