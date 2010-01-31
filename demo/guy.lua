
function create()
  local guy_ud = Image("/media/$res/lineboy_default.png")
  local guy_r = StripeSprite("/media/$res/lineboy_r.png", 9)
  local guy_l = StripeSprite("/media/$res/lineboy_l.png", 9)
  local guy_default = DirectionAnimation(4)

  guy_default:setAnimation(0, guy_ud)
  guy_default:setAnimation(1, guy_r)
  guy_default:setAnimation(2, guy_ud)
  guy_default:setAnimation(3, guy_l)

  local _guy = Actor("Guy")
  _guy:addAnimation("default", guy_default)

  GAME:setMainCharacter(_guy)
  GAME:getViewport():setFollowing(_guy)

  guy.guy = _guy
  return _guy
end

