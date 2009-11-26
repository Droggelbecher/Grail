
GAME:getViewport():setup(800, 600, false)



GAME:registerChapter(0, function(n)
  print("Welcome to chapter 0")

  bg = Image:chapterNamed("bg_grey_fading", "/media/$res/grey_fading.jpg")
  start = Scene:chapter()

  start:setBackground(bg)

  guy = Actor:chapterNamed("guy")

  lineboy_ud = Image:chapter("/media/$res/lineboy_default.png")
  lineboy_r = StripeSprite:chapter("/media/$res/lineboy_r.png", 9)
  lineboy_l = StripeSprite:chapter("/media/$res/lineboy_l.png", 9)
  lineboy_default = DirectionAnimation:chapter(4)
  lineboy_default:setAnimation(0, lineboy_ud)
  lineboy_default:setAnimation(1, lineboy_r)
  lineboy_default:setAnimation(2, lineboy_ud)
  lineboy_default:setAnimation(3, lineboy_l)

  guy:addAnimation("default", lineboy_default)
  guy:setMode("default")
  guy:setPosition(p(2000, 2000))

  start:addActor(guy)
  GAME:getViewport():keepCentering(guy)

  GAME:goToScene(start)
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


