#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"

//Readme example
using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;
    mainWindow.SetRenderSize(glm::vec2(450, 80));
    mainWindow.AddExtension<ssGUI::Extensions::Layout>();

    auto* text = mainWindow.AddChild<ssGUI::Text>();
    text->SetText("Click on the button to show the message");
    
    auto* button = mainWindow.AddChildWithWrapper<ssGUI::StandardButton>(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);
    button->SetSize(glm::vec2(50, 30));
                                                                                
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject(&mainWindow);
    guiManager.AddPostGUIUpdateEventListener                                    
    (
        [&]()
        {
            if(button->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                text->SetText(L"(`oωo´)");
        }
    );
    guiManager.StartRunning();
    return 0;
}