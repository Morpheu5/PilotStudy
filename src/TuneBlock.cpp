#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/app/AppBasic.h"

#include "TuneBlock.h"

using namespace ci;

TuneBlock::TuneBlock(void) {
}

TuneBlock::TuneBlock(ci::Vec2f p) : _position(p) {
	_artwork = svg::Doc::create(app::getAssetPath("TuneBlock.svg"));
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	app::console() << rect << std::endl;
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
}


TuneBlock::~TuneBlock(void) {
	/*
	_artwork = svg::Doc::create(app::getAssetPath("TuneBlock.svg"));
	ci::Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
	*/
}

void TuneBlock::update() {

}

void TuneBlock::draw() {
	gl::draw(_texture, _position - _artwork->getBoundingBox().getCenter());
}