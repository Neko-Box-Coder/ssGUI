#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Shape.hpp"
#include "ssGUI/Extensions/Outline.hpp"
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

    auto shapeEx = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
    shapeEx->AddAdditionalCircle(glm::vec2(-100, -100), glm::vec2(200, 200), glm::u8vec4(255, 0, 0, 255), true);
    window.AddExtension(shapeEx);
    window.RemoveExtension(ssGUI::Extensions::Border::EXTENSION_NAME);
    auto outlineEx = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
    outlineEx->SetInnerOutline(false);
    outlineEx->SetOutlineThickness(10);
    window.AddExtension(outlineEx);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}