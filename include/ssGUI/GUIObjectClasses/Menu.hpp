#ifndef SSGUI_MENU
#define SSGUI_MENU

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
    }
    
    /*class: ssGUI::Menu
    This class manages <ssGUI::MenuItem> to show menus such as right click menu or menu bar menu

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        glm::vec2 SpawnGlobalPosition;
        ssGUI::Enums::MenuSpawnDirection CurrentMenuSpawnDirection;
        ssGUI::GUIObject* MenuTarget;
    =================================================================
    ============================== C++ ==============================
    Menu::Menu() : SpawnGlobalPosition(), CurrentMenuSpawnDirection(ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT), MenuTarget(nullptr)
    {
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Layout>());
        AddExtension(ssGUI::Factory::Create<ssGUI::Extensions::Border>());
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
            glm::vec2 SpawnGlobalPosition;
            ssGUI::Enums::MenuSpawnDirection CurrentMenuSpawnDirection;
            ssGUI::GUIObject* MenuTarget;

            Menu(Menu const& other);

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            Menu();
            virtual ~Menu() override;
            
            //function: GetMenuSpawnDirection
            //When spawning the menu, it can be positioned differently depending on the spawn position.
            //This returns the spawn direction against the globalSpawnPosition
            ssGUI::Enums::MenuSpawnDirection GetMenuSpawnDirection(glm::vec2 globalSpawnPosition);
            
            //function: SpawnMenu
            //Spawns the menu at globalPosition
            void SpawnMenu(glm::vec2 globalPosition);

            //function: CanForceSpawnMenu
            //You can spawn the menu with a specified spawn direction, this function returns if such operation is possible.
            bool CanForceSpawnMenu(glm::vec2 globalSpawnPosition, ssGUI::Enums::MenuSpawnDirection direction);
            
            //function: ForceSpawnMenu
            //Spawn the menu with a spawn direction specified.
            void ForceSpawnMenu(glm::vec2 globalPosition, ssGUI::Enums::MenuSpawnDirection direction);

            //function: RegisterMenuItem
            //Sets the menu item's parent to the menu and register different callbacks internally
            void RegisterMenuItem(ssGUI::MenuItem* menuItem);
            
            //function: RemoveMenuItem
            //Deletes the menu item
            void RemoveMenuItem(ssGUI::MenuItem* menuItem);

            //function: LinkMenuItemToSubMenu
            //Links a sub menu against a menu item, the sub menu must be parented to the menu in order for it to work properly.
            void LinkMenuItemToSubMenu(ssGUI::MenuItem* menuItem, ssGUI::Menu* subMenu);

            //function: SetMenuTarget
            //Sets the target of the menu that is spawned on, meaning the GUI Object that "spawns" the menu. 
            //Please note that this should be called together with <SpawnMenu> or <ForceSpawnMenu>
            //To unset the target, set it to nullptr.
            void SetMenuTarget(ssGUI::GUIObject* target);
            
            //function: GetMenuTarget
            //Gets the target of the menu that is spawned on, meaning the GUI Object that "spawns" the menu.
            //This can return nullptr if not a specific GUI Object spawned it.
            ssGUI::GUIObject* GetMenuTarget() const;

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