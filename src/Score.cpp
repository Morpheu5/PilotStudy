#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

#include "Cell.h"
#include "Score.h"

using namespace ci;

Score::Score() {

}

Score::~Score() {
	_artwork.reset();
}

void Score::init() {
	_artwork = svg::Doc::create(app::getAssetPath("flower.svg"));

	/*
	auto l = _artwork->getChildren();
	for(auto it = l.begin(); it != l.end(); ++it) {
		svg::Node* n = *it;
		svg::Paint p(ColorA8u(100.0, 100.0, 100.0));
		svg::Style s;
		s.setFill(p);
		n->setStyle(s);
	}
	*/

	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
	_texture.enableAndBind();	
}

void Score::update() {
	auto children = _artwork->getChildren();
	for(auto cellsIt = _cells.begin(); cellsIt != _cells.end(); ++cellsIt) {
		for(auto childrenIt = children.begin(); childrenIt != children.end(); ++childrenIt) {

		}
	}
}

void Score::draw() {
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
	_texture.enableAndBind();
	gl::draw(_texture, _position - _artwork->getSize()/2);
}