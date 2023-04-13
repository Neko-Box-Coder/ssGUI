#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Image* TestImage = nullptr;
ssGUI::Image* ClonedImage = nullptr;
ssGUI::ImageData TestImgData;

#if defined(_MSC_VER) && !defined(__INTEL_COMPILER)
    std::string ResourcesFolderPath = "..\\Resources\\";
#else
    std::string ResourcesFolderPath = "./Resources/";
#endif

ssGUI::Enums::GenericButtonAndKeyInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestImgData.LoadFromPath(ResourcesFolderPath+"sd.png");
    TestImage = MainWindow->AddChild<ssGUI::Image>();
    TestImage->SetSize(glm::vec2(400, 250));
    TestImage->AddExtension<ssGUI::Extensions::Border>();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call ImageDataTest");
    ssLOG_SIMPLE("Press 2 to call FittingTest");
    ssLOG_SIMPLE("Press 3 to call ImageTintTest");
    ssLOG_SIMPLE("Press 4 to call GetTypeTest");
    ssLOG_SIMPLE("Press 5 to call CloneTest");
    ssLOG_SIMPLE("");
}

void ImageDataTest(bool firstRun)   //1
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 1 to go back");
        ssLOG_SIMPLE("Press 2 to GetImageData");
        ssLOG_SIMPLE("Press 3 to SetImageData");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
    {
        ssLOG_SIMPLE("GetImageData: "<<TestImage->GetImageData());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("ImageData Set");
        TestImage->SetImageData(&TestImgData);
    }
}

void FittingTest(bool firstRun) //2
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 2 to go back");
        ssLOG_SIMPLE("Press 3 to GetFitting");
        ssLOG_SIMPLE("Press 4 to cycle fitting by SetFitting");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("GetFitting: "<<ssGUI::Enums::ImageFittingToString(TestImage->GetFitting()));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        auto newFitting = static_cast<ssGUI::Enums::ImageFitting>(((int)TestImage->GetFitting() + 1) % (int)ssGUI::Enums::ImageFitting::COUNT);
        TestImage->SetFitting(newFitting);
        ssLOG_SIMPLE("New Fitting set");
    }
}

void ImageTintTest(bool firstRun)   //3
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 3 to go back");
        ssLOG_SIMPLE("Press 4 to SetImageTint to red");
        ssLOG_SIMPLE("Press 5 to GetImageTint");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        TestImage->SetImageTint(glm::u8vec4(255, 127, 127, 255));
        ssLOG_SIMPLE("Image tint set");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        auto tint = TestImage->GetImageTint();
        ssLOG_SIMPLE("ImageTint: "<<(int)tint.r<<", "<<(int)tint.g<<", "<<(int)tint.b<<", "<<(int)tint.a);
    }
}

void GetTypeTest()  //4
{
    if(TestImage->GetType() == (ssGUI::Enums::GUIObjectType::IMAGE | ssGUI::Enums::GUIObjectType::WIDGET))
    {
        ssLOG_SIMPLE("GetType working");
    }
    else
    {
        ssLOG_SIMPLE("GetType failed");
    }
}

void CloneTest()    //5
{
    if(ClonedImage == nullptr)
    {
        ClonedImage = TestImage->Clone(true);
        ClonedImage->SetPosition(ClonedImage->GetPosition() + glm::vec2(50, 50));
    }
    else
    {
        ssLOG_SIMPLE("Image is already cloned");
    }
}

int main()
{
    Instructions();
    
    SetUp();
    
    Manager->AddRootGUIObject(MainWindow);
    
    Manager->AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Enums::GenericButtonAndKeyInput lastTestMode = TestMode;
            if( TestMode == ssGUI::Enums::NO_INPUT &&
                !Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses().empty() &&
                Manager->GetBackendInputInterface()->GetLastButtonAndKeyPresses().empty())
            {
                TestMode = Manager->GetBackendInputInterface()->GetCurrentButtonAndKeyPresses()[0];
            }
            
            ssGUI::Enums::GenericButtonAndKeyInput selectedTestMode = TestMode;
            
            #define QUICK_CAST(x) static_cast<ssGUI::Enums::GenericButtonAndKeyInput>(x)
            
            switch(TestMode)
            {
                //Call tests depending on mode
                case QUICK_CAST(ssGUI::Enums::NumberKey::ONE):
                    ImageDataTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ONE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    FittingTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    ImageTintTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
                    GetTypeTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FIVE):
                    CloneTest();
                    TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                default:
                    TestMode = ssGUI::Enums::NO_INPUT;
                    return;
            }
            
            if(selectedTestMode != ssGUI::Enums::NO_INPUT && TestMode == ssGUI::Enums::NO_INPUT)
                Instructions();
        }
    );
    
    Manager->StartRunning();
    
    CleanUp();   
}