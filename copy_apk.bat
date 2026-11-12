@echo off
setlocal enabledelayedexpansion

echo =========================================================================
echo       CYBER-RELAY RTOS // APK EXPORT AND COPY UTILITY
echo =========================================================================

set "SOURCE_APK=android_app\app\build\outputs\apk\debug\app-debug.apk"
set "DEST_APK=CyberRelay-RTOS.apk"
set "DEST_DEBUG=app-debug.apk"

if not exist "%SOURCE_APK%" (
    echo [ERROR] Source APK not found at: %SOURCE_APK%
    echo Please run build_apk.bat first or build the project in Android Studio.
    pause
    exit /b 1
)

echo [INFO] Found source APK at: %SOURCE_APK%
echo [COPY] Copying APK to main outside folder...

copy /Y "%SOURCE_APK%" "%DEST_APK%" >nul
copy /Y "%SOURCE_APK%" "%DEST_DEBUG%" >nul

if exist "%DEST_APK%" (
    echo [SUCCESS] Successfully copied APK to main root directory:
    echo   - %CD%\%DEST_APK%
    echo   - %CD%\%DEST_DEBUG%
    echo.
    echo You can now transfer CyberRelay-RTOS.apk to your Android phone and install it!
) else (
    echo [ERROR] Failed to copy APK.
)

echo =========================================================================
pause
