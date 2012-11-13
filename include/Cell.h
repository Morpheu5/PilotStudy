#pragma once
#include "cinder/Vector.h"
#include "cinder/svg/Svg.h"
#include "cinder/gl/Texture.h"

class Cell {

	ci::Vec2f _position;
	int _id;
	std::string _loopName;

	ci::svg::DocRef _artwork;
	ci::gl::Texture _texture;

public:
	Cell(void);
	Cell(ci::Vec2f& p);
	virtual ~Cell(void);

	void update();
	void draw();

	bool hit(const ci::Vec2f& p);
	void moveBy(const ci::Vec2f& p);

	/*** ACCESSORS ***/

	const ci::Vec2f& position() const { return _position; }
	void position(const ci::Vec2f& p) { _position = p; }

	const int id() const { return _id; }
	void id(const int i) { _id = i; }

	const std::string loopName() const { return _loopName; }
	void loopName(std::string& n) { _loopName = n; }
};

