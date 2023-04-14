#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/HeaderGroups/InputGroup.hpp"
#include "ssLogger/ssLog.hpp"

#include <thread>

ssGUI::Backend::BackendDrawingInterface* BackendDrawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* TestWindow = nullptr;
ssGUI::Backend::BackendSystemInputInterface* BackendInput = nullptr;

ssGUI::ImageData TestImage;

uint64_t NextFocusTime = 0;
bool Focus = false;
bool UseFocus = false;

uint64_t NextIsFocusTime = 0;
bool UseIsFocus = false;

int FocusEventIndex = -1;

void SetUp()
{
    BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    TestWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    BackendInput = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(BackendDrawing);
    ssGUI::Factory::Dispose(TestWindow);
    ssGUI::Factory::Dispose(BackendInput);
}

void GenerateImage()
{
    //Setup dummy image for icon
    const int imgWidth = 64;
    const int imgHeight = 64;
    const int columnWidth = imgWidth / 4;
    uint8_t dummyImg[imgWidth * imgHeight * 4] = {0};
    
    for(int y = 0; y < imgHeight; y++)
    {
        for(int x = 0; x < imgWidth; x++)
        {
            dummyImg[(y * imgWidth + x) * 4 + 0] = x < columnWidth ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 1] = x >= columnWidth && x < columnWidth * 2 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 2] = x >= columnWidth * 2 && x < columnWidth * 3 ? 255 : 0;
            dummyImg[(y * imgWidth + x) * 4 + 3] = x >= columnWidth * 3 ? 127 : 255;
        }
    }
    
    bool result = TestImage.LoadRawFromMemory(dummyImg, ssGUI::ImageFormat(), glm::ivec2(64, 64));
    ssLOG_LINE("Icon Image Loading result: "<<result);
}

void Instructions()
{
    ssLOG_SIMPLE("Press 1 to test SetIcon");
    ssLOG_SIMPLE("Press 2 to test SetFocus");
    ssLOG_SIMPLE("Press 3 to test IsFocused");
    ssLOG_SIMPLE("Press 4 to test AddFocusChangedByUserEvent");
    ssLOG_SIMPLE("Press 5 to test RemoveFocusChangedByUserEvent");
    ssLOG_SIMPLE("Press 6 to test Clone");
}

void SetIconTest()
{
    //SetIcon
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
    {
        TestWindow->SetIcon(*TestImage.GetBackendImageInterface());
        ssLOG_SIMPLE("Icon set, you should be able to see a RGB column icon.");
    }
}

void SetFocusTest()
{
    //Set focus timer
    if(UseFocus && BackendInput->GetElapsedTime() > NextFocusTime)
    {
        UseFocus = false;
        TestWindow->SetFocus(Focus, false);
        ssLOG_SIMPLE("Focus is now set to "<<Focus);
    }

    //SetFocus
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
    {
        NextFocusTime = BackendInput->GetElapsedTime() + 5000;
        Focus = !Focus;
        UseFocus = true;
        
        ssLOG_SIMPLE("Focus will be set to "<<Focus<<" in 5 seconds");
    }
}

void IsFocusedTest()
{
    //Is Focus timer
    if(UseIsFocus && BackendInput->GetElapsedTime() > NextIsFocusTime)
    {
        UseIsFocus = false;
        ssLOG_SIMPLE("Focus status is: "<<TestWindow->IsFocused());
    }

    //IsFocus
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
    {
        NextIsFocusTime = BackendInput->GetElapsedTime() + 5000;
        UseIsFocus = true;
        
        ssLOG_SIMPLE("Focus status will be shown in 5 seconds");
    }
}

void AddFocusChangedByUserEventTest()
{
    //AddFocusChangedByUserEvent
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        if(FocusEventIndex < 0)
        {
            FocusEventIndex = TestWindow->AddFocusChangedByUserEvent([](bool focus)
            {
                ssLOG_SIMPLE("Focus event called: "<<focus);
            });
            
            ssLOG_SIMPLE("Focus event added");
        }
        else
        {
            ssLOG_SIMPLE("Focus event already exists");
        }
    }
}

void RemoveFocusChangedByUserEventTest()
{
    //RemoveFocusChangedByUserEvent
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FIVE) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        if(FocusEventIndex >= 0)
        {
            TestWindow->RemoveFocusChangedByUserEvent(FocusEventIndex);
            FocusEventIndex = -1;
            ssLOG_SIMPLE("Focus event removed");
        }
        else
        {
            ssLOG_SIMPLE("Focus event already removed");
        }
    }
}

void CloneTest()
{
    //RemoveFocusChangedByUserEvent
    if( !BackendInput->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::SIX) &&
        BackendInput->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::SIX))
    {
        TestWindow->Clone();
    }
}

int main()
{
    SetUp();
    
    Instructions();
    
    GenerateImage();
    
    //Setup window and run it
    TestWindow->SetRenderSize(glm::ivec2(1280, 720));
    BackendInput->UpdateInput();

    while(!TestWindow->IsClosed())
    {
        //drawing->ClearBackBuffer(glm::u8vec4());
        BackendInput->UpdateInput();
        if(!TestWindow->IsClosed())
        {
            SetIconTest();       
        
            SetFocusTest();
            
            IsFocusedTest();
        
            AddFocusChangedByUserEventTest();
            
            RemoveFocusChangedByUserEventTest();
            
            CloneTest();
            
            //TODO: SetGLContext
        
            //drawing->DrawEntities(pos, uv, color, counts, props);
            BackendDrawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    CleanUp();

    return 0;
}

