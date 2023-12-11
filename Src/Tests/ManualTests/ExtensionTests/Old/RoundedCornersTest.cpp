#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"

#include "ssLogger/ssLog.hpp"



//Rounded corners test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    //mainWindow.GetBackendWindowInterface()->SetMSAA(4);
    ssLOG_LINE(mainWindow.GetBackendWindowInterface()->GetAntiAliasingLevel());

    ssGUI::Widget widget;
    widget.SetPosition(glm::ivec2(100, 100));
    widget.SetSize(glm::ivec2(300, 300));
    widget.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));

    auto roundedCornersEx = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
    roundedCornersEx->SetRoundedCornersRadius(25);

    roundedCornersEx->ClearTargetShapes();
    roundedCornersEx->AddTargetVertex(0);

    widget.AddExtension(roundedCornersEx);
    widget.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
