#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"
#include <ctime>

//** DONT RUN THIS TEST UNLESS THE COPY CONSTRUCTOR IN BACKEND IS IMPLEMENTED **
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(4);

    ssGUI::MainWindow* mainWindow2 = mainWindow.Clone(true);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.AddGUIObject((ssGUI::GUIObject*)mainWindow2);

    ssGUI::ImageData img;
    if(!img.LoadFromPath("Resources/sd.png"))
        ssLOG_LINE("Failed to load image");

    mainWindow.SetIcon(img);

    guiManager.StartRunning();

    ssGUI::Factory::Dispose(mainWindow2);

    return 0;
}