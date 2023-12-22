#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssLogger/ssLog.hpp"

//Standard window test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    ssLOG_LINE(mainWindow.GetBackendWindowInterface()->GetAntiAliasingLevel());

    ssGUI::StandardWindow sw;
    sw.GetWindowTitleObject()->SetText("Test title");
    sw.SetSize(glm::vec2(200, 200));
    // sw.AddExtension(new ssGUI::Extensions::Dockable());
    // sw.AddExtension(new ssGUI::Extensions::Outline());
    //sw.RemoveExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME);
    
    auto sw2 = sw.Clone(true);
    // sw2->SetBackgroundColor(glm::u8vec4(200, 127, 127, 255));
    sw2->SetTitlebarColor(glm::u8vec4(255, 255, 255, 255));

    sw.SetParent(&mainWindow);
    sw2->SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            auto backendInput = guiManager.GetBackendInputInterface();
            if( backendInput->IsInputExistLastFrame(ssGUI::Enums::SystemKey::ENTER) &&
                !backendInput->IsInputExistCurrentFrame(ssGUI::Enums::SystemKey::ENTER))
            {
                auto titlebarColor = glm::ivec4(sw.GetTitlebarColor());
                titlebarColor.r += 20;
                titlebarColor.g += 20;
                titlebarColor.b += 20;

                if(titlebarColor.r > 255)
                    titlebarColor.r = 0;
                if(titlebarColor.g > 255)
                    titlebarColor.g = 0;
                if(titlebarColor.b > 255)
                    titlebarColor.b = 0;
                
                sw.SetTitlebarColor(glm::u8vec4(titlebarColor));
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}