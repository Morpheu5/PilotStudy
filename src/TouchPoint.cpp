#include "TouchPoint.h"

TouchPoint::TouchPoint() {

}

TouchPoint::TouchPoint(uint32_t id, const Vec2f& p) : _id(id) {
	_path.push_back(p);
}

TouchPoint::~TouchPoint() {
	
}

void TouchPoint::addPoint(const Vec2f& p) {
	_path.push_back(p);
}