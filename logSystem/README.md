# logger
log system

---
v2.0 增加缓存

[reference](https://github.com/LeechanX/Ring-Log)

其它的已经写过了，这边就不再搬运过来了，毕竟不是自己写的，后面有时间再自己写一遍。

---
v1.1 实现多线程写日志

1. 修改符合最终格式，日志等级有四个（INFO,WARNING,ERROR,FATAL）
2. setLevel设置输出日志的最低level，小于该level的日志忽略
3. setPath设置日志输出目录，当目录不存在时不会自动创建，而是直接返回false

---
v1.0 实现多线程写日志

最终日志格式：

[INFO 2020-04-17 12:12:30.618237 123 1403 main.cpp:main:14] This is a sample.
[Level   date      time         pid  tid  file:func:line]   msg

当前版本实现：

2020-04-17 12:37:53.0 8004 7 ..\logger\main.cpp:thread_func:14]hello


