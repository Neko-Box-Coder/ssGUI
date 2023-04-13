#ifndef SSGUI_IMAGE_FITTING_H
#define SSGUI_IMAGE_FITTING_H

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <string>
namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ImageFitting

    FIT_HORIZONTAL  - Fit horizontally
    FIT_VERTICAL    - Fit vertically
    FIT_WHOLE_IMAGE - Showing the whole image
    FIT_WHOLE_AREA  - Image will cover the whole image widget area
    COUNT           - Count
    */
    enum class ImageFitting
    {
        FIT_HORIZONTAL,
        FIT_VERTICAL,
        FIT_WHOLE_IMAGE,
        FIT_WHOLE_AREA,
        COUNT
    };
    
    //function: ImageFittingToString
    inline std::string ImageFittingToString(ImageFitting fitting)
    {
        static_assert((int)ImageFitting::COUNT == 4, "ToString");
        switch(fitting)
        {
            RETURN_ENUM_STRING(ImageFitting::FIT_HORIZONTAL);
            RETURN_ENUM_STRING(ImageFitting::FIT_VERTICAL);
            RETURN_ENUM_STRING(ImageFitting::FIT_WHOLE_IMAGE);
            RETURN_ENUM_STRING(ImageFitting::FIT_WHOLE_AREA);
            RETURN_ENUM_STRING(ImageFitting::COUNT);
        }
        
        return "";
    }
}

}

#endif