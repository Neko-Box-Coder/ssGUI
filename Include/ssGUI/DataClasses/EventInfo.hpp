#ifndef SSGUI_EVENT_INFO_H
#define SSGUI_EVENT_INFO_H

//namespace: ssGUI
namespace ssGUI
{
    namespace Backend
    {
        class BackendSystemInputInterface;
    }

    class GUIObject;
    class ObjectsReferences;
    class InputStatus;
    
    //struct: ssGUI::ObjectUpdateInfo
    struct ObjectUpdateInfo
    {
        //var: InputInterface
        ssGUI::Backend::BackendSystemInputInterface* InputInterface;
        
        //var: InputStatus
        ssGUI::InputStatus& CurrentInputStatus;
        
        //var: LastInputStatus
        const ssGUI::InputStatus& LastInputStatus;
        
        //var: MainWindow
        ssGUI::GUIObject* MainWindow;
    };

    //struct: ssGUI::EventInfo
    //This is the structure that holds the information for an event that can be consumed by event listeners
    struct EventInfo
    {
        //var: EventSource
        //GUI Object that triggered the callback
        GUIObject* EventSource = nullptr;

        //var: Container
        //GUI Object that contains the event callback
        GUIObject* Container = nullptr;

        //var: References
        //<ObjectsReferences> that this event callback has
        ObjectsReferences* References = nullptr;
        
        //var: DeleteCurrentListener
        //If current listener is not needed, setting it to true will delete the current listener.
        bool DeleteCurrentListener = false;
    
        //var: CustomInfo
        void* CustomInfo = nullptr;
    };
}

#endif