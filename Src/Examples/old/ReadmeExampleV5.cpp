#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Layout.hpp"

//Readme example
using namespace ssGUI::Enums;
int main()
{
    ssGUI::MainWindow mainWindow;                                               //Create the main window for showing content
    mainWindow.SetRenderSize(glm::vec2(450, 80));
    auto* layout = mainWindow.AddExtension<ssGUI::Extensions::Layout>();        //Add layout for auto sizing child GUI objects

    ssGUI::Text text;                                                           //Create a text widget and set the respective properties
    text.SetNewTextFontSize(17);
    text.SetText("Click on the button to show the message");
    text.SetTextAlignment(AlignmentHorizontal::CENTER, AlignmentVertical::CENTER);  //We center the text right above the button we will be adding later
    text.SetParent(&mainWindow);                                                //Attach text to main window, the layout will control its size.
    
    ssGUI::StandardButton button;                                               //Create a standard button, just a fancier button.
    button.SetSize(glm::vec2(50, 30));
    layout->AddChildWithAlignment(&button,  AlignmentHorizontal::CENTER,        //Attach button to main window with alignment, so that the size
                                            AlignmentVertical::CENTER);         //      stays the same and won't be changed by layout
                                                                                
    ssGUI::ssGUIManager guiManager;                                             //Create the GUIManager, which manages the flow of the program.
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);                    //Add the main window (which has both text and button parented to it)
    guiManager.AddPostGUIUpdateEventListener                                    
    (
        [&]()
        {
            if(button.GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)   //Then we want to check if the button is pressed every frame
                text.SetText(L"(`oωo´)");                                       //If it is, we change the text to a cute little face :)
        }
    );
    guiManager.StartRunning();                                                  //Finally we start running the program
    return 0;
}