-- vim: set noexpandtab:

require 'user_interface'
require 'main_character'

function initChapter(n)
	print("This is initChapter("..n..")")
	
	if n == 0 then
		local s = Scene(VP(8000, 3000))
		GAME:registerScene(s, "scene1")
		
		s:addBackground(Image("/media/$res/scene1/static_background.png"), VP(0, 0), 0.0, 1.0)
		s:addBackground(Image("/media/$res/scene1/tower.png"), VP(1000, 700), 0.1, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains01.png"), VP(0, 1500), 0.25, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains02.png"), VP(0, 1900), 0.50, 1.0)
		s:addBackground(Image("/media/$res/scene1/mountains03.png"), VP(0, 2050), 0.75, 1.0)
		s:addBackground(Image("/media/$res/scene1/walkable_area.png"), VP(0, 2500), 1.0, 1.0)
		s:addForeground(Image("/media/$res/scene1/foreground.png"), VP(700, 590), 1.2, 1.0)
		
		local scarecrow = Actor("Scarecrow")
		scarecrow:addAnimation("default", Image("/media/$res/scarecrow.png"))
		scarecrow:setPosition(VP(6000, 2700))
		s:addActor(scarecrow)
		
		local pumpkin = Actor("Pumpkin")
		pumpkin:addAnimation("default", Image("/media/$res/pumpkin.png"))
		pumpkin:setPosition(VP(5950, 2700))
		pumpkin:setYOffset(-700)
		s:addActor(pumpkin)
		
		main_character.mc:setPosition(VP(250, 2700))
		s:addActor(mc)
		
		local Audio = Audio()
		bgmusic = Audio:prepSound("sounds/20moment.mp3",2,100)
		bgmusic:start()

		GAME:goToScene(s)

		--- WaitTask test/example ---
		print("--- Before blocking timer (3s)")
		WaitTask(3000):block()
		print("--- After blocking timer")
		print("--- Before nonblocking timer (3s)")
		WaitTask(3000):start()
		print("--- After nonblocking timer")
		------------------------------
		
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

