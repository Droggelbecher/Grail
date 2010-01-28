
class 'MyUI' (UserInterface)

function MyUI:__init()
  UserInterface.__init(self)
end

function MyUI:handleEvent(evt, frameDuration)

  if evt:getType() == EVT_ACTORCLICK then
    print("You clicked actor '"..tostring(evt:getActor()).."' with button "..tostring(evt:getButton()).." at "..tostring(evt:getPosition()))

  elseif evt:getType() == EVT_SCENECLICK then
    print("You clicked the scene with button "..tostring(evt:getButton()).." at "..tostring(evt:getPosition()))
    --get("guy"):walkStraight(evt.position)

  end
  return EVENT_STATE_UNHANDLED
end

function initChapter(n)
  print("This is initChapter("..n..")")

  if n == 0 then
    local s = Scene(Image("/media/$res/grey_fading.jpg"))
    GAME:registerScene(s, "grey_room")

    local guy = Actor("Line boy")
    local lineboy_ud = Image("/media/$res/lineboy_default.png")
    local lineboy_r = StripeSprite("/media/$res/lineboy_r.png", 9)
    local lineboy_l = StripeSprite("/media/$res/lineboy_l.png", 9)
    local lineboy_default = DirectionAnimation(4)
    lineboy_default:setAnimation(0, lineboy_ud)
    lineboy_default:setAnimation(1, lineboy_r)
    lineboy_default:setAnimation(2, lineboy_ud)
    lineboy_default:setAnimation(3, lineboy_l)

    guy:addAnimation("default", lineboy_default)
    guy:setPosition(VP(2000, 2000))
    GAME:getViewport():keepCentering(guy)

    s:addActor(guy)
    GAME:goToScene(s)
  end
end

GAME:setInitChapterCallback(initChapter)

print(1)
ui = MyUI()
print(2)
GAME:setUserInterface(ui)
print(3)
GAME:getViewport():setup(800, 600, false)

GAME:runChapter(0)





--[[


--s = _GRAIL.loadSceneDefinition("/media/test.scene")

GAME:registerChapterConstructor(0, function(n)
  print("Welcome to chapter 0")
  local bg = Image("/media/$res/grey_fading.jpg")
  local start = Scene()
  dbg_print(start)
  start:setBackground(bg)

  -- Lineboy

  local guy = Actor()

  local lineboy_ud = Image("/media/$res/lineboy_default.png")
  local lineboy_r = StripeSprite("/media/$res/lineboy_r.png", 9)
  local lineboy_l = StripeSprite("/media/$res/lineboy_l.png", 9)

  local lineboy_default = DirectionAnimation(4)
  dbg_print(lineboy_default)
  lineboy_default:setAnimation(0, lineboy_ud)
  lineboy_default:setAnimation(1, lineboy_r)
  lineboy_default:setAnimation(2, lineboy_ud)
  lineboy_default:setAnimation(3, lineboy_l)

  guy:addAnimation("default", lineboy_default)
  guy:setPosition(p(2000, 2000))
  GAME:getViewport():keepCentering(guy)

  -- Coke

  local coke = Actor("coke")
  coke:addAnimation("default", Image("/media/$res/coke.jpg"))
  coke:setPosition(p(3000, 3000))

  start:addActor(guy)
  start:addActor(coke)

  GAME:goToScene(start)

  return {
    coke = coke,
    start = start
  }
end)

GAME:getUserInterface().handleEvent = function(self, evt, duration)
  print("handleEvent")
  if evt.type == EVT_ACTOR_CLICK then
    print("You clicked the actor with button " .. tostring(evt.button))
  elseif evt.type == EVT_SCENE_CLICK then
    print("pos=", evt.position)
    get("guy"):walkStraight(evt.position)
  end
  return EVENT_STATE_UNHANDLED
end

GAME:runChapter(0)


]]--

