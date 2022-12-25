#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include <iostream>
#include <vector>

int main()
{
    ssGUI::Backend::BackendDrawingWin32_OpenGL3_3 drawing;
    ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3 window;
    ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3 inputs;

    //std::vector<glm::vec2> pos {glm::ivec2(50, 50), glm::ivec2(100, 50), glm::ivec2(100, 100), glm::ivec2(50, 100)};
    std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    std::vector<glm::vec2> uv;
    std::vector<glm::u8vec4> color {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    std::vector<int> counts { 3};
    std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};

    //drawing.ClearBackBuffer(glm::u8vec4());
    //drawing.DrawEntities(pos, uv, color, counts, props);

    while(!window.IsClosed())
    {
        drawing.ClearBackBuffer(glm::u8vec4());
        inputs.UpdateInput();
        if(!window.IsClosed())
        {
            drawing.DrawEntities(pos, uv, color, counts, props);
            drawing.Render(glm::u8vec3(255, 255, 255));
        }
    }

    //std::cout << "Test\n";


    return 0;
}

