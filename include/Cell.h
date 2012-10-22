#pragma once
#include "cinder/Vector.h"
#include "cinder/svg/Svg.h"
#include "cinder/gl/Texture.h"

class Cell {

	ci::Vec2f _position;

	ci::svg::DocRef _artwork;
	ci::gl::Texture _texture;

public:
	Cell(void);
	Cell(ci::Vec2f& p);
	virtual ~Cell(void);

	void update();
	void draw();

	bool hit(ci::Vec2i& p);

	const ci::Vec2f& position() { return _position; }
	void position(ci::Vec2f p) { _position = p; }
};

