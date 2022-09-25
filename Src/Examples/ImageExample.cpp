#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"



//Image example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating an image widget
    ssGUI::Image image;
    image.SetSize(glm::vec2(300, 300));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);

    ssGUI::ImageData data;
    data.LoadFromPath("Resources/sd.png");
    image.SetImageData(&data);
    image.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}