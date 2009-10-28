
--[[ prelude internal functions ]]--

_wrappers = {}
local function getwrapper(classname)
  return _wrappers[classname]
end

local function makewrapper(classname, tbl)
  if tbl == nil then tbl = {} end
  tbl.__index = tbl
  _wrappers[classname] = tbl
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
})

-- Global preconstructed objects

GAME = application("Game", "GAME")

print("[*] Loaded lua prelude")


