
doresource("/prelude/constants.lua")
doresource("/prelude/debug.lua")

-- Make wrapper bases appear in global scope for easy use
-- and provide them with constructors

table.foreach(_GRAIL.wrapperBases, function(k, v)
  _G[k] = v
  v.classname = k
  v.chapter = function(self, ...)
    r = v.ctor(...)
    _GRAIL.registerChapter(r, '')
    return r
  end
  v.application = function(self, ...)
    r = v.ctor(...)
    _GRAIL.registerApplication(r, '')
    return r
  end
  v.chapterNamed = function(self, name, ...)
    r = v.ctor(...)
    _GRAIL.registerChapter(r, name)
    return r
  end
  v.applicationNamed = function(self, name, ...)
    r = v.ctor(...)
    _GRAIL.registerApplication(r, name)
    return r
  end
end)

-- Global functions

get = _GRAIL.get

-- Extensions for GAME

GAME = _GRAIL.wrapperBases.Game.getInstance()

GAME._chapter_ctors = {}
function GAME:registerChapter(n, fn)
  self._chapter_ctors[n] = fn
end


function p(x, y)
  return {x = x, y = y}
end


