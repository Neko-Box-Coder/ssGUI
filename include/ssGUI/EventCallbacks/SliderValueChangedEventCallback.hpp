#ifndef SLIDER_VALUE_CHANGED_EVENT_CALLBACK
#define SLIDER_VALUE_CHANGED_EVENT_CALLBACK

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

//namespace: ssGUI::EventCallbacks
namespace ssGUI::EventCallbacks
{
    //class: ssGUI::EventCallbacks::SliderValueChangedEventCallback
    //This event callback is triggered *shortly after* the value of the slider is changed.
    //This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
    class SliderValueChangedEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            SliderValueChangedEventCallback() = default;
            SliderValueChangedEventCallback(SliderValueChangedEventCallback const &) = default;
            SliderValueChangedEventCallback& operator=(SliderValueChangedEventCallback const &) = default;
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
            virtual SliderValueChangedEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

#endif