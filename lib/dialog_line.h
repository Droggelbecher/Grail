// vim: set noexpandtab:

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <string>
#include <boost/shared_ptr.hpp>

#include "SDL.h"

namespace grail {

	class DialogLine {

		public:
			typedef boost::shared_ptr<DialogLine> Ptr;

			DialogLine(std::string, uint32_t);

			void eachFrame();
			std::string getText() { return text; };
			bool isComplete();

		protected:
			std::string text;

			uint32_t timeToLive;
			uint32_t timeStarted;

			bool complete;
	};

} // namespace grail

#endif // DIALOGLINE_H

