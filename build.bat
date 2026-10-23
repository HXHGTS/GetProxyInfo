@echo off
setlocal
rem Build GetProxyInfo.exe with MSVC.
rem Run this from "x64 Native Tools Command Prompt for VS" or after vcvars64.bat.

where cl >nul 2>nul
if errorlevel 1 (
    echo [ERROR] cl.exe not found in PATH.
    echo         Open "x64 Native Tools Command Prompt for VS" and run build.bat again,
    echo         or run vcvars64.bat first.
    exit /b 1
)

cl /nologo /utf-8 /W4 /O2 getproxyinfo.c main.c ^
   /link /MANIFEST:EMBED /MANIFESTINPUT:app.manifest /SUBSYSTEM:CONSOLE

if errorlevel 1 (
    echo [ERROR] build failed.
    exit /b 1
)

echo [OK] GetProxyInfo.exe built.
