#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Outline.hpp"
#include "ssGUI/Extensions/RoundedCorners.hpp"
#include "ssGUI/Extensions/Shape.hpp"


//Outline test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::ivec2(800, 800));
    mainWindow.GetBackendWindowInterface()->SetMSAA(4);
    DEBUG_LINE(mainWindow.GetBackendWindowInterface()->GetMSAA());

    ssGUI::Widget widget;
    widget.SetPosition(glm::ivec2(100, 100));
    widget.SetSize(glm::ivec2(300, 300));
    widget.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    // widget.AddExtension(new ssGUI::Extensions::RoundedCorners());

    // auto widgetClone = widget.Clone(false);
    // widgetClone->SetBackgroundColor(glm::u8vec4(127, 100, 100, 255));

    auto outline = ssGUI::Factory::Create<ssGUI::Extensions::Outline>();
    outline->SetOutlineThickness(20);

    // outline->ClearTargetShapes();
    // outline->AddTargetVertex(0);
    // outline->AddTargetVertex(1);
    // outline->SetSimpleOutline(false);

    // auto illegalShape = ssGUI::Factory::Create<ssGUI::Extensions::Shape>();
    // illegalShape->RemoveGUIObjectShape(0);
    // std::vector<glm::vec2> illegalVertices = {glm::vec2(0, 0), glm::vec2(1, 0)};
    // std::vector<glm::u8vec4> illegalColors = {glm::u8vec4(0, 0, 0, 255), glm::u8vec4(0, 0, 0, 255)};
    // illegalShape->AddAdditionalPolygon(illegalVertices, illegalColors, false);
    // widget.AddExtension(illegalShape);

    widget.AddExtension(outline);
    
    widget.SetParent(&mainWindow);
    // widgetClone->SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}