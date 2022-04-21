#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

//Window example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetSize(glm::vec2(150, 150));
    window.SetAdaptiveTitlebarColor(true);
    window.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            auto backendInput = guiManager.GetBackendInputInterface();
            if(backendInput->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !backendInput->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                auto bgColor = window.GetBackgroundColor();
                int red = bgColor.r;
                red += 20;
                red %= 255;
                bgColor.r = red;
                window.SetBackgroundColor(bgColor);
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
