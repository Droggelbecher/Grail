
function initChapter(n)
  print("This is initChapter("..n..")")

  if n == 0 then
    local s = Scene(Image("/media/$res/grey_fading.jpg"))
    GAME:registerScene(s, "grey_room")

    guy.guy:setPosition(VP(2000, 2000))
    s:addActor(guy.guy)

    GAME:goToScene(s)

    local coke = Actor("coke")
    coke:addAnimation("default", Image("/media/$res/coke.jpg"))
    coke:setPosition(VP(3000, 3000))
    s:addActor(coke)

    local lamp = Actor("lamp")
    lamp:addAnimation("default", Image("/media/$res/lamp.png"))
    lamp:setPosition(VP(5000, 4500))
    s:addActor(lamp)

  end
end

GAME:setInitChapterCallback(initChapter)
GAME:getViewport():setup(800, 600, false)

import('user_interface')
import('guy')

guy.create()

GAME:runChapter(0)


