#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"
#include "cinder/Filesystem.h"

#include "Cell.h"

using namespace ci;

Cell::Cell(void) {

}

Cell::Cell(ci::Vec2f& p) {
	_position = ci::Vec2f(p);

	_artwork = svg::Doc::create(app::getAssetPath("Cell.svg"));
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
}

Cell::~Cell() {
	_artwork.reset();
}

void Cell::loopName(std::string& n) {
	_loopName = n;

	fs::path fileName = app::getAssetPath(_loopName + ".svg");
	if(fileName.empty()) {
		_artwork = svg::Doc::create(app::getAssetPath("Cell.svg"));
	} else {
		_artwork = svg::Doc::create(fileName);
	}
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
}

void Cell::update() {

}

void Cell::draw() {
	_texture.enableAndBind();
	gl::draw(_texture, _position - _artwork->getSize()/2);
	_texture.disable();
}

bool Cell::hit(const ci::Vec2f& p) {
	int r = (_artwork->getSize()).x/2;
	return p.distance(_position) < r;
}

void Cell::moveBy(const ci::Vec2f& p) {
	_position += p;
}