#ifndef SSGUI_BACKEND_FACTORY_HPP
#define SSGUI_BACKEND_FACTORY_HPP

namespace ssGUI 
{ 
    
//namespace: ssGUI::Backend
namespace Backend
{
    class BackendDrawingInterface;
    class BackendFontInterface;
    class BackendImageInterface;
    class BackendSystemInputInterface;
    class BackendMainWindowInterface;

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
    };
}

}

#endif