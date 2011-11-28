// vim: set noexpandtab:

#include <cassert>
#include <list>
using std::list;
#include <iostream>
using std::cerr;
using std::endl;

#include <SDL_gfxPrimitives.h>

#include "scene.h"
#include "game.h"
#include "viewport.h"
#include "rect.h"
#include "actor.h"
#include "image.h"

namespace grail {

Scene::Scene(Animation::Ptr background) :
	background(background), _actorsMoved(false), _drawWalls(false) {
}

Scene::Scene(const std::string& backgroundPath) :
	_actorsMoved(false), _drawWalls(false) {
	this->background = Animation::Ptr(new Image(backgroundPath));
}

Scene::Scene(VirtualSize size) :
	size(size), _actorsMoved(false), _drawWalls(false) {
}

Scene::~Scene() {
	std::list<Parallax*>::iterator iter;
	for(iter = backgrounds.begin(); iter != backgrounds.end(); ++iter) {
		delete *iter;
	}
	for(iter = foregrounds.begin(); iter != foregrounds.end(); ++iter) {
		delete *iter;
	}
}

VirtualSize Scene::getSize() const {
	if(size == VirtualSize() && background) { return background->getSize(); }
	return size;
}

void Scene::setBackground(Animation::Ptr background) {
	background = background;
}

void Scene::addBackground(Animation::Ptr background, VirtualPosition offset, double scrollFactorX, double scrollFactorY) {
	Parallax* bg = new Parallax(background, offset, scrollFactorX, scrollFactorY);
	backgrounds.push_back(bg);
}

void Scene::addForeground(Animation::Ptr foreground, VirtualPosition offset, double scrollFactorX, double scrollFactorY) {
	Parallax* bg = new Parallax(foreground, offset, scrollFactorX, scrollFactorY);
	foregrounds.push_back(bg);
}

void Scene::eachFrame(uint32_t ticks) {
	if(_actorsMoved) {
		actors.sort(Actor::CompareByY());
		_actorsMoved = false;
	}
	
	list<Actor::Ptr>::const_iterator iter;
	for(iter = actors.begin(); iter != actors.end(); iter++) {
		(*iter)->eachFrame(ticks);
	}
}

void Scene::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
	assert(target);
	if(background) {
		background->renderAt(target, ticks, p);
	}
	
	list<Parallax*>::const_iterator piter;
	for(piter = backgrounds.begin(); piter != backgrounds.end(); ++piter) {
		(*piter)->animation->renderAt(target, ticks,
				VirtualPosition(
					p.getX() * (*piter)->scrollFactorX,
					p.getY() * (*piter)->scrollFactorY
					) + (*piter)->offset
		);
	}
	
	list<Actor::Ptr>::const_iterator iter;
	for(iter = actors.begin(); iter != actors.end(); ++iter) {
		(*iter)->renderAt(target, ticks, p);
	}
	
	for(piter = foregrounds.begin(); piter != foregrounds.end(); ++piter) {
		(*piter)->animation->renderAt(target, ticks,
				VirtualPosition(
					p.getX() * (*piter)->scrollFactorX,
					p.getY() * (*piter)->scrollFactorY
					) + (*piter)->offset
		);
	}
	
	if(_drawWalls) {
		/*
		list<Line>::const_iterator iter;
		for(iter = ground.getWalls().begin(); iter != ground.getWalls().end(); iter++) {
			PhysicalPosition a = conv<VirtualPosition, PhysicalPosition>(iter->getA() + p);
			PhysicalPosition b = conv<VirtualPosition, PhysicalPosition>(iter->getB() + p);
			aalineColor(target, a.getX(), a.getY(), b.getX(), b.getY(), 0xffffffff);
		}
		*/
	}
	
} // renderAt

EventState Scene::handleEvent(SDL_Event& event, uint32_t ticks) {
	if(event.type == SDL_MOUSEBUTTONDOWN) {
		VirtualPosition pos = conv<const SDL_MouseButtonEvent&, VirtualPosition>(event.button);
		
		if(Rect(getSize()).hasPoint(pos)) {
			list<Actor::Ptr>::const_iterator iter;
			bool event_sent = false;
			VirtualPosition cam = Game::getInstance().getViewport().getCameraPosition();
			
			for(iter = actors.begin(); iter != actors.end(); iter++) {
				if((*iter)->hasPoint(pos + cam)) {
					Event::actorClick(*iter, pos + cam, event.button.button)->push();
					
					event_sent = true;
					break;
				}
			} // for
			if(!event_sent) {
				Event::sceneClick(pos + cam, event.button.button)->push();
			}
		} // if area has point
	} // if mouse button down
	
	else if(event.type == SDL_MOUSEMOTION) {
		VirtualPosition pos = conv<const SDL_MouseMotionEvent&, VirtualPosition>(event.motion);
		
		if(Rect(getSize()).hasPoint(pos)) {
			UserInterface::Ptr ui = Game::getInstance().getUserInterface();
			
			list<Actor::Ptr>::const_iterator iter;
			VirtualPosition cam = Game::getInstance().getViewport().getCameraPosition();
			
			bool hovered_something = false;
			for(iter = actors.begin(); iter != actors.end(); iter++) {
				if((*iter)->hasPoint(pos + cam)) {
					if(ui) {
						hovered_something = true;
						ui->setHovering(*iter);
						break;
					}
				}
			}
			if(!hovered_something) {
				ui->setHovering(Actor::Ptr());
			}
		} // if area has point
	} // if mouse motion
	
	return EVENT_STATE_UNHANDLED;
}

} // namespace grail

