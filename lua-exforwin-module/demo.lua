--本文件为使用本模块的一个小demo，注意目标编码一定要是UTF-16LE

local ICONV_MODULE = require "iconv" --使用luaiconv模块
local iconv_converter = ICONV_MODULE.new('utf-16le','utf-8') --new一个编码转换器，源编码是u8，目标编码是u16（小端）
local efw = require "efw" --引入我的exforwin模块
local str = "echo 哈哈" --当前demo的编码为UTF-8，所以这个字符串是utf-8
os.execute(str) --输出乱码
local utf16_str = iconv_converter:iconv(str) --将字符串转为UTF-16
efw.execute(utf16_str) --成功输出'哈哈'