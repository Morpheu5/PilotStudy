#pragma once

#include "Common.h"
#include "Cell.h"

class CellController {

	std::map<int, Cell> _cells;
	TouchMap _activeTouches;
	std::map<int, int> _touchedCells;

public:
	CellController();
	virtual ~CellController();

	void init();

	void addTouches(std::list<TouchPoint>& m);
	void updateTouches(std::list<TouchPoint>& m);
	void removeTouches(std::list<TouchPoint>& m);
	void update();
	void draw();
};