#ifndef SSGUI_DRAWING_PROPERTIES
#define SSGUI_DRAWING_PROPERTIES

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
            //DrawingProperty& operator=(DrawingProperty const& other) = default;

            //var: fontP
            ssGUI::Backend::BackendFontInterface* fontP = nullptr;
            
            //var: characterSize
            int characterSize = -1;

            //var: imageP
            ssGUI::Backend::BackendImageInterface* imageP = nullptr;
    };
}

#endif