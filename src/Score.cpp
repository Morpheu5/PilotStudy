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

std::list<int> Score::cellsInBar(int bar) {
	std::list<int> cells;
	for(int track = 0; track < 4; track++) {
		std::pair<int, int> nId(track, bar);
		int cId = _activeCells[nId];
		if(cId != 0) {
			cells.push_back(_activeCells[nId]);
		}
	}
	return cells;
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
	_activeCells.clear();

	s.setFill(highlightPaint);
	for(int bar = 0; bar < 8; bar++) {
		for(int track = 0; track < 4; track++) {
			std::stringstream sstr;
			sstr << "m" << track << bar;
			std::string id = sstr.str();
			svg::Node* n = const_cast<svg::Node*>(_artwork->findNode(id));

			for(auto cIt = _cells.begin(); cIt != _cells.end(); ++cIt) {
				Vec2f p = cIt->second.position() - (app::getWindowSize() - _artwork->getSize())/2;
				if(n->containsPoint(p)) {
					n->setStyle(s);
					// set the cell as active
					//if(cIt->first != 0) {
						std::pair<int, int> nId(track, bar);
						_activeCells[nId] = cIt->first;
					//}
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