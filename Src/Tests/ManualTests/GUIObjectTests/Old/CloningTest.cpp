#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/Extensions/Dockable.hpp"
#include "ssGUI/Extensions/Docker.hpp"
#include "ssGUI/Extensions/Mask.hpp"
#include "ssGUI/Extensions/Layout.hpp"

#include "ssLogger/ssLog.hpp"

// Cloning test
int main()
{
    ssGUI::MainWindow mainWindow;

    //Creating window
    ssGUI::Window window;
    window.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    window.SetSize(glm::vec2(150, 150));
    window.SetParent(&mainWindow);
    ssGUI::Extensions::Dockable* dock = ssGUI::Factory::Create<ssGUI::Extensions::Dockable>();
    window.AddExtension(dock);

    //Creating window
    auto window2 = window.Clone(false);
    static_cast<ssGUI::Window*>(window2)->SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));    

    auto docker = ssGUI::Factory::Create<ssGUI::Extensions::Docker>();
    docker->SetChildrenDockerUseThisSettings(false);
    
    auto bor = ssGUI::Factory::Create<ssGUI::Extensions::Border>();
    
    ssGUI::Widget mainWindowWidget;

    mainWindowWidget.AddExtension(docker);
    mainWindowWidget.AddExtension(bor);
    mainWindowWidget.SetBackgroundColor(glm::u8vec4(200, 200, 200, 255));
    mainWindowWidget.SetMinSize(glm::vec2(200, 200));
    mainWindowWidget.SetParent(&mainWindow);

    auto layout = ssGUI::Factory::Create<ssGUI::Extensions::Layout>();
    layout->SetCoverFullLength(false);
    layout->SetHorizontalLayout(true);
    mainWindow.AddExtension(layout);

    mainWindow.MoveChildrenIteratorToLast();
    ssGUI::Hierarchy::ChildToken lastIt = mainWindow.GetCurrentChildToken();
    mainWindow.MoveChildrenIteratorToFirst();
    ssGUI::Hierarchy::ChildToken firstIt = mainWindow.GetCurrentChildToken();
    mainWindow.ChangeChildOrderToBeforePosition(lastIt, firstIt);

    //Creating ssGUIManager and run it
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();
            
            if( inputInterface->IsInputExistCurrentFrame(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->IsInputExistLastFrame(ssGUI::Enums::SystemKey::ENTER))
            {
                mainWindowWidget.Clone(true);
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();

    return 0;
}