#include "ssGUI/GUIObjectClasses/Menu.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Border.hpp"
#include "ssGUI/ssGUITags.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    Menu::Menu(Menu const& other) : Widget(other)
    {
        SpawnGlobalPosition = other.SpawnGlobalPosition;
        CurrentMenuSpawnDirection = other.CurrentMenuSpawnDirection;
        MenuTarget = other.MenuTarget;
    }

    void Menu::CloseRootMenu(ssGUI::GUIObject* currentMenu)
    {
        while (true)
        {
            currentMenu->SetEnabled(false);
            auto newParent = currentMenu->GetParent();
            if(newParent != nullptr && newParent->GetType() == ssGUI::Enums::GUIObjectType::MENU)
                currentMenu = newParent;
            else
                break;
        }
        
        currentMenu->SetEnabled(false);        
    }

    void Menu::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        if(!IsFocused())
        {
            SetEnabled(false);
            return;
        }

        //The reason for putting this here is because layout can change the size of the menu,
        //and it changes the size only after this is set to enabled
        switch (CurrentMenuSpawnDirection)
        {
            case ssGUI::Enums::MenuSpawnDirection::TOP_LEFT:
                SetGlobalPosition(SpawnGlobalPosition - GetSize());
                break;
            case ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT:
                SetGlobalPosition(SpawnGlobalPosition - glm::vec2(0, GetSize().y));
                break;
            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT:
                SetGlobalPosition(SpawnGlobalPosition);
                break;
            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT:
                SetGlobalPosition(SpawnGlobalPosition - glm::vec2(GetSize().x, 0));
                break;
        }
    }

    const std::string Menu::ListenerKey = "Menu";
    
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

    Menu::~Menu()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
    }

    ssGUI::Enums::MenuSpawnDirection Menu::GetMenuSpawnDirection(glm::vec2 globalSpawnPosition)
    {
        if(GetParent() == nullptr)
            return ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT;

        auto curParent = GetParent();
        while (curParent->GetType() == ssGUI::Enums::GUIObjectType::MENU)
            curParent = curParent->GetParent();

        glm::vec2 centerPoint = curParent->GetSize() * 0.5f - curParent->GetGlobalPosition();
        auto spawnPos = globalSpawnPosition - curParent->GetGlobalPosition();

        if(spawnPos.x > centerPoint.x && spawnPos.y > centerPoint.y)
            return ssGUI::Enums::MenuSpawnDirection::TOP_LEFT;
        else if(spawnPos.x < centerPoint.x && spawnPos.y > centerPoint.y)
            return ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT;
        else if(spawnPos.x > centerPoint.x && spawnPos.y < centerPoint.y)
            return ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT;
        else
            return ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT;
    }

    void Menu::SpawnMenu(glm::vec2 globalPosition)
    {
        SpawnGlobalPosition = globalPosition;
        CurrentMenuSpawnDirection = GetMenuSpawnDirection(globalPosition);
        SetEnabled(true);
        SetFocus(true);
    }

    bool Menu::CanSpawnMenu(glm::vec2 globalSpawnPosition, ssGUI::Enums::MenuSpawnDirection direction)
    {
        if(GetParent() == nullptr)
            return false;

        auto curParent = GetParent();
        while (curParent->GetType() == ssGUI::Enums::GUIObjectType::MENU)
            curParent = curParent->GetParent();
        
        auto parentTopLeft = curParent->GetGlobalPosition();
        auto parentBottomRight = curParent->GetGlobalPosition() + curParent->GetSize();
        
        switch (direction)
        {
            case ssGUI::Enums::MenuSpawnDirection::TOP_LEFT:
            {
                auto topLeft = globalSpawnPosition - GetSize();
                return topLeft.x > parentTopLeft.x && topLeft.y > parentTopLeft.y;
            }
            case ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT:
            {
                auto topRight = globalSpawnPosition + glm::vec2(GetSize().x, 0);
                return topRight.x < parentBottomRight.x && topRight.y > parentTopLeft.y;
            }
            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT:
            {
                auto botRight = globalSpawnPosition + GetSize();
                return botRight.x < parentBottomRight.x && botRight.y < parentBottomRight.y;
            }
            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT:
            {
                auto botLeft = globalSpawnPosition + glm::vec2(0, GetSize().y);
                return botLeft.x > parentTopLeft.x && botLeft.y < parentBottomRight.y;
            }
        }

        return false;
    }

    void Menu::ForceSpawnMenu(glm::vec2 globalPosition, ssGUI::Enums::MenuSpawnDirection direction)
    {
        SpawnGlobalPosition = globalPosition;
        CurrentMenuSpawnDirection = direction;
        SetEnabled(true);
        SetFocus(true);
    }


    void Menu::RegisterMenuItem(ssGUI::MenuItem* menuItem)
    {
        if(menuItem == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Nullptr is passed to RegisterMenuItem, this is invalid.");
            return;
        }
    
        if(!menuItem->IsEventCallbackExist(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED))
            return;

        auto ecb = menuItem->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        int menuIndex = ecb->AddObjectReference(this);        
        ecb->AddEventListener
        (
            ListenerKey, this,
            [menuIndex](ssGUI::EventInfo& info)
            {
                auto btn = static_cast<ssGUI::Button*>(info.EventSource);
                if(btn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                {
                    if(info.References->GetObjectReference(menuIndex) != nullptr)
                    {
                        auto curParent = info.References->GetObjectReference(menuIndex);
                        ssGUI::Menu::CloseRootMenu(curParent);
                    }
                }
                else if(btn->GetButtonState() == ssGUI::Enums::ButtonState::HOVER)
                    btn->SetFocus(true);
            }
        );
        
        if(menuItem->GetParent() != this)
            menuItem->SetParent(this);
    }

    void Menu::RemoveMenuItem(ssGUI::MenuItem* menuItem)
    {
        glm::vec2 itemSize = menuItem->GetSize();
        menuItem->Delete();
        
        //Validation check
        if(!IsAnyExtensionExist<ssGUI::Extensions::Layout>())
            return;
        
        //Removing space created by the menu item that we deleted
        auto* layout = GetAnyExtension<ssGUI::Extensions::Layout>();
        float layoutItemSpacing = layout->GetSpacing() + layout->GetPadding() * 2;
        
        if(layout->IsHorizontalLayout())
            SetSize(glm::vec2(GetSize().x - itemSize.x - layoutItemSpacing, GetSize().y));
        else
            SetSize(glm::vec2(GetSize().x, GetSize().y - itemSize.y - layoutItemSpacing));
    }
    
    void Menu::ClearMenuItems()
    {
        std::vector<ssGUI::GUIObject*> children = GetListOfChildren();
        
        for(int i = 0; i < children.size(); i++)
        {
            if(children[i]->GetType() == ssGUI::Enums::GUIObjectType::MENU_ITEM)
                RemoveMenuItem(static_cast<ssGUI::MenuItem*>(children[i]));
        }
    }

    ssGUI::MenuItem* Menu::AddMenuItem()
    {
        ssGUI::MenuItem* menuItem = AddChild<ssGUI::MenuItem>();
        RegisterMenuItem(menuItem);
        return menuItem;
    }

    void Menu::LinkMenuItemToSubMenu(ssGUI::MenuItem* menuItem, ssGUI::Menu* subMenu)
    {
        if(!menuItem->IsEventCallbackExist(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED))
            return;
        
        //If the menu item is already registered with event callback, remove it
        auto ecb = menuItem->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        if(ecb->IsEventListenerExist(ListenerKey, this))
        {
            ecb->RemoveEventListener(ListenerKey, this);
        }

        //Add the event listener for submenu
        int subMenuIndex = ecb->AddObjectReference(subMenu);
        int menuIndex = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey, this,
            [subMenuIndex, menuIndex](ssGUI::EventInfo& info)
            {
                auto btn = static_cast<ssGUI::Button*>(info.EventSource);
                if(btn->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                {
                    if(info.References->GetObjectReference(menuIndex) != nullptr)
                    {
                        auto curParent = info.References->GetObjectReference(menuIndex);
                        ssGUI::Menu::CloseRootMenu(curParent);
                    }
                }
                else if(btn->GetButtonState() == ssGUI::Enums::ButtonState::HOVER)
                {
                    btn->SetFocus(true);
                    
                    //Show the submenu when submenu item being hovered
                    if(info.References->GetObjectReference(subMenuIndex) != nullptr)
                    {
                        auto curSubMenu = static_cast<ssGUI::Menu*>(info.References->GetObjectReference(subMenuIndex));

                        //Update the menu target
                        if(info.References->GetObjectReference(menuIndex) != nullptr)
                            curSubMenu->SetMenuTarget(static_cast<ssGUI::Menu*>(info.References->GetObjectReference(menuIndex))->GetMenuTarget());
                        
                        ssGUI::Enums::MenuSpawnDirection preferDir = curSubMenu->GetMenuSpawnDirection(btn->GetGlobalPosition() + btn->GetSize() / 2.f);
                        
                        static_assert(static_cast<int>(ssGUI::Enums::MenuSpawnDirection::COUNT) == 4, "LinkMenuItemToSubMenu");
                        switch(preferDir)
                        {
                            case ssGUI::Enums::MenuSpawnDirection::TOP_LEFT:
                                curSubMenu->ForceSpawnMenu(btn->GetGlobalPosition() + glm::vec2(0, btn->GetSize().y), ssGUI::Enums::MenuSpawnDirection::TOP_LEFT);
                                break;
                            case ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT:
                                curSubMenu->ForceSpawnMenu(btn->GetGlobalPosition() + btn->GetSize(), ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT);
                                break;
                            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT:
                                curSubMenu->ForceSpawnMenu(btn->GetGlobalPosition() + glm::vec2(btn->GetSize().x, 0), ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT);
                                break;
                            case ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT:
                                curSubMenu->ForceSpawnMenu(btn->GetGlobalPosition(), ssGUI::Enums::MenuSpawnDirection::BOTTOM_LEFT);
                                break;
                        }
                    }
                }
            }
        );
    }

    void Menu::SetMenuTarget(ssGUI::GUIObject* target)
    {
        MenuTarget = target;
    }

    ssGUI::GUIObject* Menu::GetMenuTarget() const
    {
        return MenuTarget;
    }

    std::vector<ssGUI::MenuItem*> Menu::GetListOfMenuItems() const
    {
        std::vector<ssGUI::GUIObject*> children = GetListOfChildren();
        std::vector<ssGUI::MenuItem*> items;
        
        for(int i = 0; i < children.size(); i++)
        {
            if(children[i]->GetType() == ssGUI::Enums::GUIObjectType::MENU_ITEM)
                items.push_back(static_cast<ssGUI::MenuItem*>(children[i]));
        }
        
        return items;
    }
    

    void Menu::SetFocus(bool focus)
    {       
        if(focus)
            SetParent(GetParent());

        GUIObject::SetFocus(focus);
    }

    void Menu::Internal_SetSelfFocus(bool focus)
    {
        if(focus)
            SetParent(GetParent());
        
        GUIObject::Internal_SetSelfFocus(focus);
    }

    ssGUI::Enums::GUIObjectType Menu::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::MENU;
    }

    Menu* Menu::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Menu* temp = new Menu(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                ssLOG_FUNC_EXIT();
                return nullptr;
            }
        }

        ssLOG_FUNC_EXIT();
        return temp;
    }
}