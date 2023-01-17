#include "ssGUI/HeaderGroups/StandardGroup.hpp"



//Image Test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating an image widget
    ssGUI::Image image;
    image.SetSize(glm::vec2(200, 200));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);
    image.SetImageTint(glm::u8vec4(255, 148, 148, 255));

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