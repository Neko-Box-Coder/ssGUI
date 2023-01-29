#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/DataClasses/ImageData.hpp"
#include "ssGUI/Factory.hpp"
#include "ssLogger/ssLog.hpp"
#include "ssGUI/HeaderGroups/KeyGroup.hpp"

#include <thread>

ssGUI::Backend::BackendDrawingInterface* drawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* window = nullptr;
ssGUI::Backend::BackendSystemInputInterface* inputs = nullptr;

ssGUI::ImageData TestImage;

uint64_t NextFocusTime = 0;
bool Focus = false;
bool UseFocus = false;

uint64_t NextIsFocusTime = 0;
bool UseIsFocus = false;

int FocusEventIndex = -1;

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
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::ONE) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::ONE))
    {
        window->SetIcon(*TestImage.GetBackendImageInterface());
        ssLOG_SIMPLE("Icon set, you should be able to see a RGB column icon.");
    }
}

void SetFocusTest()
{
    //Set focus timer
    if(UseFocus && inputs->GetElapsedTime() > NextFocusTime)
    {
        UseFocus = false;
        window->SetFocus(Focus, false);
        ssLOG_SIMPLE("Focus is now set to "<<Focus);
    }

    //SetFocus
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::TWO) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::TWO))
    {
        NextFocusTime = inputs->GetElapsedTime() + 5000;
        Focus = !Focus;
        UseFocus = true;
        
        ssLOG_SIMPLE("Focus will be set to "<<Focus<<" in 5 seconds");
    }
}

void IsFocusedTest()
{
    //Is Focus timer
    if(UseIsFocus && inputs->GetElapsedTime() > NextIsFocusTime)
    {
        UseIsFocus = false;
        ssLOG_SIMPLE("Focus status is: "<<window->IsFocused());
    }

    //IsFocus
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::THREE) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::THREE))
    {
        NextIsFocusTime = inputs->GetElapsedTime() + 5000;
        UseIsFocus = true;
        
        ssLOG_SIMPLE("Focus status will be shown in 5 seconds");
    }
}

void AddFocusChangedByUserEventTest()
{
    //AddFocusChangedByUserEvent
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FOUR) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FOUR))
    {
        if(FocusEventIndex < 0)
        {
            FocusEventIndex = window->AddFocusChangedByUserEvent([](bool focus)
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
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::FIVE) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::FIVE))
    {
        if(FocusEventIndex >= 0)
        {
            window->RemoveFocusChangedByUserEvent(FocusEventIndex);
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
    if( !inputs->IsButtonOrKeyPressExistLastFrame(ssGUI::Enums::NumberKey::SIX) &&
        inputs->IsButtonOrKeyPressExistCurrentFrame(ssGUI::Enums::NumberKey::SIX))
    {
        window->Clone();
    }
}

int main()
{
    SetUp();
    
    Instructions();
    
    GenerateImage();
    
    //Setup window and run it
    window->SetRenderSize(glm::ivec2(1280, 720));
    inputs->UpdateInput();

    while(!window->IsClosed())
    {
        //drawing->ClearBackBuffer(glm::u8vec4());
        inputs->UpdateInput();
        if(!window->IsClosed())
        {
            SetIconTest();       
        
            SetFocusTest();
            
            IsFocusedTest();
        
            AddFocusChangedByUserEventTest();
            
            RemoveFocusChangedByUserEventTest();
            
            CloneTest();
            
            //TODO: SetGLContext
        
            //drawing->DrawEntities(pos, uv, color, counts, props);
            drawing->Render(glm::u8vec3(255, 255, 255));
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    CleanUp();

    return 0;
}

