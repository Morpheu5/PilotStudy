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

	int width = 70;

	// Try to keep pools down to two rows tops
	int cols = std::max(8, (int)ceil(loops.size()/2.0));
	int borderPadding = (45 + width * ((loops.size()-1) / cols));
	int hDisplacement = floor(width * (std::min(cols, (int)loops.size()) - 1) * 0.5);

	_cells.clear();

	switch(players) {
		case 4: {
			i = 0;
			j = 0;
			for(k = 0; k < loops.size(); k++) {
				p = Vec2f(app::getWindowWidth() - borderPadding + (width*j), app::getWindowHeight()/2 + hDisplacement - (width*(i % cols)));

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
				p = Vec2f(borderPadding - (width*j), app::getWindowHeight()/2 - hDisplacement + (width*(i % cols)));
				
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
				p = Vec2f(app::getWindowWidth()/2 + hDisplacement - (width * (i % cols)), borderPadding - (width*j));
				
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
				p = Vec2f(app::getWindowWidth()/2 - hDisplacement + (width * (i % cols)), app::getWindowHeight() - borderPadding + (width*j));
				
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
		TouchPoint touch = *touchIt;
		for(auto cellIt = _cells.begin(); cellIt != _cells.end(); ++cellIt) {
			Cell cell = cellIt->second;
			if(cell.hit(touch.position())) {
				int tid = touch.id();

				_activeTouches[tid] = touch;
				_touchedCells[tid] = cell.id();
			}
		}
	}
}

void CellController::updateTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		TouchPoint touch = *touchIt;
		if(_activeTouches.find(touch.id()) != _activeTouches.end()) {
			int tid = touch.id();
			int cid = _touchedCells[tid];
			ci::Vec2f lastMovement = touch.lastMovement();
			ci::Vec2f position = touch.position();

			_activeTouches[tid].addPoint(position);
			_cells[cid].moveBy(lastMovement);
		}
	}
}

void CellController::removeTouches(std::list<TouchPoint>& l) {
	for(auto touchIt = l.begin(); touchIt != l.end(); ++touchIt) {
		TouchPoint touch = *touchIt;
		int tid = touch.id();
		_activeTouches.erase(tid);
		_touchedCells.erase(tid);
	}
}

void CellController::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		Cell c = it->second;
		c.update();
	}
}

void CellController::draw() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		Cell c = it->second;
		c.draw();
	}
}
