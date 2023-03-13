#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"

#include "ssGUI/EmbeddedResources.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Menu.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"
#include "ssGUI/Extensions/Layout.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    ssGUI::ImageData* Dropdown::DefaultDropdownArrowImageData = nullptr;
    
    Dropdown::Dropdown(Dropdown const& other) : StandardButton(other)
    {
        DropdownMenu = other.DropdownMenu;
        SelectedIndex = other.GetSelectedIndex();
        Items = other.Items;
        Toggle = other.Toggle;
    }

    void Dropdown::AddItemListener(ssGUI::EventCallback* ecb, int index)
    {
        ssGUI::ssGUIObjectIndex dropdownRefIndex = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [index, dropdownRefIndex](ssGUI::EventInfo info)
            {
                auto itemContainer = static_cast<ssGUI::MenuItem*>(info.Container);
                auto dropdown = static_cast<ssGUI::Dropdown*>(info.References->GetObjectReference(dropdownRefIndex));

                if(itemContainer == nullptr || dropdown == nullptr)
                    return;
                
                if(itemContainer->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                    dropdown->SetSelectedItem(index);
            }
        );
    }

    void Dropdown::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                            ssGUI::GUIObject* mainWindow)
    {
        if(DropdownMenu != -1 && CurrentObjectsReferences.GetObjectReference(DropdownMenu) != nullptr &&
            CurrentObjectsReferences.GetObjectReference(DropdownMenu)->GetParent() != mainWindow)
        {
            CurrentObjectsReferences.GetObjectReference(DropdownMenu)->SetParent(mainWindow);
        }

        StandardButton::MainLogic(inputInterface, inputStatus, mainWindow);
    }

    const std::string Dropdown::ListenerKey = "Dropdown";
    
    Dropdown::Dropdown() :  DropdownMenu(-1),
                            SelectedIndex(-1),
                            Items(),
                            Toggle(false)
    {
        SetSize(glm::vec2(100, 35));
        
        //Swap the order of text and icon
        MoveChildrenIteratorToFirst();
        ssGUI::Hierarchy::ChildToken firstIt = GetCurrentChildToken();
        MoveChildrenIteratorToLast();
        ssGUI::Hierarchy::ChildToken lastIt = GetCurrentChildToken();
        ChangeChildOrderToAfterPosition(firstIt, lastIt);

        //Swap size multiplier
        SetButtonMode(ssGUI::StandardButton::Mode::BOTH);
        auto layout = GetAnyExtension<ssGUI::Extensions::Layout>();
        float iconMulti = layout->GetPreferredSizeMultiplier(0);
        float textMulti = layout->GetPreferredSizeMultiplier(1);
        layout->SetPreferredSizeMultiplier(0, textMulti);
        layout->SetPreferredSizeMultiplier(1, iconMulti);

        InitiateDefaultResources();
        //Set icon to dropdown arrow
        if(DefaultDropdownArrowImageData != nullptr)
            GetButtonIconObject()->SetImageData(DefaultDropdownArrowImageData);

        //Create dropdown menu
        auto dropdownMenu = ssGUI::Factory::Create<ssGUI::Menu>();
        dropdownMenu->SetParent(this, true);
        dropdownMenu->SetEnabled(false);
        dropdownMenu->SetUserCreated(false);
        dropdownMenu->SetMenuTarget(this);
        dropdownMenu->SetSize(glm::vec2(GetSize().x, dropdownMenu->GetSize().y));
        DropdownMenu = CurrentObjectsReferences.AddObjectReference(dropdownMenu);

        //Add event listener to show dropdown menu when toggled
        auto ecb = GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo info)
            {
                auto dropdownContainer = static_cast<ssGUI::Dropdown*>(info.Container);
                
                auto dropdownMenu = dropdownContainer->Internal_GetObjectsReferences()->GetObjectReference(dropdownContainer->DropdownMenu);
                if(dropdownMenu == nullptr)
                    return;

                //Update size
                if(dropdownContainer->GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK)
                {
                    dropdownMenu->SetSize(glm::vec2(dropdownContainer->GetSize().x, dropdownMenu->GetSize().y));
                    if(!dropdownMenu->IsEnabled())
                        dropdownContainer->Toggle = false;
                }

                if(dropdownContainer->GetButtonState() != ssGUI::Enums::ButtonState::CLICKED)
                    return;

                if(dropdownContainer->Toggle)
                {
                    dropdownContainer->SetFocus(true);
                    dropdownContainer->Toggle = false;
                    return;
                }

                dropdownContainer->Toggle = true;
                
                auto castedDropdownMenu = static_cast<ssGUI::Menu*>(dropdownMenu);
                if(castedDropdownMenu->CanForceSpawnMenu(info.Container->GetGlobalPosition() + glm::vec2(0, info.Container->GetSize().y), 
                    ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT))
                {
                    castedDropdownMenu->ForceSpawnMenu(info.Container->GetGlobalPosition() + glm::vec2(0, info.Container->GetSize().y),
                        ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT);
                }
                else
                    castedDropdownMenu->ForceSpawnMenu(info.Container->GetGlobalPosition(), ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT);
            }
        );
    }

    Dropdown::~Dropdown()
    {
        NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        //If the object deallocation is not handled by ssGUIManager
        if(!Internal_IsDeleted())
            Internal_ManualDeletion(std::vector<ssGUI::ssGUIObjectIndex>{DropdownMenu});
    }

    int Dropdown::GetSelectedIndex() const
    {
        return SelectedIndex;
    }

    void Dropdown::SetSelectedItem(int index)
    {
        if(index >= Items.size())
            return;
        
        SelectedIndex = index;

        if(GetButtonTextObject() != nullptr)
        {
            if(index < 0)
                GetButtonTextObject()->SetText("");
            else
                GetButtonTextObject()->SetText(Items[index].first);
        }

        if(IsEventCallbackExist(ssGUI::Enums::EventType::ITEM_SELECTED))
            GetEventCallback(ssGUI::Enums::EventType::ITEM_SELECTED)->Notify(this);
    }

    int Dropdown::AddItem(std::string item)
    {
        if(DropdownMenu == -1)
            return -1;
        
        ssGUI::Menu* menu = static_cast<ssGUI::Menu*>(CurrentObjectsReferences.GetObjectReference(DropdownMenu));
        
        if(menu == nullptr)
            return -1;
        
        ssGUI::MenuItem* menuItem = ssGUI::Factory::Create<ssGUI::MenuItem>();
        menuItem->SetUserCreated(false);
        ssGUI::ssGUIObjectIndex menuItemIndex = CurrentObjectsReferences.AddObjectReference(menuItem);
        menuItem->GetButtonTextObject()->SetText(item);
        menu->RegisterMenuItem(menuItem);

        Items.push_back(std::make_pair(item, menuItemIndex));

        auto ecb = menuItem->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
        if(ecb == nullptr)
            return -1;

        int addIndex = Items.size() - 1;
        AddItemListener(ecb, addIndex);
        return addIndex;
    }

    void Dropdown::SetItem(int index, std::string item)
    {
        if(index >= 0 && index < Items.size())
            return;
        
        Items[index].first = item;
        if(GetSelectedIndex() == index)
            SetSelectedItem(index);
    }

    std::string Dropdown::GetItem(int index)
    {
        if(index >= 0 && index < Items.size())
            return Items[index].first;
        
        return std::string();
    }

    void Dropdown::RemoveItem(int index)
    {
        if(index < 0 || index >= Items.size() || DropdownMenu == -1)
            return;
        
        ssGUI::Menu* menu = static_cast<ssGUI::Menu*>(CurrentObjectsReferences.GetObjectReference(DropdownMenu));
        
        if(menu == nullptr)
            return;

        for(int i = index + 1; i < Items.size(); i++)
        {
            ssGUI::MenuItem* currentItem = static_cast<ssGUI::MenuItem*>(CurrentObjectsReferences.GetObjectReference(Items[i].second));

            if(currentItem == nullptr)
                continue;

            auto ecb = currentItem->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);
            if(ecb == nullptr)
                continue;
            
            ecb->RemoveEventListener(ListenerKey, this);
            AddItemListener(ecb, i - 1);
        }
        
        ssGUI::GUIObject* menuItem = CurrentObjectsReferences.GetObjectReference(Items[index].second);
        if(menuItem != nullptr)
            menu->RemoveMenuItem(static_cast<ssGUI::MenuItem*>(menuItem));

        Items.erase(Items.begin() + index);

        if(SelectedIndex == index)
            SetSelectedItem(-1);
    }

    void Dropdown::ClearItems()
    {
        for(int i = 0; i < Items.size(); i++)
        {
            ssGUI::MenuItem* currentItem = static_cast<ssGUI::MenuItem*>(CurrentObjectsReferences.GetObjectReference(Items[i].second));

            if(currentItem == nullptr)
                continue;
            
            currentItem->Delete();
        }
        Items.clear();

        SetSelectedItem(-1);
    }

    int Dropdown::GetItemsCount() const
    {
        return Items.size();
    }

    ssGUI::Menu* Dropdown::GetDropdownMenu()
    {
        return static_cast<ssGUI::Menu*>(CurrentObjectsReferences.GetObjectReference(DropdownMenu));
    }

    void Dropdown::SetDropdownMenu(ssGUI::Menu* menu)
    {
        ssGUI::Menu* currentMenu = static_cast<ssGUI::Menu*>(CurrentObjectsReferences.GetObjectReference(DropdownMenu));

        if(currentMenu != nullptr)
            currentMenu->Delete();
        
        if(menu == nullptr)
            DropdownMenu = -1;
        else
            DropdownMenu = CurrentObjectsReferences.AddObjectReference(menu);

        glm::vec2 globalPos = menu->GetGlobalPosition();
        menu->SetParent(this, true);
        menu->SetGlobalPosition(globalPos);

        auto itemsCopy = Items;
        Items.clear();
        for(int i = 0; i < Items.size(); i++)
            AddItem(Items[i].first);
    }

    ssGUI::Enums::GUIObjectType Dropdown::GetType() const
    {
        return ssGUI::Enums::GUIObjectType::WIDGET | ssGUI::Enums::GUIObjectType::BUTTON |
            ssGUI::Enums::GUIObjectType::STANDARD_BUTTON | ssGUI::Enums::GUIObjectType::DROPDOWN;
    }

    Dropdown* Dropdown::Clone(bool cloneChildren)
    {
        ssLOG_FUNC_ENTRY();
        Dropdown* temp = new Dropdown(*this);
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
    
    void Dropdown::InitiateDefaultResources()
    {
        ssLOG_FUNC_ENTRY();
        if(DefaultDropdownArrowImageData != nullptr)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        ssGUI::ImageData* defaultImg;

        auto data = ssGUI::Factory::Create<ssGUI::ImageData>();
        size_t fileSize = 0;
        const char* fileContent = find_embedded_file("DownArrow.png", &fileSize);
        
        if(fileContent == nullptr)
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to load embedded dropdown icon");
            ssGUI::Factory::Dispose(data);
            return;
        }        
        
        if(!data->LoadImgFileFromMemory(fileContent, fileSize))
        {
            ssGUI_WARNING(ssGUI_GUI_OBJECT_TAG, "Failed to load default font");
            ssGUI::Factory::Dispose<ssGUI::ImageData>(data);
        }
        else
            DefaultDropdownArrowImageData = data;

        ssLOG_FUNC_EXIT();
    }
    
    void Dropdown::CleanUpDefaultDropdownArrowImage()
    {
        if(DefaultDropdownArrowImageData != nullptr)
        {
            ssGUI::Factory::Dispose(DefaultDropdownArrowImageData);
            DefaultDropdownArrowImageData = nullptr;
        }
    }
}