#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"


//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::Image image;
    ssGUI::ImageData imgData;
    imgData.LoadFromPath("Resources/sd.png");
    image.SetImageData(&imgData);
    image.SetBackgroundColor(glm::u8vec4(0, 0, 0, 0));

    ssGUI::Extensions::AdvancedSize* as = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
    as->SetHorizontalPercentage(0.85);
    as->SetVerticalPercentage(0.85);

    image.AddExtension(as);
    
    ssGUI::Window window;
    window.SetSize(glm::vec2(400, 400));
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());

    ssGUI::Window window2;
    window2.SetSize(glm::vec2(200, 200));
    window2.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>());

    ssGUI::Window window3;
    window3.SetSize(glm::vec2(100, 100));
    window3.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());
    window3.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));

    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);
    image.SetParent(&window3);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
