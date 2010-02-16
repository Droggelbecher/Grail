
require 'user_interface'
require 'main_character'

function initChapter(n)
  print("This is initChapter("..n..")")

  if n == 0 then
    local s = Scene(Image("/media/$res/scene1/background.png"))
    GAME:registerScene(s, "scene1")

    main_character.mc:setPosition(VP(2000, 2000))
    s:addActor(mc)

    GAME:goToScene(s)

    local lamp = Actor("lamp")
    lamp:addAnimation("default", Image("/media/$res/lamp.png"))
    lamp:setPosition(VP(3000, 3000))
    s:addActor(lamp)

  end
end

GAME:setInitChapterCallback(initChapter)
GAME:getViewport():setup(800, 600, false)

-- UI
local ui = user_interface.create()
GAME:setUserInterface(ui)

-- Main Character
mc = main_character.create()
GAME:setMainCharacter(mc)
GAME:getViewport():setFollowing(mc)

-- Run!
GAME:runChapter(0)

