@setlocal enableextensions enabledelayedexpansion
@echo off
set PHP_DIR=%~1
set TEXTURE_PACKER_DIR=%~2
set BUILD_PROJECT=%~3
set PLATFORM=%~4

set ANDROID_PROJ=%~dp0


echo PHP_DIR: %PHP_DIR%
echo TEXTURE_PACKER_DIR: %TEXTURE_PACKER_DIR%
echo.
echo BUILD_PROJECT: %BUILD_PROJECT%
echo BUILD_PLATFORM: %PLATFORM%
echo.
echo PROJECT_DIR: %ANDROID_PROJ%
echo.
echo.

echo ===============================
echo Building resources
echo ===============================

set CHMOD="%CYGWIN_DIR%\chmod.exe"


cd %ANDROID_PROJ%
pwd
cd ../../Resource-Compiler

set startdir=%~dp0

for %%A in ("%~dp0.") do set startdir=%%~nA

set params="texture_packer=%TEXTURE_PACKER_DIR%;platform=%PLATFORM%;project=%BUILD_PROJECT%;source=../projects/%startdir%/Resources_storage;destination=../projects/%startdir%/Resources;translate=en;translation_dir=../Resources_storage/translations"

echo "%PHP_DIR%" ResourceCompiler.php %params%
"%PHP_DIR%" ResourceCompiler.php %params%

echo Texture build finished