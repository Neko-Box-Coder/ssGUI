#ifndef SSGUI_EVENT_INFO_H
#define SSGUI_EVENT_INFO_H

//namespace: ssGUI
namespace ssGUI
{
    class GUIObject;
    class ObjectsReferences;

    //struct: ssGUI::EventInfo
    //This is the structure that holds the information for an event that can be consumed by event listeners
    struct EventInfo
    {
        //var: GUI Object that triggered the callback
        GUIObject* EventSource = nullptr;

        //var: GUI Object that contains the event callback
        GUIObject* EventCallbackContainer = nullptr;

        //var: <ObjectsReferences> that this event callback has
        ObjectsReferences* EventCallbackReferences = nullptr;
    };
}

#endif