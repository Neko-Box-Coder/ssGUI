#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(450, 650));

    //A window for holding 3 more windows
    ssGUI::Window window;
    window.SetSize(glm::vec2(300, 600));
    window.SetParent(&mainWindow);
    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));
    
    //The 3 windows with different titlebar color
    ssGUI::Window window2;
    window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window2.SetParent(&window);
    ssGUI::Window window3;
    window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window3.SetParent(&window);
    ssGUI::Window window4;
    window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
    window4.SetParent(&window);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText("Some Text");
    text.SetSize(glm::vec2(150, 30));
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());             //Adding border to better visualize the text widget
    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());   //AdvancedPosition for centering the text widget
    text.SetParent(&window2);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}