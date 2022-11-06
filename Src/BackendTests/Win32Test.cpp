#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include <iostream>

int main()
{
    ssGUI::Backend::BackendDrawingWin32_OpenGL3_3 drawing;
    ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3 window;
    ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3 inputs;

    while(!window.IsClosed())
    {
        inputs.UpdateInput();

        if(!window.IsClosed())
            drawing.Render(glm::u8vec3());
    }

    //std::cout << "Test\n";


    return 0;
}