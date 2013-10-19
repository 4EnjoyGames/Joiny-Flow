@echo off

rem =========CONFIG=============
set PHP_DIR="E:\WebServer2013\usr\bin\php.exe"
set TEXTURE_PACKER_DIR="C:\Program Files (x86)\CodeAndWeb\TexturePacker\bin\TexturePacker.exe"
set ANT_DIR="C:\ant\bin"
set ANDROID_SDK_DIR="C:\Program Files (x86)\Android\android-sdk"
set CYGWIN_DIR="C:\cygwin\bin"

set BUILD_PROJECT="joiny"
set BUILD_MODE=debug
rem =========CONFIG end=============

call do_build.bat %PHP_DIR% %TEXTURE_PACKER_DIR% %ANT_DIR% %ANDROID_SDK_DIR% %CYGWIN_DIR% %BUILD_PROJECT% %BUILD_MODE%

