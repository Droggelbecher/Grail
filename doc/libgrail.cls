// vim: set noexpandtab ft=cpp:

/*
 * C-style-comments are allowed, too.
 */

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
	+ walk(VirtualPosition): WalkTask
	+ handleEvent(SDL_Event, uint32_t): EventState
	+ eachFrame(uint32_t): void
	+ renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Actor -> Animation[*]
Actor <>- State[1]
Actor -> WalkTask[0..*]

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

class AnimationCycleTask: Task {
	--
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
	--
}
Game -> Scene[0..*]
Game <>- Viewport[1]
Game <>- ResourceManager[1]
Game {}- UserInterface

class Ground {
	walls: Line[]
	--
	+ getPath(VirtualPosition start, VirtualPosition target): VirtualPosition[]
	+ isDirectReachable(VirtualPosition start, VirtualPosition target): bool
}
Ground -> Waypoint[1..*]

class Image: Animation {
	--
	+ eachFrame(uint32_t): void
	+ renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
Image -> Surface[1]

class InventoryItem: Subject {
	--
	+ renderAt(SDL_Surface, uint32_t, VirtualPosition): void
}
InventoryItem -- Animation[1..2]
InventoryItem <>- State[1]

class InventoryBox: UserInterfaceElement {
	rows: size_t
	columns: size_t
	itemOffset: VirtualSize
	--
}

class MainLoop {
	--
	+ run(): void
	+ run(Task[] waitForTasks): void
	+ exit(): void
}
MainLoop -> Game[1]
MainLoop -- Task[0..*]

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
MenuModelEntry -- Button[0..1] (buttonPrototype >)
MenuModelEntry {}- MenuModel[0..*]

class MouseHandle : UserInterfaceElement {
	--
}
MouseHandle -> UserInterfaceElement[0..*]

class Rect : Area {
	size: VirtualSize
	--
}

class ResourceManager {
	--
	+ getRW(string path, ResourceMode mode): SDL_RWops*
	+ fileExists(string path): bool;
	+ beginListing(string path): DirectoryIterator;
	+ endListing(string path): DirectoryIterator;
}
ResourceManager {}- ResourceHandler[1..*]

<<Interface>>
class ResourceHandler {
	--
	+ getRW(string path, ResourceMode mode): SDL_RWops*
	+ fileExists(string path): bool;
	+ beginListing(string path): DirectoryIterator;
	+ endListing(string path): DirectoryIterator;
}

class DirectoryResourceHandler: ResourceHandler {
	--
}

class ZipfileResourceHandler: ResourceHandler {
	--
}

class Polygon: Area {
	nodes: list<VirtualPosition>
	--
}

class Scene {
	--
	+ handleEvent(SDL_Event, uint32_t): EventState
	+ eachFrame(uint32_t): void
	+ renderAt(SDL_Surface, uint32_t, VirtualPosition): void
	+ addBackground(Animation, scrollFactorX: double, scrollFactorY: double): void

	+ onEnter(Scene previous): void
	+ onLeave(Scene next): void
}
Scene <>- Ground[1]
Scene[1..*] -- Actor[0..*]
Scene -> Animation[1..*] (Foregrounds, backgrounds)
Scene <>- State[1]

class Sprite : Animation {
	--
	+ eachFrame(uint32_t): void
	+ renderAt(SDL_Surface, uint32_t, VirtualPosition): void
	+ waitForCycleEnd(): AnimationCycleTask
}
Sprite -> Image[0..*]
Sprite -> AnimationCycleTask[0..*]

<<Interface>>
class SpeechDisplay: UserInterfaceElement {
	--
	enqueue(sentence: string): TalkTask
}
SpeechDisplay {}- TalkTask[0..*]

class SimpleSpeechText: SpeechDisplay {
	position: VirtualPosition
	durationPerChar: size_t
	minimalDuration: size_t
	--
}
SimpleSpeechText <>- Text[1]

class SpeechBubbles: SpeechDisplay {
	--
}
SpeechBubbles {}- Text[0..*]

class State {
	variables: StateVariable[string]
	--
}
State {}- StateVariable[0..*]

class StateVariable {
	name: string
	type: enum(STRING, NUMBER, TABLE)
	value: string / number / State*
	--
}
StateVariable {}- State[0..1]

<<Interface>>
class Subject {
	--
	+ getName(): string
}

class Surface {
	--
}

<<Interface>>
class Task {
	sendEventWhenDone: bool
	--
	markDone(): void
	isDone(): bool
	block(): void
	cancel(): void
}

class TalkTask: Task {
	sentence: string
	--
}
TalkTask {}- Actor

class Text: Animation {
	text: string
	--
}
Text -> Surface[0..2]

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

class WalkTask: Task {
	target: VirtualPosition
	path: VirtualPosition[]
	--
}

class Waypoint {
	--
}
Waypoint -> Waypoint[0..*] (Neighbours)

