--[[
local cjson_module = require "cjson"
local cjson_codec = cjson_module.new()

local jsonfile = io.open(".\\entry.json","r")
local jsonstr = jsonfile:read("*a")
local jsonobj = cjson_codec.decode(jsonstr)
print(jsonobj['title'])
--]]

print(arg[1])
print(arg[2])

res = {
	retc = 2,
	retv = {"返回的第一个结果","返回的第二个结果"}
}
return res
