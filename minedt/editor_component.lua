
local __minedt = {
	state = 'init'
}

function __minedt:changeState(new_state)
	self.state = new_state
end

function __minedt:handleEvent(evt)
	--print(evt:getType())
end

function __minedt.handleEventCallback(evt)
	return __minedt:handleEvent(evt)
end

GAME:registerEarlyEventHandler(__minedt.handleEventCallback)

print("\027[34;1m--> \027[m\027[34mMinEdt linked in")
print("\027[m")

