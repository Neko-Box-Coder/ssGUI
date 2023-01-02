//#include "ssGUI/Backend/Win32_OpenGL3_3/BackendMainWindowWin32_OpenGL3_3.hpp"
//#include "ssGUI/Backend/Win32_OpenGL3_3/BackendSystemInputWin32_OpenGL3_3.hpp"
//#include "ssGUI/Backend/Win32_OpenGL3_3/BackendDrawingWin32_OpenGL3_3.hpp"

#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssLogger/ssLog.hpp"

#include <iostream>
#include <thread>
#include <unistd.h>
#include <vector>

int main()
{
    ssGUI::Backend::BackendDrawingInterface* drawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    ssGUI::Backend::BackendMainWindowInterface* window = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    ssGUI::Backend::BackendSystemInputInterface* inputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();

    //ssGUI::Backend::BackendDrawingWin32_OpenGL3_3 drawing;
    //ssGUI::Backend::BackendMainWindowWin32_OpenGL3_3 window;
    //ssGUI::Backend::BackendSystemInputWin32_OpenGL3_3 inputs;

    //std::vector<glm::vec2> pos {glm::ivec2(50, 50), glm::ivec2(100, 50), glm::ivec2(100, 100), glm::ivec2(50, 100)};
    std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    std::vector<glm::vec2> uv;
    std::vector<glm::u8vec4> color {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    std::vector<int> counts { 3};
    std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};

    //drawing.ClearBackBuffer(glm::u8vec4());
    //drawing.DrawEntities(pos, uv, color, counts, props);
    int count = 0;

    window->SetRenderSize(glm::ivec2(1280, 720));
    inputs->UpdateInput();

    //ssGUI::ImageData testImgData;
    //testImgData.LoadFromPath("../Resources/sd.png");
    //sleep(1);
    //window->SetIcon(*testImgData.GetBackendImageInterface());
    
    //sleep(1);
    //window->SetWindowMode(ssGUI::Enums::WindowMode::FULLSCREEN);
    //sleep(1);
    //window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);



    while(!window->IsClosed())
    {
        drawing->ClearBackBuffer(glm::u8vec4());
        inputs->UpdateInput();
        if(!window->IsClosed())
        {
            drawing->DrawEntities(pos, uv, color, counts, props);
            drawing->Render(glm::u8vec3(255, 255, 255));
        }
           
        //Every 5 seconds
        //ssLOG_LINE("Frame: "<<count);
        if(count >= 300)
        {
            ssLOG_LINE("5 seconds past");
            //window->SetWindowPosition(glm::ivec2(0, 0));
            //glm::ivec2 windowPos = window->GetWindowPosition();
            //ssLOG_LINE("windowPos: "<<windowPos.x<<", "<<windowPos.y);
            
            //window->SetTitlebar(!window->HasTitlebar());
            //window->SetTitle(L"ウインドウです！");
            //window->SetResizable(!window->IsResizable());
            //window->SetCloseButton(false);
            //window->SetVSync(true);
            //window->SetVisible(!window->IsVisible());
            //glm::ivec2 offset = window->GetPositionOffset();
            //ssLOG_LINE("offset: "<<offset.x<<", "<<offset.y);
            //window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
            
            //ssLOG_LINE("VSync: "<< window->IsVSync());
            //{
            //    window->SetWindowMode((ssGUI::Enums::WindowMode)(2 - windowMode ));
            //    windowMode++;
            //    windowMode %= 3;
            //}
            count = 0;
        }
        
        count++;
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        
    }
    
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);

    //std::cout << "Test\n";


    return 0;
}

