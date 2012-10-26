#include "cinder/app/AppBasic.h"
#include "cinder/Vector.h"
#include "cinder/Rand.h"

#include "CellController.h"

//using namespace PilotStudy;
using namespace ci;

CellController::CellController() {
	for(int i = 0; i < 10; i++) {
		ci::Vec2f p = Vec2f(randFloat(app::getWindowWidth()), randFloat(app::getWindowHeight()));
		Cell c(p);
		c.id(i);
		_cells.push_back(c);
	}
}

CellController::~CellController() {

}

void CellController::update() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->update();
	}
}

void CellController::draw() {
	for(auto it = _cells.begin(); it != _cells.end(); ++it) {
		it->draw();
	}
}
