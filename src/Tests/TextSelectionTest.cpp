#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"


//Text test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    //mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    //mainWindow.SetSize(glm::ivec2(500, 150));



    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
    text.SetFontSize(20);
    text.SetTextUnderlined(true);
    text.SetTextColor(glm::u8vec4(255, 0, 0, 255));
    text.SetText(L"Just some text for testing selection.\nHere is a newline for you to test :D");

    text.SetBlockInput(true);
    text.SetStartSelectionIndex(5);
    text.SetEndSelectionIndex(15);

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            if(!guiManager.GetBackendInputInterface()->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::SPACE) &&
                guiManager.GetBackendInputInterface()->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::SPACE))
            {
                DEBUG_LINE("text.GetStartSelectionIndex(): "<<text.GetStartSelectionIndex());    
                DEBUG_LINE("text.GetEndSelectionIndex(): "<<text.GetEndSelectionIndex());    
            }
        }
    );


    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}