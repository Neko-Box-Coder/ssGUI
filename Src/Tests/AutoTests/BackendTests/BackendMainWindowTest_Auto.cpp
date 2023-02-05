#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssLogger/ssLog.hpp"
#include "ssGUI/Factory.hpp"
#include "ssTest.hpp"

ssGUI::Backend::BackendDrawingInterface* drawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* window = nullptr;
ssGUI::Backend::BackendSystemInputInterface* inputs = nullptr;

ssTEST_INIT();

ssTEST_SET_UP
{
    drawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    window = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    inputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
    inputs->UpdateInput();
}

ssTEST_CLEAN_UP
{
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);
}

ssTEST("WindowPositionTest")
{
    glm::ivec2 pos(300, 300);
    window->SetWindowPosition(pos);
    
    ssTEST_OUTPUT_ASSERT(window->GetWindowPosition() == pos);
    //ssLOG_LINE("window->GetWindowPosition(): "<<window->GetWindowPosition().x<<", "<<window->GetWindowPosition().y);
}

ssTEST("GetPositionOffsetTest")
{
    ssTEST_OUTPUT_ASSERT(window->GetPositionOffset().x >= 0 && window->GetPositionOffset().y >= 0);
    
    //ssLOG_LINE("window->GetPositionOffset(): "<<window->GetPositionOffset().x<<", "<<window->GetPositionOffset().y);
}

ssTEST("WindowSizeTest")
{
    glm::ivec2 size(500, 500);
    window->SetWindowSize(size);
    
    glm::ivec2 windowSize = window->GetWindowSize();
    //ssLOG_LINE("windowSize: "<<windowSize.x<<", "<<windowSize.y);
    ssTEST_OUTPUT_ASSERT(window->GetWindowSize() == size);
}

ssTEST("RenderSizeTest")
{
    glm::ivec2 size(500, 500);
    window->SetRenderSize(size);
    
    ssTEST_OUTPUT_ASSERT(window->GetRenderSize() == size);
    
    //SFML can't return window size
    #ifndef SSGUI_MAIN_BACKEND_SFML
        ssTEST_OUTPUT_ASSERT("Against Window Size", window->GetRenderSize() != window->GetWindowSize());
    #else
        ssTEST_OUTPUT_SKIP("Against Window Size");
    #endif
}

ssTEST("ValidateBothSizesTest")
{
    window->SetRenderSize(glm::ivec2(1280, 720));
    glm::ivec2 windowSize = window->GetWindowSize();
    window->SetWindowSize(windowSize);
    ssTEST_OUTPUT_ASSERT("1", window->GetRenderSize() == glm::ivec2(1280, 720));
    
    window->SetWindowSize(glm::ivec2(600, 400));
    glm::ivec2 renderSize = window->GetRenderSize();
    window->SetRenderSize(renderSize);
    ssTEST_OUTPUT_ASSERT("2", window->GetWindowSize() == glm::ivec2(600, 400));
}

ssTEST("CloseTest")
{
    ssTEST_OUTPUT_ASSERT("Not Closed", !window->IsClosed());
    window->Close();
    ssTEST_OUTPUT_ASSERT(window->IsClosed());   
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
}

ssTEST("CloseEventTest")
{
    bool called = false;
    int id = window->AddOnCloseEvent(   [&]()
                                        {
                                            called = true;
                                            window->AbortClosing();
                                        });
    window->Close();
    
    ssTEST_OUTPUT_ASSERT("Event Called", called);
    ssTEST_OUTPUT_ASSERT("Close Abort", !window->IsClosed());
    
    if(!window->IsClosed())
    {
        called = false;
        window->RemoveOnCloseEvent(id);
        window->Close();
        ssTEST_OUTPUT_ASSERT("Event Remove", !called);
    }
    
    ssTEST_CALL_CLEAN_UP();
    ssTEST_CALL_SET_UP();
}

ssTEST("TitleTest")
{
    std::wstring someTitle = L"someTitle";
    window->SetTitle(someTitle);
    ssTEST_OUTPUT_ASSERT(window->GetTitle() == someTitle);
}

ssTEST("VisibleTest")
{
    window->SetVisible(false);
    ssTEST_OUTPUT_ASSERT("False", !window->IsVisible());
    window->SetVisible(true);
    ssTEST_OUTPUT_ASSERT("True", window->IsVisible());
}

ssTEST("VsyncTest()")
{
    window->SetVSync(false);
    ssTEST_OUTPUT_ASSERT("False", !window->IsVSync());
    window->SetVSync(true);
    ssTEST_OUTPUT_ASSERT("True", window->IsVSync());
}

ssTEST("MSAATest()")
{
    window->SetMSAA(4);
    ssTEST_OUTPUT_ASSERT(window->GetMSAA() == 4);
}

ssTEST("TitlebarTest()")
{
    window->SetTitlebar(false);
    ssTEST_OUTPUT_ASSERT("False", !window->HasTitlebar());
    window->SetTitlebar(true);
    ssTEST_OUTPUT_ASSERT("True", window->HasTitlebar());
}

ssTEST("ResizableTest()")
{
    window->SetResizable(false);
    ssTEST_OUTPUT_ASSERT("False", !window->IsResizable());
    window->SetResizable(true);
    ssTEST_OUTPUT_ASSERT("True", window->IsResizable());
}

ssTEST("CloseButtonTest()")
{
    window->SetCloseButton(false);
    ssTEST_OUTPUT_ASSERT("False", !window->HasCloseButton());
    window->SetCloseButton(true);
    ssTEST_OUTPUT_ASSERT("True", window->HasCloseButton());
}

ssTEST("WindowModeTest()")
{
    window->SetWindowMode(ssGUI::Enums::WindowMode::BORDERLESS);
    ssTEST_OUTPUT_ASSERT("BORDERLESS", window->GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS);
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
    ssTEST_OUTPUT_ASSERT("NORMAL", window->GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL);
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::FULLSCREEN);
    ssTEST_OUTPUT_ASSERT("FULLSCREEN", window->GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN);
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
}

ssTEST("CloneTest()")
{
    auto* windowClone = window->Clone();
    ssTEST_OUTPUT_ASSERT(windowClone != nullptr);
    ssGUI::Factory::Dispose(windowClone);
}

ssTEST("GetRawHandleTest()")
{
    ssTEST_OUTPUT_ASSERT(window->GetRawHandle() != nullptr);
}

ssTEST_END();
