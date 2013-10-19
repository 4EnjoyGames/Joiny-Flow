@echo off
set PHP_DIR=%~1
set TEXTURE_PACKER_DIR=%~2
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
echo PHP_DIR: %PHP_DIR%
echo TEXTURE_PACKER_DIR: %TEXTURE_PACKER_DIR%
echo ANT_DIR: %ANT_DIR%
echo ANDROID_SDK_DIR: %ANDROID_SDK_DIR%
echo CYGWIN_DIR: %CYGWIN_DIR%
echo.
echo BUILD_PROJECT: %BUILD_PROJECT%
echo BUILD_MODE: %BUILD_MODE%
echo.
echo PROJECT_DIR: %ANDROID_PROJ%
echo.
echo.

echo ===============================
echo Building resources
echo ===============================

set CHMOD="%CYGWIN_DIR%\chmod.exe"


cd %ANDROID_PROJ%
cd ..\ResourceCompiler

call "%PHP_DIR%" ResourceCompiler.php "texture_packer=%TEXTURE_PACKER_DIR%;platform=android;project=%BUILD_PROJECT%;source=../Resources_storage;destination=../Resources;language=en,ru"

echo Texture build finished
cd %ANDROID_PROJ%
cd ..\Resources
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
cd %ANDROID_PROJ%
call "%ANT_DIR%\ant.bat" %BUILD_MODE% install -Dsdk.dir="%ANDROID_SDK_DIR%"
echo Build finished