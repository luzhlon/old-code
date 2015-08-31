-- vim: ft=lua
require "vk"
require "winvim_core"

wv = winvim_core

--[[                            Test Mouse function
--wv.MouseDown("right")
wv.MouseDown("left")
--wv.MouseUp("right")
wv.MouseUp("left")
while true do
    wv.Sleep(1000)
    print(wv.GetCursorPos())
end
ex.sleep(2)
wv.MouseMove(0,0)
--]]

--[[                            Test Keyboard function
wv.DownKey(vk.VK_MENU)
wv.DownKey(vk.VK_F4)
wv.UpKey(vk.VK_F4)
wv.UpKey(vk.VK_MENU)
--]]
while true do 
    wv.Sleep(1)
    print(wv.IsKeyDown(vk.VK_LCONTROL), wv.IsKeyToggled(vk.VK_LCONTROL))
end
while true do
    for i = vk.VK_A,vk.VK_Z do
        if wv.IsKeyDown(i) then print(vk[i]) end
    end
    print ""
    print ""
    wv.Sleep(1)
end


--[[                            Test Window function
topwins = wv.GetTopWindows()
for k,v in pairs(topwins) do
    print(v, wv.GetWindowName(v))
end
---]]
--[[
for k,v in pairs(wv.GetChildWindows(0)) do
    print(v, wv.GetWindowName(v))
end
--]]
--
--
