#ifndef SSGUI_SLIDER_VALUE_CHANGED_VIA_GUI_EVENT_CALLBACK_H
#define SSGUI_SLIDER_VALUE_CHANGED_VIA_GUI_EVENT_CALLBACK_H

#include "ssGUI/EventCallbacks/BaseEventCallback.hpp"

namespace ssGUI
{

//namespace: ssGUI::EventCallbacks
namespace EventCallbacks
{
    //class: ssGUI::EventCallbacks::SliderValueChangedViaGuiEventCallback
    //This event callback is triggered *shortly after* the value of the slider is changed.
    //This is triggered under the <ssGUI::Slider::MainLogic> method therefore the event is not immediate.
    class SliderValueChangedViaGuiEventCallback : public BaseEventCallback
    {        
        public:
            friend class ssGUI::Factory;
        
        protected:
            SliderValueChangedViaGuiEventCallback() = default;
            SliderValueChangedViaGuiEventCallback(SliderValueChangedViaGuiEventCallback const &) = default;
            SliderValueChangedViaGuiEventCallback& operator=(SliderValueChangedViaGuiEventCallback const &) = default;
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
            virtual SliderValueChangedViaGuiEventCallback* Clone(ssGUI::GUIObject* newContainer, bool copyListeners) override;
            
            //const: EVENT_NAME
            //See <BaseEventCallback::EVENT_NAME>
            static const std::string EVENT_NAME;
    };
}

}

#endif