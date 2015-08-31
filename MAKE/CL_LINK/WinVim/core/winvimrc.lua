
wv.RegisterHotKey(vk.VK_8, "a")                 --注册热键 Alt+8

--方向键
wv.RegisterCommand("j", function(n)
    for i=1,n do wv.ClickKey(vk.VK_DOWN) end
end)
wv.RegisterCommand("k", function(n)
    for i=1,n do wv.ClickKey(vk.VK_UP) end
end)
wv.RegisterCommand("h", function(n)
    for i=1,n do wv.ClickKey(vk.VK_LEFT) end
end)
wv.RegisterCommand("l", function(n)
    for i=1,n do wv.ClickKey(vk.VK_RIGHT) end
end)
--复制、粘贴、撤消
wv.RegisterCommand("y", function(n)
    wv.CoDownKeys(vk.VK_LCONTROL, vk.VK_C)
end)
wv.RegisterCommand("u", function(n)
    wv.CoDownKeys(vk.VK_LCONTROL, vk.VK_Z)
end)
wv.RegisterCommand("u", function(n)
    wv.CoDownKeys(vk.VK_LCONTROL, vk.VK_V)
end)

wv.RegisterCommand(",", function(n)
    print("Waiting somekey down")
    local k = wv.WaitKeyDown()
    wv.DownKey(k)
    print("Waited keydown "..vk[k])
    wv.WaitKeyUp(k)
    wv.UpKey(k)
    print("Waited keyup "..vk[k])
end)

wv.RegisterKey(vk.VK_RETURN, function(n)
    if wv.IsKeyDown(vk.VK_LSHIFT) then
        wv.TextOut("WinVim quiting...")
        os.exit()
    end
    wv.WaitKeyUp(vk.VK_RETURN)
    wv.UninstallHook()
    wv.TextOut("Uninstall success.")
end)--回车键
--反激活&&退出
wv.RegisterCommand(string.char(13), function(n)
    if wv.IsKeyDown(vk.VK_LSHIFT) then
        wv.TextOut("WinVim quiting...")
        os.exit()
    end
    wv.UninstallHook()
    wv.TextOut("Uninstall success.")
end)--回车键

--鼠标单击
wv.RegisterCommand("I", function(n)
    wv.MouseClick("left")
end)
--鼠标右击
wv.RegisterCommand("O", function(n)
    wv.MouseClick("right")
end)
--鼠标移动
local size = 32
wv.RegisterCommand("T", function(n)
    size = size/(2*n)
    if size<=4 then size = 64 end
    wv.TextOut(size)
end)--切换移动距离
wv.RegisterCommand("J", function(n)
    wv.MouseMove(0, n*size)
end)
wv.RegisterCommand("K", function(n)
    wv.MouseMove(0, -n*size)
end)
wv.RegisterCommand("H", function(n)
    wv.MouseMove(-n*size)
end)
wv.RegisterCommand("L", function(n)
    wv.MouseMove(n*size)
end)
--PageDown,PageUp,Home,End
wv.RegisterCommand("n", function(n)
    for i=1,n do wv.ClickKey(vk.VK_NEXT) end
end)
wv.RegisterCommand("p", function(n)
    for i=1,n do wv.ClickKey(vk.VK_PRIOR) end
end)
wv.RegisterCommand("b", function(n)
    wv.ClickKey(vk.VK_HOME)
end)
wv.RegisterCommand("e", function(n)
    wv.ClickKey(vk.VK_END)
end)
--Bacnspace,Delete
wv.RegisterCommand("x", function(n)
    for i=1,n do wv.ClickKey(vk.VK_BACK) end
end)
wv.RegisterCommand("d", function(n)
    for i=1,n do wv.ClickKey(vk.VK_DELETE) end
end)
--Tab
wv.RegisterCommand("\t", function(n)
    for i=1,n do wv.ClickKey(vk.VK_TAB) end
end)

