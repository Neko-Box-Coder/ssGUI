#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));

    ssGUI::StandardWindow sWindow;
    sWindow.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    uint64_t lastCountTime = 0;
    int framecount = 0;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(guiManager.GetBackendInputInterface()->GetElapsedTime() - lastCountTime > 1000)
            {
                lastCountTime = guiManager.GetBackendInputInterface()->GetElapsedTime();
                
                ssLOG_LINE("fps: "<<framecount);
                framecount = 0;
            }
            else
                framecount++;

            if(guiManager.GetBackendInputInterface()->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::A))
            {
                guiManager.SetTargetFramerate(100);
            }
            else if(guiManager.GetBackendInputInterface()->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::B))
            {
                guiManager.SetTargetFramerate(50);
            }
            else if(guiManager.GetBackendInputInterface()->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::ENTER))
            {
                ssGUI::Factory::Create<ssGUI::StandardWindow>()->SetParent(&mainWindow);
            }
        }
    );

    guiManager.StartRunning();
    return 0;
}