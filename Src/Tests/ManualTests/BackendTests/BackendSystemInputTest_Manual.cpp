#include "ssGUI/Backend/Factory.hpp"
#include "ssGUI/Backend/Interfaces/SystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/DrawingInterface.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssGUI/HelperClasses/GenericInputToString.hpp"
#include "TestsResources.h"

#include <iostream>
#include <thread>
#include <vector>

ssGUI::Backend::DrawingInterface* BackendDrawing = nullptr;
ssGUI::Backend::MainWindowInterface* TestWindow = nullptr;
ssGUI::Backend::SystemInputInterface* BackendInput = nullptr;
bool ShowInfo = true;

void SetUp()
{
    ssGUI::Backend::Factory::Initialize();
    TestWindow = ssGUI::Backend::Factory::CreateMainWindowInterface();
    BackendDrawing = ssGUI::Backend::Factory::CreateDrawingInterface(TestWindow);
    BackendInput = ssGUI::Backend::Factory::CreateInputInterface();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(BackendDrawing);
    ssGUI::Factory::Dispose(TestWindow);
    ssGUI::Factory::Dispose(BackendInput);
    ssGUI::Backend::Factory::Cleanup();
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to go into KeyAndButtonTest");
    ssLOG_SIMPLE("Press 2 to go into MousePositionTest");
    ssLOG_SIMPLE("Press 3 to go into MouseScrollDeltaTest");
    ssLOG_SIMPLE("Press 4 to go into TextInputTest");
    ssLOG_SIMPLE("Press 5 to go into CursorTypeTest");
    ssLOG_SIMPLE("Press 6 to go into CustomCursorTest");
    ssLOG_SIMPLE("Press 7 to go into ClipboardTest");
    ssLOG_SIMPLE("");
}

void KeyAndButtonTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("Press different keys and mouse buttons and it should be printed out correctly");
        ssLOG_SIMPLE("When done, simply press 1 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }

    std::vector<ssGUI::Enums::GenericInput> currentInputs = BackendInput->GetCurrentInputs();
    std::vector<ssGUI::Enums::GenericInput> lastInputs = BackendInput->GetLastInputs();

    for(int i = 0; i < currentInputs.size(); i++)
    {
        ssLOG_SIMPLE("Current Input: "<<ssGUI::GenericInputToString(currentInputs[i]));
        assert(BackendInput->IsInputExistCurrentFrame(currentInputs[i]));
    }

    for(int i = 0; i < lastInputs.size(); i++)
    {
        ssLOG_SIMPLE("Last Input: "<<ssGUI::GenericInputToString(lastInputs[i]));
        assert(BackendInput->IsInputExistLastFrame(lastInputs[i]));
    }
    
    if(!currentInputs.empty() || !lastInputs.empty())
        ssLOG_SIMPLE("");
}

void MousePositionTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("This prints your mouse position every second");
        ssLOG_SIMPLE("You can press 3 to set mouse position to (50, 50) GLOBALLY");
        ssLOG_SIMPLE("You can press 4 to set mouse position to (50, 50) relative to THIS WINDOW");
        ssLOG_SIMPLE("When done, simply press 2 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }
    
    static uint64_t lastTime = BackendInput->GetElapsedTime();
    uint64_t curTime = BackendInput->GetElapsedTime();
    
    if(curTime - lastTime > 1000)
    {
        glm::ivec2 currentMousePos = BackendInput->GetCurrentMousePosition(nullptr);
        glm::ivec2 lastMousePos = BackendInput->GetLastMousePosition(nullptr);
        ssLOG_SIMPLE("Current Mouse position: "<<currentMousePos.x<<", "<<currentMousePos.y);
        ssLOG_SIMPLE("Last Mouse position: "<<lastMousePos.x<<", "<<lastMousePos.y);
        ssLOG_SIMPLE("");
        lastTime = curTime;
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::THREE) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::THREE))
    {
        BackendInput->SetMousePosition(glm::ivec2(50, 50), nullptr);
        ssLOG_SIMPLE("Mouse position set");
    }
    else if(BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR) &&
            !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        BackendInput->SetMousePosition(glm::ivec2(50, 50), TestWindow);
        ssLOG_SIMPLE("Mouse position set");
    }
}

void MouseScrollDeltaTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("This prints the scroll delta whenever you scroll");
        ssLOG_SIMPLE("When done, simply press 3 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }

    glm::vec2 curScroll = BackendInput->GetCurrentMouseScrollDelta();
    if(curScroll != glm::vec2(0, 0))
    {
        ssLOG_SIMPLE("Scrolled: "<<curScroll.x <<", "<<curScroll.y);
    }
}

//TODO: Test RealtimeInputs after reworking it

void TextInputTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("This prints all the text characters entered");
        ssLOG_SIMPLE("When done, simply press 4 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }

    std::string charactersEntered;
    BackendInput->GetTextInput(charactersEntered);
    
    if(!charactersEntered.empty())
    {
        ssLOG_SIMPLE("charactersEntered: "<<charactersEntered);
        ssLOG_SIMPLE("");
    }
}

void CursorTypeTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("This cycles through different cursor type every second");
        ssLOG_SIMPLE("When done, simply press 5 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }

    static uint64_t lastTime = BackendInput->GetElapsedTime();
    static int currentCursorIndex = 0;
    
    uint64_t curTime = BackendInput->GetElapsedTime();
    
    if(curTime - lastTime > 1000)
    {
        BackendInput->SetCursorType((ssGUI::Enums::CursorType)currentCursorIndex);
        BackendInput->UpdateCursor();
        ssLOG_SIMPLE("Current Cursor Type: "<<ssGUI::Enums::CursorTypeToString((ssGUI::Enums::CursorType)currentCursorIndex));
        
        currentCursorIndex++;
        currentCursorIndex = currentCursorIndex >=  (int)ssGUI::Enums::CursorType::COUNT ? 
                                                    (int)ssGUI::Enums::CursorType::NONE : 
                                                    currentCursorIndex;
        
        lastTime = curTime;
    }
}

void CustomCursorTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("Press 1 to load and set custom cursor");
        ssLOG_SIMPLE("When done, simply press 6 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::ONE) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::ONE))
    {
        if(BackendInput->HasCustomCursor("custom normal cursor"))
        {
            ssLOG_SIMPLE("Custom cursor already Set");
            return;
        }

        ssGUI::Backend::ImageInterface* img = ssGUI::Backend::Factory::CreateImageInterface();
        //if(!img->LoadFromPath(ResourcesFolderPath+"CustomCursor.png"))           //https://www.pngwing.com/en/free-png-zgwrc
        if(!img->LoadImgFileFromMemory(ssGUI_Test_sd, ssGUI_Test_sd_size))
        // if(!img.LoadFromPath(ResourcesFolderPath+"BlueCursor.png"))          //https://www.subpng.com/png-ttdxzq/
        {
            ssLOG_SIMPLE("Failed to load");
        }
        
        //inputs->CreateCustomCursor(img, "custom normal cursor", glm::ivec2(13, 20), glm::ivec2(3, 3));
        BackendInput->CreateCustomCursor(img, "custom normal cursor", glm::ivec2(50, 50), glm::ivec2(3, 3));
        BackendInput->SetCurrentCustomCursor("custom normal cursor");
        
        ssLOG_SIMPLE("Custom cursor Set");
        ssLOG_SIMPLE("Run CursorTypeTest to see custom cursor");
    }

}

void ClipboardTest()
{
    if(ShowInfo)
    {
        ssLOG_SIMPLE("This tests different clipboard functions");
        ssLOG_SIMPLE("Press 1 to test ClearClipboard");
        ssLOG_SIMPLE("Press 2 to test ClipbaordHasText");
        ssLOG_SIMPLE("Press 3 to test ClipbaordHasImage");
        ssLOG_SIMPLE("Press 4 to test SetClipboardImage (WIP)");
        ssLOG_SIMPLE("Press 5 to test SetClipboardText");
        ssLOG_SIMPLE("Press 6 to test GetClipboardImage (WIP)");
        ssLOG_SIMPLE("Press 8 to test GetClipboardText");
        ssLOG_SIMPLE("When done, simply press 7 again to go back");
        ssLOG_SIMPLE("");
        ShowInfo = false;
    }

    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::ONE) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::ONE))
    {
        ssLOG_SIMPLE("ClearClipboard: "<<BackendInput->ClearClipboard());
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::TWO) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::TWO))
    {
        ssLOG_SIMPLE("ClipbaordHasText: "<<BackendInput->ClipbaordHasText());
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::THREE) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("ClipbaordHasImage: "<<BackendInput->ClipbaordHasImage());
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        //TODO: SetClipboardImage
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        ssLOG_SIMPLE("SetClipboardText: "<<BackendInput->SetClipboardText(U"Clipboard Text"));
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::SIX) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::SIX))
    {
        //TODO: GetClipboardImage
    }
    
    if( BackendInput->IsInputExistCurrentFrame(ssGUI::Enums::NumberKey::EIGHT) &&
        !BackendInput->IsInputExistLastFrame(ssGUI::Enums::NumberKey::EIGHT))
    {
        std::u32string clipboardTest = U"";
        ssLOG_SIMPLE("GetClipboardText: "<<BackendInput->GetClipboardText(clipboardTest));
        
        std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
        ssLOG_SIMPLE("GetClipboardText: "<<converter.to_bytes(clipboardTest));
    }
}


//TODO: BackendSystemInput is quite complicated to test, will deal with it later
int main()
{
    SetUp();
    
    Instructions();
        
    //Setup window and run it
    TestWindow->SetRenderSize(glm::ivec2(1280, 720));
    BackendInput->UpdateInput(&TestWindow, 1);
    
    int currentModeIndex = -1;
    const int maxNumberOfModes = 7;
    
    auto modeTogglePressed = [&](ssGUI::Enums::NumberKey numKey)
    {
        return( BackendInput->IsInputExistCurrentFrame(numKey) &&
                !BackendInput->IsInputExistLastFrame(numKey));
    };
    
    auto updateTestModes = [&]()
    {
        if(currentModeIndex >= 0)
        {
            if(modeTogglePressed((ssGUI::Enums::NumberKey)((int)ssGUI::Enums::NumberKey::ONE + currentModeIndex)))
            {
                currentModeIndex = -1;
                ssLOG_SIMPLE("Exited back to test selection mode");
                ShowInfo = true;
                Instructions();
            }
        }
        else
        {
            for(int i = 0; i < maxNumberOfModes; i++)
            {
                if(modeTogglePressed((ssGUI::Enums::NumberKey)((int)ssGUI::Enums::NumberKey::ONE + i)))
                {
                    currentModeIndex = i;
                    break;
                }
            }
        }
    };

    while(!TestWindow->IsClosed())
    {
        BackendInput->UpdateInput(&TestWindow, 1);
        if(!TestWindow->IsClosed())
        {
            updateTestModes();
            
            switch(currentModeIndex)
            {
                case -1:
                    break;
                case 0:
                    KeyAndButtonTest();
                    break;
                case 1:
                    MousePositionTest();
                    break;
                case 2:
                    MouseScrollDeltaTest();
                    break;
                case 3:
                    TextInputTest();
                    break;
                case 4:
                    CursorTypeTest();
                    break;
                case 5:
                    CustomCursorTest();
                    break;
                case 6:
                    ClipboardTest();
                    break;
                default:
                    ssLOG_EXIT_PROGRAM();
            }
        
            BackendDrawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    CleanUp();   
}

