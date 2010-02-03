
#include "user_interface_element.h"

namespace grail {
  UserInterfaceElement::UserInterfaceElement() : alignmentX(0), alignmentY(0) {
  }

  VirtualPosition UserInterfaceElement::getUpperLeftCorner() const {
    return VirtualPosition(
        position.getX() - alignmentX * getSize().getX(),
        position.getY() - alignmentY * getSize().getY()
        );
  }
} // namespace grail


