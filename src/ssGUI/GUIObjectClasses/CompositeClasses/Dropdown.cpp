#include "ssGUI/GUIObjectClasses/CompositeClasses/Dropdown.hpp"

#include "ssGUI/GUIObjectClasses/MainWindow.hpp" //For getting mouse position

#include "ssGUI/GUIObjectClasses/Image.hpp"
#include "ssGUI/GUIObjectClasses/Menu.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/MenuItem.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/EventCallbacks/ItemSelectedEventCallback.hpp"

namespace ssGUI
{
    Dropdown::Dropdown(Dropdown const& other) : StandardButton(other)
    {
        DropdownMenu = other.DropdownMenu;
        SelectedIndex = other.GetSelectedIndex();
        Items = other.Items;
        Toggle = other.Toggle;
    }

    ssGUI::ImageData* Dropdown::DefaultDropdownArrowImageData = []()->ssGUI::ImageData*
    {
        FUNC_DEBUG_ENTRY("LoadDefaultDropdownArrowImage");

        ssGUI::ImageData* defaultImg;

        auto dropdownArrow = ssGUI::Factory::Create<ssGUI::ImageData>();
        if(!dropdownArrow->LoadFromPath("Resources/DownArrow.png"))
        {
            DEBUG_LINE("Failed to load default font");
            ssGUI::Factory::Dispose<ssGUI::ImageData>(dropdownArrow);
            FUNC_DEBUG_EXIT("LoadDefaultDropdownArrowImage");
            return nullptr;
        }
        
        FUNC_DEBUG_EXIT("LoadDefaultDropdownArrowImage");
        return dropdownArrow;
    }();    //Brackets at the end to call this lambda, pretty cool.
    
    void Dropdown::AddItemListener(ssGUI::EventCallbacks::EventCallback* ecb, int index)
    {
        ssGUI::ssGUIObjectIndex dropdownRefIndex = ecb->AddObjectReference(this);
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [index, dropdownRefIndex](ssGUI::GUIObject* src, ssGUI::GUIObject* container, ssGUI::ObjectsReferences* refs)
            {
                auto itemContainer = static_cast<ssGUI::MenuItem*>(container);
                auto dropdown = static_cast<ssGUI::Dropdown*>(refs->GetObjectReference(dropdownRefIndex));

                if(itemContainer == nullptr || dropdown == nullptr)
                    return;
                
                if(itemContainer->GetButtonState() == ssGUI::Enums::ButtonState::CLICKED)
                    dropdown->SetSelectedItem(index);
            }
        );
    }

    void Dropdown::MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, 
                ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow)
    {
        if(DropdownMenu != -1 && CurrentObjectsReferences.GetObjectReference(DropdownMenu) != nullptr &&
            CurrentObjectsReferences.GetObjectReference(DropdownMenu)->GetParent() != mainWindow)
        {
            CurrentObjectsReferences.GetObjectReference(DropdownMenu)->SetParent(mainWindow);
        }

        StandardButton::MainLogic(inputInterface, globalInputStatus, windowInputStatus, mainWindow);
    }

    const std::string Dropdown::ListenerKey = "Dropdown";
    
    Dropdown::Dropdown() : DropdownMenu(-1), SelectedIndex(-1), Items(), Toggle(false)
    {
        //Swap the order of text and icon
        MoveChildrenIteratorToFirst();
        auto firstIt = GetCurrentChildReferenceIterator();
        MoveChildrenIteratorToLast();
        auto lastIt = GetCurrentChildReferenceIterator();
        ChangeChildOrderToAfterPosition(firstIt, lastIt);

        //Swap size multiplier
        SetButtonMode(ssGUI::StandardButton::Mode::BOTH);
        auto layout = GetAnyExtension<ssGUI::Extensions::Layout>();
        float iconMulti = layout->GetPreferredSizeMultiplier(0);
        float textMulti = layout->GetPreferredSizeMultiplier(1);
        layout->SetPreferredSizeMultiplier(0, textMulti);
        layout->SetPreferredSizeMultiplier(1, iconMulti);

        //Set icon to dropdown arrow
        if(DefaultDropdownArrowImageData != nullptr)
            GetButtonIconObject()->SetImageData(DefaultDropdownArrowImageData);

        //Create dropdown menu
        auto dropdownMenu = ssGUI::Factory::Create<ssGUI::Menu>();
        dropdownMenu->SetParent(this);
        dropdownMenu->SetVisible(false);
        dropdownMenu->SetUserCreated(false);
        dropdownMenu->SetSize(glm::vec2(GetSize().x, dropdownMenu->GetSize().y));
        DropdownMenu = CurrentObjectsReferences.AddObjectReference(dropdownMenu);

        //Add event listener to show dropdown menu when toggled
        auto ecb = GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [](auto src, auto container, auto refs)
            {
                auto dropdownContainer = static_cast<ssGUI::Dropdown*>(container);
                
                auto dropdownMenu = dropdownContainer->Internal_GetObjectsReferences()->GetObjectReference(dropdownContainer->DropdownMenu);
                if(dropdownMenu == nullptr)
                    return;


                //Update size
                if(dropdownContainer->GetButtonState() == ssGUI::Enums::ButtonState::ON_CLICK)
                {
                    dropdownMenu->SetSize(glm::vec2(dropdownContainer->GetSize().x, dropdownMenu->GetSize().y));
                    if(!dropdownMenu->IsVisible())
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
                if(castedDropdownMenu->CanForceSpawnMenu(container->GetGlobalPosition() + glm::vec2(0, container->GetSize().y), 
                    ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT))
                {
                    castedDropdownMenu->ForceSpawnMenu(container->GetGlobalPosition() + glm::vec2(0, container->GetSize().y),
                        ssGUI::Enums::MenuSpawnDirection::BOTTOM_RIGHT);
                }
                else
                    castedDropdownMenu->ForceSpawnMenu(container->GetGlobalPosition(), ssGUI::Enums::MenuSpawnDirection::TOP_RIGHT);
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

        if(IsAnyEventCallbackExist<ssGUI::EventCallbacks::ItemSelectedEventCallback>())
            GetAnyEventCallback<ssGUI::EventCallbacks::ItemSelectedEventCallback>()->Notify(this);
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

        auto ecb = menuItem->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
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

            auto ecb = currentItem->GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
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
        FUNC_DEBUG_ENTRY();
        Dropdown* temp = new Dropdown(*this);
        CloneExtensionsAndEventCallbacks(temp);   
        
        if(cloneChildren)
        {
            if(CloneChildren(this, temp) == nullptr)
            {
                FUNC_DEBUG_EXIT();
                return nullptr;
            }
        }

        FUNC_DEBUG_EXIT();
        return temp;
    }
}