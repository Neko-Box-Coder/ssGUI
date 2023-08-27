#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ExamplesResources.h"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    auto* text = mainWindow.AddChild<ssGUI::Text>();

    text->SetSize(glm::vec2(300, 150));
    text->SetPosition(glm::vec2(50, 50));
    text->SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
    text->SetTextAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT, ssGUI::Enums::AlignmentVertical::TOP);
    text->SetText("This is an example sentence.\nThis is another example sentence.\nThird sentence.");
    text->SetTextSelectionAllowed(true);
    text->SetInteractable(true);
    text->SetBlockInput(true);
    text->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    
    return 0;
}