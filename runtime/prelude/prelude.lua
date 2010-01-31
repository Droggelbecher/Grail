
doresource("/prelude/constants.lua")
doresource("/prelude/debug.lua")

GAME = Game.getInstance()

VP = VirtualPosition

local _import = function(path, P)
  setfenv(1, P)
  doresource(path)
end

function import(name)
  local path = name .. '.lua'
  local P = {
    _path = path,
    _name = name
  }
  _G[name] = P
  setmetatable(P, {__index = _G})
  _import(path, P)
end

