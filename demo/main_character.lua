
module('main_character', package.seeall)

function create()
  local mc_walk = DirectionAnimation(2, -0.5*math.pi)
  mc_walk:setAnimation(0, ImageSprite("/media/$res/actors/luise/right/", 150))
  mc_walk:setAnimation(1, ImageSprite("/media/$res/actors/luise/left/", 150))

  local mc_stand = DirectionAnimation(2, -0.5*math.pi)
  mc_stand:setAnimation(0, Image("/media/$res/actors/luise/right/right01.png"))
  mc_stand:setAnimation(1, Image("/media/$res/actors/luise/left/left01.png"))

  local mc = Actor("Luise")
  mc:addAnimation("default", mc_stand)
  mc:addAnimation("walk", mc_walk)

  return mc
end

