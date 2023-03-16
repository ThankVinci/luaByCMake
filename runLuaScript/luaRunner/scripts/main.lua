--本模块仅用于加载json数据中对应key的value值
local retable = { retc = 0,retv = {},rets = {} } --默认返回值

--用于往返回值列表中添加返回值的函数
local function addRetTable(...)
	local args = {...}
	retable.retc = #args
	local idx = 1
	while(idx <= #args)
	do
		retable.rets[idx] = #args[idx]
		retable.retv[idx] = args[idx]
		idx = idx + 1
	end
	
end

--如果传入的参数数量小于2的话，就得返回错误
if(#arg < 2) then
	print("Invaild Argument Counts")
	--返回两个数据，一个表示状态码，另一个表示MSG
	addRetTable("\1","Invaild Argument Counts\0")
	return retable
end

--加载模块的函数
local function load_module(module_name)
   return require(module_name) --返回模块的table或者nil
end

--加载模块的调用
--返回值load_status如果为true的话，cjson_module的值就是cjson模块的table，否则就错误的打印堆栈
local load_status,cjson_module = pcall(load_module,"cjson") 

if(load_status == true)
then
    print("load cjson success")
else
    print("load cjson failed")
	addRetTable("\1","load cjson failed\0")
	return retable
end

--print(cjson_module)
print(arg[1])

local cjson_codec = cjson_module.new() --创建json编解码器

--第一个参数是json字符串,在这里要把字符串解码为table对象
local function json_decode(codec,str)
	return codec.decode(arg[1])
end
local decode_status,jsonobj = pcall(json_decode,cjson_codec,arg[1])
--print(jsonobj)
--若不是table对象则返回错误
if(decode_status ~= true) then 
	print("Arg[1] is not a Valid JsonStr.")
	addRetTable("\1","Arg[1] is not a Valid JsonStr.\0")
	return retable
end

local key = arg[2]
--print(jsonobj)
local value = jsonobj[key]
print(value)
if(value ~=  nil) then
	addRetTable("\0",value.."\0")
	return retable
end

return retable
