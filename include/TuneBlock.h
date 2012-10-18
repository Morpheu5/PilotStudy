#pragma once
#include "cinder/Vector.h"
#include "cinder/svg/Svg.h"
#include "cinder/gl/Texture.h"

class TuneBlock {

	ci::Vec2f _position;

	ci::svg::DocRef _artwork;
	ci::gl::Texture _texture;

public:
	TuneBlock(void);
	TuneBlock(ci::Vec2f p);
	virtual ~TuneBlock(void);

	void update();
	void draw();

	const ci::Vec2f& position() { return _position; }
	void position(ci::Vec2f p) { _position = p; }
};

