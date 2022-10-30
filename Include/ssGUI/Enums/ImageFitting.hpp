#ifndef SSGUI_IMAGE_FITTING
#define SSGUI_IMAGE_FITTING

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
    */
    enum class ImageFitting
    {
        FIT_HORIZONTAL,
        FIT_VERTICAL,
        FIT_WHOLE_IMAGE,
        FIT_WHOLE_AREA
    };
}

}

#endif