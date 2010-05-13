
#include "to_lua_table.h"
#include "interpreter.h"

luabind::object toLuaTable(const grail::Event& evt) {
	luabind::object o(luabind::newtable(interpreter.L));
	o["type"] = evt.getType();
	o["button"] = evt.getButton();
	o["button_state"] = evt.getButtonState();

	luabind::object pos(luabind::newtable(interpreter.L));
	pos["x"] = evt.getPosition().getX();
	pos["y"] = evt.getPosition().getY();
	o["position"] = pos;
	o["actor"] = evt.getActor();

	luabind::object keysym(luabind::newtable(interpreter.L));
	keysym["scancode"] = evt.getKeysym().scancode;
	keysym["sym"] = evt.getKeysym().sym;
	keysym["mod"] = evt.getKeysym().mod;
	keysym["unicode"] = evt.getKeysym().unicode;
	o["keysym"] = keysym;
	return o;
}

