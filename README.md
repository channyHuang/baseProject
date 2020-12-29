# baseProject
abstract some projects to a base project so that can use it directory in the future when needed

## mycommon
some common funcs

## DllBase
base project of dll. Usually when develop a device dependent module. For example, if I want to use a camera sdk, I can develop a dll to envelope this sdk interface to a standard interface, then it can be used to different projects if and only if they are all following the standard interface.

## DllLoadBase
base project of loading dll. Just for test with the above.

## ReadIniBase
base project of reading ini files, and unit test in qt. 

## readXmlLabels
read labels of a xml file

## socketClient/socketServer
network socket communication
