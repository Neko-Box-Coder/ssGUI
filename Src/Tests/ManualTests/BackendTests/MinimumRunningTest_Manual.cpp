#include "ssGUI/Backend/Factory.hpp"
#include "ssGUI/Backend/Interfaces/MainWindowInterface.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

#include <iostream>
#include <thread>
#include <vector>

int main()
{
    ssGUI::Backend::Factory::Initialize();
    
    ssGUI::Backend::MainWindowInterface* BackendWindow = 
        ssGUI::Backend::Factory::CreateMainWindowInterface();
    
    ssGUI::Backend::DrawingInterface* BackendDrawing = 
        ssGUI::Backend::Factory::CreateDrawingInterface(BackendWindow);
    
    ssGUI::Backend::SystemInputInterface* BackendInput = 
        ssGUI::Backend::Factory::CreateInputInterface();

    //std::vector<glm::vec2> pos {glm::ivec2(100, 100), glm::ivec2(100, 50), glm::ivec2(50, 50) };
    //std::vector<glm::vec2> uv;
    //std::vector<glm::u8vec4> color {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    //std::vector<int> counts { 3};
    //std::vector<ssGUI::DrawingProperty> props {ssGUI::DrawingProperty()};


    BackendWindow->SetRenderSize(glm::ivec2(1280, 720));
    BackendInput->UpdateInput(&BackendWindow, 1);

    while(!BackendWindow->IsClosed())
    {
        //drawing->ClearBackBuffer(glm::u8vec4());
        BackendInput->UpdateInput(&BackendWindow, 1);
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
    ssGUI::Backend::Factory::Cleanup();

    return 0;
}

