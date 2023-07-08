@ECHO OFF

SETLOCAL ENABLEEXTENSIONS

SET MODE=
IF "%1"=="" (GOTO :DEFAULT) ELSE (GOTO :PAREM)


:DEFAULT
GOTO :FINAL


:PAREM
IF "%1"=="-r" (
    SET MODE=Release\
)
IF "%1"=="-d" (
    SET MODE=Debug\
)
GOTO :FINAL


:RUN_TEST <testFile>
    @REM Setlocal EnableDelayedExpansion
    IF NOT EXIST %~1 (
        ECHO "[Auto Test Warning] %~1 doesn't exist, skipping"
        ECHO ""
        EXIT /b
    )
    PUSHD %~dp1
    CALL %~1
    IF NOT %errorlevel% == 0 (
        ECHO "Failed: %errorlevel%"
        GOTO :FAILED
    )
    POPD
    EXIT /b


:FINAL
CALL :RUN_TEST %~dp0BackendTests\%MODE%BackendFontTest_Auto.exe
CALL :RUN_TEST %~dp0BackendTests\%MODE%BackendImageTest_Auto.exe
CALL :RUN_TEST %~dp0BackendTests\%MODE%BackendMainWindowTest_Auto.exe
CALL :RUN_TEST %~dp0BackendTests\%MODE%DynamicAtlasTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%BackgroundColorChangedEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%BaseEventCallbackTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildAddedEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildPositionChangedEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%ChildRemovedEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%MinMaxSizeChangedEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%OnFontChangeEventTest_Auto.exe
CALL :RUN_TEST %~dp0EventCallbackTests\%MODE%OnObjectDestroyEventTest_Auto.exe
CALL :RUN_TEST %~dp0GUIObjectTests\%MODE%TextTest_Auto.exe
CALL :RUN_TEST %~dp0GUIObjectTests\%MODE%WidgetTest_Auto.exe
CALL :RUN_TEST %~dp0GUIObjectTests\%MODE%WindowTest_Auto.exe

EXIT 0

:FAILED
EXIT 1