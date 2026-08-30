@echo off
echo [LetoCore] Собираем проект...
setlocal 
cd /d "%~dp0"
call preset_setup.bat LetoCore win-debug
call preset_setup.bat LetoCore stm32f411xe-debug
endlocal
