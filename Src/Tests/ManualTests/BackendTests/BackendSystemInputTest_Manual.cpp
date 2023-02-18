#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssLogger/ssLog.hpp"
#include "ssGUI/HelperClasses/GenericInputToString.hpp"

#include <iostream>
#include <thread>
#include <vector>

ssGUI::Backend::BackendDrawingInterface* drawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* window = nullptr;
ssGUI::Backend::BackendSystemInputInterface* inputs = nullptr;
bool showInfo = true;

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

void SetUp()
{
    drawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    window = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    inputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);
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
    if(showInfo)
    {
        ssLOG_SIMPLE("Press different keys and mouse buttons and it should be printed out correctly");
        ssLOG_SIMPLE("When done, simply press 1 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }

    std::vector<ssGUI::Enums::GenericButtonAndKeyInput> currentInputs = inputs->GetCurrentButtonAndKeyPresses();
    std::vector<ssGUI::Enums::GenericButtonAndKeyInput> lastInputs = inputs->GetLastButtonAndKeyPresses();

    for(int i = 0; i < currentInputs.size(); i++)
    {
        ssLOG_SIMPLE("Current Input: "<<ssGUI::GenericInputToString(currentInputs[i]));
        assert(inputs->IsButtonOrKeyPressExistCurrentFrame(currentInputs[i]));
        
        if(ssGUI::Enums::InputIsMouseButton(currentInputs[i]))
            assert(inputs->GetCurrentMouseButton((ssGUI::Enums::MouseButton)currentInputs[i]));
    }

    for(int i = 0; i < lastInputs.size(); i++)
    {
        ssLOG_SIMPLE("Last Input: "<<ssGUI::GenericInputToString(lastInputs[i]));
        assert(inputs->IsButtonOrKeyPressExistLastFrame(lastInputs[i]));
        
        if(ssGUI::Enums::InputIsMouseButton(lastInputs[i]))
            assert(inputs->GetLastMouseButton((ssGUI::Enums::MouseButton)lastInputs[i]));
    }
    
    if(!currentInputs.empty() || !lastInputs.empty())
        ssLOG_SIMPLE("");
}

void MousePositionTest()
{
    if(showInfo)
    {
        ssLOG_SIMPLE("This prints your mouse position every second");
        ssLOG_SIMPLE("You can press 3 to set mouse position to (50, 50) GLOBALLY");
        ssLOG_SIMPLE("You can press 4 to set mouse position to (50, 50) relative to THIS WINDOW");
        ssLOG_SIMPLE("When done, simply press 2 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }
    
    static uint64_t lastTime = inputs->GetElapsedTime();
    uint64_t curTime = inputs->GetElapsedTime();
    
    if(curTime - lastTime > 1000)
    {
        glm::ivec2 currentMousePos = inputs->GetCurrentMousePosition(nullptr);
        glm::ivec2 lastMousePos = inputs->GetLastMousePosition(nullptr);
        ssLOG_SIMPLE("Current Mouse position: "<<currentMousePos.x<<", "<<currentMousePos.y);
        ssLOG_SIMPLE("Last Mouse position: "<<lastMousePos.x<<", "<<lastMousePos.y);
        ssLOG_SIMPLE("");
        lastTime = curTime;
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE))
    {
        inputs->SetMousePosition(glm::ivec2(50, 50), nullptr);
        ssLOG_SIMPLE("Mouse position set");
    }
    else if(inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR) &&
            !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        inputs->SetMousePosition(glm::ivec2(50, 50), window);
        ssLOG_SIMPLE("Mouse position set");
    }
}

void MouseScrollDeltaTest()
{
    if(showInfo)
    {
        ssLOG_SIMPLE("This prints the scroll delta whenever you scroll");
        ssLOG_SIMPLE("When done, simply press 3 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }

    glm::vec2 curScroll = inputs->GetCurrentMouseScrollDelta();
    if(curScroll != glm::vec2(0, 0))
    {
        ssLOG_SIMPLE("Scrolled: "<<curScroll.x <<", "<<curScroll.y);
    }
}

//TODO: Test RealtimeInputs after reworking it

void TextInputTest()
{
    if(showInfo)
    {
        ssLOG_SIMPLE("This prints all the text characters entered");
        ssLOG_SIMPLE("When done, simply press 4 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }

    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::string charactersEntered = converter.to_bytes(inputs->GetTextInput());
    
    if(!charactersEntered.empty())
    {
        ssLOG_SIMPLE("charactersEntered: "<<charactersEntered);
        ssLOG_SIMPLE("");
    }
}

void CursorTypeTest()
{
    if(showInfo)
    {
        ssLOG_SIMPLE("This cycles through different cursor type every second");
        ssLOG_SIMPLE("When done, simply press 5 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }

    static uint64_t lastTime = inputs->GetElapsedTime();
    static int currentCursorIndex = 0;
    
    uint64_t curTime = inputs->GetElapsedTime();
    
    if(curTime - lastTime > 1000)
    {
        inputs->SetCursorType((ssGUI::Enums::CursorType)currentCursorIndex);
        inputs->UpdateCursor();
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
    if(showInfo)
    {
        ssLOG_SIMPLE("Press 1 to load and set custom cursor");
        ssLOG_SIMPLE("When done, simply press 6 again to go back");
        ssLOG_SIMPLE("");
        showInfo = false;
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE))
    {
        if(inputs->HasCustomCursor("custom normal cursor"))
        {
            ssLOG_SIMPLE("Custom cursor already Set");
            return;
        }

        ssGUI::Backend::BackendImageInterface* img = ssGUI::Backend::BackendFactory::CreateBackendImageInterface();
        if(!img->LoadFromPath(ResourcesFolderPath+"CustomCursor.png"))           //https://www.pngwing.com/en/free-png-zgwrc
        // if(!img.LoadFromPath(ResourcesFolderPath+"BlueCursor.png"))          //https://www.subpng.com/png-ttdxzq/
        {
            ssLOG_SIMPLE("Failed to load");
        }
        
        inputs->CreateCustomCursor(img, "custom normal cursor", glm::ivec2(13, 20), glm::ivec2(3, 3));
        inputs->SetCurrentCustomCursor("custom normal cursor");
        
        ssLOG_SIMPLE("Custom cursor Set");
        ssLOG_SIMPLE("Run CursorTypeTest to see custom cursor");
    }

}

void ClipboardTest()
{
    if(showInfo)
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
        showInfo = false;
    }

    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE))
    {
        ssLOG_SIMPLE("ClearClipboard: "<<inputs->ClearClipboard());
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO))
    {
        ssLOG_SIMPLE("ClipbaordHasText: "<<inputs->ClipbaordHasText());
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("ClipbaordHasImage: "<<inputs->ClipbaordHasImage());
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        //TODO: SetClipboardImage
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        ssLOG_SIMPLE("SetClipboardText: "<<inputs->SetClipboardText(L"Clipboard Text"));
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::SIX) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::SIX))
    {
        //TODO: GetClipboardImage
    }
    
    if( inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::EIGHT) &&
        !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::EIGHT))
    {
        std::wstring clipboardTest = L"";
        ssLOG_SIMPLE("GetClipboardText: "<<inputs->GetClipboardText(clipboardTest));
        
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
        ssLOG_SIMPLE("GetClipboardText: "<<converter.to_bytes(clipboardTest));
    }
}


//TODO: BackendSystemInput is quite complicated to test, will deal with it later
int main()
{
    SetUp();
    
    Instructions();
        
    //Setup window and run it
    window->SetRenderSize(glm::ivec2(1280, 720));
    inputs->UpdateInput();
    
    int currentModeIndex = -1;
    const int maxNumberOfModes = 7;
    
    auto modeTogglePressed = [&](ssGUI::Enums::NumberKey numKey)
    {
        return( inputs->IsButtonOrKeyPressExistCurrentFrame(numKey) &&
                !inputs->IsButtonOrKeyPressExistLastFrame(numKey));
    };
    
    auto updateTestModes = [&]()
    {
        if(currentModeIndex >= 0)
        {
            if(modeTogglePressed((ssGUI::Enums::NumberKey)((int)ssGUI::Enums::NumberKey::ONE + currentModeIndex)))
            {
                currentModeIndex = -1;
                ssLOG_SIMPLE("Exited back to test selection mode");
                showInfo = true;
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

    while(!window->IsClosed())
    {
        inputs->UpdateInput();
        if(!window->IsClosed())
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
        
            drawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    
    CleanUp();   
}

