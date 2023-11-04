@echo off
cls

@rem Modify the path for your vcvars setup
set "__localVCVarsPath=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
set __outputMessage=

@rem Setup vcvars environment
if not defined VSCMD_ARG_TGT_ARCH (
    if exist "%__localVCVarsPath%" (
        call "%__localVCVarsPath%"
    ) else (
        set "__outputMessage=Unable to locate vcvars path at: %__localVCVarsPath%"
        goto end
    )
)

setlocal ENABLEDELAYEDEXPANSION

    if "%~1"=="-d" (
        set "debugMode=1"
    ) else (
        set "debugMode=0"
    )

    set "exeName=nes.exe"
    set "compilerFlags=-W4 -WX -nologo -std:c++20 -Zc:strictStrings -GR- -favor:INTEL64 -cgthreads8 -MP"
    set "ignoreWarnings=-wd4100 -wd4101 -wd4189 -wd4806"
    set "linkerFlags=-INCREMENTAL:NO"

    if %debugMode%==0 (
        set "compilerFlags=%compilerFlags% -O2"
    ) else (
        set "compilerFlags=%compilerFlags% -Od -FC -Zi -MTd"
    )

    if exist *.exe del *.exe
    if exist *.pdb del *.pdb
    if not exist build\NUL mkdir build

    pushd build

        @rem Compilation
        cl %compilerFlags% %ignoreWarnings% ..\src\main.cpp -Fe..\%exeName% -link %linkerFlags%

        if %ERRORLEVEL%==0 (
        set "__outputMessage=Build successful"
        ) else (
        set "__outputMessage=Build failed"  
        )

    popd

    @rem Cleanup
    if %debugMode%==0 (
        rmdir /s /q build
    )

    echo %__outputMessage%

endlocal

:end
set __localVCVarsPath=
set __outputMessage=