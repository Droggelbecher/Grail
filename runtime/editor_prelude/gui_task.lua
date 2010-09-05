
local print_old = print
require("wx")
local print = print_old

-- module('gui_task', package.seeall)

class 'GUITask' (Task)

function GUITask:__init()
	Task.__init(self, TASK_ENDLESS)

	local xml = wx.wxXmlResource()
	xml:InitAllHandlers()
	local r = xml:Load('runtime/editor_prelude/gui.xrc')
	local dialog = wx.wxDialog()
	xml:LoadDialog(dialog, wx.NULL, "Grail")
	dialog:Show(true)

	self.app = wx.wxGetApp()
	self:eachFrame(0)
end

function GUITask:eachFrame(ticks)
	wx.wxYield()
	
	while self.app:Pending() do
		self.app:Dispatch()
	end
end

