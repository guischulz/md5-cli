@echo off
setlocal

rem Visual Studio 2010 C++ build tools
call "%ProgramFiles(x86)%\Microsoft Visual Studio 10.0\VC\vcvarsall.bat" amd64

rem Microsoft Windows SDK
call "%ProgramFiles%\Microsoft SDKs\Windows\v7.1\Bin\SetEnv.Cmd" /x64 /Release

set CCFLAGS=/c /nologo /MD /O2 /W3 /D "NDEBUG" -DWIN32 -D_CRT_SECURE_NO_WARNINGS
cl %CCFLAGS% extern/postgres/md5.c md5cli.c
link md5.obj md5cli.obj msvcrt.lib kernel32.lib

if exist md5.exe (
  if not exist %~dp0x64\Release md %~dp0x64\Release
  copy /y md5.exe %~dp0x64\Release\md5.exe
)
if exist md5.exe del md5.exe
if exist md5.obj del md5.obj
if exist md5cli.obj del md5cli.obj