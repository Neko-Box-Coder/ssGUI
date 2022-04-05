#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"




//Rounded corners test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    //mainWindow.GetBackendWindowInterface()->SetMSAA(4);
    DEBUG_LINE(mainWindow.GetBackendWindowInterface()->GetMSAA());

    ssGUI::Widget widget;
    widget.SetPosition(glm::ivec2(100, 100));
    widget.SetSize(glm::ivec2(300, 300));
    widget.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    widget.AddExtension(new ssGUI::Extensions::RoundedCorners());
    static_cast<ssGUI::Extensions::RoundedCorners*>(widget.GetExtension(ssGUI::Extensions::RoundedCorners::EXTENSION_NAME))->SetRoundedCornersRadius(25);
    widget.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
