
GAME = Game.getInstance()

doresource("/prelude/packages.lua")

doresource("/prelude/constants.lua")
print("------------------------ a")
doresource("/prelude/gui_task.lua")
print("------------------------")

VP = VirtualPosition

local t = GUITask()
t:start()

