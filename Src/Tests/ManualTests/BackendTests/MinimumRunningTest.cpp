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

    //std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    //std::vector<glm::vec2> uv;
    //std::vector<glm::u8vec4> color {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    //std::vector<int> counts { 3};
    //std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};


    window->SetRenderSize(glm::ivec2(1280, 720));
    inputs->UpdateInput();

    while(!window->IsClosed())
    {
        //drawing->ClearBackBuffer(glm::u8vec4());
        inputs->UpdateInput();
        if(!window->IsClosed())
        {
            //drawing->DrawEntities(pos, uv, color, counts, props);
            drawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);

    return 0;
}

