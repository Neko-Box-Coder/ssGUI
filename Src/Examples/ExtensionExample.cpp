#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"


int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(600, 350));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    ssGUI::Window window;
    window.SetPosition(glm::vec2(20, 20));
    window.SetMinSize(glm::vec2(100, 100));
    window.SetSize(glm::vec2(325, 280));
    window.RemoveExtension<ssGUI::Extensions::Border>();
    window.SetTitlebarHeight(25);
    window.SetParent(&mainWindow);

    auto borderButton = ssGUI::Factory::Create<ssGUI::StandardButton>();
    borderButton->SetPosition(glm::vec2(400, 20));
    borderButton->SetSize(glm::vec2(130, 50));
    borderButton->GetButtonTextObject()->SetNewTextFontSize(15);
    borderButton->GetButtonTextObject()->SetText("Border");
    borderButton->SetParent(&mainWindow);

    auto boxShadowButton = borderButton->Clone(true);
    boxShadowButton->GetButtonTextObject()->SetText("Box shadow");
    boxShadowButton->SetPosition(glm::vec2(400, 90));

    auto roundedCornersButton = borderButton->Clone(true);
    roundedCornersButton->GetButtonTextObject()->SetText("Rounded corners");
    roundedCornersButton->SetPosition(glm::vec2(400, 160));

    auto outlineButton = borderButton->Clone(true);
    outlineButton->GetButtonTextObject()->SetText("Outline");
    outlineButton->SetPosition(glm::vec2(400, 230));
    
    borderButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(static_cast<ssGUI::Button*>(info.Container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsExtensionExist<ssGUI::Extensions::Border>())
                    window.RemoveExtension<ssGUI::Extensions::Border>();
                else
                {
                    window.AddExtension<ssGUI::Extensions::Border>();
                    window.GetExtension<ssGUI::Extensions::Border>()->SetBorderWidth(2);
                }
            }
        }
    );

    boxShadowButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(static_cast<ssGUI::Button*>(info.Container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsExtensionExist<ssGUI::Extensions::BoxShadow>())
                    window.RemoveExtension<ssGUI::Extensions::BoxShadow>();
                else
                {
                    window.AddExtension<ssGUI::Extensions::BoxShadow>();
                }
            }
        }
    );

    roundedCornersButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(static_cast<ssGUI::Button*>(info.Container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsExtensionExist<ssGUI::Extensions::RoundedCorners>())
                    window.RemoveExtension<ssGUI::Extensions::RoundedCorners>();
                else
                {
                    auto rc = window.AddExtension<ssGUI::Extensions::RoundedCorners>();
                    rc->SetRoundedCornersRadius(15);
                    rc->ClearTargetShapes();
                    ssGUI::TargetShape titlebarShape(ssGUI::Window::WINDOW_TITLEBAR_SHAPE_NAME);
                    ssGUI::TargetShape baseShape(ssGUI::Window::WINDOW_BASE_SHAPE_NAME);

                    rc->AddTargetVertex(titlebarShape, 0);
                    rc->AddTargetVertex(titlebarShape, 1);
                    rc->AddTargetVertex(baseShape, 0);
                    rc->AddTargetVertex(baseShape, 1);
                    rc->AddTargetVertex(baseShape, 2);
                    rc->AddTargetVertex(baseShape, 3);
                    
                    window.ChangeExtensionDrawOrder<ssGUI::Extensions::RoundedCorners>(0);
                }
            }
        }
    );

    outlineButton->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::EventInfo& info)
        {
            if(static_cast<ssGUI::Button*>(info.Container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsExtensionExist<ssGUI::Extensions::Outline>())
                    window.RemoveExtension<ssGUI::Extensions::Outline>();
                else
                {
                    auto windowOutline = window.AddExtension<ssGUI::Extensions::Outline>();
                    windowOutline->SetOutlineThickness(2);
                }
            }
        }
    );

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}