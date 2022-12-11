#ifndef SSGUI_BACKEND_FACTORY_H
#define SSGUI_BACKEND_FACTORY_H

#include "ssLogger/ssLog.hpp"

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


#ifdef SSGUI_MAIN_BACKEND_WIN32_OPENGL
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"
    #include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
#endif

#ifdef SSGUI_FONT_BACKEND_FREE_TYPE
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
#endif

#ifdef SSGUI_IMAGE_BACKEND_STB_IMAGE
    #include "ssGUI/Backend/stb_image/BackendImageStbImage.hpp"
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
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    return static_cast<ssGUI::Backend::BackendDrawingInterface*>(new ssGUI::Backend::BackendDrawingWin32_OpenGL3_3());
                #else
                    ssLOG_LINE("Unimplemented backend");
                    ssLOG_EXIT_PROGRAM();
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
                #else
                    ssLOG_LINE("Unimplemented backend");
                    ssLOG_EXIT_PROGRAM();
                #endif
                
                return nullptr;
            }

            //function: CreateBackendImageInterface
            static ssGUI::Backend::BackendImageInterface* CreateBackendImageInterface()
            {
                #ifdef SSGUI_IMAGE_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageSFML());
                #elif defined SSGUI_IMAGE_BACKEND_STB_IMAGE
                    return static_cast<ssGUI::Backend::BackendImageInterface*>(new ssGUI::Backend::BackendImageStbImage());
                #else
                    ssLOG_LINE("Unimplemented backend");
                    ssLOG_EXIT_PROGRAM();
                #endif
                return nullptr;
            }

            //function: CreateBackendInputInterface
            static ssGUI::Backend::BackendSystemInputInterface* CreateBackendInputInterface()
            {
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputSFML());
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    return static_cast<ssGUI::Backend::BackendSystemInputInterface*>(new ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3());
                #else
                    ssLOG_LINE("Unimplemented backend");
                    ssLOG_EXIT_PROGRAM();
                #endif
                return nullptr;
            }
            
            //function: CreateBackendMainWindowInterface
            static ssGUI::Backend::BackendMainWindowInterface* CreateBackendMainWindowInterface()
            {
                #ifdef SSGUI_MAIN_BACKEND_SFML
                    return static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowSFML());
                #elif defined SSGUI_MAIN_BACKEND_WIN32_OPENGL
                    return static_cast<ssGUI::Backend::BackendMainWindowInterface*>(new ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3());
                #else
                    ssLOG_LINE("Unimplemented backend");
                    ssLOG_EXIT_PROGRAM();
                #endif
                return nullptr;
            }
    };
}

}

#endif