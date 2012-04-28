
module('scene1', package.seeall)

class 'Scene1' (Scene)

function Scene1:__init(size)
	print("scene1 init")
	Scene.__init(self, size)
end

function Scene1:onEnter()
	print("onEnter")
	local mc = GAME:getMainCharacter()
	
	--GAME:enableUserControl(false);

	--mc:say("Wooo, dark", 4000)
	--mc:say("I wonder if Deckard came this way?", 6000)
	
	--WaitTask(9000):block()

	--self.pumpkin1:say("GROG!", 3000 );
	--WaitTask(400):block()
	--self.pumpkin2:say("GROG!", 3000 );
	--WaitTask(400):block()
	--self.pumpkin3:say("GROG!", 3000 );
	--WaitTask(400):block()

	--WaitTask(3200):block()
	
	--mc:say("OMG! WTF?", 5000);
	
	--GAME:enableUserControl(true);
end

