#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"

#include "ssLogger/ssLog.hpp"

int main()
{
    ssGUI::MainWindow mainWindow;

    ssGUI::Dropdown dropdown;
    dropdown.AddItem("Item 1");
    dropdown.AddItem("Item 2");
    dropdown.AddItem("Item 3");
    dropdown.AddItem("Item 4");
    dropdown.SetParent(&mainWindow);
    dropdown.SetPosition(glm::vec2(100, 100));

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}