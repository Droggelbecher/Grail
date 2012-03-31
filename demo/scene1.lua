
module('scene1', package.seeall)

class 'Scene1' (Scene)

function Scene1:__init(size)
	print("scene1 init")
	Scene.__init(self, size)
end

function Scene1:onEnter()
	print("onEnter")
	local mc = GAME:getMainCharacter()
	
	GAME:enableUserControl(false);

	mc:say("I am just demonstrating...", 4000)

	mc:say("...how I can speak.", 4000)

	self.pumpkin2:say("We need a way to stop me speaking over Luise.", 3000)
	
	GAME:enableUserControl(true);
end
