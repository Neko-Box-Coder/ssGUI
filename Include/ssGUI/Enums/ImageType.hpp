#ifndef SSGUI_IMAGE_TYPE_H
#define SSGUI_IMAGE_TYPE_H

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: ImageType

    RGB     -   RGB
    MONO    -   Grayscale, Not to be confused with FT_Pixel_Mode::FT_PIXEL_MODE_MONO.
                The correct corresponding binding to FreeType is FT_Pixel_Mode::FT_PIXEL_MODE_GRAY (if 8 bit)
    */
    enum class ImageType
    {
        RGB,
        MONO
    };
}

}

#endif