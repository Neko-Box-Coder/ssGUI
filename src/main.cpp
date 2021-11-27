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
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"



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
        window.SetPosition(window.GetPosition() + glm::ivec2(-1, 0));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT))
        window.SetPosition(window.GetPosition() + glm::ivec2(1, 0));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::DOWN))
        window.SetPosition(window.GetPosition() + glm::ivec2(0, 1));
    if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::UP))
        window.SetPosition(window.GetPosition() + glm::ivec2(0, -1));

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



/*
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetBackgroundColour(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParentP(&mainWindow);

    //Creating window
    ssGUI::Window window2;
    window2.SetBackgroundColour(glm::u8vec4(127, 255, 127, 255));
    window2.SetSize(glm::ivec2(150, 150));
    window2.SetParentP(&mainWindow);

    std::vector<ssGUI::GUIObject *> children;
    ssGUI::Window* window3 = static_cast<ssGUI::Window*>(window2.Clone(children, false));
    window3->SetBackgroundColour(glm::u8vec4(127, 127, 255, 255));

    ssGUI::Window* window4 = static_cast<ssGUI::Window*>(window2.Clone(children, false));
    window4->SetBackgroundColour(glm::u8vec4(127, 127, 127, 255));



    //window3->SetParentP(&mainWindow);

    ssGUI::Extensions::Dockable* dock = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock2 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock3 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock4 = new ssGUI::Extensions::Dockable();

    
    window.AddExtension(dock);
    window2.AddExtension(dock2);
    window3->AddExtension(dock3);
    window4->AddExtension(dock4);


    ssGUI::Extensions::Docker* docker = new ssGUI::Extensions::Docker();
    docker->SetChildrenDockerUseThisSettings(false);

    ssGUI::Extensions::AdvancedPosition* ap = new ssGUI::Extensions::AdvancedPosition();
    ssGUI::Extensions::AdvancedSize* as = new ssGUI::Extensions::AdvancedSize();
    ssGUI::Extensions::Border* bor = new ssGUI::Extensions::Border();

    ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
    ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
    as->SetHorizontalPercentage(0.8);
    as->SetHorizontalUsePercentage(true);
    as->SetVerticalPercentage(0.8);
    as->SetVerticalUsePercentage(true);
    
    ssGUI::Widget mainWindowWidget;

    mainWindowWidget.AddExtension(docker);
    static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetPadding(0);
    mainWindowWidget.AddExtension(ap);
    //mainWindowWidget.AddExtension(as);
    mainWindowWidget.AddExtension(bor);
    mainWindowWidget.SetBackgroundColour(glm::u8vec4(200, 200, 200, 255));
    
    mainWindowWidget.SetSize(glm::ivec2(400, 400));
    
    mainWindowWidget.SetParentP(&mainWindow);
    mainWindow.ChangeChildOrder(--mainWindow.GetChildrenEndIterator(), mainWindow.GetChildrenStartIterator());

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/

/*//Text test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    //mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    //mainWindow.SetSize(glm::ivec2(500, 150));

    //Loading the font
    ssGUI::Font font;
    if(font.GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf"))
    // if(font.GetBackendFontInterface()->LoadFromPath("SourceHanSansJP-Normal.otf"))
        std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    text.AddExtension(new ssGUI::Extensions::Border());
    text.SetFont(&font);
    text.SetFontSize(20);
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");
    text.SetText(L"Click on the button to change this text.");



    //Add the text and button widget to the main window
    text.SetParentP(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}*/


/*//Introduction Example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);
    mainWindow.SetSize(glm::ivec2(500, 150));

    //Loading the font
    ssGUI::Font font;
    if(font.GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf"))
        std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetFont(&font);
    text.SetFontSize(20);
    text.SetText(L"Click on the button to change this text.");

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::ivec2(50, 30));
    button.SetPosition(glm::ivec2(225, 85));
    button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
    (
        [&](ssGUI::GUIObject* src)
        {
            if(((ssGUI::Button*)src)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                text.SetText(L"Button pressed and this text has changed.");
            }
        }
    );

    //Add the text and button widget to the main window
    text.SetParentP(&mainWindow);
    button.SetParentP(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
*/

/*//Event callback example
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
*/

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