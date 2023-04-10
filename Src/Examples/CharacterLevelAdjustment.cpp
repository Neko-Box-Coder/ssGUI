#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "glm/geometric.hpp"

//Readme example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 110));
    mainWindow.SetResizeType(ssGUI::Enums::ResizeType::NONE);

    
    ssGUI::Font boldFont;
    ssGUI::Font normalFont;
    if(!boldFont.GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Bold.ttf"))
    {
        std::cout<<"failed to import font\n";
        return 0;
    }
    if(!normalFont.GetBackendFontInterface()->LoadFromPath("Resources/NotoSans-Regular.ttf"))
    {
        std::cout<<"failed to import font\n";
        return 0;
    }
    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.AddFont(&normalFont);
    text.AddFont(&boldFont);
    text.SetHorizontalAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER);
    text.SetVerticalAlignment(ssGUI::Enums::AlignmentVertical::BOTTOM);
    text.SetPosition(glm::vec2(0, -45));
    text.SetSize(glm::vec2(450, 110));
    text.SetParent(&mainWindow);
    std::string s = "Character Level Adjustment";
    
    for(int i = 0; i < (int)s.size(); i++)
    {
        ssGUI::CharacterDetails detail;
        detail.Character = s[i];
        detail.FontIndex = 0;
        text.AddCharacterDetails(detail);
    }

    int foundIndex = -1;

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            glm::ivec2 curMousePos = guiManager.GetBackendInputInterface()->GetCurrentMousePosition(mainWindow.GetBackendWindowInterface());
            
            //Reset the previous highlighted character
            if(foundIndex != -1)
            {
                auto curDetail = text.GetCharacterDetails(foundIndex);
                curDetail.FontIndex = 0;
                curDetail.FontSize = 20;
                text.SetCharacterDetails(foundIndex, curDetail);
            }

            // foundIndex = text.GetNearestCharacterIndexFromPos(curMousePos, false);
            foundIndex = text.GetContainedCharacterIndexFromPos(curMousePos);
            //Highlight character
            if(foundIndex != -1)
            {
                auto curDetail = text.GetCharacterDetails(foundIndex);
                curDetail.FontIndex = 1;
                curDetail.FontSize = 30;
                text.SetCharacterDetails(foundIndex, curDetail);
            }
        }
    );
    guiManager.StartRunning();
    return 0;
}