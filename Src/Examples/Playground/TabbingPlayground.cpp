
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/TabArea.hpp"
#include "ssGUI/Extensions/Tabbable.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardWindow.hpp"
#include "ssGUI/HeaderGroups/StandardGroup.hpp"

#include <tuple>

//Layout example
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(450, 650));

    ssGUI::Window* content1 = mainWindow.AddChild<ssGUI::Window>();
    ssGUI::Window* content2 = mainWindow.AddChild<ssGUI::Window>();

    content1->AddExtension<ssGUI::Extensions::Tabbable>();
    content2->AddExtension<ssGUI::Extensions::Tabbable>();
    content1->SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
    ssGUI::Text* testText =
        content1->AddChild<ssGUI::Text>(ssGUI::Enums::AlignmentHorizontal::CENTER,
                                        ssGUI::Enums::AlignmentVertical::CENTER);
    testText->SetText("Hello World!");

    content2->SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));

    ssGUI::Window window;

    window.SetPosition(glm::vec2(25, 25));
    window.SetSize(glm::vec2(300, 600));
    //window.SetMinSize(glm::vec2(300, 600));

    window.SetParent(&mainWindow);
    window.AddExtension<ssGUI::Extensions::TabArea>();
    //window.GetExtension<ssGUI::Extensions::TabArea>()->SetReverseTabsOrder(true);

    ssGUI::Tab* tab = window.GetExtension<ssGUI::Extensions::TabArea>()->AddContent(content1);
    tab->SetTabColor(glm::u8vec4(255, 127, 127, 255));
    tab->SetSelectedColor(glm::u8vec4(255, 127, 127, 255));

    ssGUI::Tab* tab2 = window.GetExtension<ssGUI::Extensions::TabArea>()->AddContent(content2);
    tab2->SetTabColor(glm::u8vec4(127, 255, 127, 255));
    tab2->SetSelectedColor(glm::u8vec4(127, 255, 127, 255));

    //tab->SetTabTitleObject(nullptr);
    //tab2->SetTabTitleObject(nullptr);
    //window.GetExtension<ssGUI::Extensions::TabArea>()->SetTabBarHorizontal(true);

    //window.AddExtension<ssGUI::Extensions::DockingArea>();
    // window.GetExtension<ssGUI::Extensions::Layout>()->SetPadding(5);
    window.SetBackgroundColor(glm::u8vec4(180, 180, 180, 255));

#if 0
        ssGUI::StandardWindow window2;
        window2.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
        window2.SetTitlebarColor(glm::u8vec4(255, 127, 127, 255));
        //window2.AddExtension<ssGUI::Extensions::DockableV2>();
        window2.SetPosition(window2.GetPosition() + glm::vec2(5, 5));
        window2.AddExtension<ssGUI::Extensions::TabArea>();
        window2.SetParent(&mainWindow);
#endif

#if 0
        ssGUI::StandardWindow window3;
        window3.SetTitlebarColor(glm::u8vec4(127, 255, 127, 255));
        window3.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
        window3.AddExtension<ssGUI::Extensions::DockableV2>();
        window3.SetPosition(window2.GetPosition() + glm::vec2(5, 5));
        //window3.AddExtension<ssGUI::Extensions::Layout>();
        
        ssGUI::StandardWindow window4;
        window4.SetTitlebarColor(glm::u8vec4(127, 127, 255, 255));
        window4.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
        window4.AddExtension<ssGUI::Extensions::DockableV2>();
        window4.SetPosition(window3.GetPosition() + glm::vec2(5, 5));

        ssGUI::StandardWindow window5;
        // window5.SetTitlebarColor(glm::u8vec4(127, 127, 127, 255));
        window5.SetBackgroundColor(glm::u8vec4(100, 100, 100, 255));
        window5.AddExtension<ssGUI::Extensions::DockableV2>();
        window5.SetPosition(window4.GetPosition() + glm::vec2(5, 5));
#endif

    //window3.SetParent(&mainWindow);
    //window4.SetParent(&mainWindow);
    //window5.SetParent(&mainWindow);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;

    guiManager.AddPostGUIUpdateEventListener(
        [&]()
        {
            //ssLOG_LINE("content1->GetResizeType(): " <<
            //ssGUI::Enums::ResizeTypeToString(content1->GetResizeType()));
            //ssLOG_LINE("content2->GetResizeType(): " <<
            //ssGUI::Enums::ResizeTypeToString(content2->GetResizeType()));

            //ssLOG_LINE("window: " << &window);
            //ssLOG_LINE("window.GetMinSize(): " << window.GetMinSize());
            //ssLOG_LINE("window.GetMaxSize(): " << window.GetMaxSize());
            //ssLOG_LINE("window.GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength(): "
            //<< window.GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength());
        });

    guiManager.AddRootGUIObject((ssGUI::GUIObject*)&mainWindow);

    guiManager.StartRunning();
    return 0;
}
