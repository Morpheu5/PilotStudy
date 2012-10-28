#pragma once
#include "cinder/Vector.h"
#include "cinder/PolyLine.h"

using namespace ci;

class TouchPoint {

	uint32_t _id;
	PolyLine<Vec2f> _path;
	Vec2f _position;

public:
	TouchPoint();
	TouchPoint(uint32_t id, const Vec2f& p);
	virtual ~TouchPoint();

	void addPoint(const Vec2f& p);
	const ci::Vec2f lastMovement();
	
	/*** ACCESSORS ***/

	const uint32_t id() const { return _id; }
	void id(int i) { _id = i; }
	
	const PolyLine<Vec2f>& path() const { return _path; }

	const Vec2f& position() const { return _position; }
};

