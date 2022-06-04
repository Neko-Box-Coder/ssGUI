#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
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
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());

    //Create buttons for aligning text
    for(int i = 0; i < 9; i++)
    {
        auto button = ssGUI::Factory::Create<ssGUI::StandardButton>();
        auto ap = ssGUI::Factory::Create<ssGUI::Extensions::AdvancedPosition>();
        button->SetSize(glm::vec2(70, 50));
        button->GetButtonTextObject()->SetWrappingMode(ssGUI::Enums::TextWrapping::WORD_WRAPPING);
        button->GetButtonTextObject()->SetFontSize(15);
        button->AddExtension(ap);

        using ssGUI::Extensions::AdvancedPosition;
        //Set buttons text and position
        switch (i)
        {
            case 0:
                button->GetButtonTextObject()->SetText("Top Left");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::LEFT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::TOP);
                break;
            case 1:
                button->GetButtonTextObject()->SetText("Top Center");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::CENTER);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::TOP);
                break;
            case 2:
                button->GetButtonTextObject()->SetText("Top Right");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::RIGHT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::TOP);
                break;
            case 3:
                button->GetButtonTextObject()->SetText("Middle Left");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::LEFT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::CENTER);
                break;
            case 4:
                button->GetButtonTextObject()->SetText("Middle Center");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::CENTER);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::CENTER);
                break;
            case 5:
                button->GetButtonTextObject()->SetText("Middle Right");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::RIGHT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::CENTER);
                break;
            case 6:
                button->GetButtonTextObject()->SetText("Bottom Left");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::LEFT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::BOTTOM);
                break;
            case 7:
                button->GetButtonTextObject()->SetText("Bottom Center");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::CENTER);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::BOTTOM);
                break;
            case 8:
                button->GetButtonTextObject()->SetText("Bottom Right");
                ap->SetHorizontalAnchor(AdvancedPosition::HorizontalAnchor::RIGHT);
                ap->SetVerticalAnchor(AdvancedPosition::VerticalAnchor::BOTTOM);
                break;
        }

        //Set button to align text when pressed
        button->GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
        (
            "AnyKey",
            [&text, i](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                if(((ssGUI::Button*)src)->GetButtonState() != ssGUI::Enums::ButtonState::CLICKED)
                    return;

                switch (i)
                {
                    case 0:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP);
                        break;
                    case 1:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP);
                        break;
                    case 2:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::TOP);
                        break;
                    case 3:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
                        break;
                    case 4:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);
                        break;
                    case 5:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::CENTER);               
                        break;
                    case 6:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::LEFT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
                        break;
                    case 7:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
                        break;
                    case 8:
                        text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::RIGHT);
                        text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
                        break;
                }
            }
        );

        button->SetParent(&buttonsContainer);
    }
    
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}