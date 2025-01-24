@echo off

call build_debug.bat
IF %ERRORLEVEL% NEQ 0 EXIT 1
call main.exe