-- vim: ft=lua
require "wx"

local frame = wx.wxFrame(wx.NULL, wx.wxID_ANY, "My first wxLua wxFrame", 
                                    wx.wxDefaultPosition, wx.wxDefaultSize)

frame:Show(true) 
wx.wxGetApp():MainLoop()
