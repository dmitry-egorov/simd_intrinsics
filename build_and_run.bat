@echo off

call build.bat
IF %ERRORLEVEL% NEQ 0 EXIT 1
call main.exe