#ifndef SSGUI_EVENT_CALLBACK_MANAGER_H
#define SSGUI_EVENT_CALLBACK_MANAGER_H

#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/EventCallback.hpp"
#include <unordered_map>
#include <type_traits>
#include <string>
#include <vector>

//namespace: ssGUI
namespace ssGUI
{
    class Renderer;
    class GUIObject;

    /*class: ssGUI::EventCallbackManager
    This manages all the EventCallbacks for a GUI Object

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        std::unordered_map<ssGUI::Enums::EventType, ssGUI::EventCallback*> EventCallbacks;      //See <GetEventCallback>
        ssGUI::Renderer* CurrentRenderer;                                                       //(Internal variable) Used for sending redraw signal
        ssGUI::GUIObject* CurrentObject;                                                        //(Internal variable) Used to register container
    =================================================================
    ============================== C++ ==============================
    EventCallbackManager::EventCallbackManager() :  EventCallbacks(),
                                                    CurrentRenderer(nullptr),
                                                    CurrentObject(nullptr)
    {}
    =================================================================
    */
    class EventCallbackManager
    {
        protected:
            std::unordered_map<ssGUI::Enums::EventType, ssGUI::EventCallback*> EventCallbacks;      //See <GetEventCallback>
            ssGUI::Renderer* CurrentRenderer;                                                       //(Internal variable) Used for sending redraw signal
            ssGUI::GUIObject* CurrentObject;                                                        //(Internal variable) Used to register container

            EventCallbackManager(EventCallbackManager const& other);

        public:
            EventCallbackManager();
            virtual ~EventCallbackManager();

            //function: SetDependentComponents
            //(Internal ssGUI function) Sets the required compnents for this to work 
            virtual void SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj);

            //function: AddEventCallback
            //Adds an eventCallback to this GUI Object. 
            //If the eventCallback already exists, it will return the existing one event callback.
            virtual ssGUI::EventCallback* AddEventCallback(ssGUI::Enums::EventType eventType);
            
            //function: AddEventCallbackCopy
            //Adds an eventCallback to this GUI Object by copying an already existing eventCallback. 
            //If the eventCallback already exists, it will return the existing one event callback.
            virtual ssGUI::EventCallback* AddEventCallbackCopy(ssGUI::EventCallback* copy, bool copyListeners);

            //function: GetEventCallback
            //Gets the eventCallback by the name of it
            virtual ssGUI::EventCallback* GetEventCallback(ssGUI::Enums::EventType eventType);

            //function: IsEventCallbackExist
            //Returns true if the eventCallback exists on this GUI Object
            virtual bool IsEventCallbackExist(ssGUI::Enums::EventType eventType) const;

            //function: RemoveEventCallback
            //Removes the eventCallback by the name of it
            virtual void RemoveEventCallback(ssGUI::Enums::EventType eventType);

            //function: GetListOfEventCallbacks
            //Returns all the event callbacks on the GUI Object
            virtual std::vector<ssGUI::EventCallback*> GetListOfEventCallbacks();
            
            //function: ForwardEvent
            //Forwards the specified event we received to the specified GUI Object
            virtual void ForwardEvent(ssGUI::GUIObject* objToForwardTo, ssGUI::Enums::EventType forwardEvent);
    };
}

#endif