#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 110));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    mainWindow.AddExtension<ssGUI::Extensions::Layout>();
    mainWindow.GetAnyExtension<ssGUI::Extensions::Layout>()->AddPreferredSizeMultiplier(0.5);
    mainWindow.GetAnyExtension<ssGUI::Extensions::Layout>()->AddPreferredSizeMultiplier(0.5);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetNewTextFontSize(17);
    text.SetText("Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
    text.AddExtension<ssGUI::Extensions::Border>();

    //Create a button
    ssGUI::StandardButton button;
    button.AddExtension<ssGUI::Extensions::AdvancedPosition>();
    ssGUI::GUIObject wrapper;
    wrapper.AddExtension<ssGUI::Extensions::Border>();

    //Set the parents
    text.SetParent(&mainWindow);
    button.SetParent(&wrapper);
    wrapper.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
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