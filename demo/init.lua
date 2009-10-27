
Game:getViewport():setup(800, 600)

Game:registerChapter(0, function(n)
  print("Welcome to chapter 0")

  local image = createImage("/home/henning/coke.png")

  local s = createScene()
  s:setBackground(image)

  Game:goToScene(s)

end)

Game:runChapter(0)

