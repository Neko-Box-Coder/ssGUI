#ifndef SSGUI_Factory_H
#define SSGUI_Factory_H

#include <type_traits>

//namespace: ssGUI
namespace ssGUI
{   
    class GUIObject;
    
    //class: ssGUI::Factory
    //<Extensions: Extensions::Extension> & <EventCallbacks: EventCallback> must be created and disposed via this class.
    //You can also create GUI Objects via this class but it is not required.
    class Factory
    {
        public:
            static void SetHeapAllocatedFlagForGUIObject(ssGUI::GUIObject* obj);
            
            //function: Create
            //Creates the object on the heap
            template< template<typename T> class Wrapper, typename T> 
            static Wrapper<T>* Create()
            {
                return new Wrapper<T>();
            }

            //function: Create 
            //Creates the object on the heap
            template<typename T> 
            static T* Create()
            {
                if(std::is_base_of<ssGUI::GUIObject, T>::value)
                {
                    T* guiObj = new T();
                    // For some reason GCC is checing in compile time with dynamic cast, so this won't compile
                    // SetHeapAllocatedFlagForGUIObject(dynamic_cast<ssGUI::GUIObject*>(guiObj));
                    SetHeapAllocatedFlagForGUIObject((ssGUI::GUIObject*)(guiObj));
                    return guiObj;
                }
                
                return new T();
            }

            //function: Dispose
            //Delete the object allocated on the heap
            template<typename T>
            static void Dispose(T* obj)
            {
                //static_assert(std::is_base_of<ssGUI::Extensions::Extension, T>::value);
                delete obj;
            }
    };
    
    //namespace: ssGUI

    //function: Create
    //See <ssGUI::Factory::Create>
    template< template<typename T> class Wrapper, typename T> 
    Wrapper<T>* Create()
    {
        return ssGUI::Factory::Create<Wrapper<T>, T>();
    }
    
    //function: Create
    //See <ssGUI::Factory::Create>
    template<typename T> 
    T* Create()
    {
        return ssGUI::Factory::Create<T>();
    }
    
    //function: Dispose
    //See <ssGUI::Factory::Dispose>
    template<typename T>
    void Dispose(T* obj)
    {
        ssGUI::Factory::Dispose(obj);
    }
    
    //function: CleanUpDefaultResources
    //Cleans up all default resources used by ssGUI. This is called automatically by <ssGUIManager>
    void CleanUpDefaultResources();
}

#endif