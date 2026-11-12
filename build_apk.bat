@echo off
setlocal
echo =========================================================================
echo       CYBER-RELAY RTOS // ANDROID APK 1-CLICK BUILD AND EXPORT
echo =========================================================================

python scripts\build_apk.py
if errorlevel 1 (
    echo [ERROR] Build failed! Check error details above.
    pause
    exit /b 1
)

pause
