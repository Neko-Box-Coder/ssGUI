#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

ssGUI::Window* TestWindow = nullptr;

int main()
{
    ssTEST_INIT();

    ssTEST_SET_UP
    {
        TestWindow = ssGUI::Create<ssGUI::Window>();
    }

    ssTEST_CLEAN_UP
    {
        ssGUI::Dispose(TestWindow);
    }

    ssTEST("CloseTest")
    {
        TestWindow->SetDeleteAfterClosed(true);
        ssTEST_OUTPUT_ASSERT("IsDeleteAfterClosed (True)", TestWindow->IsDeleteAfterClosed())
        TestWindow->Close();
        ssTEST_OUTPUT_ASSERT("Closed/Deleted", TestWindow->IsClosed() && TestWindow->Internal_IsDeleted());
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        
        TestWindow->SetDeleteAfterClosed(false);
        ssTEST_OUTPUT_ASSERT("IsDeleteAfterClosed (False)", !TestWindow->IsDeleteAfterClosed())
        TestWindow->Close();
        ssTEST_OUTPUT_ASSERT("SetDeleteAfterClosed (False)", TestWindow->IsClosed() && !TestWindow->Internal_IsDeleted());
        
        ssTEST_CALL_CLEAN_UP();
        ssTEST_CALL_SET_UP();
        
        TestWindow->AddEventCallback(ssGUI::Enums::EventType::BEFORE_WINDOW_CLOSE)->AddEventListener
        (
            "Key",
            [&](ssGUI::EventInfo info)
            {
                TestWindow->AbortClosing();
            }
        );
        TestWindow->Close();
        
        ssTEST_OUTPUT_ASSERT("Abort closing", !TestWindow->IsClosed() && !TestWindow->Internal_IsDeleted());
        ssTEST_OUTPUT_SKIP("Internal_OnClose");
    }
    
    ssTEST("ClosableTest")
    {
        TestWindow->SetClosable(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->IsClosable());
        TestWindow->SetClosable(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->IsClosable());
    }
    
    ssTEST("TitlebarTest")
    {
        TestWindow->SetTitlebar(true);
        ssTEST_OUTPUT_ASSERT("True", TestWindow->HasTitlebar());
        TestWindow->SetTitlebar(false);
        ssTEST_OUTPUT_ASSERT("False", !TestWindow->HasTitlebar());
        TestWindow->SetTitlebar(true);
        TestWindow->SetTitlebarHeight(50);
        ssTEST_OUTPUT_ASSERT("Height", TestWindow->GetTitlebarHeight() == 50);
        TestWindow->SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
        ssTEST_OUTPUT_ASSERT("Color", TestWindow->GetTitlebarColor() == glm::u8vec4(255, 127, 127, 255));
    
        TestWindow->SetAdaptiveTitlebarColor(true);
        TestWindow->SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
        ssTEST_OUTPUT_ASSERT("Adaptive", TestWindow->GetTitlebarColor() == glm::u8vec4(228, 100, 100, 255));
    }

    ssTEST_SKIP("ResizeTest")
    {
    }
    
    ssTEST_SKIP("DragTest")
    {
    }
    
    ssTEST_SKIP("OnTopWhenFocusedTest")
    {
    }
    
    ssTEST("RenderSizeTest")
    {
        TestWindow->SetRenderSize(glm::ivec2(70, 70));
        ssTEST_OUTPUT_ASSERT(   TestWindow->GetRenderSize() == glm::ivec2(70, 70) && 
                                TestWindow->GetSize() - glm::vec2(0, TestWindow->GetTitlebarHeight()) == glm::vec2(70, 70));
    }
    
    ssTEST("BackgroundColorTest")
    {
        TestWindow->SetBackgroundColor(glm::u8vec4(255, 127, 127, 255));
        ssTEST_OUTPUT_ASSERT(TestWindow->GetBackgroundColor() == glm::u8vec4(255, 127, 127, 255));
    }
    
    ssTEST("GetTypeTest")
    {
        ssTEST_OUTPUT_ASSERT(TestWindow->GetType() == ssGUI::Enums::GUIObjectType::WINDOW);
    }
    
    ssTEST_SKIP("CloneTest")
    {
    }

    ssTEST_END();
}
