#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"                     //Includes all the core ssGUI classes
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Mask.hpp"

//Position example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 450));

    auto positionExtension = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();

    //AdvancedSize extension allows sizing a GUI object by percentage.
    auto sizeExtension = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
    sizeExtension->SetHorizontalPercentage(0.6);
    sizeExtension->SetVerticalPercentage(0.6);

    //A fixed non resizable non draggable window
    ssGUI::Window fixedWindow;
    fixedWindow.SetDraggable(false);
    fixedWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    //AdvancedPosition extension allows more option to position a GUI Object. By default it will center the GUI object.
    fixedWindow.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
    fixedWindow.AddExtension(sizeExtension);
    fixedWindow.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());            //Adding a mask extension for masking the draggable window
    fixedWindow.SetBackgroundColor(glm::vec4(127, 127, 127, 255));

    //Text of showing local and global position for draggableWindow
    ssGUI::Text positionText;
    positionText.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
    positionText.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
    
    //Second AdvancedPosition extension for the text
    ssGUI::Extensions::AdvancedPosition* positionExtension2 = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
    positionExtension2->SetVerticalPercentage(-0.4);

    //Draggable window inside the fixed window
    ssGUI::Window draggableWindow;
    draggableWindow.SetTitlebarColor(glm::vec4(100, 100, 100, 255));
    draggableWindow.SetPosition(glm::vec2(50, 50));
    //draggableWindow.SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_RIGHT);    //You can uncomment this line to anchor the window to different anchor points

    //Add the GUI objects to the main window
    fixedWindow.SetParent(&mainWindow);
    positionText.SetParent(&mainWindow);
    draggableWindow.SetParent(&fixedWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //Add update event listener for updating the text for showing the local and global position of the text
    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            std::string showText = 
                "Local Position: " + std::to_string((int)draggableWindow.GetPosition().x) + ", " + std::to_string((int)draggableWindow.GetPosition().y) + "\n" +
                "Global Position: " + std::to_string((int)draggableWindow.GetGlobalPosition().x) + ", " + std::to_string((int)draggableWindow.GetGlobalPosition().y);
            positionText.SetText(showText);
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}