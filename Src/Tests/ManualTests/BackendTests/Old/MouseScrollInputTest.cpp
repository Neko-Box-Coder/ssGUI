#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssLogger/ssLog.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    // ssGUI::StandardWindow window;
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            glm::vec2 mouseScroll = guiManager.GetBackendInputInterface()->GetCurrentMouseScrollDelta();
            
            if(mouseScroll != glm::vec2())
                ssLOG_LINE("Mouse Scroll: "<<mouseScroll.x<<", "<<mouseScroll.y);
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}