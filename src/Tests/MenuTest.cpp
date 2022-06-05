#include "ssGUI/HeaderGroups/StandardGroup.hpp"
#include "ssGUI/DebugAndBuild/ssGUIDebugInit.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"
#include "ssGUI/GUIObjectClasses/Menu.hpp"


//Mask test
int main()
{
    //Create the main window
    ssGUI::MainWindow mainWindow;
    mainWindow.SetSize(glm::vec2(800, 800));

    ssGUI::Menu menu;
    menu.SetPosition(glm::vec2(100, 100));
    menu.SetParent(&mainWindow);
    menu.SetVisible(false);

    ssGUI::MenuItem menuItem;
    menuItem.GetButtonTextObject()->SetText("Test menu item");
    menuItem.SetButtonColor(menu.GetBackgroundColor());
    menu.RegisterMenuItem(menuItem.Clone(true));
    menu.RegisterMenuItem(menuItem.Clone(true));
    menu.RegisterMenuItem(menuItem.Clone(true));
    menu.RegisterMenuItem(menuItem.Clone(true));
    auto menuItem5 = menuItem.Clone(true);
    menuItem5->SetInteractable(false);

    auto subMenu = menu.Clone(true);
    subMenu->SetParent(&menu);
    auto subMenuItem = menuItem.Clone(true);
    subMenuItem->GetButtonTextObject()->SetText("Call sub menu >");
    menu.RegisterMenuItem(subMenuItem);
    menu.LinkMenuItemToSubMenu(subMenuItem, subMenu);

    ssGUI::StandardWindow standardWindow;
    standardWindow.SetParent(&mainWindow);
    
    // auto standardWindow2 = standardWindow.Clone(true);
    
    standardWindow.RegisterRightClickMenu(&menu);

    //Create the GUIManager, add the main window and start running
    ssGUI::ssGUIManager guiManager;
    guiManager.AddPostUpdateEventListener
    (
        [&]()
        {
            ssGUI::Backend::BackendSystemInputInterface* inputInterface = guiManager.GetBackendInputInterface();

            // if(inputInterface->GetCurrentMouseButton(ssGUI::Enums::MouseButton::RIGHT) &&
            //     !inputInterface->GetLastMouseButton(ssGUI::Enums::MouseButton::RIGHT))
            // {
            //     //DEBUG_LINE("menuItem.GetButtonState(): "<<(int)menuItem.GetButtonState());
            //     menu.SpawnMenu(inputInterface->GetCurrentMousePosition(&mainWindow));
            // }

            if(inputInterface->GetCurrentKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER) &&
                !inputInterface->GetLastKeyPresses().IsSystemKeyPresent(ssGUI::Enums::SystemKey::ENTER))
            {
                DEBUG_LINE("menu.IsFocused(): "<<menu.IsFocused());
                DEBUG_LINE("menu.IsRedrawNeeded(): "<<menu.IsRedrawNeeded());
                DEBUG_LINE("menuItem5.IsVisible(): "<<menuItem5->IsVisible());
            }
        }
    );

    guiManager.AddGUIObject((ssGUI::GUIObject*)&mainWindow);
    guiManager.StartRunning();
    return 0;
}
