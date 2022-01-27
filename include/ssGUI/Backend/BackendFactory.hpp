#ifndef SSGUI_BACKEND_FACTORY
#define SSGUI_BACKEND_FACTORY

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

#ifdef USE_SFML_BACKEND
    #include "ssGUI/Backend/SFML/BackendDrawingSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendFontSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendImageSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendSystemInputSFML.hpp"
    #include "ssGUI/Backend/SFML/BackendMainWindowSFML.hpp"
#endif


//namespace: ssGUI::Backend
namespace ssGUI::Backend
{
    //class: BackendFactory
    //BackendFactory is used to create backend objects without referencing the actual backend by using interfaces.
    class BackendFactory
    {
        public:
            //function: CreateBackendDrawingInterface
            static ssGUI::Backend::BackendDrawingInterface* CreateBackendDrawingInterface()
            {
                #ifdef USE_SFML_BACKEND
                    return static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingSFML());
                #endif
            }

            //function: CreateBackendFontInterface
            static ssGUI::Backend::BackendFontInterface* CreateBackendFontInterface()
            {
                #ifdef USE_SFML_BACKEND
                    return static_cast<ssGUI::Backend::BackendFontInterface*>(new ssGUI::Backend::BackendFontSFML());
                #endif
            }

            //function: CreateBackendImageInterface
            static ssGUI::Backend::BackendImageInterface* CreateBackendImageInterface()
            {
                #ifdef USE_SFML_BACKEND
                    return static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageSFML());
                #endif
            }

            //function: CreateBackendInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* CreateBackendInputInterface()
            {
                #ifdef USE_SFML_BACKEND
                    return static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputSFML());
                #endif
            }
            
            //function: CreateBackendMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* CreateBackendMainWindowInterface()
            {
                #ifdef USE_SFML_BACKEND
                    return static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowSFML());
                #endif
            }
    };
}


#endif