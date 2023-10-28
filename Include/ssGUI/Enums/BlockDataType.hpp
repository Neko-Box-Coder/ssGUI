#ifndef SSGUI_BLOCK_DATA_TYPE_HPP
#define SSGUI_BLOCK_DATA_TYPE_HPP

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <cstdint>
#include <string>

namespace ssGUI
{
    //namespace: ssGUI::Enums
    namespace Enums
    {
        /*enum: BlockDataType
    
        NONE            - Nothing is blocking 
        GUI_OBJECT      - <ssGUI::GUIObject> is blocking
        EXTENSION       - <ssGUI::Extensions::Extension> is blocking
        EVENT_CALLBACK  - <ssGUI::EventCallback> is blocking
        CUSTOM          - Custom data (such as user event) is blocking
        COUNT            - Count
        */
        enum class BlockDataType : uint16_t
        {
            NONE,
            GUI_OBJECT,
            EXTENSION,
            EVENT_CALLBACK,
            CUSTOM,
            COUNT   
        };
        
        //function: BlockDataTypeToString
        inline std::string BlockDataTypeToString(BlockDataType input)
        {
            static_assert((int)BlockDataType::COUNT == 5, "ToString");
            switch(input)
            {
                RETURN_ENUM_STRING(BlockDataType::NONE);
                RETURN_ENUM_STRING(BlockDataType::GUI_OBJECT);
                RETURN_ENUM_STRING(BlockDataType::EXTENSION);
                RETURN_ENUM_STRING(BlockDataType::EVENT_CALLBACK);
                RETURN_ENUM_STRING(BlockDataType::CUSTOM);
                RETURN_ENUM_STRING(BlockDataType::COUNT);
            }
            
            return "";
        }
    }
}

#endif