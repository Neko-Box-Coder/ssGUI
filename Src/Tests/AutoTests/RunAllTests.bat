@echo off

setlocal enableextensions

set MODE=
if "%1"=="" (goto :DEFAULT) else (goto :PAREM)


:DEFAULT
goto :FINAL


:PAREM
if "%1"=="-r" (
    set MODE=Release\
)
if "%1"=="-d" (
    set MODE=Debug\
)
goto :FINAL


:RUN_TEST <testFile>
    @REM Setlocal EnableDelayedExpansion
    pushd %~dp1
    call %~1
    if NOT %errorlevel% == 0 (
        echo "Failed: %errorlevel%"
        goto :FAILED
    )
    popd
    exit /b


:FINAL
@REM call :RUN_TEST %~dp0BackendTests\%MODE%BackendFontTest_Auto.exe
@REM call :RUN_TEST %~dp0BackendTests\%MODE%BackendImageTest_Auto.exe
@REM call :RUN_TEST %~dp0BackendTests\%MODE%BackendMainWindowTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%BackgroundColorChangedEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%BaseEventCallbackTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildAddedEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildPositionChangedEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildRemovedEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%MinMaxSizeChangedEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%OnFontChangeEventTest_Auto.exe
@REM call :RUN_TEST %~dp0EventCallbackTests\%MODE%OnObjectDestroyEventTest_Auto.exe
call :RUN_TEST %~dp0GUIObjectTests\%MODE%TextTest_Auto.exe
@REM call :RUN_TEST %~dp0GUIObjectTests\%MODE%WidgetTest_Auto.exe
@REM call :RUN_TEST %~dp0GUIObjectTests\%MODE%WindowTest_Auto.exe

exit 0

:FAILED
exit 1