// vim: set noexpandtab:

#include "actor.h"

#include <algorithm>
using std::copy;
#include <cassert>

#include "game.h"
#include "scene.h"

namespace grail {

Actor::Actor(std::string name) :
	mode("default"), name(name), yOffset(0), alignmentX(0.5), alignmentY(1.0), speed(1000.0), dialogGapTime(300) {
}

std::string Actor::getName() const {
	return name;
}

bool Actor::hasPoint(VirtualPosition p) const {
	if(area) {
		return area->hasPoint(p - (getUpperLeftCorner() + VirtualPosition(0, yOffset)));
	}
	else if(animation) {
		return animation->hasPoint(p - (getUpperLeftCorner() + VirtualPosition(0, yOffset)));
	}
	else {
		return false;
	}
}

void Actor::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
	if(animation) {
		animation->renderAt(target, ticks, getUpperLeftCorner() + p + VirtualPosition(0, yOffset));
	}
	#if VISUALIZE_HOTSPOTS
		PhysicalPosition pos = conv<VirtualPosition, PhysicalPosition>(getPosition() + p);
		aalineColor(target, pos.getX() - 10, pos.getY() - 10, pos.getX() + 10, pos.getY() + 10, 0x00ff00ff);
		aalineColor(target, pos.getX() - 10, pos.getY() + 10, pos.getX() + 10, pos.getY() - 10, 0x00ff00ff);
	#endif
	
	#if VISUALIZE_WALKPATH
		VirtualPosition prev = position;
		for(Path::const_iterator iter = walkPath.begin(); iter != walkPath.end(); ++iter) {
			PhysicalPosition a = conv<VirtualPosition, PhysicalPosition>(prev + p);
			PhysicalPosition b = conv<VirtualPosition, PhysicalPosition>(*iter + p);
			aalineColor(target, a.getX(), a.getY(), b.getX(), b.getY(), 0xff0000ff);
			prev = *iter;
		}
	#endif
}

void Actor::addAnimation(std::string mode, Animation::Ptr animation) {
	animationModes[mode] = animation;
	if(animationModes.count(this->mode)) {
		this->animation = animationModes[mode];
	}
}

void Actor::setMode(std::string mode) {
	this->mode = mode;
	if(animationModes.count(mode)) {
		Animation::Ptr previous = animation;
		animation = animationModes[mode];
		animation->makeContinuationOf(*previous);
	}
}

void Actor::setAlignment(double x, double y) {
	alignmentX = x;
	alignmentY = y;
}

VirtualPosition Actor::getUpperLeftCorner() const {
	if(animation) {
		return VirtualPosition(
			position.getX() - alignmentX * animation->getSize().getX(),
			position.getY() - alignmentY * animation->getSize().getY()
			);
	}
	else {
		return position;
	}
}

void Actor::walkTo(Actor::Ptr actor) {
	walkTo(actor->getInteractionPosition());
}

void Actor::walkTo(VirtualPosition p) {
	//walkStraight(p);
	cdbg << "walkTo " << p << "\n";
	Scene::Ptr s = Game::getInstance().getCurrentScene();
	if(s) {
		walkPath.clear();
		s->getGround().getPath(position, p, walkPath);
	}
}

void Actor::walk(const Path& path) {
	walkPath.clear();
	copy(path.begin(), path.end(), walkPath.begin());
	assert(walkPath.size() == path.size());
}

void Actor::walkStraight(VirtualPosition p) {
	walkPath.clear();
	walkPath.push_back(p);
}

void Actor::eachFrame(uint32_t ticks) {
	if(animation) {
		animation->eachFrame(ticks);
	}

	if (isSpeaking()) {
		// start the next dialog  in the queue
		if (!getDialogLine()->isStarted()) {
			getDialogLine()->start();
		}

		getDialogLine()->eachFrame();

		// remove if finished
		if (getDialogLine()->isComplete()) {
			dialogLines.pop();
		}
	}
	
	if(!walkPath.empty()) {
		if(!isWalking){// start walking
			isWalking = true;
			setMode("walk");
			Game::getInstance().event("actorMove", *this);
		}

		VirtualPosition target = walkPath.front();
		VirtualPosition diff = target - position;
		
		if(animation && diff.length()) {
			animation->setDirection(diff);
		}
		
		if(diff.length() <= (speed * ticks / 1000.0)) {
			position = target;
		}
		else {
			double steplen = (speed * ticks / (1000.0 * diff.length()));
			VirtualPosition step = diff * steplen;
			
			position = position + step;
		}
		
		if(diff.length()) {
			Game::getInstance().getCurrentScene()->actorsMoved();
		}
		
		if(position == target) {
			walkPath.pop_front();
			if(walkPath.empty()) {
				isWalking = false;
				setMode("default");
				Game::getInstance().event("actorArrived", *this);
			}
		}
	} else {
		if(isWalking){
			isWalking = false;
			setMode("default");
			Game::getInstance().event("abortWalking", *this);
			Game::getInstance().event("actorArrived", *this);
		} 
	}// if walkPath not empty
}

void Actor::say(std::string statement, uint32_t displayTime) {
	boost::shared_ptr<DialogLine> line(new DialogLine(shared_from_this(),statement,displayTime));
	dialogLines.push(line);

	// add a pause after the dialog
	boost::shared_ptr<DialogLine> gap(new DialogLine(shared_from_this(),"", dialogGapTime));
	dialogLines.push(gap);

	Game::getInstance().getDialogFrontend()->say(line);
}

bool Actor::isSpeaking() {
	return !(dialogLines.empty());
}

DialogLine::Ptr Actor::getDialogLine() {
	return dialogLines.front();
}

std::ostream& operator<<(std::ostream& os, const Actor& actor) {
	os << actor.getName();
	return os;
}

} // namespace grail

