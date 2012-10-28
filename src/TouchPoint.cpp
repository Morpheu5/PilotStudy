#include "TouchPoint.h"

TouchPoint::TouchPoint() {

}

TouchPoint::TouchPoint(uint32_t id, const Vec2f& p) : _id(id), _position(p) {
	_path.push_back(p);
}

TouchPoint::~TouchPoint() {
	
}

void TouchPoint::addPoint(const Vec2f& p) {
	_position = p;
	_path.push_back(p);
}

const ci::Vec2f TouchPoint::lastMovement() {
	ci::Vec2f v = _position - *(_path.end()-2);
	return v;
}