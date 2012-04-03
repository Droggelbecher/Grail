-- vim: set noexpandtab:

module('user_interface', package.seeall)

class 'MyUI' (UserInterface)

function MyUI:__init()
	UserInterface.__init(self)
	
	-- Panel
	local atbg = UserInterfaceAnimation(Box(VP(4000, 200)))
	atbg.position = VP(0, 0)
	self:addElement(atbg)
	
	local atborder = UserInterfaceAnimation(Box(VP(4000, 5), 0x303030))
	atborder.position = VP(0, 200)
	self:addElement(atborder)
	
	-- Hand
	local hand = Button(Image("media/$res/ui/hand.png"), function() print("--- Clicked Hand-Button"); return 0 end)
	hand.position = VP(10, 0)
	self:addElement(hand)
	
	local walk = UserInterfaceAnimation(Image("media/$res/ui/walk.png"))
	walk.position = VP(250, 0)
	self:addElement(walk)
	
	-- Action text
	local at = ActionText(Font("fonts/tommy_holloway.ttf", 40, 1))
	at.position = VP(600, 120)
	at.alignmentX = 0.0
	at.alignmentY = 0.5
	
	local go = Action("Go to ", 1)
	self:setAction(go)
	
	self:addElement(at)

	-- Dialog display
	local speechbubbles = SpeechBubbles()
	self:addElement(speechbubbles)
	-- local subtitles = Subtitles()
	-- subtitles.position = VP(2000,2800)
	-- subtitles.font = Font("fonts/tommy_holloway.ttf", 45, 1)
	-- subtitles.centered = true
	-- subtitles.showSpeakersName = true
	-- subtitles.useActorsFont = true
	-- self:addElement(subtitles)
end

function MyUI:handleEvent(evt, frameDuration)
	local r = UserInterface.handleEvent(self, evt, frameDuration)
	if r == EVENT_STATE_HANDLED then
		return EVENT_STATE_HANDLED
	end
	
	if evt:getType() == EVT_ACTORCLICK then
		GAME:getMainCharacter():walkTo(evt:getActor())
		print("You clicked actor '"..tostring(evt:getActor())..
			"' with button "..tostring(evt:getButton())..
			" at "..tostring(evt:getPosition()))
		
	elseif evt:getType() == EVT_SCENECLICK then
		print("You clicked the scene with button "..tostring(evt:getButton())..
			" at "..tostring(evt:getPosition()))
		GAME:getMainCharacter():walkStraight(evt:getPosition())
		return EVENT_STATE_HANDLED
	end
	return EVENT_STATE_UNHANDLED
end


function create()
	return MyUI()
end


