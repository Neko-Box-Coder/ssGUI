#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
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
    window->AddExtension<ssGUI::Extensions::Layout>()->SetCoverFullLength(false);
    auto* slider = window->AddChild<ssGUI::StandardSlider>();
    //slider->AddExtension<ssGUI::Extensions::Border>();
    //slider->AddExtension<ssGUI::Extensions::AdvancedPosition>();
    //auto* as = slider->AddExtension<ssGUI::Extensions::AdvancedSize>();
    //as->SetVerticalPixel(slider->GetSize().y);
    //as->SetHorizontalPercentage(1);


    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            //auto* layout = slider->GetExtension<ssGUI::Extensions::Layout>();
            //ssLOG_LINE("layout->GetPreferredSizeMultiplier(0): "<<layout->GetPreferredSizeMultiplier(0));
            //ssLOG_LINE("layout->GetPreferredSizeMultiplier(1): "<<layout->GetPreferredSizeMultiplier(1));
            //ssLOG_LINE("layout->GetPreferredSizeMultiplier(2): "<<layout->GetPreferredSizeMultiplier(2));
        }
    );

    guiManager.StartRunning();
    
    return 0;
}