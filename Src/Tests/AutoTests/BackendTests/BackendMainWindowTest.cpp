#include "ssGUI/Backend/BackendFactory.hpp"
#include "ssLogger/ssLog.hpp"
#include "ssGUI/TestBase.hpp"
#include <thread>

ssGUI::Backend::BackendDrawingInterface* drawing = nullptr;
ssGUI::Backend::BackendMainWindowInterface* window = nullptr;
ssGUI::Backend::BackendSystemInputInterface* inputs = nullptr;

SSGUI_TEST_INIT();

void SetUp()
{
    drawing = ssGUI::Backend::BackendFactory::CreateBackendDrawingInterface();
    window = ssGUI::Backend::BackendFactory::CreateBackendMainWindowInterface();
    inputs = ssGUI::Backend::BackendFactory::CreateBackendInputInterface();
    inputs->UpdateInput();
}

void CleanUp()
{
    ssGUI::Factory::Dispose(drawing);
    ssGUI::Factory::Dispose(window);
    ssGUI::Factory::Dispose(inputs);
}

void WindowPositionTest()
{
    //SSGUI_TEST_TITLE("SetWindowPosition And GetWindowPosition");
    
    glm::ivec2 pos(300, 300);
    window->SetWindowPosition(pos);
    inputs->UpdateInput();
    
    //This is needed for X11 
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->GetWindowPosition() == pos);
}

void GetPositionOffsetTest()
{
    SSGUI_TEST_OUTPUT_ASSERT(   __func__, 
                                window->GetPositionOffset().x >= 0 && window->GetPositionOffset().y);
}

void WindowSizeTest()
{
    glm::ivec2 size(500, 500);
    window->SetWindowSize(size);
    inputs->UpdateInput();
    
    //This is needed for X11 
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->GetWindowSize() == size);
}

void RenderSizeTest()
{
    glm::ivec2 size(500, 500);
    window->SetRenderSize(size);
    inputs->UpdateInput();
    
    //This is needed for X11 
    std::this_thread::sleep_for(std::chrono::milliseconds(16));
    
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->GetRenderSize() == size);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(Against Window Size)"), window->GetRenderSize() != window->GetWindowSize());
}

void CloseTest()
{
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(Not Closed)"), !window->IsClosed());
    window->Close();
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->IsClosed());
    CleanUp();
    SetUp();
}

void CloseEventTest()
{
    bool called = false;
    int id = window->AddOnCloseEvent(   [&]()
                                        {
                                            called = true;
                                            window->AbortClosing();
                                        });
    window->Close();
    
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(Event Called)"), called);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(Close Abort)"), !window->IsClosed());
    
    if(!window->IsClosed())
    {
        called = false;
        window->RemoveOnCloseEvent(id);
        window->Close();
        SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(Event Remove)"), !called);
    }
    
    CleanUp();
    SetUp();
}

void TitleTest()
{
    std::wstring someTitle = L"someTitle";
    window->SetTitle(someTitle);
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->GetTitle() == someTitle);
}

void VisibleTest()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    window->SetVisible(false);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(False)"), !window->IsVisible());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    window->SetVisible(true);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(True)"), window->IsVisible());
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void VsyncTest()
{
    window->SetVSync(false);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(False)"), !window->IsVSync());
    window->SetVSync(true);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(True)"), window->IsVSync());
}

void MSAATest()
{
    window->SetMSAA(4);
    SSGUI_TEST_OUTPUT_ASSERT(__func__, window->GetMSAA() == 4);
}

void TitlebarTest()
{
    window->SetTitlebar(false);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(False)"), !window->HasTitlebar());
    window->SetTitlebar(true);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(True)"), window->HasTitlebar());
}

void ResizableTest()
{
    window->SetResizable(false);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(False)"), !window->IsResizable());
    window->SetResizable(true);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(True)"), window->IsResizable());
}

void CloseButtonTest()
{
    window->SetCloseButton(false);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(False)"), !window->HasCloseButton());
    window->SetCloseButton(true);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(True)"), window->HasCloseButton());
}

void WindowModeTest()
{
    window->SetWindowMode(ssGUI::Enums::WindowMode::BORDERLESS);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(BORDERLESS)"), window->GetWindowMode() == ssGUI::Enums::WindowMode::BORDERLESS);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(NORMAL)"), window->GetWindowMode() == ssGUI::Enums::WindowMode::NORMAL);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::FULLSCREEN);
    SSGUI_TEST_OUTPUT_ASSERT(__func__+std::string("(FULLSCREEN)"), window->GetWindowMode() == ssGUI::Enums::WindowMode::FULLSCREEN);
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    
    window->SetWindowMode(ssGUI::Enums::WindowMode::NORMAL);
}

void CloneTest()
{
    try
    {
        auto* windowClone = window->Clone();
        SSGUI_TEST_OUTPUT_ASSERT(__func__, false);
    }
    catch(const std::exception& ex)
    {
        ssLOG_LINE("Exception caught: "<<ex.what());
        ssLOG_LINE("Don't worry, this is expected 😉")
        SSGUI_TEST_OUTPUT_ASSERT(__func__, true);
    }
    catch(...)
    {
        throw;
        SSGUI_TEST_OUTPUT_ASSERT(__func__, false);   
    }
}

int main()
{
    SetUp();

    SSGUI_TEST_TITLE(SSGUI_TEST_FILE_NAME());
    
    try
    {
        WindowPositionTest();
        GetPositionOffsetTest();
        WindowSizeTest();
        RenderSizeTest();
        CloseTest();
        CloseEventTest();
        TitleTest();
        //Can't test SetIcon
        VisibleTest();
        VsyncTest();
        //Can't test SetFocus
        //Can't test IsFocused
        //Can't test AddFocusChangedByUserEvent
        //Can't test RemoveFocusChangedByUserEvent
        MSAATest();
        TitlebarTest();
        ResizableTest();
        CloseButtonTest();
        WindowModeTest();
        //Can't test SetGLContext
        CloneTest();
        //Can't test GetRawHandle
    }
    catch(...)
    {
        CleanUp();
        throw;
    }
    
    CleanUp();

    SSGUI_TEST_END();
}

