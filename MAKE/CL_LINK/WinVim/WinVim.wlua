-- vim: ft=lua
require "vk"
require "winvim_core"
-------------------------------------------------------------------------
----------------------------------CORE-----------------------------------
-------------------------------------------------------------------------
wv = winvim_core

wv._ON_KEY_DOWN = {}
wv._ON_KEY_UP = {}
wv._ON_CHAR = {}

wv._CurRotine = nil     --当前的协程
wv._WaitList = {}       --等待列表

function wv.RegisterKey(vk, func, ifdown)
    assert(type(vk)=="number", "[ASSERT] The first argument must be a number")
    assert(type(func)=="function", "[ASSERT] The second argument must be a function")
    local down = ifdown or true
    if down then
        wv._ON_KEY_DOWN[vk] = func
    else
        wv._ON_KEY_UP[vk] = func
    end
    return true
end

function wv.RegisterCommand(ch, func)
    assert(type(ch)=="string", "[ASSERT] The first argument must be a string")
    assert(type(func)=="function", "[ASSERT] The second argument must be a function")
    wv._ON_CHAR[ch] = func
    return true
end

local _Count
function wv.DealNumber(ch)
    local c,b1,b9 = ch:byte(),string.byte("1"),string.byte("9")
    if _Count then b1 = b1-1 end   --已经开始了解析数字
    if c>=b1 and c<=b9 then
       if _Count then
           _Count = _Count..ch
       else
           _Count = ch
       end
       wv.TextOut(_Count)
       return true
    end
    _Count = nil
    return false
end
function wv.GetNumber()
    return tonumber(_Count) or 1
end

--Wait系列函数只能从回调函数里面调用
function wv.WaitChar(ch)
    table.insert(wv._WaitList, {_type="char", _key=ch, _co=wv._CurRotine})
    return coroutine.yield()
end
function wv.WaitKeyDown(key)
    table.insert(wv._WaitList, {_type="keydown", _key=key, _co=wv._CurRotine})
    return coroutine.yield()
end
function wv.WaitKeyUp(key)
    table.insert(wv._WaitList, {_type="keyup", _key=key, _co=wv._CurRotine})
    return coroutine.yield()
end

function wv.CheckWait(key, Type)
    local wait
    local i = 1
    wait = wv._WaitList[i]
    while wait do
        if wait._type == Type then       --判断等待类型是否相同
            local k = wait._key
            if (not k) or k==key then   --判断期望的键是否相同
                table.remove(wv._WaitList, i)--从等待列表中移除
                coroutine.resume(wait._co, key)--唤醒此协程
                return true
            else
                print("wait return false")
                return false
            end
        end
        i = i + 1
        wait = wv._WaitList[i]
    end
    return false
end

_CALLBACK = function(vkCode, bDown)         --沟通WinVim与其core的桥梁,返回true 代表已经处理了
    --wv.TextOut(vk[vkCode])
    local ch = wv.KeyToChar(vkCode)
    local fun, ck
    local wait_type, deal_table

    if bDown then                           
        wait_type = "keydown"
        deal_table = wv._ON_KEY_DOWN
    else
        wait_type = "keyup"
        deal_table = wv._ON_KEY_UP
    end
    --print("callback "..vk[vkCode].." \t"..wait_type)
                                            --按键事件优先于字符事件处理
    if wv.CheckWait(vkCode, wait_type) then print("Wait return "..wait_type) return true end
    fun = deal_table[vkCode]
    if fun then
        wv._CurRotine = coroutine.create(fun)
        co = wv._CurRotine
        local _, ret = coroutine.resume(co, vkCode) 
        return not ret
    end
                                            --处理字符事件
    if ch ~= "" then
        if not bDown then return true end                  --只有在KeyDown时处理Char
        if ch:byte()<31 then return false end              --控制字符
        if wv.CheckWait(ch, "char") then return true end
        if wv.DealNumber(ch) then return true end
        fun = wv._ON_CHAR[ch]
        if fun then
            wv._CurRotine = coroutine.create(fun)
            co = wv._CurRotine
            coroutine.resume(co, wv.GetNumber()) 
        end
        return true
    else
        return false
    end

    return true
end

----------------------------------------------------------------------------
--鼠标函数扩展
function wv.MouseMoveTo(x,y)
    local y = y or 0
    local cx,cy = wv.GetCursorPos()
    wv.MouseMove(x-cx, y-cy)
end
function wv.MouseClick(key)--单击某个鼠标键
    wv.MouseDown(key)
    wv.MouseUp(key)
end
----------------------------------------------------------------------------
--键盘函数扩展
function wv.ClickKey(vkCode)--按下某键
    wv.DownKey(vkCode)
    wv.UpKey(vkCode)
end
function wv.CoDownKeys(...)--同时按下许多键
    local count = #arg
    if count == 0 then return end
    for i=1,count do
        wv.DownKey(arg[i])
    end
    for i=count,1,-1 do
        wv.UpKey(arg[i])
    end
end

----------------------------------------------------------------------------

--加载自定义设置
require "winvimrc"
--frame:Show(true) 
--进行消息循环，否则钩子捕获不到按键消息
wv.MsgLoop()
