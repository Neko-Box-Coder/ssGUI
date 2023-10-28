#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/ImageCanvas.hpp"
#include "ssGUI/Extensions/Border.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ExamplesResources.h"

//Image Test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    auto* window = mainWindow.AddChild<ssGUI::StandardWindow>();
    auto* slider = window->AddChild<ssGUI::Slider>();
    slider->AddExtension<ssGUI::Extensions::AdvancedPosition>();

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.StartRunning();
    
    return 0;
}