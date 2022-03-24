#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/CompositeClasses/StandardWindow.hpp"




//Standard window test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.GetBackendWindowInterface()->SetMSAA(8);

    ssGUI::StandardWindow sw;
    sw.GetWindowTitleObject()->SetText("Test title");
    sw.SetSize(glm::vec2(100, 100));
    // sw.AddExtension(new ssGUI::Extensions::Dockable());
    // sw.AddExtension(new ssGUI::Extensions::Outline());
    //sw.RemoveExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME);
    
    auto sw2 = sw.Clone(true);
    // sw2->SetBackgroundColor(glm::u8vec4(200, 127, 127, 255));

    sw.SetParent(&mainWindow);
    sw2->SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}