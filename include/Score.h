#pragma once

#include "cinder/Vector.h"
#include "cinder/svg/Svg.h"
#include "cinder/gl/Texture.h"

#include "Cell.h"

class Score {

	ci::Vec2f _position;
	std::map<int, Cell> _cells;

	ci::svg::DocRef _artwork;
	ci::gl::Texture _texture;
	std::list<ci::svg::Node*> _activeNodes;

public:
	Score();
	virtual ~Score();

	void init();
	void update();
	void draw();

	/*** ACCESSORS ***/

	const ci::Vec2f& position() const { return _position; }
	void position(const ci::Vec2f& p) { _position = p; }

	void cells(std::map<int, Cell> c) { _cells = c; }
};

