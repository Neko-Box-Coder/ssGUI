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
    DrawingInterface* Factory::CreateDrawingInterface(MainWindowInterface* mainWindowInterface)
    {
        DrawingInterface* drawingInterface = nullptr;
        
        #if SSGUI_MAIN_BACKEND_SDL2
            drawingInterface = new DrawingSDL2();
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            return static_cast<DrawingInterface*>(new BackendDrawingTemplate(mainWindowInterface));
        #elif SSGUI_MAIN_BACKEND_MOCK
            return static_cast<DrawingInterface*>(new BackendDrawingMock(nullptr, 
                                                                                mainWindowInterface));
        #else
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
        
        if(!drawingInterface->Initialize(mainWindowInterface))
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Drawing Interface wasn't initialized successfully");
            delete drawingInterface;
            return nullptr;
        }
        
        return drawingInterface;
    }

    FontInterface* Factory::CreateFontInterface()
    {
        FontInterface* fontInterface = nullptr;
        
        #if SSGUI_MAIN_BACKEND_SDL2
            fontInterface = new FontFreeType();
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            fontInterface = new BackendFontTemplate();
        #elif SSGUI_MAIN_BACKEND_MOCK
            fontInterface = new BackendFontMock(nullptr);
        #else
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
        
        if(!fontInterface->Initialize())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Font Interface wasn't initialized successfully");
            delete fontInterface;
            return nullptr;
        }
        
        return fontInterface;
    }

    ImageInterface* Factory::CreateImageInterface()
    {
        ImageInterface* imageInterface = nullptr;
        
        #if SSGUI_MAIN_BACKEND_SDL2
            imageInterface = new ImageStbImage();
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            imageInterface = new BackendImageTemplate();
        #elif SSGUI_MAIN_BACKEND_MOCK
            imageInterface = new BackendImageMock(nullptr);
        #else
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
    
        if(!imageInterface->Initialize())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Image Interface wasn't initialized successfully");
            delete imageInterface;
            return nullptr;
        }
        
        return imageInterface;
    }

    SystemInputInterface* Factory::CreateInputInterface()
    {
        SystemInputInterface* inputInterface = nullptr;
        
        #if SSGUI_MAIN_BACKEND_SDL2
            inputInterface = new SystemInputSDL2();
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            inputInterface = new BackendSystemInputTemplate();
        #elif SSGUI_MAIN_BACKEND_MOCK
            inputInterface = new BackendSystemInputMock(nullptr);
        #else
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
        
        if(!inputInterface->Initialize())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Input Interface wasn't initialized successfully");
            delete inputInterface;
            return nullptr;
        }
        
        return inputInterface;
    }
    
    MainWindowInterface* Factory::CreateMainWindowInterface()
    {
        MainWindowInterface* mainWindowInterface = nullptr;
        
        #if SSGUI_MAIN_BACKEND_SDL2
            mainWindowInterface = new MainWindowSDL2();
        #elif SSGUI_MAIN_BACKEND_TEMPLATE
            mainWindowInterface = new BackendMainWindowTemplate();
        #elif SSGUI_MAIN_BACKEND_MOCK
            mainWindowInterface = new BackendMainWindowMock(nullptr);
        #else
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Unimplemented backend");
            ssLOG_EXIT_PROGRAM();
            return nullptr;
        #endif
        
        if(!mainWindowInterface->Initialize())
        {
            ssGUI_ERROR(ssGUI_TAG_BACKEND, "Main Window Interface wasn't initialized successfully");
            delete mainWindowInterface;
            return nullptr;
        }
        
        return mainWindowInterface;
    }
    
    bool Factory::Initialize()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            if(SDL_Init(SDL_INIT_VIDEO) != 0)
            {
                ssGUI_ERROR(ssGUI_TAG_BACKEND,  "SDL2 couldn't be initialized, error: " << 
                                                SDL_GetError());
                
                return false;
            }
            
            if(!FontFreeType::InitializeFreeType())
            {
                ssGUI_ERROR(ssGUI_TAG_BACKEND, "FreeType couldn't be initialized");
                return false;
            }
        #endif
        
        return true;
    }
    
    bool Factory::Cleanup()
    {
        #if SSGUI_MAIN_BACKEND_SDL2
            SDL_Quit();
            
            if(!FontFreeType::CleanupFreeType())
            {
                ssGUI_ERROR(ssGUI_TAG_BACKEND, "FreeType couldn't be cleaned up");
                return false;
            }
        #endif
        
        return true;
    }
}

}