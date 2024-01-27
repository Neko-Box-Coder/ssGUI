#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Factory.hpp"
#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"
#include "ssTest.hpp"

#ifdef SSGUI_MAIN_BACKEND_SFML
    #include <thread>
#endif 

#ifdef SSGUI_MAIN_BACKEND_MOCK
    #include "ssGUI/Backend/Mocks/BackendMainWindowMock.hpp"
#endif

ssGUI::Backend::BackendDrawingInterface* BackendDrawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* TestWindow = nullptr;
ssGUI::Backend::BackendSystemInputInterface* BackendInputs = nullptr;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        ssGUI::Backend::BackendFactory::Initialize();
        TestWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface(TestWindow);
        BackendInputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
        BackendInputs->UpdateInput(&TestWindow, 1);
    };

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(BackendDrawing);
        ssGUI::Factory::Dispose(TestWindow);
        ssGUI::Factory::Dispose(BackendInputs);
        ssGUI::Backend::BackendFactory::Cleanup();
    };

    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("WindowPositionTest")
    {
        glm::ivec2 pos(300, 300);
        TestWindow->SetWindowPosition(pos);
        
        #ifdef SSGUI_MAIN_BACKEND_SFML
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        #endif 
        ssTEST_OUTPUT_ASSERT(TestWindow->GetWindowPosition() == pos);
        //ssLOG_LINE(   "TestWindow->GetWindowPosition(): " << 
        //              TestWindow->GetWindowPosition().x <<
        //              ", " <<
        //              TestWindow->GetWindowPosition().y);
    };

    ssTEST("GetDecorationOffsetsTest")
    {
        glm::ivec2 topLeft;
        glm::ivec2 bottomRight;
        TestWindow->GetDecorationOffsets(topLeft, bottomRight);
        
        ssTEST_OUTPUT_ASSERT(topLeft.x >= 0 && topLeft.y >= 0);
        
        //ssLOG_LINE(   "TestWindow->GetPositionOffset(): " << 
        //              TestWindow->GetPositionOffset().x << 
        //              ", " << 
        //              TestWindow->GetPositionOffset().y);
    };

    ssTEST("WindowSizeTest")
    {
        glm::ivec2 size(500, 500);
        TestWindow->SetWindowSize(size);
        
        glm::ivec2 windowSize = TestWindow->GetWindowSize();
        //ssLOG_LINE("windowSize: "<<windowSize.x<<", "<<windowSize.y);
        ssTEST_OUTPUT_ASSERT(TestWindow->GetWindowSize() == size);
    };

    ssTEST("RenderSizeTest")
    {
        glm::ivec2 size(500, 500);
        TestWindow->SetRenderSize(size);
        
        ssTEST_OUTPUT_ASSERT(TestWindow->GetRenderSize() == size);
        
        //SFML can't return window size
        #ifndef SSGUI_MAIN_BACKEND_SFML
            ssTEST_OUTPUT_ASSERT(   "Against Window Size", 
                                    TestWindow->GetRenderSize() != TestWindow->GetWindowSize());
        #else
            ssTEST_OUTPUT_SKIP("Against Window Size");
        #endif
    };

    ssTEST("ValidateBothSizesTest")
    {
        TestWindow->SetRenderSize(glm::ivec2(1280, 720));
        glm::ivec2 windowSize = TestWindow->GetWindowSize();
        TestWindow->SetWindowSize(windowSize);
        ssTEST_OUTPUT_ASSERT("1", TestWindow->GetRenderSize() == glm::ivec2(1280, 720));
        
        TestWindow->SetWindowSize(glm::ivec2(600, 400));
        glm::ivec2 renderSize = TestWindow->GetRenderSize();
        TestWindow->SetRenderSize(renderSize);
        ssTEST_OUTPUT_ASSERT("2", TestWindow->GetWindowSize() == glm::ivec2(600, 400));
    };

    ssTEST("CloseTest")
    {
        ssTEST_OUTPUT_ASSERT("Not Closed", !TestWindow->IsClosed());
        TestWindow->Close();
        ssTEST_OUTPUT_ASSERT(TestWindow->IsClosed());   
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    };

    ssTEST("CloseEventTest")
    {
        bool valid = false;
        int id =    TestWindow->AddOnCloseEvent
                    ( 
                        [&](ssGUI::Backend::BackendMainWindowInterface* mainWindow)
                        {
                            valid = TestWindow == mainWindow;
                            TestWindow->AbortClosing();
                        }
                    );
        TestWindow->Close();
        
        ssTEST_OUTPUT_ASSERT("Event Called", valid);
        ssTEST_OUTPUT_ASSERT("Close Abort", !TestWindow->IsClosed());
        
        if(!TestWindow->IsClosed())
        {
            valid = false;
            TestWindow->RemoveOnCloseEvent(id);
            TestWindow->Close();
            ssTEST_OUTPUT_ASSERT("Event Remove", !valid);
        }
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    };

    ssTEST("TitleTest")
    {
        std::u32string someTitle = U"someTitle";
        TestWindow->SetTitle(someTitle);
        std::u32string retrievedTitle;
        TestWindow->GetTitle(retrievedTitle);
        ssTEST_OUTPUT_ASSERT(retrievedTitle == someTitle);
    };

    ssTEST("VisibleTest")
    {
        TestWindow->SetVisible(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsVisible());
        TestWindow->SetVisible(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsVisible());
    };

    ssTEST("VsyncTest()")
    {
        TestWindow->SetVSync(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsVSync());
        TestWindow->SetVSync(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsVSync());
    };

    ssTEST("MSAATest()")
    {
        TestWindow->SetAntiAliasingLevel(4);
        ssTEST_OUTPUT_ASSERT(TestWindow->GetAntiAliasingLevel() == 4);
    };

    ssTEST("ResizableTest()")
    {
        TestWindow->SetResizable(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsResizable());
        TestWindow->SetResizable(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsResizable());
    };

    //TODO(NOW): Window decoration test

    ssTEST("WindowModeTest()")
    {
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::BORDERLESS);
        ssTEST_OUTPUT_ASSERT(   "BORDERLESS", 
                                TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
        ssTEST_OUTPUT_ASSERT(   "NORMAL", 
                                TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::FULLSCREEN);
        ssTEST_OUTPUT_ASSERT(   "FULLSCREEN", 
                                TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
    };

    ssTEST("CloneTest()")
    {
        auto* windowClone = TestWindow->Clone();
        ssTEST_OUTPUT_ASSERT(windowClone != nullptr);
        ssGUI::Factory::Dispose(windowClone);
    };

    ssTEST("GetRawHandleTest()")
    {
        #ifdef SSGUI_MAIN_BACKEND_MOCK
            using MainWindowMock = ssGUI::Backend::BackendMainWindowMock;
            
            CO_OVERRIDE_RETURNS ((*static_cast<MainWindowMock*>(TestWindow)), GetRawHandle())
                                .Returns((void*)1);
        #endif
        
        ssTEST_OUTPUT_ASSERT(TestWindow->GetRawHandle() != nullptr);
    };

    ssTEST_END();
}