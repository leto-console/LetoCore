@echo off
echo [LetoCore] Собираем проект...
setlocal enabledelayedexpansion
cd /d "%~dp0" || exit /b !errorlevel!
call preset_setup.bat LetoCore win-debug || exit /b !errorlevel!
call preset_setup.bat LetoCore stm32f411xe-debug || exit /b !errorlevel!
endlocal
