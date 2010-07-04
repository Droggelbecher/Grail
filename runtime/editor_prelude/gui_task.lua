
require("wx")

-- module('gui_task', package.seeall)

class 'GUITask' (Task)

function GUITask:__init()
	print("aaaaaaa")
	Task.__init(self, TASK_ENDLESS)
	print("aaaaaaa-----")

	self.frame = wx.wxFrame(
		wx.NULL, wx.wxID_ANY, "wx Test",
		wx.wxDefaultPosition, wx.wxSize(450, 450),
		wx.wxDEFAULT_FRAME_STYLE
		)
	self.frame:Show(true)

	self.app = wx.wxGetApp()
	print("aaaaaaax")
end

function GUITask:eachFrame(ticks)
	--print("tach")
	if self.app:Pending() then
		self.app:Dispatch()
	end
end

