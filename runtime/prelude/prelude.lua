
--[[ prelude internal functions ]]--

_wrappers = {}
local function getwrapper(classname)
  return _wrappers[classname]
end

local function makewrapper(classname, tbl)
  if tbl == nil then tbl = {} end
  tbl.__index = tbl
  _wrappers[classname] = tbl

  -- Somewhat "prettier" creation: Image:chapter(...)
  tbl.chapter = function(...) return chapter(classname, ...) end
  tbl.application = function(...) return application(classname, ...) end
  _G[classname] = tbl
end

local function wrap(classname, ptr)
  -- we need a silly table around ptr so it can have a metatable
  container = {}
  container._ptr = ptr

  setmetatable(container, getwrapper(classname))
  return container
end

local function wrapped(fn)
  function fn_wrapped(...)
    cls, ptr = fn(...)
    return wrap(cls, ptr)
  end
  return fn_wrapped
end


--[[ exported ctor/getterfunctions ]]--

function get(name)
  classname, ptr = _get(name)
  return wrap(classname, ptr)
end

function chapter(classname, ...)
  cls, ptr = _create(classname, 1, ...)
  return wrap(cls, ptr)
end

function application(classname, ...)
  cls, ptr = _create(classname, 0, ...)
  return wrap(cls, ptr)
end



--[[ wrappers ]]--

makewrapper("Image")
makewrapper("Game", {
  _chapter_ctors = {},
  registerChapter = function(self, n, f) self._chapter_ctors[n] = f end,
  runChapter = _Game_runChapter,
  goToScene = _Game_goToScene,
  getViewport = wrapped(_Game_getViewport),
})
makewrapper("Viewport", {
  setup = _Viewport_setup,
})
makewrapper("Scene", {
  setBackground = _Scene_setBackground,
  addActor = _Scene_addActor
})
makewrapper("Actor", {
  addAnimation = _Actor_addAnimation,
  setMode = _Actor_setMode,
  setPosition = _Actor_setPosition,
})

-- Global preconstructed objects

local game_name, game_ptr = _Game_instance()
GAME = wrap(game_name, game_ptr)

print("[*] Loaded lua prelude")


