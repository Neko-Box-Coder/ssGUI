#ifndef SSGUI_DIRECTION_HPP
#define SSGUI_DIRECTION_HPP

#include "ssGUI/HelperClasses/EnumToStringMacro.hpp"
#include <cstdint>
#include <string>

namespace ssGUI
{

    //namespace: ssGUI::Enums
    namespace Enums
    {
        /*enum: Direction
        Bitfiled type that represents a combination of directions.

        TOP     - Top
        RIGHT   - Right
        BOTTOM  - Bottom
        LEFT    - Left
        COUNT   - Count
        */
        enum class Direction : uint8_t
        {
            TOP    = 1 << 0,
            RIGHT  = 1 << 1,
            BOTTOM = 1 << 2,
            LEFT   = 1 << 3,
            COUNT  = 1 << 4
        };

        namespace
        {
            inline std::string Internal_DirectionToString(Direction anchor)
            {
                static_assert((uint8_t)Direction::COUNT == 1 << 4, "ToString");
                switch(anchor)
                {
                    RETURN_ENUM_STRING(Direction::TOP);
                    RETURN_ENUM_STRING(Direction::RIGHT);
                    RETURN_ENUM_STRING(Direction::BOTTOM);
                    RETURN_ENUM_STRING(Direction::LEFT);
                    RETURN_ENUM_STRING(Direction::COUNT);
                }

                return "";
            }
        }

        inline ssGUI::Enums::Direction operator|(ssGUI::Enums::Direction a,
                                                 ssGUI::Enums::Direction b)
        {
            return static_cast<ssGUI::Enums::Direction>(static_cast<uint8_t>(a) |
                                                        static_cast<uint8_t>(b));
        };

        inline ssGUI::Enums::Direction operator&(ssGUI::Enums::Direction a,
                                                 ssGUI::Enums::Direction b)
        {
            return static_cast<ssGUI::Enums::Direction>(static_cast<uint8_t>(a) &
                                                        static_cast<uint8_t>(b));
        };

        inline bool operator==(ssGUI::Enums::Direction a, ssGUI::Enums::Direction b)
        {
            return (static_cast<uint8_t>(a & b) == static_cast<uint8_t>(b));
        };

        inline bool operator!=(ssGUI::Enums::Direction a, ssGUI::Enums::Direction b)
        {
            return !(a == b);
        };

        //function: DirectionToString
        inline std::string DirectionToString(Direction direction)
        {
            static_assert((uint8_t)Direction::COUNT == 1 << 4, "ToString");

            std::string curString;
            for(int i = 0; i < 4; ++i)
            {
                std::string returnString =
                    Internal_DirectionToString(direction & (Direction)(1 << i));
                if(!returnString.empty())
                    curString += returnString + ", ";
            }

            //Remove last comma
            if(!curString.empty())
                curString.erase(curString.begin() + curString.size() - 2);

            return curString;
        }
    }

}

#endif