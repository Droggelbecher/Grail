// vim: set noexpandtab:

#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>
#include <iostream>
#include <list>
#include <queue>

#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include "classes.h"
#include "vector2d.h"
#include "animation.h"
#include "area.h"
#include "debug.h"

class dialogLine;
class font;

namespace grail {
	
	typedef std::list<VirtualPosition> Path;
	
	/**
	 * Anything in the game that can walk around (e.g. people)
	 */
	class Actor : public Area, public boost::enable_shared_from_this<Actor> {
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

			// speaking
			std::queue<boost::shared_ptr<DialogLine> > dialogLines;
			uint32_t dialogGapTime; // the length of time between each line of dialog
			boost::shared_ptr<Font> font; // the font to use for their dialog if required
			
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
			
			/**
			 * Name of the actor as displayed by user interface elements
			 */
			std::string getName() const;
			
			/**
			 * Overriden from Area. Returns true if the given point belongs to
			 * the actor.
			 */
			bool hasPoint(VirtualPosition p) const;
			
			void eachFrame(uint32_t ticks);
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
			
			/**
			 * Add animation for the given mode
			 */
			void addAnimation(std::string mode, Animation::Ptr animation);
			void setAlignment(double x, double y);
			
			/**
			 * Set the animation mode of the character e.g. "walking",
			 * "bored", etc...
			 */
			void setMode(std::string mode);
			std::string getMode() const { return mode; }
			
			VirtualPosition getPosition() const { return position; }
			void setPosition(VirtualPosition position) { this->position = position; }
			VirtualPosition::Y getYOffset() { return yOffset; }
			void setYOffset(VirtualPosition::Y offset) { yOffset = offset; }
			VirtualPosition getInteractionPosition() const { return position; }
			
			void walk(const Path& path);
			void walkTo(Actor::Ptr actor);
			void walkTo(VirtualPosition p);
			void walkStraight(VirtualPosition p);

			void say(std::string);
			void say(std::string, uint32_t);
			bool isSpeaking();
			boost::shared_ptr<DialogLine> getDialogLine();
			boost::shared_ptr<Font> getFont();
			void setFont(boost::shared_ptr<Font>);
	};
	
	std::ostream& operator<<(std::ostream& os, const Actor& actor);
	
} // namespace grail

#endif // ACTOR_H

