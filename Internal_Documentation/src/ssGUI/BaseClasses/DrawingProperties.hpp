#ifndef SSGUI_DRAWING_PROPERTIES
#define SSGUI_DRAWING_PROPERTIES

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: DrawingProperty
    struct DrawingProperty
    {
        public:
            //var: fontP
            const ssGUI::Backend::BackendFontInterface* fontP = nullptr;
            
            //var: characterSize
            const int characterSize = -1;

            //var: imageP
            const ssGUI::Backend::BackendImageInterface* imageP = nullptr;
    };
}

#endif