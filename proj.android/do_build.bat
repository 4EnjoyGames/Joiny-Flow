@echo off
set ANT_DIR=%~3
set ANDROID_SDK_DIR=%~4
set CYGWIN_DIR=%~5

set BUILD_PROJECT=%~6
set BUILD_MODE=%~7

IF [%BUILD_MODE%] == [] set BUILD_MODE=debug
set ANDROID_PROJ=%~dp0

echo ===============================
echo Building for Android
echo ===============================
echo ANT_DIR: %ANT_DIR%
echo ANDROID_SDK_DIR: %ANDROID_SDK_DIR%
echo CYGWIN_DIR: %CYGWIN_DIR%
echo.
echo BUILD_PROJECT: %BUILD_PROJECT%
echo BUILD_MODE: %BUILD_MODE%
echo.
echo PROJECT_DIR: %ANDROID_PROJ%
echo.

echo ===============================
echo Building resources
echo ===============================

set CHMOD="%CYGWIN_DIR%\chmod.exe"


cd %ANDROID_PROJ%

call "..\build_resources.local.bat" %BUILD_PROJECT% android
set ANDROID_PROJ=%~dp0

cd %ANDROID_PROJ%
echo Texture chmod start
cd ..
cd "Resources"

%CHMOD% 0777 -R * 
echo Texture chmoded

echo.
echo.
echo ===============================
echo Build C++ part
echo ===============================
cd %ANDROID_PROJ%
call "%CYGWIN_DIR%\bash.exe" build_native.sh
echo Build finish
%CHMOD% 0777 -R * 
echo Chmod Done

echo.
echo.
echo ===============================
echo Build Java part
echo ===============================
pwd
echo "%ANT_DIR%\ant.bat"
call "%ANT_DIR%\ant.bat" %BUILD_MODE% install -Dsdk.dir="%ANDROID_SDK_DIR%"
echo Build finished