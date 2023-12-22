#ifndef SSGUI_BACKEND_FACTORY_HPP
#define SSGUI_BACKEND_FACTORY_HPP

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
    //BackendFactory is used to create backend objects without referencing 
    //the actual backend by using interfaces.
    class BackendFactory
    {
        //TODO: Add exit code or log when no backends are found
        public:
            //function: CreateBackendDrawingInterface
            static BackendDrawingInterface* 
                CreateBackendDrawingInterface(BackendMainWindowInterface* mainWindowInterface);
            
            //function: CreateBackendFontInterface
            static BackendFontInterface* CreateBackendFontInterface();

            //function: CreateBackendImageInterface
            static BackendImageInterface* CreateBackendImageInterface();

            //function: CreateBackendInputInterface
            static BackendSystemInputInterface* CreateBackendInputInterface();
            
            //function: CreateBackendMainWindowInterface
            static BackendMainWindowInterface* CreateBackendMainWindowInterface();
            
            static bool Initialize();
            static bool Cleanup();
    };
}

}

#endif