
doresource("/prelude/constants.lua")
doresource("/prelude/debug.lua")

-- Make wrapper bases appear in global scope for easy use
-- and provide them with constructors

table.foreach(_GRAIL.wrapperBases, function(k, v)
  v.classname = k
  setmetatable(v, { __call = function(tbl, ...) return v.ctor(...) end })
  _G[k] = v
end)

-- Extensions for GAME

GAME = _GRAIL.wrapperBases.Game.getInstance()

function p(x, y)
  return {x = x, y = y}
end


