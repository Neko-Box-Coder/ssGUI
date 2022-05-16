#ifndef SSGUI_REALTIME_INPUT_INFO
#define SSGUI_REALTIME_INPUT_INFO

#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/DataClasses/KeyPresses.hpp"
#include "ssGUI/Enums/MouseButton.hpp"
#include "glm/vec2.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: RealtimeInputInfo
    //An input info object that records the input status for each input changes within a frame
    class RealtimeInputInfo
    {
        public:
            //var: CurrentKeyChanged
            ssGUI::KeyPresses CurrentKeyChanged = ssGUI::KeyPresses();
            
            //var: CurrentKeyPresses
            ssGUI::KeyPresses CurrentKeyPresses = ssGUI::KeyPresses();
            
            //var: CurrentMouseButtonChanged
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtonChanged = std::vector<ssGUI::Enums::MouseButton>();
            
            //var: CurrentMouseButtonPresses
            std::vector<ssGUI::Enums::MouseButton> CurrentMouseButtonPresses = std::vector<ssGUI::Enums::MouseButton>();
            
            //var: MouseMoved
            bool MouseMoved = false;

            //var: CurrentMousePosition
            glm::ivec2 CurrentMousePosition = glm::ivec2();
    };

    //namespace: ssGUI
    //function: GetRelativeMousePosition
    //A helper function that can return the relative position to mainWindow. Normally use with <RealtimeInputInfo>
    inline glm::ivec2 GetRelativeMousePosition(glm::ivec2 mousePos, ssGUI::MainWindow* mainWindow)
    {
        return mousePos - mainWindow->GetDisplayPosition() - mainWindow->GetPositionOffset();
    };
}

#endif
