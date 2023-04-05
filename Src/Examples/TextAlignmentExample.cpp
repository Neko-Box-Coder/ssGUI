#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"

int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(700, 350));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    ssGUI::GUIObject buttonsContainer;
    buttonsContainer.SetPosition(glm::vec2(360, 40));
    buttonsContainer.SetSize(glm::vec2(300, 250));
    buttonsContainer.SetParent(&mainWindow);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::vec2(20, 20));
    text.SetSize(glm::vec2(300, 300));
    text.SetText("Align me!!");
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
    text.AddExtension<ssGUI::Extensions::Border>();

    //Create buttons for aligning text
    for(int i = 0; i < 9; i++)
    {
        auto button = ssGUI::Factory::Create<ssGUI::StandardButton>();
        auto ap = button->AddExtension<ssGUI::Extensions::AdvancedPosition>();
        button->SetSize(glm::vec2(70, 50));
        button->GetButtonTextObject()->SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
        button->GetButtonTextObject()->SetNewCharacterFontSize(15);

        using ssGUI::Extensions::AdvancedPosition;
        //Set buttons text and position
        switch (i)
        {
            case 0:
                button->GetButtonTextObject()->SetText("Top Left");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                break;
            case 1:
                button->GetButtonTextObject()->SetText("Top Center");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                break;
            case 2:
                button->GetButtonTextObject()->SetText("Top Right");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                break;
            case 3:
                button->GetButtonTextObject()->SetText("Middle Left");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
                break;
            case 4:
                button->GetButtonTextObject()->SetText("Middle Center");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
                break;
            case 5:
                button->GetButtonTextObject()->SetText("Middle Right");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
                break;
            case 6:
                button->GetButtonTextObject()->SetText("Bottom Left");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                break;
            case 7:
                button->GetButtonTextObject()->SetText("Bottom Center");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                break;
            case 8:
                button->GetButtonTextObject()->SetText("Bottom Right");
                ap->SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                ap->SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                break;
        }

        //Set button to align text when pressed
        button->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
        (
            "AnyKey",
            [&text, i](ssGUI::EventInfo info)
            {
                if(((ssGUI::Button*)info.EventSource)->GetButtonState() != ssGUI::Enums::ButtonState::CLICKED)
                    return;

                switch (i)
                {
                    case 0:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                        break;
                    case 1:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                        break;
                    case 2:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::TOP);
                        break;
                    case 3:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
                        break;
                    case 4:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);
                        break;
                    case 5:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::CENTER);               
                        break;
                    case 6:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                        break;
                    case 7:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                        break;
                    case 8:
                        text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
                        break;
                }
            }
        );

        button->SetParent(&buttonsContainer);
    }
    
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}