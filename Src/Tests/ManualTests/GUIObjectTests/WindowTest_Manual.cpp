#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssLogger/ssLog.hpp"

ssGUI::MainWindow* MainWindow = nullptr;
ssGUI::ssGUIManager* Manager = nullptr;
ssGUI::Window* TestWindow = nullptr;

ssGUI::Enums::GenericButtonAndKeyInput TestMode = ssGUI::Enums::NO_INPUT;

void SetUp()
{
    MainWindow = ssGUI::Create<ssGUI::MainWindow>();
    Manager = ssGUI::Create<ssGUI::ssGUIManager>();
    TestWindow = MainWindow->AddChild<ssGUI::Window>();
}

void CleanUp()
{
    ssGUI::Dispose(Manager);
}

void Instructions()
{
    ssLOG_SIMPLE("");
    ssLOG_SIMPLE("Press 1 to call ResizeTest");
    ssLOG_SIMPLE("Press 2 to call DragTest");
    ssLOG_SIMPLE("Press 3 to call OnTopWhenFocusedTest");
    ssLOG_SIMPLE("Press 4 to call CloneTest");
    ssLOG_SIMPLE("");
}

void ResizeTest(bool firstRun)  //1
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 1 to go back");
        ssLOG_SIMPLE("Press 2 to cycle SetResizeType");
        ssLOG_SIMPLE("Press 3 to GetResizeType");
        ssLOG_SIMPLE("Press 4 to GetResizeHitbox");
        ssLOG_SIMPLE("Press 5 to SetResizeHitbox(10)");
        ssLOG_SIMPLE("Press 6 to IsResizing");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
    {
        auto newResizeType = static_cast<ssGUI::Enums::ResizeType>
            (((int)TestWindow->GetResizeType() + 1 ) % ((int)ssGUI::Enums::ResizeType::COUNT));
        TestWindow->SetResizeType(newResizeType);
        
        ssLOG_SIMPLE("SetResizeType: "<<ssGUI::Enums::ResizeTypeToString(newResizeType));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        ssLOG_SIMPLE("GetResizeType: "<<ssGUI::Enums::ResizeTypeToString(TestWindow->GetResizeType()));
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        ssLOG_SIMPLE("GetResizeHitbox: "<<TestWindow->GetResizeHitbox());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        TestWindow->SetResizeHitbox(10);
        ssLOG_SIMPLE("SetResizeHitbox called");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        ssLOG_SIMPLE("IsResizing: "<<TestWindow->IsResizing());
    }
}

void DragTest(bool firstRun)    //2
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 2 to go back");
        ssLOG_SIMPLE("Press 3 to cycle SetDraggable");
        ssLOG_SIMPLE("Press 4 to IsDraggable");
        ssLOG_SIMPLE("Press 5 to toggle GetWindowDragState when changed");
        ssLOG_SIMPLE("Press 6 to IsDragging");
        ssLOG_SIMPLE("Press 7 to toggle print GetResizeDragData every second");
        ssLOG_SIMPLE("Press 8 to toggle SetResizeDragData every second");
    }
    
    static bool GetResizeDragDataMode = false;
    static uint64_t LastGetDataTime = 0;
    static bool SetResizeDragDataMode = false;
    static uint64_t LastSetDataTime = 0;
    static bool GetWindowDragStateMode = false;
    static auto LastDragState = ssGUI::Enums::WindowDragState::NONE;
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
    {
        bool newDraggable = !TestWindow->IsDraggable();
        TestWindow->SetDraggable(newDraggable);
        ssLOG_SIMPLE("SetDraggable: "<<newDraggable);
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        ssLOG_SIMPLE("IsDraggable: "<<TestWindow->IsDraggable());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        GetWindowDragStateMode = !GetWindowDragStateMode;
        ssLOG_SIMPLE("Toggled");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SIX))
    {
        ssLOG_SIMPLE("IsDragging: "<<TestWindow->IsDragging());
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::SEVEN))
    {
        GetResizeDragDataMode = !GetResizeDragDataMode;
        ssLOG_SIMPLE("Toggled");
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::EIGHT))
    {
        SetResizeDragDataMode = !SetResizeDragDataMode;
        ssLOG_SIMPLE("Toggled");
    }
    
    if(GetResizeDragDataMode)
    {
        if(Manager->GetElapsedTimeInMillisecond() - LastGetDataTime > 1000)
        {
            auto curResizeDragData = TestWindow->GetResizeDragData();
            
            ssLOG_SIMPLE("CurrentDragState: "<<ssGUI::Enums::WindowDragStateToString(curResizeDragData.CurrentDragState));
            ssLOG_SIMPLE("ResizingTop: "<<curResizeDragData.ResizingTop);
            ssLOG_SIMPLE("ResizingBot: "<<curResizeDragData.ResizingBot);
            ssLOG_SIMPLE("ResizingLeft: "<<curResizeDragData.ResizingLeft);
            ssLOG_SIMPLE("ResizingRight: "<<curResizeDragData.ResizingRight);
            ssLOG_SIMPLE("Dragging: "<<curResizeDragData.Dragging);
            ssLOG_SIMPLE("TransformTotalMovedDistance: "<<curResizeDragData.TransformTotalMovedDistance.x<<", "<<curResizeDragData.TransformTotalMovedDistance.y);
            ssLOG_SIMPLE("OnTransformBeginSize: "<<curResizeDragData.OnTransformBeginSize.x<<", "<<curResizeDragData.OnTransformBeginSize.y);
            ssLOG_SIMPLE("MouseDownPosition: "<<curResizeDragData.MouseDownPosition.x<<", "<<curResizeDragData.MouseDownPosition.y);
            ssLOG_SIMPLE("");
            
            LastGetDataTime = Manager->GetElapsedTimeInMillisecond();
        }
    }
    else
    {
        LastGetDataTime = 0;
    }
    
    if(SetResizeDragDataMode)
    {
        if(Manager->GetElapsedTimeInMillisecond() - LastSetDataTime > 1000)
        {
            auto curResizeDragData = TestWindow->GetResizeDragData();
            curResizeDragData.TransformTotalMovedDistance = glm::vec2();
            TestWindow->SetResizeDragData(curResizeDragData);
            ssLOG_SIMPLE("TransformTotalMovedDistance reset by calling SetResizeDragData");
            LastSetDataTime = Manager->GetElapsedTimeInMillisecond();
        }
    }
    else
    {
        LastSetDataTime = 0;
    }
    
    if(GetWindowDragStateMode)
    {
        auto curState = TestWindow->GetWindowDragState();
        if(curState != LastDragState)
        {
            ssLOG_SIMPLE("GetWindowDragState: "<<ssGUI::Enums::WindowDragStateToString(curState));
            LastDragState = curState;
        }
    }
    else
    {
        LastDragState = ssGUI::Enums::WindowDragState::NONE;
    }
}

void OnTopWhenFocusedTest(bool firstRun)    //3
{
    if(firstRun)
    {
        ssLOG_SIMPLE("Press 3 to go back");
        ssLOG_SIMPLE("Press 4 to cycle SetOnTopWhenFocused");
        ssLOG_SIMPLE("Press 5 to IsOnTopWhenFocused");
    }
    
    if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FOUR))
    {
        bool newFocus = !TestWindow->IsOnTopWhenFocused();
        TestWindow->SetOnTopWhenFocused(newFocus);
        ssLOG_SIMPLE("SetOnTopWhenFocused: "<<newFocus);
    }
    else if(Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::FIVE))
    {
        ssLOG_SIMPLE("IsOnTopWhenFocused: "<<TestWindow->IsOnTopWhenFocused());
    }
}

void CloneTest()    //4
{
    TestWindow->Clone(true)->SetPosition(TestWindow->GetPosition() + glm::vec2(150, 150));
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
                    ResizeTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::ONE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::TWO):
                    DragTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::TWO))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::THREE):
                    OnTopWhenFocusedTest(lastTestMode == ssGUI::Enums::NO_INPUT);
                    if(lastTestMode != ssGUI::Enums::NO_INPUT && Manager->IsButtonOrKeyDown(ssGUI::Enums::NumberKey::THREE))
                        TestMode = ssGUI::Enums::NO_INPUT;
                    break;
                case QUICK_CAST(ssGUI::Enums::NumberKey::FOUR):
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