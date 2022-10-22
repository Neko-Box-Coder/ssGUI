
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/Dockable.hpp"

#include "ssLogger/ssLog.hpp"

//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window window;
    // window.SetTitlebarHeight(25);
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    // window.AddExtension(new ssGUI::Extensions::Layout());
    // window.RemoveExtension(ssGUI::Extensions::Dockable::EXTENSION_NAME);
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Docker>());
    // window.GetAnyExtension<ssGUI::Extensions::Layout>()->SetPadding(5);
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    ssGUI::StandardWindow window2;
    window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    ssGUI::StandardWindow window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
    ssGUI::StandardWindow window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));

    ssGUI::StandardWindow window5;
    // window5.SetTitlebarColor(glm::u8vec4(127, 127, 127, 255));
    window5.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));


    window2.SetParent(&window);
    window3.SetParent(&window);
    window4.SetParent(&window);
    window5.SetParent(&window);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                auto obj = window2.GetParent();
                auto obj2 = window5.GetParent();
                ssLOG_LINE("window: "<<&window);
                ssLOG_LINE("window2: "<<&window2);
                ssLOG_LINE("window3: "<<&window3);
                ssLOG_LINE("window4: "<<&window4);
                ssLOG_LINE("window5: "<<&window5);

                ssLOG_LINE("window2 parent: "<<obj);
                ssLOG_LINE("window5 parent: "<<obj2);
                ssLOG_LINE("window2 parent min size: "<<obj->GetMinSize().x<<", "<<obj->GetMinSize().y);
                ssLOG_LINE("window5 parent min size2: "<<obj2->GetMinSize().x<<", "<<obj2->GetMinSize().y);
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
