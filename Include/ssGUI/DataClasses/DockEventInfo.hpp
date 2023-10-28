#ifndef SSGUI_DOCK_EVENT_INFO_HPP
#define SSGUI_DOCK_EVENT_INFO_HPP

#include "ssGUI/Enums/DockSide.hpp"
namespace ssGUI
{
    //struct: ssGUI::DockEventInfo
    struct DockEventInfo
    {
        public:
            //var: AbortDocking
            bool AbortDocking = false;
            
            //var: CurrentDockSide
            ssGUI::Enums::DockSide CurrentDockSide;
    };
}


#endif