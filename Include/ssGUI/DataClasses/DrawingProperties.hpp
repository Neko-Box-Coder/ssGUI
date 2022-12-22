#ifndef SSGUI_DRAWING_PROPERTIES_H
#define SSGUI_DRAWING_PROPERTIES_H

#include "ssGUI/Backend/Interfaces/BackendFontInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendImageInterface.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //struct: ssGUI::DrawingProperty
    //Structure used to pass to BackendDrawing to draw entities
    struct DrawingProperty
    {
        public:
            //var: character
            uint32_t character = 0;

            //var: fontP
            ssGUI::Backend::BackendFontInterface* fontP = nullptr;
            
            //var: characterSize
            int characterSize = -1;

            //var: imageP
            ssGUI::Backend::BackendImageInterface* imageP = nullptr;
    };
}

#endif