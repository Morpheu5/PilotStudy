#pragma once

#include "cinder/Vector.h"
#include "cinder/svg/Svg.h"
#include "cinder/gl/Texture.h"

class Score {

	ci::Vec2f _position;

	ci::svg::DocRef _artwork;
	ci::gl::Texture _texture;

public:
	Score();
	virtual ~Score();

	void init();
	void update();
	void draw();

	/*** ACCESSORS ***/

	const ci::Vec2f& position() const { return _position; }
	void position(const ci::Vec2f& p) { _position = p; }
};

