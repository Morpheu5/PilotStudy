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
	_activeNodes.clear();
	auto children = _artwork->getChildren();
	for(auto cellsIt = _cells.begin(); cellsIt != _cells.end(); ++cellsIt) {
		for(auto childrenIt = children.begin(); childrenIt != children.end(); ++childrenIt) {
			svg::Node *n = *childrenIt;
			Vec2f p = cellsIt->second.position() - (app::getWindowSize() - _artwork->getSize())/2;
			if(n->containsPoint(p)) {
				_activeNodes.push_back(n);
			}
		}
	}
}

void Score::draw() {
	/* TODO try editing the styles with const_cast<>...
	 Rectf rect = _artwork->getBoundingBox();
	 cairo::SurfaceImage sImg(rect.getWidth(), rect.getHeight(), true);
	 cairo::Context ctx(sImg);
	 ctx.scale(rect.getWidth() / _artwork->getWidth(), rect.getHeight() / _artwork->getHeight());
	 ctx.render(*_artwork);
	 _texture = sImg.getSurface();
	 _texture.enableAndBind();
	 gl::draw(_texture, _position - _artwork->getSize()/2);
	/**/

	for(auto nIt = _artwork->getChildren().begin(); nIt != _artwork->getChildren().end(); ++nIt) {
		svg::Node* n = *nIt;
		svg::Paint p(ColorA8u(100.0, 100.0, 100.0));
		svg::Style s;
		s.setFill(p);
		n->setStyle(s);

		Rectf nRect = _artwork->getBoundingBox();
		cairo::SurfaceImage nImg(nRect.getWidth(), nRect.getHeight(), true);
		cairo::Context nCtx(nImg);
		nCtx.scale(nRect.getWidth() / _artwork->getWidth(), nRect.getHeight() / _artwork->getHeight());
		nCtx.render(*n);
		ci::gl::Texture nTx = nImg.getSurface();
		//nTx.enableAndBind();
		gl::draw(nTx, _position - _artwork->getSize()/2);
	}

	for(auto nIt = _activeNodes.begin(); nIt != _activeNodes.end(); ++nIt) {
		svg::Node* n = *nIt;
		svg::Paint p(ColorA8u(255.0, 255.0, 255.0));
		svg::Style s;
		s.setFill(p);
		n->setStyle(s);
		
		Rectf nRect = _artwork->getBoundingBox();
		cairo::SurfaceImage nImg(nRect.getWidth(), nRect.getHeight(), true);
		cairo::Context nCtx(nImg);
		nCtx.scale(nRect.getWidth() / _artwork->getWidth(), nRect.getHeight() / _artwork->getHeight());
		nCtx.render(*n);
		ci::gl::Texture nTx = nImg.getSurface();
		//nTx.enableAndBind();
		gl::draw(nTx, _position - _artwork->getSize()/2);
	}
}