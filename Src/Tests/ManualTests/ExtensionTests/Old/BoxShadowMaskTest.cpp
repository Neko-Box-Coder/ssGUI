#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

#include "ssLogger/ssLog.hpp"

//TODO: This is not working atm
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    // ssGUI::StandardWindow window;
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));

    auto bs = ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>();
    bs->SetBlurRadius(20);
    bs->SetPositionOffset(glm::vec2(10, 10));

    window.AddExtension(bs);

    ssGUI::Window window2;
    window2.SetSize(glm::vec2(300, 300));
    window2.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());
    
    
    window.SetParent(&window2);
    window2.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}