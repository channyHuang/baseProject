# baseProject
abstract some projects to a base project so that can use it directory in the future when needed

使用Qt写一些功能模板

## common / mycommon
some common funcs

## logSystem
async log system

异步日志系统

## demoDll
export dll. base project of dll. Usually when develop a device dependent module. For example, if I want to use a camera sdk, I can develop a dll to envelope this sdk interface to a standard interface, then it can be used to different projects if and only if they are all following the standard interface.

生成导出Dll。

## DllLoadBase
base project of loading dll. Just for test with the above.

使用QLibrary加载dll动态库，该动态库的编译方式必须与加载项目的构建方式一致，如都使用wingw编译或都使用vs构建。

## encodeOidPdf
识别Oid文字

## performance
统计性能

# Files Reading/Writing 文件读写
## ReadIniBase
base project of reading ini files, and unit test in qt. 

使用QSettings读写ini配置文件

## readXmlLabels
read labels of a xml file

使用QRegExp正则表达式读取xml文件

# Commulation 通信
## httpTest
使用mongoose进行http通信

## socketClient/socketServer
network socket communication

使用QTCPServer/QTCPSocket进行TCP通信

## qtHttpSendImg
使用QNetworkAccessManager/QNetworkRequest/QHttpMultiPart/QNetworkReply进行http通信