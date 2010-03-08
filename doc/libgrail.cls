
class Action {
  verb: string
  subjects: size_t
  allowScene: bool
  --
  + getText(Subject[]): string
}

class Actor : Area, Subject {
  position: VirtualPosition
  name: string
  mode: string
  alignment: double x double
  yOffset: VirtualPosition::Y
  --
  + walk(VirtualPosition): void
  + handleEvent(SDL_Event, uint32_t): EventState
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Actor -> Animation[*]

class ActionText : UserInterfaceElement {
  font: Font
  color: Color
  height: VirtualPosition::Y
  --
}
ActionText <>- Text

<<Interface>>
class Animation: Area {
  --
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}

<<Interface>>
class Area {
  --
  + hasPoint(VirtualPosition): bool
}

class Button : UserInterfaceElement {
  --
}
Button -- Animation[0..*]

<<Singleton>>
class Game {
}
Game -> Scene[0..*]
Game {}- MainLoop[1..*]
Game <>- Viewport[1]
Game <>- ResourceManager[1]
Game {}- UserInterface

class Ground {
}

class Image: Animation {
  --
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Image -- Surface[1]

class InventoryItem: Subject {
  --
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
InventoryItem -- Animation[1..2]

class InventoryBox: UserInterfaceElement {
}

class MainLoop {
  --
  + handleEvent(SDL_Event, uint32_t): EventState
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}

class Menu: UserInterfaceElement {
  --
}
Menu {}- MenuModel[1]

class MenuModel {
  entries: MenuModelEntry[]
  --
}
MenuModel <>- MenuModelEntry[0..*]

class MenuModelEntry {
  text: string
  action: method*
  --
}
MenuModelEntry -> MenuModel[0..*]

class MouseHandle : UserInterfaceElement {
  --
}
MouseHandle -- Animation[0..*]

class Rect : Area {
  size: VirtualSize
  --
}

class ResourceManager {
}
ResourceManager {}- ResourceHandler[1..*]

class ResourceHandler {
}

class Polygon : Area {
  nodes: list<VirtualPosition>
  --
}

class Scene {
  --
  + handleEvent(SDL_Event, uint32_t): EventState
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Scene <>- Ground
Scene[1..*] -- Actor[0..*]
Scene -> Animation[1..*]

class Sprite : Animation {
  --
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Sprite -> Image[*]

class SpeechText: UserInterfaceElement {
  --
}
SpeechText <>- Text

<<Interface>>
class Subject {
  --
  + getName(): string
}

class Surface {
  --
}

class Text: Animation {
  --
}

class UserInterface {
  --
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
UserInterface {}- UserInterfaceElement[*]
UserInterface -- Subject[*]
UserInterface -- Action[1]

<<Interface>>
class UserInterfaceElement {
  --
  + eachFrame(uint32_t): void
  + renderAt(SDL_Surface, uint32_t, VirtualPosition): void
  + handleEvent(SDL_Event, uint32_t): EventState
}

class Viewport {
  virtualScreenSize: VirtualSize
  physicalScreenSize: PhysicalSize
  cameraPosition: VirtualPosition
  --
  + eachFrame(uint32_t): void
  + renderScene(Scene): void
  + follow(Actor): void
}
Viewport -- Actor[0..1]

