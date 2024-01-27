#ifndef SSGUI_BACKEND_FACTORY_HPP
#define SSGUI_BACKEND_FACTORY_HPP

#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include "ssGUI/Backend/Interfaces/FontInterface.hpp"
#include "ssGUI/Backend/Interfaces/ImageInterface.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"

namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    //class: ssGUI::Backend::Factory
    //Factory is used to create backend objects without referencing 
    //the actual backend by using interfaces.
    class Factory
    {
        //TODO: Add exit code or log when no backends are found
        public:
            //function: CreateDrawingInterface
            static DrawingInterface* 
                CreateDrawingInterface(MainWindowInterface* mainWindowInterface);
            
            //function: CreateFontInterface
            static FontInterface* CreateFontInterface();

            //function: CreateImageInterface
            static ImageInterface* CreateImageInterface();

            //function: CreateInputInterface
            static SystemInputInterface* CreateInputInterface();
            
            //function: CreateMainWindowInterface
            static MainWindowInterface* CreateMainWindowInterface();
            
            static bool Initialize();
            static bool Cleanup();
    };
}

}

#endif