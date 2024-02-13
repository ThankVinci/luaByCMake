set(MODULE_NAME demo) #模块名称
set(MODULE_NAME_WITH_MODULE ${MODULE_NAME}_module)

project(${MODULE_NAME})
# 设置模块依赖目录
set(MODULE_LINK_PATH ${CMAKE_CURRENT_SOURCE_DIR}/lib/ )
include("../subcommon.cmake")
include("./cmake/module.cmake") #需要更改
add_sub_module()