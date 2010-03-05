
#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <SDL/SDL_mixer.h>

namespace grail {

class Audio {
  protected:


  public:
    Audio();
    ~Audio();
    void playSound(std::string soundfile);
  private:
    Mix_Music *sound;
};

} // namespace grail

#endif // AUDIO_H