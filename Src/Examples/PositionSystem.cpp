#include "ssGUI/HeaderGroups/StandardGroup.hpp"                     //Includes all the core ssGUI classes
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/Extensions/Layout.hpp"

//Position example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(850, 600));

    //A fixed parent widget
    ssGUI::Widget parentWidget;
    parentWidget.SetParent(&mainWindow);
    parentWidget.SetPosition(glm::vec2(50, 70));
    parentWidget.SetSize(glm::vec2(350, 350));
    parentWidget.SetBackgroundColor(glm::vec4(200, 200, 200, 255));
    parentWidget.AddExtension<ssGUI::Extensions::Border>();                 //Adding a border to indicate where the widget is
    parentWidget.AddExtension<ssGUI::Extensions::Mask>();                   //Adding a mask extension for masking the draggable window

    //A caption text for parent widget
    ssGUI::Text parentWidgetCaption;
    parentWidgetCaption.SetText("Parent widget GUI Object");
    parentWidgetCaption.SetSize(glm::vec2(350, 40));                                                //We set the text widget size same as the parent
    parentWidgetCaption.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);      //So if we align the text to center
    parentWidgetCaption.SetTextVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);          //It will be the center of the widget
    parentWidgetCaption.SetParent(&parentWidget);

    //Indicator line to visualize the position system
    ssGUI::Widget lineWidget;
    auto shapeEx = lineWidget.AddExtension<ssGUI::Extensions::Shape>();
    int lineIndex = shapeEx->AddAdditionalLine();                                                   //we will set the values of the line later
    lineWidget.AddTag(ssGUI::Tags::OVERLAY);                                                        //By adding an overlay tag, it will always show in front
    lineWidget.SetParent(&mainWindow);

    //A child window
    ssGUI::StandardWindow widgetChild;
    widgetChild.GetWindowTitleObject()->SetText("Child GUI Object");
    widgetChild.SetSize(glm::vec2(200, 200));
    widgetChild.RemoveExtension<ssGUI::Extensions::Dockable>();                                  //By default, it can dock to other windows, we don't need that
    widgetChild.SetParent(&parentWidget);                                                           //Also by default, dockable extention reparent the GUI object
                                                                                                    //After dragging the window and we don't want that.
    //A different parent window
    ssGUI::StandardWindow parentWindow;
    parentWindow.SetPosition(glm::vec2(450, 70));
    parentWindow.GetWindowTitleObject()->SetText("Parent Window GUI Object");
    parentWindow.SetSize(glm::vec2(350, 350));
    parentWindow.AddExtension<ssGUI::Extensions::Mask>();                   //Adding a mask extension for masking the child window
    parentWindow.RemoveExtension<ssGUI::Extensions::Dockable>();                                 //Remove it for now
    parentWindow.SetParent(&mainWindow);

    //A child window inside the parent window
    ssGUI::StandardWindow* windowChild = widgetChild.Clone(true);                                   //We can just clone the child we created before
    windowChild->SetBackgroundColor(glm::vec4(200, 200, 200, 255));
    windowChild->SetParent(&parentWindow);
    
    //Global and local button to visualizing the position system
    ssGUI::StandardButton globalButton;
    globalButton.SetButtonColor(glm::u8vec4(59, 155, 245, 255));
    globalButton.GetButtonTextObject()->SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
    globalButton.GetButtonTextObject()->SetText("Global Position");
    globalButton.SetPosition(glm::vec2(300, 445));
    globalButton.SetSize(glm::vec2(120, 40));
    globalButton.SetParent(&mainWindow);

    ssGUI::StandardButton* localButton = globalButton.Clone(true);
    localButton->SetPosition(glm::vec2(430, 445));
    localButton->GetButtonTextObject()->SetText("Local Position");

    //All the anchor buttons for the 4 corners
    ssGUI::StandardButton* anchorTopLeftBtn = ssGUI::Factory::Create<ssGUI::StandardButton>();
    anchorTopLeftBtn->SetParent(&mainWindow);
    anchorTopLeftBtn->SetButtonColor(glm::u8vec4(161, 59, 245, 255));
    anchorTopLeftBtn->GetButtonTextObject()->SetNewTextColor(glm::u8vec4(255, 255, 255, 255));
    anchorTopLeftBtn->GetButtonTextObject()->SetText("Anchor Top Left");
    anchorTopLeftBtn->SetPosition(glm::vec2(130, 500));
    anchorTopLeftBtn->SetSize(glm::vec2(140, 40));

    ssGUI::StandardButton* anchorTopRightBtn = anchorTopLeftBtn->Clone(true);
    anchorTopRightBtn->GetButtonTextObject()->SetText("Anchor Top Right");
    anchorTopRightBtn->SetPosition(glm::vec2(280, 500));

    ssGUI::StandardButton* anchorBotLeftBtn = anchorTopRightBtn->Clone(true);
    anchorBotLeftBtn->GetButtonTextObject()->SetText("Anchor Bot Left");
    anchorBotLeftBtn->SetPosition(glm::vec2(430, 500));

    ssGUI::StandardButton* anchorBotRightBtn = anchorBotLeftBtn->Clone(true);
    anchorBotRightBtn->GetButtonTextObject()->SetText("Anchor Bot Right");
    anchorBotRightBtn->SetPosition(glm::vec2(580, 500));

    //Text of showing local and global position for draggableWindow
    ssGUI::Text positionText;
    positionText.SetPosition(glm::vec2(320, 10));
    positionText.SetSize(glm::vec2(200, 40));
    positionText.SetTextHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    positionText.SetParent(&mainWindow);

    //Status to keep track of
    ssGUI::GUIObject* currentFocusedObj = &widgetChild;
    bool isLocal = true;
    ssGUI::Enums::AnchorType currentAnchor = ssGUI::Enums::AnchorType::TOP_LEFT;

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    //Add update event listener for updating the text for showing the local and global position of the text
    //We could have used <ssGUI::EventCallback> but for the sake of simplicity, we are doing everything
    //in the post GUI update listener.
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            //If the mainWindow is closed, just exit.
            //The reason why we need this is because the parent for GUI objects will be nullptr (when mainWindow is closed)
            //even though the GUI objects themselves are still valid
            if(mainWindow.IsClosed())
                return;

            //Set the status variables depending which window is focused
            if(currentFocusedObj == &widgetChild && windowChild->IsFocused())
            {
                currentFocusedObj = windowChild;
                currentAnchor = currentFocusedObj->GetAnchorType();
            }
            else if(currentFocusedObj == windowChild && widgetChild.IsFocused())
            {
                currentFocusedObj = &widgetChild;
                currentAnchor = currentFocusedObj->GetAnchorType();
            }

            //Set the isLocal if either of the buttons is clicked
            if(globalButton.GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                isLocal = false;
            else if(localButton->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                isLocal = true;

            //Set the anchor for the child GUI object depending which button is clicked
            if(anchorTopLeftBtn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                currentAnchor = ssGUI::Enums::AnchorType::TOP_LEFT;
                currentFocusedObj->SetAnchorType(currentAnchor);
            }
            else if(anchorTopRightBtn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                currentAnchor = ssGUI::Enums::AnchorType::TOP_RIGHT;
                currentFocusedObj->SetAnchorType(currentAnchor);
            }
            else if(anchorBotLeftBtn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                currentAnchor = ssGUI::Enums::AnchorType::BOTTOM_LEFT;
                currentFocusedObj->SetAnchorType(currentAnchor);
            }
            else if(anchorBotRightBtn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                currentAnchor = ssGUI::Enums::AnchorType::BOTTOM_RIGHT;
                currentFocusedObj->SetAnchorType(currentAnchor);
            }

            //Modify the visualization line to show the position system
            glm::vec2 startPos = isLocal ? currentFocusedObj->GetParent()->GetGlobalPosition() : glm::vec2();
            glm::vec2 endPos = currentFocusedObj->GetGlobalPosition();

            if(isLocal)
            {
                if(currentAnchor == ssGUI::Enums::AnchorType::TOP_RIGHT)
                {
                    startPos.x += currentFocusedObj->GetParent()->GetSize().x;
                    endPos.x += currentFocusedObj->GetSize().x;
                }
                else if(currentAnchor == ssGUI::Enums::AnchorType::BOTTOM_RIGHT)
                {
                    startPos.x += currentFocusedObj->GetParent()->GetSize().x;
                    startPos.y += currentFocusedObj->GetParent()->GetSize().y;
                    endPos += currentFocusedObj->GetSize();
                }
                else if(currentAnchor == ssGUI::Enums::AnchorType::BOTTOM_LEFT)
                {
                    startPos.y += currentFocusedObj->GetParent()->GetSize().y;
                    endPos.y += currentFocusedObj->GetSize().y;
                }

                if(currentFocusedObj == windowChild && 
                    (currentAnchor == ssGUI::Enums::AnchorType::TOP_LEFT || currentAnchor == ssGUI::Enums::AnchorType::TOP_RIGHT))
                {
                    startPos += glm::vec2(0, static_cast<ssGUI::Window*>(currentFocusedObj->GetParent())->GetTitlebarHeight());   
                }
            }
            
            shapeEx->SetAdditionalLine(lineIndex, endPos, startPos, 2, 2, glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), false);

            //Show the local and global position in text
            std::string showText = 
                "Local Position: " + std::to_string((int)currentFocusedObj->GetPosition().x) + ", " + std::to_string((int)currentFocusedObj->GetPosition().y) + "\n" +
                "Global Position: " + std::to_string((int)currentFocusedObj->GetGlobalPosition().x) + ", " + std::to_string((int)currentFocusedObj->GetGlobalPosition().y);
            positionText.SetText(showText);
        }
    );

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}