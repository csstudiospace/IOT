@ECHO off
SETLOCAL
CALL :find_dp0

IF EXIST "%dp0%\/bin/bash.exe" (
  SET "_prog=%dp0%\/bin/bash.exe"
) ELSE (
  SET "_prog=/bin/bash"
  SET PATHEXT=%PATHEXT:;.JS;=;%
)

"%_prog%"  "%dp0%\node_modules\node-red\bin\node-red-pi" %*
ENDLOCAL
EXIT /b %errorlevel%
:find_dp0
SET dp0=%~dp0
EXIT /b
