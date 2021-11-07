/*
#include <iostream>
#include "ProjectConfig.hpp"
#include "ssGUI/ssGUIManager.hpp"
#include "ssGUI/BaseClasses/Text.hpp"
#include "ssGUI/BaseClasses/Button.hpp"
*/

#include <iostream>
#include <vector>
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"



void PrintEnter(ssGUI::ssGUIManager& manager)
{
    ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();

    if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
        !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
        std::cout<<"Enter key down\n";

    else if(!inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
            inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
        std::cout<<"Enter key up\n";
}

void MoveWindow(ssGUI::ssGUIManager& manager, ssGUI::Window& window, ssGUI::Window& childWindow)
{
    ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();
    ssGUI::KeyPresses currentKeyPresses = inputInterface->GetCurrentKeyPresses();

    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT))
        window.SetPosition(window.GetPositon() + glm::ivec2(-1, 0));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT))
        window.SetPosition(window.GetPositon() + glm::ivec2(1, 0));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::DOWN))
        window.SetPosition(window.GetPositon() + glm::ivec2(0, 1));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::UP))
        window.SetPosition(window.GetPositon() + glm::ivec2(0, -1));

    if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::A))
        window.SetSize(window.GetSize() + glm::ivec2(-1, 0));
    if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::D))
        window.SetSize(window.GetSize() + glm::ivec2(1, 0));
    if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::S))
        window.SetSize(window.GetSize() + glm::ivec2(0, 1));
    if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::W))
        window.SetSize(window.GetSize() + glm::ivec2(0, -1));

    if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
        !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
    {
        window.SetVisible(window.IsVisible() ? false : true);
        //std::cout<<"WindowPos: "<<window.GetGlobalPosition().x<<", "<<window.GetGlobalPosition().y<<"\n";
        //std::cout<<"childWindowPos: "<<childWindow.GetGlobalPosition().x<<", "<<childWindow.GetGlobalPosition().y<<"\n";
    }

}

int main()
{
    ssGUI::MainWindow mainWindow;

    ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback* callback = new ssGUI::EventCallbacks::RecursiveChildrenAddedEventCallback();
    callback->AddEventListener([&](ssGUI::GUIObject* source) {std::cout<<"child added\n";});

    mainWindow.AddEventCallback(callback);

    //Creating window
    ssGUI::Window window;
    window.SetBackgroundColour(glm::u8vec4(127, 127, 127, 255));
    window.SetSize(glm::ivec2(400, 300));
    window.SetPosition(glm::ivec2(50, 200));
    window.SetParentP(&mainWindow);     //This should trigger the event callback
    ssGUI::Extensions::Border* b2 = new ssGUI::Extensions::Border();
    window.AddExtension(b2);


    ssGUI::Widget widget;
    widget.SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_RIGHT);
    widget.SetPosition(glm::ivec2(75, 75));
    widget.SetParentP(&window);

    ssGUI::Extensions::Border* b = new ssGUI::Extensions::Border();
    widget.AddExtension(b);
            

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}

/*//Window example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetBackgroundColour(glm::u8vec4(127, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParentP(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/



/*//IMage example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating an image widget
    ssGUI::Image image;
    image.SetSize(glm::ivec2(300, 300));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);
    image.GetBackendImageInterface()->LoadFromPath("sd.png");
    image.SetParentP(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/

/*//Text example
int main()
{
    ssGUI::MainWindow mainWindow;
    
    //Loading the font
    ssGUI::Font font;
    //if(font.GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf"))
    if(font.GetBackendFontInterface()->LoadFromPath("SourceHanSansJP-Normal.otf"))
        std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(300, 70));
    text.SetFont(&font);
    text.SetFontSize(20);
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::CHARACTER_WRAPPING);
    text.SetText(L"これは非常に長い日本語の文章です~~~~~");
    text.SetParentP(&mainWindow);

    text.AddExtension(new ssGUI::Extensions::Border());

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/