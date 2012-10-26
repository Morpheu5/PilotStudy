#pragma once
#include "Cell.h"

#include <list>

class CellController {

	std::list<Cell> _cells;

public:
	CellController();
	virtual ~CellController();

	void init();

	void update();
	void draw();
};