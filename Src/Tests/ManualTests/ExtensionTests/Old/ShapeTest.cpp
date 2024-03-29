#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssLogger/ssLog.hpp"

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
    shape->AddAdditionalRectangle(glm::vec2(), glm::vec2(150, 150), glm::u8vec4(255, 0, 0, 255), true);
    window.AddExtension(shape);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}