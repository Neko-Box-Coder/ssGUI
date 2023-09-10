#ifndef SSGUI_DRAG_DATA_TYPE_HPP
#define SSGUI_DRAG_DATA_TYPE_HPP

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <cstdint>
#include <string>

namespace ssGUI
{

//namespace: ssGUI::Enums
namespace Enums
{
    /*enum: DragDataType
    NONE            - Nothing is being dragged
    GUI_OBJECT      - GUI Object is being dragged
    CUSTOM          - Custom data is being dragged
    COUNT           - Count
    */
    enum class DragDataType : uint16_t
    {
        NONE,
        GUI_OBJECT,
        CUSTOM,
        COUNT   
    };
    
    //function: DragDataTypeToString
    inline std::string DragDataTypeToString(DragDataType input)
    {
        static_assert((int)DragDataType::COUNT == 3, "ToString");
        switch(input)
        {
            RETURN_ENUM_STRING(DragDataType::NONE);
            RETURN_ENUM_STRING(DragDataType::GUI_OBJECT);
            RETURN_ENUM_STRING(DragDataType::CUSTOM);
            RETURN_ENUM_STRING(DragDataType::COUNT);
        }
        
        return "";
    }
}

}

#endif