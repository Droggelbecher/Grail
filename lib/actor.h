// vim: set noexpandtab:

#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>
#include <iostream>
#include <list>

#include <boost/shared_ptr.hpp>

#include "classes.h"
#include "vector2d.h"
#include "animation.h"
#include "area.h"
#include "debug.h"

namespace grail {
	
	typedef std::list<VirtualPosition> Path;
	
	class Actor : public Area {
			Animation::Ptr animation;
			std::map<std::string, Animation::Ptr> animationModes;
			std::string mode;
			std::string name;
			
			VirtualPosition position;
			VirtualPosition::Y yOffset;
			double alignmentX, alignmentY;
			Area::Ptr area;
			double speed; ///< Unit is virtual pixels / second
			
			Path walkPath;
			
		protected:
			VirtualPosition getUpperLeftCorner() const;
			
		public:
			typedef boost::shared_ptr<Actor> Ptr;
			
			struct CompareByY {
				bool operator()(Actor::Ptr a, Actor::Ptr b) const {
					return a->position.getY() < b->position.getY();
				}
			};
			
			Actor(std::string name);
			
			std::string getName() const;
			
			bool hasPoint(VirtualPosition p) const;
			
			void eachFrame(uint32_t ticks);
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
			
			void addAnimation(std::string mode, Animation::Ptr animation);
			void setAlignment(double x, double y);
			void setMode(std::string mode);
			
			VirtualPosition getPosition() const { return position; }
			void setPosition(VirtualPosition position) { this->position = position; }
			void setYOffset(VirtualPosition::Y offset) { yOffset = offset; }
			VirtualPosition getInteractionPosition() const { return position; }
			
			void walk(const Path& path);
			void walkTo(Actor::Ptr actor);
			void walkTo(VirtualPosition p);
			void walkStraight(VirtualPosition p);
	};
	
	std::ostream& operator<<(std::ostream& os, const Actor& actor);
	
} // namespace grail

#endif // ACTOR_H

