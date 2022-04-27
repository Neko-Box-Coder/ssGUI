#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

// Cloning test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    // ssGUI::StandardWindow window;
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    ssGUI::ImageData img;
    if(!img.LoadFromPath("CustomCursor.png"))
    {
        DEBUG_LINE("Failed to load");
        return 0;
    }

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.GetBackendInputInterface()->SetCustomCursor(&img, glm::ivec2(13, 20), glm::ivec2());
    
    ssGUI::ImageData imagedata;
    glm::ivec2 hotspot;
    guiManager.GetBackendInputInterface()->GetCustomCursor(imagedata, hotspot);
    DEBUG_LINE("cursor2: "<<imagedata.IsValid());

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            guiManager.GetBackendInputInterface()->SetCursorType(ssGUI::Enums::CursorType::CUSTOM);
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}