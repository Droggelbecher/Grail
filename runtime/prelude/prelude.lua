
doresource("/prelude/constants.lua")
doresource("/prelude/debug.lua")
doresource("/prelude/packages.lua")

-- Some shortcuts

GAME = Game.getInstance()
VP = VirtualPosition

function Poly(...)
	local p = VirtualPositionPolygon()
	for i=1,#arg do
		p:push_back(arg[i])
	end
	return p
end

