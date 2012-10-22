#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

#include "Cell.h"

using namespace ci;

Cell::Cell(void) {
	/*
	_artwork = svg::Doc::create(app::getAssetPath("Cell.svg"));
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
	*/
}

Cell::Cell(ci::Vec2f& p) : _position(p) {
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

void Cell::update() {
	_position += ci::randVec2f();
}

void Cell::draw() {
	gl::draw(_texture, _position - _artwork->getSize()/2);
}

bool Cell::hit(ci::Vec2i& p) {
	return p.distanceSquared(_position) < 50*50;
}