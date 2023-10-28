#ifndef SSGUI_TAGS_H
#define SSGUI_TAGS_H

#include <string>

namespace ssGUI 
{ 
    
//namespace: ssGUI::Tags
namespace Tags
{
    //var: OVERLAY
    //Overlay tag will always draw the GUI Object in the front
    const std::string OVERLAY = "overlay";
    
    //var: FLOATING
    //Floating tag will bypass layout position and size override
    const std::string FLOATING = "floating";
}

}

#endif