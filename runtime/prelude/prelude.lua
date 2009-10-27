
_ptr_to_table = {}

local function wrap(ptr, clsname)
  r = {}
  r._ptr = ptr
  _ptr_to_table[ptr] = r

  if clsname == 'game' then
    r._chapter_ctors = {}
    r.runChapter = _Game_runChapter
    r.registerChapter = function(self, n, f) self._chapter_ctors[n] = f end
    r.goToScene = _Game_goToScene
    r.getViewport = _Game_getViewport

  elseif clsname == 'viewport' then
    r.setup = _Viewport_setup

  elseif clsname == 'scene' then
    r.setBackground = _Scene_setBackground

  elseif clsname == 'resource_manager' then

  end
  return r
end

--[[
  If the given ptr has a wrapping table already, return it,
  otherwise create one with make and return that.
  ]]--
function _wrap_or_get(ptr, clsname)
  if _ptr_to_table[ptr] == nil then
    return wrap(ptr, clsname)
  else
    return _ptr_to_table[ptr]
  end
end

-- Global preconstructed objects

Game = _Game_create()

-- Constructors

createImage = _Image_create
createScene = _Scene_create

