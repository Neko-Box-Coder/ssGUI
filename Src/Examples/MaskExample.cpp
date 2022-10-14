#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"


//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::StandardWindow window;
    window.SetSize(glm::vec2(400, 400));
    window.GetWindowTitleObject()->SetText("Mask Example");
    auto maskEx = ssGUI::Factory::Create<ssGUI::Extensions::Mask>();
    
    //This will add mask enforcer to the children
    window.AddExtension(maskEx);

    ssGUI::Window window2;
    window2.SetSize(glm::vec2(200, 200));
    window2.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.SetBackgroundColor(glm::u8vec4(150, 127, 127, 255));

    ssGUI::Window window3;
    window3.SetSize(glm::vec2(100, 100));
    window3.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));

    ssGUI::Image img;
    img.SetBackgroundColor(glm::u8vec4(255, 255, 255, 0));
    auto sizeEx = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
    sizeEx->SetHorizontalPercentage(1);
    sizeEx->SetVerticalPercentage(1);
    img.AddExtension(sizeEx);
    ssGUI::ImageData imgData;
    imgData.LoadFromPath("Resources/sd.png");
    img.SetImageData(&imgData);

    //Setting parents
    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);
    img.SetParent(&window3);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
