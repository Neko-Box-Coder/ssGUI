#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"



//Docking test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    //window.SetBackgroundColor(glm::u8vec4(255, 127, 127, 255));
    window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::ivec2(150, 150));
    window.SetParent(&mainWindow);
    DEBUG_LINE("window: "<<&window);

    //Creating window
    ssGUI::Window window2;
    DEBUG_LINE("window2: "<<&window2);
    window2.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window2.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
    window2.SetSize(glm::ivec2(150, 150));
    window2.SetParent(&mainWindow);

    ssGUI::Window* window3 = new ssGUI::Window();
    window3->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window3->SetTitlebarColor(glm::u8vec4(255, 255, 255, 255));
    window3->SetSize(glm::ivec2(150, 150));
    window3->SetParent(&mainWindow);
    DEBUG_LINE("window3: "<<&window3);

    ssGUI::Window* window4 = new ssGUI::Window();
    window4->SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));
    window4->SetSize(glm::ivec2(150, 150));
    window4->SetParent(&mainWindow);
    DEBUG_LINE("window4: "<<&window4);

    ssGUI::Extensions::Dockable* dock = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock2 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock3 = new ssGUI::Extensions::Dockable();
    ssGUI::Extensions::Dockable* dock4 = new ssGUI::Extensions::Dockable();

    window.AddExtension(dock);
    window2.AddExtension(dock2);
    window3->AddExtension(dock3);
    window4->AddExtension(dock4);

    ssGUI::Extensions::Docker* docker = new ssGUI::Extensions::Docker();
    docker->SetChildrenDockerUseThisSettings(true);
    ssGUI::StandardWindow* defaultStandardWindow = new ssGUI::StandardWindow();
    defaultStandardWindow->GetWindowTitleObject()->SetText("Docker");

    ssGUI::Extensions::Docker::SetDefaultGeneratedDockerWindow(defaultStandardWindow);
    
    ssGUI::Extensions::AdvancedPosition* ap = new ssGUI::Extensions::AdvancedPosition();
    ssGUI::Extensions::AdvancedSize* as = new ssGUI::Extensions::AdvancedSize();
    ssGUI::Extensions::Border* bor = new ssGUI::Extensions::Border();
    ssGUI::Extensions::Mask* mask = new ssGUI::Extensions::Mask();

    ap->SetHorizontalAnchor(ssGUI::Extensions::AdvancedPosition::HorizontalAnchor::CENTER);
    ap->SetVerticalAnchor(ssGUI::Extensions::AdvancedPosition::VerticalAnchor::CENTER);
    as->SetHorizontalPercentage(0.8);
    as->SetHorizontalUsePercentage(true);
    as->SetVerticalPercentage(0.8);
    as->SetVerticalUsePercentage(true);
    
    ssGUI::Widget mainWindowWidget;
    DEBUG_LINE("mainWindowWidget: "<<&mainWindowWidget);
    mainWindowWidget.AddExtension(docker);
    auto dockerLayout = static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME));
    dockerLayout->SetPadding(2);

    // mainWindowWidget.AddExtension(new ssGUI::Extensions::Layout());
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->SetPadding(0);
    //static_cast<ssGUI::Extensions::Layout*>(mainWindowWidget.GetExtension(ssGUI::Extensions::Layout::EXTENSION_NAME))->
    //    ExcludeObject(window3ObjPtr);
    mainWindowWidget.AddExtension(ap);
    mainWindowWidget.AddExtension(as);
    mainWindowWidget.AddExtension(bor);
    mainWindowWidget.AddExtension(mask);
    mainWindowWidget.SetBackgroundColor(glm::u8vec4(200, 200, 200, 255));
    
    mainWindowWidget.SetSize(glm::ivec2(400, 400));
    
    mainWindowWidget.SetParent(&mainWindow);

    window4->SetParent(&mainWindowWidget);
    dock4->SetTopLevelParent(&mainWindowWidget);

    mainWindow.MoveChildrenIteratorToLast();
    std::list<ssGUI::ssGUIObjectIndex>::iterator lastIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.MoveChildrenIteratorToFirst();
    std::list<ssGUI::ssGUIObjectIndex>::iterator firstIt = mainWindow.GetCurrentChildReferenceIterator();
    mainWindow.ChangeChildOrderToBeforePosition(lastIt, firstIt);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddOnUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                // if(window.GetBackgroundColor().a == 255)
                //     window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 127));
                // else
                //     window.SetBackgroundColor(glm::u8vec4(127, 127, 127, 255));

                DEBUG_LINE(&window<<" window parent: "<<window.GetParent());
                DEBUG_LINE(&window2<<" window2 parent: "<<window2.GetParent());
                DEBUG_LINE(window3<<" window3 parent: "<<window3->GetParent());
                DEBUG_LINE(window4<<" window4 parent: "<<window4->GetParent());
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}