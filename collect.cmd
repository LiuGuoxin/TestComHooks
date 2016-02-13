@echo off

xcopy .\Client\Client.exe.manifest .\Debug /y

xcopy .\CppCom\CppCom.manifest .\Debug /y
xcopy .\CppCom\Debug\CppCom.tlb .\Debug /y
xcopy .\CppCom\Debug\Interop.CppCom.dll .\Debug /y

xcopy .\CsCom\bin\Debug\CsCom.dll .\Debug /y
xcopy .\CsCom\CsCom.manifest .\Debug /y
xcopy .\CsCom\bin\Debug\CsCom.tlb .\Debug /y
