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

    //A fixed non resizable non draggable window
    ssGUI::Window fixedWindow;
    fixedWindow.SetDraggable(false);
    fixedWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    fixedWindow.SetParent(&mainWindow);

    //AdvancedSize extension allows sizing a GUI object by percentage.
    auto sizeExtension = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedSize>();
    sizeExtension->SetHorizontalPercentage(0.6);
    sizeExtension->SetVerticalPercentage(0.6);
    fixedWindow.AddExtension(sizeExtension);

    //AdvancedPosition extension allows more option to position a GUI Object. By default it will center the GUI object.
    fixedWindow.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>());
    fixedWindow.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Mask>());            //Adding a mask extension for masking the draggable window

    //Draggable window inside the fixed window
    ssGUI::Window draggableWindow;
    draggableWindow.SetBackgroundColor(glm::vec4(200, 200, 200, 255));
    draggableWindow.SetPosition(glm::vec2(50, 50));
    draggableWindow.SetParent(&fixedWindow);
    //draggableWindow.SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_RIGHT);    //You can uncomment this line to anchor the window to different anchor points

    //Text of showing local and global position for draggableWindow
    ssGUI::Text positionText;
    positionText.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    positionText.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
    auto sizeExtension2 = sizeExtension->Clone(&positionText);
    sizeExtension2->SetHorizontalPercentage(1.0);
    sizeExtension2->SetVerticalPercentage(0.3);
    positionText.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //Add update event listener for updating the text for showing the local and global position of the text
    guiManager.AddPostUpdateEventListener
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