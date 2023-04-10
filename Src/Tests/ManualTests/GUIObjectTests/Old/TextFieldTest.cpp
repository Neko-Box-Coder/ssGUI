#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/TextField.hpp"
#include "ssLogger/ssLog.hpp"


//Text test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;

    //Create a text widget and set the respective properties
    ssGUI::TextField text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    //text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
    // text.SetFontSize(20);
    text.SetNewTextUnderlined(true);
    // text.SetTextColor(glm::u8vec4(255, 0, 0, 255));
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
    // text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::RIGHT);
    // text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
    text.SetText(L"Just some text for testing selection.\nHere is a newline for you to test :D");

    text.SetBlockInput(true);
    text.SetStartSelectionIndex(5);
    text.SetEndSelectionIndex(15);
    text.SetFocus(true);

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            /*if(!guiManager.GetBackendInputInterface()->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT_CTRL) &&
                guiManager.GetBackendInputInterface()->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT_CTRL))
            {
                ssLOG_LINE("text.GetStartSelectionIndex(): "<<text.GetStartSelectionIndex());    
                ssLOG_LINE("text.GetEndSelectionIndex(): "<<text.GetEndSelectionIndex());    
                ssLOG_LINE("text.GetCharactersDetailsCount(): "<<text.GetCharactersDetailsCount());
            }*/
        }
    );


    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}