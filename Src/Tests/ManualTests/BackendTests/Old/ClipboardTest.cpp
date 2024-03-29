#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssLogger/ssLog.hpp"

//Text font test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Create a text widget and set the respective properties
    ssGUI::Text text;
    text.SetPosition(glm::ivec2(75, 25));
    text.SetSize(glm::ivec2(300, 150));

    text.SetNewTextFontSize(20);
    text.SetWrappingMode(ssGUI::Enums::TextWrapping::NO_WRAPPING);

    text.SetParent(&mainWindow);

    text.AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());

    ssGUI::ImageData imgData;

    uint8_t colors[] = 
    {
        1,  2,  3,  4,      2,  3,  4,  5,
        3,  4,  5,  6,      4,  5,  6,  7
    };

    imgData.LoadRawFromMemory(&colors, ssGUI::ImageFormat(), glm::ivec2(2, 2));

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;
    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            auto inputInterface = guiManager.GetBackendInputInterface();
            
            /*if(!inputInterface->GetCurrentKeyPresses().IsLetterKeyPresent(ssGUI::Enums::LetterKey::V) && 
                inputInterface->GetLastKeyPresses().IsLetterKeyPresent(ssGUI::Enums::LetterKey::V))
            {
                ssLOG_LINE("v pressed: "<<inputInterface->GetElapsedTime());
            }

            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::LEFT_CTRL) ||
                inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::RIGHT_CTRL))
            {
                ssLOG_LINE("ctrl pressed"<<inputInterface->GetElapsedTime());
            }*/

            //Pasting
            if((inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_CTRL) ||
                inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_CTRL)) &&
                !inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::V) && 
                inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::V))
            {
                if(inputInterface->ClipbaordHasText())
                {
                    std::wstring str = L"";
                    if(inputInterface->GetClipboardText(str))
                        text.SetText(str);
                    
                }
                else if(inputInterface->ClipbaordHasImage())
                {               
                    if(inputInterface->GetClipboardImage(imgData))
                    {
                        ssLOG_LINE("Image Size: "<<imgData.GetSize().x<<", "<<imgData.GetSize().y);
                        ssGUI::ImageFormat format;
                        const uint8_t* rawData = static_cast<const uint8_t*>(imgData.GetBackendImageInterface()->GetPixelPtr(format));
                        ssLOG_LINE("("<<(int)rawData[0]<<", "<<(int)rawData[1]<<", "<<(int)rawData[2]<<", "<<(int)rawData[3]<<")");
                        ssLOG_LINE("("<<(int)rawData[4]<<", "<<(int)rawData[5]<<", "<<(int)rawData[6]<<", "<<(int)rawData[7]<<")");
                        ssLOG_LINE("("<<(int)rawData[8]<<", "<<(int)rawData[9]<<", "<<(int)rawData[10]<<", "<<(int)rawData[11]<<")");
                        ssLOG_LINE("("<<(int)rawData[12]<<", "<<(int)rawData[13]<<", "<<(int)rawData[14]<<", "<<(int)rawData[15]<<")");
                    }
                }
            }
            //Copying
            else if((inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::LEFT_CTRL) ||
                    inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::SystemKey::RIGHT_CTRL)) &&
                    !inputInterface->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::LetterKey::C) && 
                    inputInterface->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::LetterKey::C))
            {
                //inputInterface->SetClipboardText(L"Clipboard Test :D");

                inputInterface->SetClipboardImage(imgData);
            }
        }
    );


    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}
