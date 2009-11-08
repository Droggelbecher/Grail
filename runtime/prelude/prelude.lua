
doresource("/prelude/constants.lua")
doresource("/prelude/debug.lua")

-- Make wrapper bases appear in global scope for easy use
-- and provide them with constructors

table.foreach(_GRAIL.wrapperBases, function(k, v)
  _G[k] = v
  v.classname = k
  v.chapter = function(self, ...) return _GRAIL.create(self.classname, 0, ...) end
  v.application = function(self, ...) return _GRAIL.create(self.classname, 1, ...) end
end)

-- Global functions

get = _GRAIL.get

-- Extensions for GAME

GAME = _GRAIL.wrapperBases.Game.instance()

GAME._chapter_ctors = {}
function GAME:registerChapter(n, fn)
  self._chapter_ctors[n] = fn
end

