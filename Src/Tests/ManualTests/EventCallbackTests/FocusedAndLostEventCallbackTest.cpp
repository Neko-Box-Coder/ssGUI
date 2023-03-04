#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"


ssGUI::MainWindow* mainWindow;
ssGUI::Window* window;
ssGUI::Window* window2;
ssGUI::Widget* widget;
ssGUI::Widget* widget2;

ssGUI::ssGUIManager* manager;


void SetUp()
{
    mainWindow = ssGUI::Factory::Create<ssGUI::MainWindow>();
    window = ssGUI::Factory::Create<ssGUI::Window>();
    window2 = ssGUI::Factory::Create<ssGUI::Window>();
    widget = ssGUI::Factory::Create<ssGUI::Widget>();
    widget2 = ssGUI::Factory::Create<ssGUI::Widget>();
    manager = ssGUI::Factory::Create<ssGUI::ssGUIManager>();
    
    auto objToPtr = [&](void* ptr)
    {
        if(ptr == window)
            return "window";
        else if(ptr == window2)
            return "window2";
        else if(ptr == widget)
            return "widget";
        else if(ptr == widget2)
            return "widget2";
        else
            return "unknown";
    };
    
    auto* ecb = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusedEventCallback>();
    auto* ecb2 = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusLostEventCallback>();
    ecb->AddEventListener(  "TestKey",    
                            [&](ssGUI::EventInfo info)
                            {
                                ssLOG_SIMPLE("Focused: "<<objToPtr(info.EventSource));
                                assert(info.EventSource->IsFocused());
                            });
                            
    ecb2->AddEventListener(  "TestKey",    
                            [&](ssGUI::EventInfo info)
                            {
                                ssLOG_SIMPLE("Focus lost: "<<objToPtr(info.EventSource));
                                assert(!info.EventSource->IsFocused());
                            });
    
    window->SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
    window2->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    widget->SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
    widget2->SetBackgroundColor(glm::u8vec4(0, 0, 255, 255));
    
    window->AddEventCallback(ecb);
    window->AddEventCallback(ecb2);
    ecb->Clone(window2, true);
    ecb2->Clone(window2, true);
    ecb->Clone(widget, true);
    ecb2->Clone(widget, true);
    ecb->Clone(widget2, true);
    ecb2->Clone(widget2, true);

    window->SetParent(mainWindow);
    window2->SetParent(mainWindow);
    widget->SetParent(mainWindow);
    widget2->SetParent(window2);

    ssLOG_SIMPLE("window: Red");
    ssLOG_SIMPLE("window2: Grey");
    ssLOG_SIMPLE("widget: Green");
    ssLOG_SIMPLE("widget2: Blue");
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("");

    manager->AddGUIObject(mainWindow);
    manager->AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = manager->GetBackendInputInterface();
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            window->SetFocus(!window->IsFocused());
            ssLOG_SIMPLE("window focus set to: "<<window->IsFocused());
        }
        
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
        {
            window2->SetFocus(!window2->IsFocused());
            ssLOG_SIMPLE("window2 focus set to: "<<window2->IsFocused());
        }
        
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
        {
            widget->SetFocus(!widget->IsFocused());
            ssLOG_SIMPLE("widget focus set to: "<<widget->IsFocused());
        }
        
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
        {
            widget2->SetFocus(!widget2->IsFocused());
            ssLOG_SIMPLE("widget2 focus set to: "<<widget2->IsFocused());
        }
    });
}

void CleanUp()
{
    ssGUI::Factory::Dispose(manager);
}

void Instructions()
{
    ssLOG_SIMPLE("Use mouse to interact with windows and widgets");
    ssLOG_SIMPLE("You should be able to see focus change text in console");
    ssLOG_SIMPLE("Press 1 to toggle window focus");
    ssLOG_SIMPLE("Press 2 to toggle window2 focus");
    ssLOG_SIMPLE("Press 3 to toggle widget focus");
    ssLOG_SIMPLE("Press 4 to toggle widget2 focus");
}

int main()
{
    Instructions();
    SetUp();
    
    manager->StartRunning();
    
    CleanUp();   
}
