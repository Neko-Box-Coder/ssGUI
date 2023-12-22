#include "ssGUI/Backend/BackendFactory.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"

#ifdef SSGUI_MAIN_BACKEND_SDL2
    #include "ssGUI/Backend/SDL2/BackendDrawingSDL2.hpp"
    #include "ssGUI/Backend/SDL2/BackendSystemInputSDL2.hpp"
    #include "ssGUI/Backend/SDL2/BackendMainWindowSDL2.hpp"
    #include "ssGUI/Backend/FreeType/BackendFontFreeType.hpp"
    #include "ssGUI/Backend/stb_image/BackendImageStbImage.hpp"
    #include "SDL.h"
#endif

#ifdef SSGUI_MAIN_BACKEND_TEMPLATE
    #include "ssGUI/Backend/Template/BackendDrawingTemplate.hpp"
    #include "ssGUI/Backend/Template/BackendFontTemplate.hpp"
    #include "ssGUI/Backend/Template/BackendImageTemplate.hpp"
    #include "ssGUI/Backend/Template/BackendSystemInputTemplate.hpp"
    #include "ssGUI/Backend/Template/BackendMainWindowTemplate.hpp"
#endif

#ifdef SSGUI_MAIN_BACKEND_MOCK
    //NOTE: The Status macro is from xlib which conflicts with the Status variable in CppOverride
    #ifdef Status
        #undef Status
    #endif
    
    #include "ssGUI/Backend/Mocks/BackendDrawingMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendFontMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendImageMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendSystemInputMock.hpp"
    #include "ssGUI/Backend/Mocks/BackendMainWindowMock.hpp"
#endif

namespace ssGUI 
{ 
    
//TODO: Add exit code or log when no backends are found
namespace Backend
{
    BackendDrawingInterface* 
    BackendFactory::CreateBackendDrawingInterface(BackendMainWindowInterface* mainWindowInterface)
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<BackendDrawingInterface*>(new BackendDrawingSDL2(mainWindowInterface));
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<BackendDrawingInterface*>(new BackendDrawingTemplate(mainWindowInterface));
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<BackendDrawingInterface*>(new BackendDrawingMock(nullptr, 
                                                                                mainWindowInterface));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    BackendFontInterface* BackendFactory::CreateBackendFontInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<BackendFontInterface*>(new BackendFontFreeType());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<BackendFontInterface*>(new BackendFontTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<BackendFontInterface*>(new BackendFontMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    BackendImageInterface* BackendFactory::CreateBackendImageInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<BackendImageInterface*>(new BackendImageStbImage());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<BackendImageInterface*>(new BackendImageTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<BackendImageInterface*>(new BackendImageMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    BackendSystemInputInterface* BackendFactory::CreateBackendInputInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<BackendSystemInputInterface*>(new BackendSystemInputSDL2());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<BackendSystemInputInterface*>(new BackendSystemInputTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<BackendSystemInputInterface*>(new BackendSystemInputMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }
    
    BackendMainWindowInterface* BackendFactory::CreateBackendMainWindowInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<BackendMainWindowInterface*>(new BackendMainWindowSDL2());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<BackendMainWindowInterface*>(new BackendMainWindowTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<BackendMainWindowInterface*>(new BackendMainWindowMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }
    
    bool BackendFactory::Initialize()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            if(SDL_Init(SDL_INIT_VIDEO) != 0)
            {
                ssGUI_ERROR(ssGUI_BACKEND_TAG,  "SDL2 couldn't be initialized, error: " << 
                                                SDL_GetError());
                
                return false;
            }
        #endif
        
        return true;
    }
    
    bool BackendFactory::Cleanup()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            SDL_Quit();
        #endif
        
        return true;
    }
}

}