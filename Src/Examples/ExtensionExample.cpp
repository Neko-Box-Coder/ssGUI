#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/Extensions/BoxShadow.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Outline.hpp"

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
    window.RemoveAnyExtension<ssGUI::Extensions::Border>();
    window.SetTitlebarHeight(25);
    window.SetParent(&mainWindow);

    auto borderButton = ssGUI::Factory::Create<ssGUI::StandardButton>();
    borderButton->SetPosition(glm::vec2(400, 20));
    borderButton->SetSize(glm::vec2(130, 50));
    borderButton->GetButtonTextObject()->SetNewCharacterFontSize(15);
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
    
    borderButton->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
        {
            if(static_cast<ssGUI::Button*>(container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsAnyExtensionExist<ssGUI::Extensions::Border>())
                    window.RemoveAnyExtension<ssGUI::Extensions::Border>();
                else
                {
                    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
                    window.GetAnyExtension<ssGUI::Extensions::Border>()->SetBorderWidth(2);
                }
            }
        }
    );

    boxShadowButton->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
        {
            if(static_cast<ssGUI::Button*>(container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsAnyExtensionExist<ssGUI::Extensions::BoxShadow>())
                    window.RemoveAnyExtension<ssGUI::Extensions::BoxShadow>();
                else
                {
                    window.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::BoxShadow>());
                }
            }
        }
    );

    roundedCornersButton->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
        {
            if(static_cast<ssGUI::Button*>(container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsAnyExtensionExist<ssGUI::Extensions::RoundedCorners>())
                    window.RemoveAnyExtension<ssGUI::Extensions::RoundedCorners>();
                else
                {
                    auto rc = ssGUI::Factory::Create<ssGUI::Extensions::RoundedCorners>();
                    rc->SetRoundedCornersRadius(15);
                    rc->ClearTargetShapes();
                    rc->AddTargetVertex(0);
                    rc->AddTargetVertex(1);
                    rc->AddTargetVertex(2);
                    rc->AddTargetVertex(3);
                    rc->AddTargetVertex(4);
                    rc->AddTargetVertex(5);
                    window.AddExtension(rc);
                    window.ChangeAnyExtensionDrawOrder<ssGUI::Extensions::RoundedCorners>(0);
                }
            }
        }
    );

    outlineButton->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>()->AddEventListener
    (
        "AnyKey",
        [&](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* references)
        {
            if(static_cast<ssGUI::Button*>(container)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                if(window.IsAnyExtensionExist<ssGUI::Extensions::Outline>())
                    window.RemoveAnyExtension<ssGUI::Extensions::Outline>();
                else
                {
                    auto windowOutline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
                    windowOutline->SetOutlineThickness(2);
                    window.AddExtension(windowOutline);
                }
            }
        }
    );

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}