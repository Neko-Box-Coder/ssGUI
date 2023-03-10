#ifndef SSGUI_SLIDER_VALUE_FINISHED_CHANGING_EVENT_CALLBACK_H
#define SSGUI_SLIDER_VALUE_FINISHED_CHANGING_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::SliderValueFinishedChangingEventCallback
    //This event callback is triggered *shortly after* the value of the slider has *finished* changing.
    //Meaning this will be triggered if the slider value stayed the same for current update
    //*and* the previous update it has been changed.
    //
    //This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
    class SliderValueFinishedChangingEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            SliderValueFinishedChangingEventCallback() = default;
            SliderValueFinishedChangingEventCallback(SliderValueFinishedChangingEventCallback const &) = default;
            SliderValueFinishedChangingEventCallback& operator=(SliderValueFinishedChangingEventCallback const &) = default;
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
            virtual SliderValueFinishedChangingEventCallback* Clone(bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif