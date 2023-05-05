#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ExamplesResources.h"

//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::StandardWindow window;
    window.SetSize(glm::vec2(400, 400));
    window.GetWindowTitleObject()->SetText("Mask Example");
    auto maskEx = window.AddExtension<ssGUI::Extensions::Mask>();
    
    ssGUI::Window window2;
    window2.SetSize(glm::vec2(200, 200));
    window2.AddExtension<ssGUI::Extensions::Mask>();
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.SetBackgroundColor(glm::u8vec4(150, 127, 127, 255));

    ssGUI::Window window3;
    window3.SetSize(glm::vec2(100, 100));
    window3.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));

    ssGUI::Image img;
    img.SetBackgroundColor(glm::u8vec4(255, 255, 255, 0));
    auto sizeEx = img.AddExtension<ssGUI::Extensions::AdvancedSize>();
    sizeEx->SetHorizontalPercentage(1);
    sizeEx->SetVerticalPercentage(1);
    ssGUI::ImageData imgData;
    imgData.LoadImgFileFromMemory(ssGUI_Example_sd, ssGUI_Example_sd_size);
    img.SetImageData(&imgData);

    //Setting parents
    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);
    img.SetParent(&window3);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
