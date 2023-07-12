# lua-exforwin-module



## 项目作用以及编码需知

模块的作用旨在于为Windows的UTF-16的的操作进行扩展，比如说文件io操作和执行command的操作，lua原生的操作在Windows上只支持ANSI，在简中环境上就是GBK编码，限制是很大的。因此了解了这部分需求之后，为Windows上的lua提供UTF-16的支持，方法也很简单，不过是使用标准C库给Windows提供的`_wsystem`函数。
