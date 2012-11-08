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
    _playingBar = 0;
}

Score::~Score() {
	_artwork.reset();
}

void Score::init() {
	_artwork = svg::Doc::create(app::getAssetPath("flower.svg"));
}

std::list<int> Score::cellsInBar(int bar) {
    _playingBar = bar;
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
	svg::Paint normalPaint(ColorA8u(61, 76, 92));
	svg::Paint highlightPaint(ColorA8u(133, 153, 173));
    svg::Paint playingPaint(ColorA8u(82, 102, 122));
    svg::Paint playingHighlightPaint(ColorA8u(188, 199, 210));
	svg::Style s, p, hp;
    p.setFill(playingPaint);
	s.setFill(normalPaint);
    hp.setFill(playingHighlightPaint);
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
            
            if(_playingBar == bar) {
                n->setStyle(p);
            }

			for(auto cIt = _cells.begin(); cIt != _cells.end(); ++cIt) {
				Vec2f p = cIt->second.position() - (app::getWindowSize() - _artwork->getSize())/2;
				if(n->containsPoint(p)) {
                    if(_playingBar == bar) {
                        n->setStyle(hp);
                    } else {
                        n->setStyle(s);
                    }
                    std::pair<int, int> nId(track, bar);
                    _activeCells[nId] = cIt->first;
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