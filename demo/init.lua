
GAME:getViewport():setup(800, 600, false)

GAME:registerChapter(0, function(n)
  print("Welcome to chapter 0")

  -- Register objects by creating them and giving them a name
  -- chapter() also returns the created objects so you can directly pass it
  -- around in a lua variable. Here we use get() instead for demonstration
  -- purposes

  Image:chapter("bg_grey_fading", "/media/$res/grey_fading.jpg")
  start_scene = Scene:chapter("start")

  get("start"):setBackground(get("bg_grey_fading"))
  -- Same as:
  -- start_scene:setBackground(get("coke"))

  guy = Actor:chapter("guy")
  lineboy_ud = Image:chapter("img_lineboy_ud", "/media/$res/lineboy_default.png")
  lineboy_r = StripeSprite:chapter("img_lineboy_r", "/media/$res/lineboy_r.png", 9)
  lineboy_l = StripeSprite:chapter("img_lineboy_l", "/media/$res/lineboy_l.png", 9)
  lineboy_default = DirectionAnimation:chapter("", 4)
  lineboy_default:setAnimation(0, lineboy_ud)
  lineboy_default:setAnimation(1, lineboy_r)
  lineboy_default:setAnimation(2, lineboy_ud)
  lineboy_default:setAnimation(3, lineboy_l)

  guy:addAnimation("default", lineboy_default)
  guy:setMode("default")
  guy:setPosition({x = 2000, y = 2000})

  start_scene:addActor(guy)
  GAME:getViewport():keepCentering(guy)

  GAME:goToScene(get("start"))

end)

GAME:getUserInterface().handleEvent = function(self, evt, duration)
  if evt.type == EVT_ACTOR_CLICK then
    print("You clicked the actor with button " .. tostring(evt.button))
  elseif evt.type == EVT_SCENE_CLICK then
    print("pos=", evt.position)
    get("guy"):walkStraight(evt.position)
  end
  return EVENT_STATE_UNHANDLED
end

GAME:runChapter(0)


