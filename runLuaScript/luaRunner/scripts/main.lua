--用于进行lua调用的demo脚本
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

local retv1 = "返回第一个接口\0"
local retv2 = "返回第二个接口\0"

local res = {
	retc = 2,
	retv = {retv1,retv2},
	rets = {#retv1,#retv2}

}
return res
