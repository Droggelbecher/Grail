// vim: set noexpandtab:

#ifndef SDL_EXCEPTION_H
#define SDL_EXCEPTION_H

#include <SDL.h>
#include <exception>

namespace grail {
	
	class SDLException : public std::exception {
			std::string info;
		public:
			SDLException(std::string info = "") throw() : info(info) {
			}
			
			~SDLException() throw() {
			}
			
			const char* what() const throw() {
				std::string r;
				if(info.length() > 0) {
					r = info + ": ";
				}
				r += std::string(SDL_GetError());
				return r.c_str();
			}
	};
	
}

#endif // SDL_EXCEPTION_H

