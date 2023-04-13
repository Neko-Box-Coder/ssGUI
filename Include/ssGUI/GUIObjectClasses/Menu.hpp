#ifndef SSGUI_MENU_H
#define SSGUI_MENU_H

#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/Enums/AnchorType.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"
#include <unordered_map>

//namespace: ssGUI
namespace ssGUI
{
    //namespace: ssGUI::Enums
    namespace Enums
    {
        //typedef: MenuSpawnDirection
        typedef ssGUI::Enums::AnchorType MenuSpawnDirection;
        
        //function: MenuSpawnDirectionToString
        inline std::string MenuSpawnDirectionToString(MenuSpawnDirection menuDirection)
        {
            return AnchorTypeToString(menuDirection);
        }
    }
    
    /*class: ssGUI::Menu
    This class manages <ssGUI::MenuItem> to show menus such as right click menu or menu bar menu.
    Normally this should be added to MainWindow directly so that it can always be shown in front.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        glm::vec2 SpawnGlobalPosition;                                  //See <SpawnMenu>
        ssGUI::Enums::MenuSpawnDirection CurrentMenuSpawnDirection;     //See <GetMenuSpawnDirection>
        ssGUI::GUIObject* MenuTarget;                                   //See <GetMenuTarget>
    =================================================================
    ============================== C++ ==============================
    Menu::Menu() :  SpawnGlobalPosition(),
                    CurrentMenuSpawnDirection(ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT),
                    MenuTarget(nullptr)
    {
        SetSize(glm::vec2(200, GetSize().y));
        AddExtension<ssGUI::Extensions::Layout>();
        AddExtension<ssGUI::Extensions::Border>();
        GetAnyExtension<ssGUI::Extensions::Layout>()->SetPadding(2);
        GetAnyExtension<ssGUI::Extensions::Layout>()->SetSpacing(0);
        SetBackgroundColor(glm::u8vec4(150, 150, 150, 255));
        AddTag(ssGUI::Tags::FLOATING);
    }
    =================================================================
    */
    class Menu : public Widget
    {
        private:
            Menu& operator=(Menu const& other) = default;

        protected:
            glm::vec2 SpawnGlobalPosition;                                  //See <SpawnMenu>
            ssGUI::Enums::MenuSpawnDirection CurrentMenuSpawnDirection;     //See <GetMenuSpawnDirection>
            ssGUI::GUIObject* MenuTarget;                                   //See <GetMenuTarget>

            Menu(Menu const& other);

            static void CloseRootMenu(ssGUI::GUIObject* currentMenu);

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            Menu();
            virtual ~Menu() override;
            
            //function: GetMenuSpawnDirection
            //When spawning the menu, it can be positioned differently depending on the spawn position.
            //This returns the spawn direction against the globalSpawnPosition
            ssGUI::Enums::MenuSpawnDirection GetMenuSpawnDirection(glm::vec2 globalSpawnPosition);
            
            //TODO: Add SpawnMenu with direction
            
            //function: SpawnMenu
            //Spawns the menu at globalPosition
            void SpawnMenu(glm::vec2 globalPosition);

            //function: CanSpawnMenu
            //You can spawn the menu with a specified spawn direction, this function returns if such operation is possible.
            bool CanSpawnMenu(glm::vec2 globalSpawnPosition, ssGUI::Enums::MenuSpawnDirection direction);
            
            //function: ForceSpawnMenu
            //Spawn the menu with a spawn direction specified.
            void ForceSpawnMenu(glm::vec2 globalPosition, ssGUI::Enums::MenuSpawnDirection direction);

            //function: RegisterMenuItem
            //Sets the menu item's parent to the menu and register different callbacks internally
            void RegisterMenuItem(ssGUI::MenuItem* menuItem);
            
            //function: RemoveMenuItem
            //Deletes the menu item
            void RemoveMenuItem(ssGUI::MenuItem* menuItem);

            //function: AddMenuItem
            //This is equivalent to
            //======================= C++ =======================
            //ssGUI::MenuItem* menuItem = AddChild<ssGUI::MenuItem>();
            //RegisterMenuItem(menuItem);
            //return menuItem;
            //===================================================
            ssGUI::MenuItem* AddMenuItem();

            //function: LinkMenuItemToSubMenu
            //Links a sub menu against a menu item, the sub menu must be parented to the menu in order for it to work properly.
            void LinkMenuItemToSubMenu(ssGUI::MenuItem* menuItem, ssGUI::Menu* subMenu);

            //function: SetMenuTarget
            //Sets the target of the menu that is spawned on, meaning the GUI Object that "spawns" the menu. 
            //This is automatically called when calling <ssGUI::GUIObject::RegisterRightClickMenu>.
            //If you want to spawn menu programmatically instead of right clicking,
            //you can be called by <SpawnMenu> or <ForceSpawnMenu>.
            //To unset the target, set it to nullptr.
            void SetMenuTarget(ssGUI::GUIObject* target);
            
            //function: GetMenuTarget
            //Gets the target of the menu that is spawned on, meaning the GUI Object that "spawns" the menu.
            //This can return nullptr if not a specific GUI Object spawned it.
            ssGUI::GUIObject* GetMenuTarget() const;
            
            //function: GetListOfMenuItems
            //Gets all the menu items (Non recursive) under this menu
            std::vector<ssGUI::MenuItem*> GetListOfMenuItems() const;

            //function: SetFocus
            //See <ssGUI::Hierarchy::SetFocus>
            virtual void SetFocus(bool focus) override;

            //function: Internal_SetSelfFocus
            //See <ssGUI::Hierarchy::Internal_SetSelfFocus>
            virtual void Internal_SetSelfFocus(bool focus) override;

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Menu* Clone(bool cloneChildren) override;
    };
}

#endif