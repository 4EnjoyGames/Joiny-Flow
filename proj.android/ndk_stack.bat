set ANDROID_PROJ=%~dp0
"C:\Program Files (x86)\Android\android-sdk\platform-tools\adb" logcat | "C:/android-ndk-r8c/ndk-stack" -sym "%ANDROID_PROJ%\obj\local\armeabi"