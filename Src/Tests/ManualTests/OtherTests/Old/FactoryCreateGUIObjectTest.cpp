#include "ssGUI/HeaderGroups/StandardGroup.hpp"

int main()
{
    //Create the main window
    // ssGUI::MainWindow mainWindow;
    // mainWindow.SetSize(glm::ivec2(400, 400));

    auto widget = ssGUI::Factory::Create<ssGUI::Widget>();
    widget->SetPosition(glm::ivec2(100, 100));
    widget->SetSize(glm::ivec2(300, 300));
    widget->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));

    ssLOG_LINE(widget->IsHeapAllocated());

    ssGUI::Factory::Dispose<ssGUI::Widget>(widget);

    return 0;
}