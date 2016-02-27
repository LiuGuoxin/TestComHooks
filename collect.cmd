@echo off

xcopy .\Client\Client.exe.manifest .\Debug /y

xcopy .\CppCom\CppCom.manifest .\Debug /y
xcopy .\CppCom\CppCom.tlb .\Debug /y
xcopy .\CppCom\Interop.CppCom.dll .\Debug /y

xcopy .\CsCom\bin\Debug\CsCom.dll .\Debug /y
xcopy .\CsCom\CsCom.manifest .\Debug /y
xcopy .\CsCom\bin\Debug\CsCom.tlb .\Debug /y
