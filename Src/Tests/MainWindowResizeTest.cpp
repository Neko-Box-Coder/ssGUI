#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(450, 125));
    // mainWindow.GetBackendWindowInterface()->SetMSAA(8);

    auto callback = ssGUI::Factory::Create<ssGUI::EventCallbacks::SizeChangedEventCallback>();
    callback->AddEventListener
    (
        "Key",
        [](ssGUI::GUIObject*src,ssGUI::GUIObject*container,ssGUI::ObjectsReferences*references){DEBUG_LINE("size changed.")}
    );
    mainWindow.AddEventCallback(callback);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}