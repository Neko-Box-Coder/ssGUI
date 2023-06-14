#ifndef SSGUI_OUTPUT_STREAM_UTIL_H
#define SSGUI_OUTPUT_STREAM_UTIL_H

#include "glm/vec2.hpp"
#include "glm/vec4.hpp"
#include "ssGUI/Enums/WindowMode.hpp"
#include <codecvt>
#include <locale>
#include <ostream>
#include <vector>


#define SSGUI_OUTPUT_CLASS_NAME(className) "(" << #className <<": "
#define SSGUI_OUTPUT_VAR(varName) #varName << ": " << other.varName<<", "
#define SSGUI_OUTPUT_LAST_VAR(varName) #varName << ": " << other.varName << ")"


inline std::ostream& operator<<(std::ostream& stream, const wchar_t& character)
{
    stream << (int)character;
    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const uint8_t& character)
{
    stream << (int)character;
    return stream;
}

inline std::ostream& operator<<(std::ostream& stream, const std::wstring& str)
{
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    stream << converter.to_bytes(str);
    return stream;
}

namespace ssGUI
{
    template<   typename EnumType,
                typename = typename std::enable_if<std::is_enum<EnumType>::value>::type>
    inline std::ostream& operator<<(std::ostream& stream, const EnumType& enumVal)
    {
        stream << typeid(EnumType).name() << ": " << reinterpret_cast<const int&>(enumVal);
        return stream;
    }
    
    template<typename VecType>
    inline std::ostream& operator<<(std::ostream& stream, const std::vector<VecType>& vec)
    {
        stream << "(Vector: ";
        for(int i = 0; i < vec.size(); i++)
        {
            if(i != vec.size() - 1)
                stream << vec[i] << ", ";
            else
                stream << vec[i] << ")" << std::endl;
        }
        return stream;
    }
}

namespace glm
{
    inline std::ostream& operator<<(std::ostream& stream, const glm::vec2& vec)
    {
        stream << "(glm::vec2: " << vec.x << ", " << vec.y << ")";
        return stream;
    }

    inline std::ostream& operator<<(std::ostream& stream, const glm::ivec2& vec)
    {
        stream << "(glm::ivec2: " << vec.x << ", " << vec.y << ")";
        return stream;
    }
    
    inline std::ostream& operator<<(std::ostream& stream, const glm::u8vec4& vec)
    {
        stream << "(glm::u8vec4: " << (int)vec.r << ", " << (int)vec.g << ", " << (int)vec.b << ", " << (int)vec.a << ")";
        return stream;
    }
}

#endif