#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssLogger/ssLog.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(4);
    ssLOG_LINE(mainWindow.GetBackendWindowInterface()->GetMSAA());

    ssGUI::Window window;
    window.SetSize(glm::ivec2(500, 500));

    ssGUI::Widget widget;
    widget.SetPosition(glm::ivec2(50, 50));
    widget.SetBackgroundColor(glm::u8vec4(0, 0, 0, 255));

    auto as = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();

    widget.AddExtension(as);
    
    widget.SetParent(&window);
    window.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}