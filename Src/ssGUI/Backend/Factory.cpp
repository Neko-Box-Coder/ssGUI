#include "ssGUI/Backend/Factory.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

#ifdef SSGUI_MAIN_BACKEND_SDL2
    #include "ssGUI/Backend/SDL2/DrawingSDL2.hpp"
    #include "ssGUI/Backend/SDL2/SystemInputSDL2.hpp"
    #include "ssGUI/Backend/SDL2/MainWindowSDL2.hpp"
    #include "ssGUI/Backend/FreeType/FontFreeType.hpp"
    #include "ssGUI/Backend/stb_image/ImageStbImage.hpp"
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
    DrawingInterface* 
    Factory::CreateDrawingInterface(MainWindowInterface* mainWindowInterface)
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<DrawingInterface*>(new DrawingSDL2(mainWindowInterface));
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<DrawingInterface*>(new BackendDrawingTemplate(mainWindowInterface));
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<DrawingInterface*>(new BackendDrawingMock(nullptr, 
                                                                                mainWindowInterface));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    FontInterface* Factory::CreateFontInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<FontInterface*>(new FontFreeType());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<FontInterface*>(new BackendFontTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<FontInterface*>(new BackendFontMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    ImageInterface* Factory::CreateImageInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<ImageInterface*>(new ImageStbImage());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<ImageInterface*>(new BackendImageTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<ImageInterface*>(new BackendImageMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }

    SystemInputInterface* Factory::CreateInputInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<SystemInputInterface*>(new SystemInputSDL2());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<SystemInputInterface*>(new BackendSystemInputTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<SystemInputInterface*>(new BackendSystemInputMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }
    
    MainWindowInterface* Factory::CreateMainWindowInterface()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            return static_cast<MainWindowInterface*>(new MainWindowSDL2());
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<MainWindowInterface*>(new BackendMainWindowTemplate());
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<MainWindowInterface*>(new BackendMainWindowMock(nullptr));
        #else
            ssGUI_ERROR(ssGUI_BACKEND_TAG, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    }
    
    bool Factory::Initialize()
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
    
    bool Factory::Cleanup()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            SDL_Quit();
        #endif
        
        return true;
    }
}

}