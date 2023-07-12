local efw = require "efw" --引入我的exforwin模块
local str = 'echo 哈哈' --当前demo的编码为UTF-8，所以这个字符串是utf-8
os.execute(str) --输出乱码
efw.execute(str) --成功输出'哈哈'
