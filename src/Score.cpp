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
}

void Score::update() {
	auto children = _artwork->getChildren();
	svg::Paint normalPaint(ColorA8u(100, 100, 100));
	svg::Paint highlightPaint(ColorA8u(255, 255, 255));
	svg::Style s;
	s.setFill(normalPaint);
	for(auto it = children.begin(); it != children.end(); ++it) {
		svg::Node *n = *it;
		n->setStyle(s);
	}

	s.setFill(highlightPaint);
	for(int m = 0; m < 8; m++) {
		for(int r = 0; r < 4; r++) {
			std::stringstream sstr;
			sstr << "m" << r << m;
			std::string id = sstr.str();
			svg::Node* n = const_cast<svg::Node*>(_artwork->findNode(id));

			for(auto cIt = _cells.begin(); cIt != _cells.end(); ++cIt) {
				Vec2f p = cIt->second.position() - (app::getWindowSize() - _artwork->getSize())/2;
				if(n->containsPoint(p)) {
					n->setStyle(s);
				}
			}
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