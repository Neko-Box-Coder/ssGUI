#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

void SetUp()
{
}

void CleanUp()
{
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to change button interactable or not");
    ssLOG_SIMPLE("Use mouse to interact with the button");
    ssLOG_SIMPLE("You should be able to see button state change text in console when interacting with the button");
}

int main()
{
    SetUp();
    
    Instructions();
    
    ssGUI::MainWindow mainWindow;
    ssGUI::Window window;
    ssGUI::Window window2;
    ssGUI::Widget widget;
    ssGUI::Widget widget2;
    
    auto* ecb = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusedEventCallback>();
    auto* ecb2 = ssGUI::Factory::Create<ssGUI::EventCallbacks::FocusLostEventCallback>();
    ecb->AddEventListener(  "TestKey",    
                            [](ssGUI::EventInfo info)
                            {
                                ssLOG_SIMPLE("Focused: "<<info.EventSource);
                                assert(info.EventSource->IsFocused());
                            });
                            
    ecb2->AddEventListener(  "TestKey",    
                            [](ssGUI::EventInfo info)
                            {
                                ssLOG_SIMPLE("Focus lost: "<<info.EventSource);
                                assert(!info.EventSource->IsFocused());
                            });
    
    window.SetBackgroundColor(glm::u8vec4(255, 0, 0, 255));
    window2.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    widget.SetBackgroundColor(glm::u8vec4(0, 255, 0, 255));
    widget2.SetBackgroundColor(glm::u8vec4(0, 0, 255, 255));
    
    window.AddEventCallback(ecb);
    window.AddEventCallback(ecb2);
    ecb->Clone(&window2, true);
    ecb2->Clone(&window2, true);
    ecb->Clone(&widget, true);
    ecb2->Clone(&widget, true);
    ecb->Clone(&widget2, true);
    ecb2->Clone(&widget2, true);

    window.SetParent(&mainWindow);
    window2.SetParent(&mainWindow);
    widget.SetParent(&mainWindow);
    widget2.SetParent(&window2);

    ssLOG_SIMPLE("window (Red): "<<&window);
    ssLOG_SIMPLE("window2 (Grey): "<<&window2);
    ssLOG_SIMPLE("widget (Green): "<<&widget);
    ssLOG_SIMPLE("widget2 (Blue): "<<&widget2);

    ssGUI::ssGUIManager manager;
    manager.AddGUIObject(&mainWindow);
    manager.AddPostGUIUpdateEventListener([&]()
    {
        auto* inputInterface = manager.GetBackendInputInterface();
        if( !inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
            inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
        {
            //button.SetInteractable(!button.IsInteractable());
        }
    });
    
    manager.StartRunning();
    
    CleanUp();   
}
