local f = io.open("vkcode.txt", "r")
local str = ""
repeat
    str = f:read("*l")
    if not str then break end
    local vks, vkn = str:match("(VK_[^%s]+)%s+(%x%x)")
    if vks then print("vk[".."0x"..vkn.."] = ".."\""..vks.."\"") end
until str == nil
