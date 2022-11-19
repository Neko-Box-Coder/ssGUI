#ifndef SSGUI_BACKEND_FACTORY
#define SSGUI_BACKEND_FACTORY

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#ifdef SSGUI_MAIN_BACKEND_SFML
    #include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendFontSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#endif

#ifdef SSGUI_FONT_BACKEND_FREE_TYPE
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
#endif


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
            static ssGUI::Backend::BackendDrawingInterface* CreateBackendDrawingInterface()
            {
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingSFML());
                #endif

                return nullptr;
            }

            //function: CreateBackendFontInterface
            static ssGUI::Backend::BackendFontInterface* CreateBackendFontInterface()
            {
                #ifdef SSGUI_FONT_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontSFML());
                #elif defined SSGUI_FONT_BACKEND_FREE_TYPE
                    return static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontFreeType());
                #endif
                
                return nullptr;
            }

            //function: CreateBackendImageInterface
            static ssGUI::Backend::BackendImageInterface* CreateBackendImageInterface()
            {
                #ifdef SSGUI_IMAGE_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageSFML());
                #endif
                return nullptr;
            }

            //function: CreateBackendInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* CreateBackendInputInterface()
            {
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputSFML());
                #endif
                return nullptr;
            }
            
            //function: CreateBackendMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* CreateBackendMainWindowInterface()
            {
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowSFML());
                #endif
                return nullptr;
            }
    };
}

}

#endif