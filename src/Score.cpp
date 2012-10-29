#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"
#include "cinder/app/AppBasic.h"
#include "cinder/Rand.h"

#include "Score.h"

using namespace ci;

Score::Score() {
	
}

Score::~Score() {
	_artwork.reset();
}

void Score::init() {
	_artwork = svg::Doc::create(app::getAssetPath("flower.svg"));
	Rectf rect = _artwork->getBoundingBox();
	cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	cairo::Context ctx(sImg);
	ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	ctx.render(*_artwork);
	_texture = sImg.getSurface();
}

void Score::update() {

}

void Score::draw() {
	gl::draw(_texture, _position - _artwork->getSize()/2);
}