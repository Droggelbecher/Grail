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
	background(background), _actorsMoved(false), _drawWalls(false), background_id_counter(0){
}

Scene::Scene(const std::string& backgroundPath) :
	_actorsMoved(false), _drawWalls(false), background_id_counter(0) {
	this->background = Animation::Ptr(new Image(backgroundPath));
}

Scene::Scene(VirtualSize size) :
	size(size), _actorsMoved(false), _drawWalls(false), background_id_counter(0) {
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

unsigned int Scene::getBackgroundId(){
	background_id_counter++;
	if(background_id_counter == UINT_MAX)
		throw Exception("-- Too much Background's added");
	return background_id_counter;
}

unsigned int Scene::addBackground(Animation::Ptr background, VirtualPosition offset, double scrollFactorX, double scrollFactorY) {
	Parallax* bg = new Parallax(background, offset, scrollFactorX, scrollFactorY);
	bg->id = getBackgroundId();
  cdbg << "Add Background id: " << bg->id << "\n";
	backgrounds.push_back(bg);
	return bg->id;
}

void Scene::delBackground(unsigned int id) {
	list<Parallax*>::const_iterator iter;
  for(iter = backgrounds.begin(); iter != backgrounds.end(); iter++){
    if(id == (*iter)->id){
      Parallax* bg = (*iter);
      cdbg << "Del Background id: " << bg->id << "\n";
      backgrounds.remove(bg);
      delete bg;
			return;
    }
  }
}

unsigned int Scene::addForeground(Animation::Ptr foreground, VirtualPosition offset, double scrollFactorX, double scrollFactorY) {
	Parallax* bg = new Parallax(foreground, offset, scrollFactorX, scrollFactorY);
	bg->id = getBackgroundId();
  cdbg << "Add Foreground id: " << bg->id << "\n";
	foregrounds.push_back(bg);
	return bg->id;
}

void Scene::delForeground(unsigned int id) {
	list<Parallax*>::const_iterator iter;
	for(iter = foregrounds.begin(); iter != foregrounds.end(); iter++){
		if(id == (*iter)->id){
			Parallax* bg = (*iter);
  		cdbg << "Del Foreground id: " << bg->id << "\n";
			foregrounds.remove(bg);
			delete bg;
			return;
		}
	}
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
	
	#if VISUALIZE_SCENE
	ground.renderAt(target, ticks, p);
	#endif

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
	
	Game::getInstance().getDialogFrontend()->renderAt(target, ticks, p);
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
					Game::getInstance().event("actorClick", *iter);
					event_sent = true;
					break;
				}
			} // for
			if(!event_sent) {
				Event::sceneClick(pos + cam, event.button.button)->push();
				Game::getInstance().event("sceneClick", pos + cam);
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
						Game::getInstance().event("actorHover", *iter);
						break;
					}
				}
			}
			if(!hovered_something) {
				ui->setHovering(Actor::Ptr());
				Game::getInstance().event("sceneHover", pos + cam);
			}
		} // if area has point
	} // if mouse motion
	
	return EVENT_STATE_UNHANDLED;
}

} // namespace grail

