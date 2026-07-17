@echo off
set "CLANG_PATH=C:\Users\AONAK YADAV\Downloads\Windhawk\Compiler\bin\clang++.exe"

if not exist "%CLANG_PATH%" (
    echo Clang compiler not found at: %CLANG_PATH%
    echo Checking system PATH for clang++...
    where clang++ >nul 2>nul
    if %ERRORLEVEL% equ 0 (
        set "CLANG_PATH=clang++"
    ) else (
        echo Error: Could not find clang++ compiler. Please install Windhawk or Visual Studio / Clang.
        pause
        exit /b 1
    )
)

echo Found compiler at: %CLANG_PATH%
echo Compiling window-dynamic-island.wh.cpp as standalone app...

"%CLANG_PATH%" --target=x86_64-w64-windows-gnu -std=c++23 -municode -static -DSTANDALONE_APP -O2 -mwindows src\window-dynamic-island.wh.cpp -o DynamicIsland.exe -lole32 -loleaut32 -lshcore -ld2d1 -ldwrite -ldwmapi -lgdi32 -luser32 -lshell32 -lruntimeobject -lwindowscodecs -lavrt -lsetupapi -lwinhttp -lpdh

if %ERRORLEVEL% equ 0 (
    echo Compilation successful! Created DynamicIsland.exe
) else (
    echo Compilation failed with error code: %ERRORLEVEL%
    pause
    exit /b %ERRORLEVEL%
)
