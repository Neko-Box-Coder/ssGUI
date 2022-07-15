#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    // ssGUI::StandardWindow window;
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);

    auto shape = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
    // shape->AddAdditionalCircle(glm::vec2(), glm::vec2(150, 150), glm::u8vec4(255, 0, 0, 255), true);
    shape->AddAdditionalRectangle(glm::vec2(), glm::vec2(200, 200), glm::u8vec4(255, 0, 0, 255), true);
    window.AddExtension(shape);
    int id1 = shape->AddAdditionalLine(glm::vec2(), glm::vec2(), 1, 1, glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0), false);
    int id2 = shape->AddAdditionalLine(glm::vec2(), glm::vec2(), 1, 1, glm::u8vec4(0,0,0,0), glm::u8vec4(0,0,0,0), false);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostUpdateEventListener
    (
        [&]()
        {
            shape->SetAdditionalLine(id1, glm::vec2(0, 0) - window.GetGlobalPosition(), glm::vec2(100, 100) - window.GetGlobalPosition(), 
                1, 2, glm::u8vec4(0, 0, 0, 255), glm::u8vec4(255, 0, 0, 255), false);

            shape->SetAdditionalLine(id2, glm::vec2(300, 100) - window.GetGlobalPosition(), glm::vec2(300, 400) - window.GetGlobalPosition(), 
                3, 4, glm::u8vec4(0, 0, 255, 255), glm::u8vec4(0, 0, 0, 255), false);
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}