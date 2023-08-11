#ifndef SSGUI_BACKEND_FACTORY_H
#define SSGUI_BACKEND_FACTORY_H

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::BackendFactory
    //BackendFactory is used to create backend objects without referencing the actual backend by using interfaces.
    class BackendFactory
    {
        //TODO: Add exit code or log when no backends are found
        public:
            //function: CreateBackendDrawingInterface
            static ssGUI::Backend::BackendDrawingInterface* CreateBackendDrawingInterface();
            
            //function: CreateBackendFontInterface
            static ssGUI::Backend::BackendFontInterface* CreateBackendFontInterface();

            //function: CreateBackendImageInterface
            static ssGUI::Backend::BackendImageInterface* CreateBackendImageInterface();

            //function: CreateBackendInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* CreateBackendInputInterface();
            
            //function: CreateBackendMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* CreateBackendMainWindowInterface();
    };
}

}

#endif