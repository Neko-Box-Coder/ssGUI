#include "ssGUI/HeaderGroups/StandardGroup.hpp"

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 110));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetSize(glm::vec2(450, 45));
    text.SetNewCharacterFontSize(17);
    text.SetText("Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);

    //Create a button
    ssGUI::StandardButton button;
    button.SetSize(glm::vec2(40, 30));
    button.SetPosition(glm::vec2(205, 60));

    //Set the parents
    button.SetParent(&mainWindow);
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(button.GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                text.SetText(L"(`oωo´)");
        }
    );
    guiManager.StartRunning();
    return 0;
}