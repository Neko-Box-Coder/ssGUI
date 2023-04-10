#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssGUI/Factory.hpp"
#include "ssLogger/ssLog.hpp"
#include "ssTest.hpp"

ssGUI::Backend::BackendDrawingInterface* BackendDrawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* TestWindow = nullptr;
ssGUI::Backend::BackendSystemInputInterface* BackendInputs = nullptr;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        BackendDrawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
        TestWindow = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
        BackendInputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
        BackendInputs->UpdateInput();
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Factory::Dispose(BackendDrawing);
        ssGUI::Factory::Dispose(TestWindow);
        ssGUI::Factory::Dispose(BackendInputs);
    }

    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();

    ssTEST("WindowPositionTest")
    {
        glm::ivec2 pos(300, 300);
        TestWindow->SetWindowPosition(pos);
        
        ssTEST_OUTPUT_ASSERT(TestWindow->GetWindowPosition() == pos);
        //ssLOG_LINE("BackendWindow->GetWindowPosition(): "<<BackendWindow->GetWindowPosition().x<<", "<<BackendWindow->GetWindowPosition().y);
    }

    ssTEST("GetPositionOffsetTest")
    {
        ssTEST_OUTPUT_ASSERT(TestWindow->GetPositionOffset().x >= 0 && TestWindow->GetPositionOffset().y >= 0);
        
        //ssLOG_LINE("BackendWindow->GetPositionOffset(): "<<BackendWindow->GetPositionOffset().x<<", "<<BackendWindow->GetPositionOffset().y);
    }

    ssTEST("WindowSizeTest")
    {
        glm::ivec2 size(500, 500);
        TestWindow->SetWindowSize(size);
        
        glm::ivec2 windowSize = TestWindow->GetWindowSize();
        //ssLOG_LINE("windowSize: "<<windowSize.x<<", "<<windowSize.y);
        ssTEST_OUTPUT_ASSERT(TestWindow->GetWindowSize() == size);
    }

    ssTEST("RenderSizeTest")
    {
        glm::ivec2 size(500, 500);
        TestWindow->SetRenderSize(size);
        
        ssTEST_OUTPUT_ASSERT(TestWindow->GetRenderSize() == size);
        
        //SFML can't return window size
        #ifndef SSGUI_MAIN_BACKEND_SFML
            ssTEST_OUTPUT_ASSERT("Against Window Size", TestWindow->GetRenderSize() != TestWindow->GetWindowSize());
        #else
            ssTEST_OUTPUT_SKIP("Against Window Size");
        #endif
    }

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
    }

    ssTEST("CloseTest")
    {
        ssTEST_OUTPUT_ASSERT("Not Closed", !TestWindow->IsClosed());
        TestWindow->Close();
        ssTEST_OUTPUT_ASSERT(TestWindow->IsClosed());   
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    }

    ssTEST("CloseEventTest")
    {
        bool called = false;
        int id = TestWindow->AddOnCloseEvent(   [&]()
                                            {
                                                called = true;
                                                TestWindow->AbortClosing();
                                            });
        TestWindow->Close();
        
        ssTEST_OUTPUT_ASSERT("Event Called", called);
        ssTEST_OUTPUT_ASSERT("Close Abort", !TestWindow->IsClosed());
        
        if(!TestWindow->IsClosed())
        {
            called = false;
            TestWindow->RemoveOnCloseEvent(id);
            TestWindow->Close();
            ssTEST_OUTPUT_ASSERT("Event Remove", !called);
        }
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
    }

    ssTEST("TitleTest")
    {
        std::wstring someTitle = L"someTitle";
        TestWindow->SetTitle(someTitle);
        ssTEST_OUTPUT_ASSERT(TestWindow->GetTitle() == someTitle);
    }

    ssTEST("VisibleTest")
    {
        TestWindow->SetVisible(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsVisible());
        TestWindow->SetVisible(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsVisible());
    }

    ssTEST("VsyncTest()")
    {
        TestWindow->SetVSync(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsVSync());
        TestWindow->SetVSync(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsVSync());
    }

    ssTEST("MSAATest()")
    {
        TestWindow->SetMSAA(4);
        ssTEST_OUTPUT_ASSERT(TestWindow->GetMSAA() == 4);
    }

    ssTEST("TitlebarTest()")
    {
        TestWindow->SetTitlebar(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->HasTitlebar());
        TestWindow->SetTitlebar(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->HasTitlebar());
    }

    ssTEST("ResizableTest()")
    {
        TestWindow->SetResizable(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsResizable());
        TestWindow->SetResizable(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsResizable());
    }

    ssTEST("CloseButtonTest()")
    {
        TestWindow->SetCloseButton(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->HasCloseButton());
        TestWindow->SetCloseButton(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->HasCloseButton());
    }

    ssTEST("WindowModeTest()")
    {
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::BORDERLESS);
        ssTEST_OUTPUT_ASSERT("BORDERLESS", TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
        ssTEST_OUTPUT_ASSERT("NORMAL", TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::FULLSCREEN);
        ssTEST_OUTPUT_ASSERT("FULLSCREEN", TestWindow->GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN);
        
        TestWindow->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
    }

    ssTEST("CloneTest()")
    {
        auto* windowClone = TestWindow->Clone();
        ssTEST_OUTPUT_ASSERT(windowClone != nullptr);
        ssGUI::Factory::Dispose(windowClone);
    }

    ssTEST("GetRawHandleTest()")
    {
        ssTEST_OUTPUT_ASSERT(TestWindow->GetRawHandle() != nullptr);
    }

    ssTEST_END();
}