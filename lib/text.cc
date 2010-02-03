
#include "text.h"

namespace grail {
  
  Text::Text(Font::Ptr font, Font::Ptr outlineFont) :
    BlitCached(2),
    font(font), outlineFont(outlineFont), color(white), outlineColor(black) {
  }

  Text::Text(Font::Ptr font) :
    BlitCached(2),
    font(font), color(white), outlineColor(black) {
  }

  Surface* Text::render(int i) const {

    if(i == 0) { // text surface
      return new Surface(TTF_RenderUTF8_Blended(font->getSDL(), text.c_str(), color));
    }
    else if(outlineFont) { // outline surface
      return new Surface(TTF_RenderUTF8_Blended(outlineFont->getSDL(), text.c_str(), outlineColor));
    }

    return 0;
  }


  void Text::setOutline(int outline) {
    //outlineFont->setOutline(outline);
    this->outline = outline;
    setChanged();
  }

}

