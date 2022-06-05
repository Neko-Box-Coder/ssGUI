#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    ssGUI::StandardButton button;
    button.GetButtonTextObject()->SetText("Click me!!");
    button.SetPosition(glm::vec2(50, 50));
    button.SetSize(glm::vec2(120, 40));
    button.SetParent(&window);


    auto data = ssGUI::Factory::Create<ssGUI::ImageData>();
    data->LoadFromPath("Resources/WindowIcon.png");
    button.GetButtonIconObject()->SetImageData(data);
    button.SetIconButtonMode(true);


    ssGUI::Button button2;
    button2.SetPosition(glm::vec2(50, 150));
    button2.SetSize(glm::vec2(125, 40));
    button2.SetParent(&window);


    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostUpdateEventListener
    (
        [&]()
        {
            auto backendInput = guiManager.GetBackendInputInterface();
            if(backendInput->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !backendInput->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                auto buttonColor = glm::ivec4(button.GetButtonColor());
                buttonColor.r += 20;
                buttonColor.g += 20;
                buttonColor.b += 20;

                if(buttonColor.r > 255)
                    buttonColor.r = 0;
                if(buttonColor.g > 255)
                    buttonColor.g = 0;
                if(buttonColor.b > 255)
                    buttonColor.b = 0;
                
                button.SetButtonColor(glm::u8vec4(buttonColor));
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    ssGUI::Factory::Dispose<ssGUI::ImageData>(data);
    return 0;
}