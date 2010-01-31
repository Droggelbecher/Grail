
class 'MyUI' (UserInterface)

function MyUI:__init()
  UserInterface.__init(self)
end

function MyUI:handleEvent(evt, frameDuration)
  if evt:getType() == EVT_ACTORCLICK then
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

ui = MyUI()
GAME:setUserInterface(ui)

