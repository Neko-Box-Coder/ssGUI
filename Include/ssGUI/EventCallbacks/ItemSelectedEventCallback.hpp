#ifndef ITEM_SELECTED_EVENT_CALLBACK
#define ITEM_SELECTED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::ItemSelectedEventCallback
    //This event callback is triggered *after* an item is selected. Dropdown(Container) will be the source for triggering this event callback.
    class ItemSelectedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;

        protected:
            ItemSelectedEventCallback() = default;
            ItemSelectedEventCallback(ItemSelectedEventCallback const &) = default;
            ItemSelectedEventCallback& operator=(ItemSelectedEventCallback const &) = default;
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
            virtual ItemSelectedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif