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

    ssGUI::ImageData img;
    if(!img.LoadFromPath("Resources/sd.png"))
        ssLOG_LINE("Failed to load image");

    mainWindow.SetIcon(img);

    guiManager.StartRunning();
    return 0;
}