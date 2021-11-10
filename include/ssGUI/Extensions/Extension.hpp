#ifndef SSGUI_EXTENSION
#define SSGUI_EXTENSION


#include <string>
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/BaseClasses/InputStatus.hpp"
#include "glm/vec2.hpp"

namespace ssGUI
{
    class GUIObject;
}

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{   
    //class: Extension
    class Extension
    {
        public:
            virtual ~Extension() = 0;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) = 0;
            
            //function: Draw
            virtual void Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) = 0;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() = 0;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) = 0;
            
            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) = 0;
    };

    inline Extension::~Extension(){}   //Pure virtual destructor needs to be defined
}

#endif