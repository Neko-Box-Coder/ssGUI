#ifndef WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK
#define WINDOW_DRAG_STATE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ssGUI::EventCallbacks::WindowDragStateChangedEventCallback
    //This event callback is triggered *after* this GUI object (window) drag state is changed. Window(Container) will be the source for triggering this event callback.
    class WindowDragStateChangedEventCallback : public BaseEventCallback
    {
        public:
            friend class ssGUI::Factory;

        protected:
            WindowDragStateChangedEventCallback() = default;
            WindowDragStateChangedEventCallback(WindowDragStateChangedEventCallback const &) = default;
            WindowDragStateChangedEventCallback& operator=(WindowDragStateChangedEventCallback const &) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
        
        public:
            
            //function: GetEventCallbackName
            //See <BaseEventCallback::GetEventCallbackName>
            virtual std::string GetEventCallbackName() const override;
            
            //function: Clone
            //See <BaseEventCallback::Clone>
            virtual WindowDragStateChangedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif