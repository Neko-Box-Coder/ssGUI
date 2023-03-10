#ifndef SSGUI_EVENT_CALLBACK_MANAGER_H
#define SSGUI_EVENT_CALLBACK_MANAGER_H

#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/EventCallbacks/EventCallback.hpp"
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
        std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*> EventCallbacks;  //See <GetEventCallback>
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
            std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*> EventCallbacks;  //See <GetEventCallback>
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
            //Adds an eventCallback to this GUI Object. If the eventCallback already exists, nothing will be modified.
            template<typename T>
            T* AddEventCallback()
            {
                if(std::is_base_of<ssGUI::EventCallbacks::EventCallback, T>::value)
                {
                    if(IsAnyEventCallbackExist<T>())
                        return GetAnyEventCallback<T>();
                        
                    auto* eventCallback = ssGUI::Factory::Create<T>();
                    EventCallbacks[eventCallback->GetEventCallbackName()] = eventCallback;
                    eventCallback->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return eventCallback;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non event callback type");
                    return nullptr;
                }
            }
            
            //function: AddEventCallbackCopy
            //Adds an eventCallback to this GUI Object by copying an already existing eventCallback. 
            //If an eventCallback already exists in this GUI object, nothing will be modified.
            template<typename T>
            T* AddEventCallbackCopy(T* copy, bool copyListeners)
            {
                if(std::is_base_of<ssGUI::EventCallbacks::EventCallback, T>::value)
                {
                    if(IsEventCallbackExist(copy->GetEventCallbackName()))
                        return static_cast<T*>(GetEventCallback(copy->GetEventCallbackName()));
                        
                    auto* eventCallback = copy->Clone(copyListeners);
                    EventCallbacks[eventCallback->GetEventCallbackName()] = eventCallback;
                    eventCallback->BindToObject(CurrentObject);
                    CurrentRenderer->RedrawObject();
                    return eventCallback;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non event callback type");
                    return nullptr;
                }
            }
            
            //function: GetEventCallback
            //Gets the eventCallback by the name of it
            virtual ssGUI::EventCallbacks::EventCallback* GetEventCallback(std::string eventCallbackName);

            //function: GetAnyEventCallback
            //Generic version of <GetEventCallback>. 
            //It has to be a different name as template function doesn't support inheritance.
            //If it had the same name, the derived version of GetAnyEventCallback will "hide" the generic version of it
            //and will just throw an error saying the template function is not found.
            template <typename T>
            T* GetAnyEventCallback()
            {
                static_assert(std::is_base_of<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                static_assert(!std::is_same<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                return static_cast<T*>(GetEventCallback(T::EVENT_NAME));
            }
            
            //function: IsEventCallbackExist
            //Returns true if the eventCallback exists on this GUI Object
            virtual bool IsEventCallbackExist(std::string eventCallbackName) const;

            //function: IsAnyEventCallbackExist
            //Generic version of <IsEventCallbackExist>. 
            //Reason for not having the same name can be found in <GetAnyEventCallback>
            template <typename T>
            bool IsAnyEventCallbackExist()
            {
                static_assert(std::is_base_of<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                static_assert(!std::is_same<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                return IsEventCallbackExist(T::EVENT_NAME);
            }
            
            //function: RemoveEventCallback
            //Removes the eventCallback by the name of it
            virtual void RemoveEventCallback(std::string eventCallbackName);

            //function: RemoveAnyEventCallback
            //Generic version of <RemoveEventCallback>. 
            //Reason for not having the same name can be found in <GetAnyExtension>
            template <typename T>
            void RemoveAnyEventCallback()
            {
                static_assert(std::is_base_of<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                static_assert(!std::is_same<ssGUI::EventCallbacks::EventCallback, T>::value, "Invalid Type to get event callback");
                RemoveEventCallback(T::EVENT_NAME);
            }

            //function: GetListOfEventCallbacks
            //Returns all the event callbacks on the GUI Object
            virtual std::vector<ssGUI::EventCallbacks::EventCallback*> GetListOfEventCallbacks();
    };
}

#endif