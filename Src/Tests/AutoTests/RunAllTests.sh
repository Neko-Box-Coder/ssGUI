#!/bin/bash

set -e

function runTest()
{
    if [ -f $1 ]; then
        chmod +x $1
        $1
    else
        echo "[Auto Test Warning] $1 doesn't exist, skipping"
        echo ""
    fi
}

runTest ./BackendTests/BackendFontTest_Auto
runTest ./BackendTests/BackendImageTest_Auto
runTest ./BackendTests/BackendMainWindowTest_Auto
runTest ./BackendTests/DynamicAtlasTest_Auto
runTest ./EventCallbackTests/BackgroundColorChangedEventTest_Auto
runTest ./EventCallbackTests/BaseEventCallbackTest_Auto
runTest ./EventCallbackTests/ChildAddedEventTest_Auto
runTest ./EventCallbackTests/ChildPositionChangedEventTest_Auto
runTest ./EventCallbackTests/ChildRemovedEventTest_Auto
runTest ./EventCallbackTests/MinMaxSizeChangedEventTest_Auto
runTest ./EventCallbackTests/OnFontChangeEventTest_Auto
runTest ./EventCallbackTests/OnObjectDestroyEventTest_Auto
runTest ./GUIObjectTests/TextTest_Auto
runTest ./GUIObjectTests/WidgetTest_Auto
runTest ./GUIObjectTests/WindowTest_Auto