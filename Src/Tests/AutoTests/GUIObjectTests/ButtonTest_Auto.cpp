#include "ssTest.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include "ssGUI/Backend/Mocks/BackendDrawingMock.hpp"
#include <thread>

int main()
{
    ssGUI::ssGUIManager* manager = nullptr;
    ssGUI::MainWindow* mainWindow = nullptr;
    ssGUI::Button* testButton = nullptr;
    ssGUI::Backend::BackendDrawingMock* backendDrawingMock = nullptr;

    ssTEST_INIT();
 
    ssTEST_SET_UP
    {
        manager = ssGUI::Create<ssGUI::ssGUIManager>();
        mainWindow = ssGUI::Create<ssGUI::MainWindow>();
        testButton = mainWindow->AddChild<ssGUI::Button>();
        backendDrawingMock = static_cast<ssGUI::Backend::BackendDrawingMock*>
            (mainWindow->GetBackendDrawingInterface());
        
        manager->AddRootGUIObject(mainWindow);
        manager->StartRunningAsync();
    };

    ssTEST_CLEAN_UP
    {
        mainWindow->Close();
        
        for(int i = 0; i < 100; i++)
        {
            if(!manager->StepFrame())
                break;
        }
        
        ssLOG_LINE();
        
        //Main window and button are released automatically
        ssGUI::Dispose(manager);
    };
 
    ssTEST_DISABLE_CLEANUP_BETWEEN_TESTS();
 
    ssTEST("Initial Visual Test")
    {
        manager->StepFrame();
        
        //TODO
        //backendDrawingMock->GetRenderedEntities();
        
        std::this_thread::sleep_for(std::chrono::seconds(5));
    };
    
    

    ssTEST_END();
}