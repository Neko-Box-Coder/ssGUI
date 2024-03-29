#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"
#include <ctime>

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(4);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);


    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(guiManager.GetBackendInputInterface()->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::A))
            {
                // ssLOG_LINE(time(0)<<": A");
                mainWindow.SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
            }
            else
                mainWindow.SetBackgroundColor(glm::u8vec4(255, 255, 255, 255));
        }
    );

    guiManager.StartRunning();
    return 0;
}