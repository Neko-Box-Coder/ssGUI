#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ExamplesResources.h"

int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    auto* window = mainWindow.AddChild<ssGUI::StandardWindow>();
    window->AddExtension<ssGUI::Extensions::Layout>()->SetCoverFullLength(false);
    auto* checkbox = window->AddChild<ssGUI::StandardCheckbox>();

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    
    return 0;
}