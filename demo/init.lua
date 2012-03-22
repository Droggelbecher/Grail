-- vim: set noexpandtab:

require 'user_interface'
require 'main_character'
require 'scene1'

function initChapter(n)
	print("This is initChapter("..n..")")
	
	if n == 0 then
		local s = Scene1(VP(8000, 3000))
		GAME:registerScene(s, "scene1")
		
		s:addBackground(Image("/media/$res/scene1/static_background.png"), VP(0, 0), 0.0, 1.0)
		s:addBackground(Image("/media/$res/scene1/tower.png"), VP(1000, 700), 0.1, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains01.png"), VP(0, 1500), 0.25, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains02.png"), VP(0, 1900), 0.50, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains03.png"), VP(0, 2050), 0.75, 1.0)
		s:addBackground(Image("/media/$res/scene1/walkable_area.png"), VP(0, 2500), 1.0, 1.0)
		s:addForeground(Image("/media/$res/scene1/foreground.png"), VP(700, 590), 1.2, 1.0)
		
		s.scarecrow = Actor("Scarecrow")
		s.scarecrow:addAnimation("default", Image("/media/$res/scarecrow.png"))
		s.scarecrow:setPosition(VP(6000, 2700))
		s:addActor(s.scarecrow)
		
		s.pumpkin1 = Actor("Pumpkin One")
		s.pumpkin1:addAnimation("default", Image("/media/$res/pumpkin.png"))
		s.pumpkin1:setPosition(VP(5950, 2700))
		s.pumpkin1:setYOffset(-700)
		s:addActor(s.pumpkin1)

		s.pumpkin2 = Actor("Pumpkin Two")
		s.pumpkin2:addAnimation("default", Image("/media/$res/pumpkin.png"))
		s.pumpkin2:setPosition(VP(6200, 3500))
		s.pumpkin2:setYOffset(-700)
		s:addActor(s.pumpkin2)

		s.pumpkin3 = Actor("Pumpkin Three")
		s.pumpkin3:addAnimation("default", Image("/media/$res/pumpkin.png"))
		s.pumpkin3:setPosition(VP(6700, 3500))
		s.pumpkin3:setYOffset(-700)
		s:addActor(s.pumpkin3)
		
		local p = Poly(
			VP(0, 2000), VP(8000, 2000), VP(8000, 3000), VP(0, 3000)
		)
		local g = s:getGround()
		g:addPolygon(p)
		--g:generateMap()
		
		main_character.mc:setPosition(VP(5000, 2700))
		s:addActor(mc)
		
		local Audio = Audio()
		bgmusic = Audio:prepareSound("sounds/20moment.mp3",2)
		bgmusic:start()

		GAME:goToScene(s)

		--[[
		--- WaitTask test/example ---
		print("--- Before blocking timer (3s)")
		WaitTask(3000):block()
		print("--- After blocking timer")
		print("--- Before nonblocking timer (3s)")
		WaitTask(3000):start()
		print("--- After nonblocking timer")
		------------------------------
		--]]

		
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
print("run ch 0")
GAME:runChapter(0)

