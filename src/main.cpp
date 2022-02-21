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
#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/MaskEnforcer.hpp"



// void PrintEnter(ssGUI::ssGUIManager& manager)
// {
//     ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();

//     if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
//         std::cout<<"Enter key down\n";
        
//     }
//     else if(!inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
        
//         std::cout<<"Enter key up\n";
//     }
// }

// void MoveWindow(ssGUI::ssGUIManager& manager, ssGUI::Window& window, ssGUI::Window& childWindow)
// {
//     ssGUI::Backend::BackendSystemInputInterface* inputInterface = manager.GetBackendInputInterface();
//     ssGUI::KeyPresses currentKeyPresses = inputInterface->GetCurrentKeyPresses();

//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT))
//         window.SetPosition(window.GetPosition() + glm::ivec2(-1, 0));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT))
//         window.SetPosition(window.GetPosition() + glm::ivec2(1, 0));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::DOWN))
//         window.SetPosition(window.GetPosition() + glm::ivec2(0, 1));
//     if(currentKeyPresses.IsSystemKeyPresent(ssGUI::Enums::SystemKey::UP))
//         window.SetPosition(window.GetPosition() + glm::ivec2(0, -1));

//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::A))
//         window.SetSize(window.GetSize() + glm::ivec2(-1, 0));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::D))
//         window.SetSize(window.GetSize() + glm::ivec2(1, 0));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::S))
//         window.SetSize(window.GetSize() + glm::ivec2(0, 1));
//     if(currentKeyPresses.IsLetterKeyPresent(ssGUI::Enums::LetterKey::W))
//         window.SetSize(window.GetSize() + glm::ivec2(0, -1));

//     if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
//         !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
//     {
//         window.SetVisible(window.IsVisible() ? false : true);
//         //std::cout<<"WindowPos: "<<window.GetGlobalPosition().x<<", "<<window.GetGlobalPosition().y<<"\n";
//         //std::cout<<"childWindowPos: "<<childWindow.GetGlobalPosition().x<<", "<<childWindow.GetGlobalPosition().y<<"\n";
//     }
// }


/*// Cloning test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    //window.SetBackgroundColor(glm::u8vec4(255, 127, 127, 255));
    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParent(&mainWindow);
    DEBUG_LINE("window: "<<&window);

    //Creating window
    ssGUI::Window window2;
    DEBUG_LINE("window2: "<<&window2);

    window2.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window2.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window2.SetSize(glm::ivec2(150, 150));
    window2.SetParent(&mainWindow);
    

    ssGUI::Extensions::Dockable* dock = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock2 = new ssGUI::Extensions::Dockable();
    
    window.AddExtension(dock);
    window2.AddExtension(dock2);

    ssGUI::Extensions::Docker* docker = new ssGUI::Extensions::Docker();
    docker->SetChildrenDockerUseThisSettings(false);
    
    ssGUI::Extensions::Border* bor = new ssGUI::Extensions::Border();
    ssGUI::Extensions::Mask* mask = new ssGUI::Extensions::Mask();
    
    mainWindow.AddExtension(new ssGUI::Extensions::Layout());
    static_cast<ssGUI::Extensions::Layout*>(mainWindow.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetCoverFullLength(false);
    static_cast<ssGUI::Extensions::Layout*>(mainWindow.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetHorizontalLayout(true);


    ssGUI::Widget mainWindowWidget;

    mainWindowWidget.AddExtension(docker);
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetPadding(0);
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->
    //    ExcludeObject(window3ObjPtr);
    mainWindowWidget.AddExtension(bor);
    mainWindowWidget.AddExtension(mask);
    mainWindowWidget.SetBackgroundColor(glm::u8vec4(200, 200, 200, 255));
    
    mainWindowWidget.SetSize(glm::ivec2(400, 400));
    
    mainWindowWidget.SetParent(&mainWindow);

    mainWindow.MoveChildrenIteratorToLast();
    std::list<ssGUI::ssGUIObjectIndex>::iterator lastIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.MoveChildrenIteratorToFirst();
    std::list<ssGUI::ssGUIObjectIndex>::iterator firstIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.ChangeChildOrderToBeforePosition(lastIt, firstIt);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                mainWindowWidget.Clone(true);
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/


/*//Docking test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    //window.SetBackgroundColor(glm::u8vec4(255, 127, 127, 255));
    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParent(&mainWindow);
    DEBUG_LINE("window: "<<&window);

    //Creating window
    ssGUI::Window window2;
    DEBUG_LINE("window2: "<<&window2);

    window2.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window2.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window2.SetSize(glm::ivec2(150, 150));
    window2.SetParent(&mainWindow);

    
    ssGUI::Window* window3 = new ssGUI::Window();//static_cast<ssGUI::Window*>(window2.Clone(false));
    window3->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window3->SetTitlebarColor(glm::u8vec4(255, 255, 255, 255));
    window3->SetSize(glm::ivec2(150, 150));
    window3->SetParent(&mainWindow);
    DEBUG_LINE("window3: "<<&window3);

    ssGUI::GUIObject* window3ObjPtr = window3;
    DEBUG_LINE("window3 obj ptr: "<<window3ObjPtr);



    ssGUI::Window* window4 = new ssGUI::Window();//static_cast<ssGUI::Window*>(window2.Clone(false));
    window4->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window4->SetSize(glm::ivec2(150, 150));
    window4->SetParent(&mainWindow);
    DEBUG_LINE("window4: "<<&window4);

    

    ssGUI::Extensions::Dockable* dock = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock2 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock3 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock4 = new ssGUI::Extensions::Dockable();

    
    window.AddExtension(dock);
    window2.AddExtension(dock2);
    window3->AddExtension(dock3);
    window4->AddExtension(dock4);


    ssGUI::Extensions::Docker* docker = new ssGUI::Extensions::Docker();
    docker->SetChildrenDockerUseThisSettings(true);
    
    ssGUI::Extensions::AdvancedPosition* ap = new ssGUI::Extensions::AdvancedPosition();
    ssGUI::Extensions::AdvancedSize* as = new ssGUI::Extensions::AdvancedSize();
    ssGUI::Extensions::Border* bor = new ssGUI::Extensions::Border();
    ssGUI::Extensions::Mask* mask = new ssGUI::Extensions::Mask();

    ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
    ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
    as->SetHorizontalPercentage(0.8);
    as->SetHorizontalUsePercentage(true);
    as->SetVerticalPercentage(0.8);
    as->SetVerticalUsePercentage(true);
    
    ssGUI::Widget mainWindowWidget;

    mainWindowWidget.AddExtension(new ssGUI::Extensions::Layout());
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetPadding(0);
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->
    //    ExcludeObject(window3ObjPtr);
    mainWindowWidget.AddExtension(ap);
    mainWindowWidget.AddExtension(as);
    mainWindowWidget.AddExtension(bor);
    mainWindowWidget.AddExtension(mask);
    mainWindowWidget.SetBackgroundColor(glm::u8vec4(200, 200, 200, 255));
    
    mainWindowWidget.SetSize(glm::ivec2(400, 400));
    
    mainWindowWidget.SetParent(&mainWindow);

    window4->SetParent(&mainWindowWidget);
    dock4->SetTopLevelParent(&mainWindowWidget);


    mainWindow.MoveChildrenIteratorToLast();
    std::list<ssGUI::ssGUIObjectIndex>::iterator lastIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.MoveChildrenIteratorToFirst();
    std::list<ssGUI::ssGUIObjectIndex>::iterator firstIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.ChangeChildOrderToBeforePosition(lastIt, firstIt);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                if(window.GetBackgroundColor().a == 255)
                    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 127));
                else
                    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
            }
        }
    );

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
    //ssGUI::Font font;
    //if(font.GetBackendFontInterface()->LoadFromPath("NotoSans-Regular.ttf"))
    // if(font.GetBackendFontInterface()->LoadFromPath("SourceHanSansJP-Normal.otf"))
    //    std::cout<<"font loaded\n";

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(500, 300));
    text.AddExtension(new ssGUI::Extensions::Border());
    //text.SetFont(&font);
    text.SetFontSize(20);
    // text.SetText(L"これは非常に長い日本語の文章です~~~~~");
    text.SetText(L"Click on the button to change this text.");

    //Add the text and button widget to the main window
    text.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}*/


//Introduction Example
/*
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
    text.SetParent(&mainWindow);
    button.SetParent(&mainWindow);

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

    ssGUI::EventCallbacks::RecursiveChildAddedEventCallback* callback = new ssGUI::EventCallbacks::RecursiveChildAddedEventCallback();
    callback->AddEventListener([&](ssGUI::GUIObject* source) {std::cout<<"child added\n";});

    mainWindow.AddEventCallback(callback);

    //Creating window
    ssGUI::Window window;
    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window.SetSize(glm::ivec2(400, 300));
    window.SetPosition(glm::ivec2(50, 200));
    window.SetParent(&mainWindow);     //This should trigger the event callback
    ssGUI::Extensions::Border* b2 = new ssGUI::Extensions::Border();
    window.AddExtension(b2);


    ssGUI::Widget widget;
    widget.SetAnchorType(ssGUI::Enums::AnchorType::BOTTOM_RIGHT);
    widget.SetPosition(glm::ivec2(75, 75));
    widget.SetParent(&window);

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
    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParent(&mainWindow);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/



/*//Image example
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating an image widget
    ssGUI::Image image;
    image.SetSize(glm::ivec2(300, 300));
    image.SetFitting(ssGUI::Enums::ImageFitting::FIT_WHOLE_IMAGE);

    ssGUI::ImageData data;
    data.LoadFromPath("sd.png");
    image.SetImageData(&data);
    image.SetParent(&mainWindow);

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
    text.SetParent(&mainWindow);

    text.AddExtension(new ssGUI::Extensions::Border());

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
*/

/*
//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(450, 125));

    //Adds a layout extension to the main window to allow GUI objects to be stacked next to each other
    ssGUI::Extensions::Layout* layoutExtension = new ssGUI::Extensions::Layout();
    layoutExtension->SetHorizontalLayout(false);
    layoutExtension->SetSpacing(10);
    mainWindow.AddExtension(layoutExtension);

    //AdvancedPosition extension allows a GUI Object to be centered.
    ssGUI::Extensions::AdvancedPosition* positionExtension = new ssGUI::Extensions::AdvancedPosition();
    positionExtension->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
    positionExtension->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::TOP);    

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetFontSize(20);
    text.SetText(L"Click on the button to change this text.");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);

    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::ivec2(50, 30));
    button.AddExtension(positionExtension);
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

    //Wraps the button inside an empty widget for consistence size under layout extension
    ssGUI::Widget emptyWidget;
    button.SetParent(&emptyWidget);

    //Add the text and button widget (wrapped inside empty widget) to the main window
    text.SetParent(&mainWindow);
    emptyWidget.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
*/

//Clone test 2
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(450, 125));

    //AdvancedPosition extension allows more option to position a GUI Object. By default it will center the GUI object.
    ssGUI::Extensions::AdvancedPosition* positionExtension = new ssGUI::Extensions::AdvancedPosition();
    positionExtension->SetVerticalUsePercentage(false);

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetText(L"Click on the button to show the message");
    text.SetHorizontalAlignment(ssGUI::Enums::TextAlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::TextAlignmentVertical::BOTTOM);
    positionExtension->SetVerticalPixel(-30);
    text.AddExtension(positionExtension);
    
    //Create a button and set an event callback to change the text when it is clicked
    ssGUI::Button button;
    button.SetSize(glm::ivec2(50, 30));
    ssGUI::ssGUIObjectIndex textIndex = 
        button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddObjectReference(&text);
    button.GetEventCallback(ssGUI::EventCallbacks::ButtonStateChangedEventCallback::EVENT_NAME)->AddEventListener
    (
        [textIndex](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
        {
            if(((ssGUI::Button*)src)->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
            {
                ssGUI::GUIObject* refText = refs->GetObjectReference(textIndex);

                if(refText != nullptr)
                    static_cast<ssGUI::Text*>(refText)->SetText(L"(`oωo´)");
            }
        }
    );
    

    //Clone the extension for the button widget
    ssGUI::Extensions::AdvancedPosition* positionExtension2 = 
        static_cast<ssGUI::Extensions::AdvancedPosition*>(positionExtension->Clone(&button));
    positionExtension2->SetVerticalPixel(20);

    ssGUI::Window window;
    window.SetSize(glm::ivec2(100, 100));
    window.AddExtension(new ssGUI::Extensions::Dockable());

    //Add the text and button widget to the main window
    text.SetParent(&window);
    button.SetParent(&window);
    window.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                window.Clone(true);
            }
        }
    );    

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}



/*//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));

    ssGUI::Window window;
    window.SetSize(glm::ivec2(400, 400));
    window.AddExtension(new ssGUI::Extensions::Mask());

    ssGUI::Window window2;
    window2.SetSize(glm::ivec2(200, 200));
    window2.AddExtension(new ssGUI::Extensions::Mask());

    ssGUI::Window window3;
    window3.SetSize(glm::ivec2(100, 100));
    window3.AddExtension(new ssGUI::Extensions::Mask());

    window.SetParent(&mainWindow);
    window2.SetParent(&window);
    window3.SetParent(&window2);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
*/