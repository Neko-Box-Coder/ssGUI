#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ExamplesResources.h"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    auto* textfield = mainWindow.AddChild<ssGUI::TextField>();

    textfield->SetSize(glm::vec2(500, 250));
    textfield->SetPosition(glm::vec2(50, 50));
    textfield->SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
    textfield->SetText("This is an example sentence.\nThis is another example sentence.\nThird sentence.");
    
    textfield->AddEventCallback(ssGUI::Enums::EventType::TEXT_CONTENT_CHANGED)->AddEventListener
    (
        "Key",
        [](ssGUI::EventInfo& info)
        {
            ssLOG_LINE("Changing...");
        }
    );

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.StartRunning();
    
    return 0;
}