#ifndef SSGUI_EXTENSION_H
#define SSGUI_EXTENSION_H

#include "ssGUI/Factory.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/DataClasses/InputStatus.hpp"
#include "ssGUI/DataClasses/ObjectsReferences.hpp"
#include "glm/vec2.hpp"
#include <string>
#include <type_traits>

namespace ssGUI
{
    class GUIObject;
}

namespace ssGUI 
{
    
//namespace: ssGUI::Extensions
namespace Extensions
{
    //class: ssGUI::Extensions::Extension
    //Extension provides additional functionality to a GUI object, without the need to create a new type of GUI object.
    //You cannot attach multiple extensions of the same type to the same GUI Object. If needed, consider attaching to an empty widget/window instead.
    class Extension
    {
        public:
            friend class ssGUI::Factory;
        
        protected:
            Extension() = default;
            Extension(Extension const &) = default;
            Extension& operator=(Extension const &) = default;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};
            virtual void ConstructRenderInfo() = 0;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) = 0;

        public:
            virtual ~Extension() = 0;

            //virtual Extension* Create() = 0;

            //virtual Extension* Dispose() = 0;

            //function: SetEnabled
            //If false, this extension is disabled and will not provide and functionality
            virtual void SetEnabled(bool enabled) = 0;

            //function: IsEnabled
            //If false, this extension is disabled and will not provide and functionality
            virtual bool IsEnabled() const = 0;

            //function: Internal_Update
            //Updates function called every frame
            virtual void Internal_Update(   bool isPreUpdate, 
                                            ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                            ssGUI::InputStatus& currentInputStatus, 
                                            ssGUI::InputStatus& lastInputStatus, 
                                            ssGUI::GUIObject* mainWindow) = 0;
            
            //function: Internal_Draw
            //Renders function called every frame
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) = 0;
            
            //function: GetExtensionName
            //Returns this extension's name
            virtual std::string GetExtensionName() const = 0;
            
            //TODO: CHange this to internal

            //function: BindToObject
            //Binds this extension to a GUI object
            virtual void BindToObject(ssGUI::GUIObject* bindObj) = 0;
            
            //function: Copy
            //Copy all values of this extension to another extension
            virtual void Copy(const ssGUI::Extensions::Extension* extension) = 0;

            //function: Internal_GetObjectsReferences
            //Returns all the referenced GUI Objects. If nullptr is returned, this extension is not referencing any GUI Object (exception for container).
            virtual ObjectsReferences* Internal_GetObjectsReferences() = 0;

            //function: Clone
            //Clones this extension to a new extension that can be attached to a different container
            virtual Extension* Clone() = 0;
    };

    inline Extension::~Extension(){}   //Pure virtual destructor needs to be defined
}

}


#endif