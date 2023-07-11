--哈喽，看我
--本脚本为试做型脚本，默认生成MinGW的Makefile文件，经测试ninja也可以正常使用，但是其他的生成器就没有测试了

local Func_GetParamFormat = function(KEY_TBL,PARAM_TBL) --参数格式化返回字符串函数
	local params = ''
	for i,v in ipairs(KEY_TBL) do
		if(PARAM_TBL[v]['EN'] == true) then
			local param = PARAM_TBL[v]['KEY']:format(PARAM_TBL[v]['VAL'])
			params = params .. param
		end
	end
	return params
end

local generators = {
	'Borland Makefiles'
	,'MSYS Makefiles'
	,'MinGW Makefiles'
	,'NMake Makefiles'
	,'NMake Makefiles JOM'
	,'Unix Makefiles'
	,'Watcom WMake' --以上为Makefile的生成器
	,'Ninja'
	,'Ninja Multi-Config' --以上为Ninja生成器
	,'Visual Studio 17 2022'
	,'Visual Studio 16 2019' --以上为VS生成器（注：NMake和VS生成器需要安装VS才有）
}

local platforms = {
	'x64'
	,'Win32'
	,'ARM'
	,'ARM64'
} --平台选项，仅在VS生成器中有效，因此选用其他生成器时，请将下面的构建参数将Arch的'EN'改为false
local generator = '"' .. generators[3] .. '"'
local platform = platforms[1]
local build_dir = '"./build"' --cmake会生成在CMakeLists文件同级的目录中

------------------------------CMAKE构建参数组装-----------------------
local build_key_tbl = {'Genor','Arch','BuildDir'}
local build_params = {
	['Genor'] = { ['EN']  = true, ['KEY'] = ' -G %s', ['VAL'] = generator  }
	,['Arch'] = { ['EN'] = false, ['KEY'] = ' -A %s', ['VAL'] = platform }
	,['BuildDir'] = { ['EN'] = true, ['KEY'] = ' -B %s', ['VAL'] = build_dir }
}

local build_param = Func_GetParamFormat(build_key_tbl,build_params)
local build_command = string.format('cmake%s',build_param)
print(build_command)
os.execute(build_command)

----------------------------------------------------------------------
----------------------------------编译参数组装------------------------
local compiler = {
	'make'
	,'mingw32-make'
	,'ninja'
}
local compile_key_tbl = {'Compiler','ChangeDir','Jobs','Install'}
local compile_params = {
	['Compiler'] = { ['EN']  = true, ['KEY'] = '%s', ['VAL'] = compiler[1]  }
	,['ChangeDir'] = { ['EN']  = true, ['KEY'] = ' -C %s', ['VAL'] = build_dir  }
	,['Jobs'] = { ['EN'] = true, ['KEY'] = ' -j %s', ['VAL'] = '4' }
	,['Install'] = { ['EN'] = false, ['KEY'] = ' %s', ['VAL'] = 'install' }
}
local compile_param = Func_GetParamFormat(compile_key_tbl,compile_params)
local compile_command = string.format('%s',compile_param)
print(compile_command)
os.execute(compile_command)

----------------------------------------------------------------------
-----------------------------------安装参数组装-----------------------
compile_params['Jobs']['EN'] = false
compile_params['Install']['EN'] = true
local install_param = Func_GetParamFormat(compile_key_tbl,compile_params)
local install_command = string.format('%s',install_param)
print(install_command)
os.execute(install_command)
----------------------------------------------------------------------