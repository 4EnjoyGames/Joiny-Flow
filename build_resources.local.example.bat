@echo off

rem =========CONFIG=============
set PHP_DIR="E:\usr\bin\php.exe"
set TEXTURE_PACKER_DIR="C:\Program Files (x86)\CodeAndWeb\TexturePacker\bin\TexturePacker.exe"

set BUILD_PROJECT=%1
set BUILD_PLATFORM=%2
rem =========CONFIG end=============

set ANDROID_PROJ=%~dp0
"%ANDROID_PROJ%\do_build_resources.bat" %PHP_DIR% %TEXTURE_PACKER_DIR% %BUILD_PROJECT% %BUILD_PLATFORM%
