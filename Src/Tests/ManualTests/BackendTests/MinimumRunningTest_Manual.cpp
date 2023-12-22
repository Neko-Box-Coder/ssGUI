#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Backend/Interfaces/BackendMainWindowInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <iostream>
#include <thread>
#include <vector>

int main()
{
    ssGUI::Backend::BackendFactory::Initialize();
    
    ssGUI::Backend::BackendMainWindowInterface* BackendWindow = 
        ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    
    ssGUI::Backend::BackendDrawingInterface* BackendDrawing = 
        ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface(BackendWindow);
    
    ssGUI::Backend::BackendSystemInputInterface* BackendInput = 
        ssGUI::Backend::BackendFactory::CreateBackendInputInterface();

    //std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    //std::vector<glm::vec2> uv;
    //std::vector<glm::u8vec4> color {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    //std::vector<int> counts { 3};
    //std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};


    BackendWindow->SetRenderSize(glm::ivec2(1280, 720));
    BackendInput->UpdateInput(BackendWindow, 1);

    while(!BackendWindow->IsClosed())
    {
        //drawing->ClearBackBuffer(glm::u8vec4());
        BackendInput->UpdateInput(BackendWindow, 1);
        if(!BackendWindow->IsClosed())
        {
            //drawing->CreateDrawingEntities(pos, uv, color, counts, props);
            BackendDrawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    ssGUI::Factory::Dispose(BackendDrawing);
    ssGUI::Factory::Dispose(BackendWindow);
    ssGUI::Factory::Dispose(BackendInput);
    ssGUI::Backend::BackendFactory::Cleanup();

    return 0;
}

