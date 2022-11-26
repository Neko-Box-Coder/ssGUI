#ifndef SSGUI_DRAW_CIRCLE
#define SSGUI_DRAW_CIRCLE

#include "glm/vec2.hpp"
#include <vector>
#include "cmath"

namespace ssGUI
{

namespace Helper
{
    inline double pi() { return std::atan(1) * 4; };
    inline void DrawCicle(std::vector<glm::vec2>& targetVec, glm::vec2 pos, glm::vec2 size, int numOfVertices, float fromAngle, float toAngle)
    {
        size *= 0.5;

        for(int i = 0; i < numOfVertices; i++)
        {
            float angle = 2.f * pi() * ((float)i / (float)numOfVertices);

            if(angle < fromAngle || angle > toAngle)
                continue;

            targetVec.push_back(pos + glm::vec2(size.x * cos(angle), size.y * sin(angle)) + size);
        }
    };
}

}

#endif