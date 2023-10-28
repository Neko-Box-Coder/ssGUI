#ifndef SSGUI_DROPDOWN_H
#define SSGUI_DROPDOWN_H

#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"

#include <vector>
#include <string>
#include <utility>

//namespace: ssGUI
namespace ssGUI
{
    class ItemMenu;
    class ImageData;
    
    /*class: ssGUI::Dropdown
    A dropdown button that allows an item to be selected from the dropdown menu.

    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUI::ssGUIObjectIndex DropdownMenu;                                                   //See <GetDropdownMenu>
        int SelectedIndex;                                                                      //See <GetSelectedIndex>
        std::vector<std::pair<std::string, ssGUI::ssGUIObjectIndex>> Items;                     //See <GetItem>
        bool Toggle;                                                                            //(Internal variable) Used to set focus

        static int DropdownObjectCount;                                                         //(Internal variable) Used for deallocating default resources
        static ssGUI::ImageData* DefaultDropdownArrowImageData;                                 //(Internal variable) used to store default arrow image
                                                                                                //TODO: Allow this to be configured
    =================================================================
    ============================== C++ ==============================
    Dropdown::Dropdown() :  DropdownMenu(-1),
                            SelectedIndex(-1),
                            Items(),
                            Toggle(false)
    {
        InitializeDefaultDropdownArrowIfNeeded();
        SetSize(glm::vec2(100, 35));
        
        //Swap the order of text and icon
        MoveChildrenIteratorToFirst();
        ssGUI::Hierarchy::ChildToken firstIt = GetCurrentChildToken();
        MoveChildrenIteratorToLast();
        ssGUI::Hierarchy::ChildToken lastIt = GetCurrentChildToken();
        ChangeChildOrderToAfterPosition(firstIt, lastIt);

        //Swap size multiplier
        SetButtonMode(ssGUI::StandardButton::Mode::BOTH);
        auto layout = GetExtension<ssGUI::Extensions::Layout>();
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
        auto ecb = GetAnyEventCallback<ssGUI::EventCallbacks::ButtonStateChangedEventCallback>();
        ecb->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo& info)
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
    
        DropdownObjectCount++;
        
        AddEventCallback(ssGUI::Enums::EventType::BEFORE_OBJECT_DESTROY)->AddEventListener
        (
            ListenerKey,
            this,
            [](ssGUI::EventInfo& info)
            {
                auto* dropdown = static_cast<ssGUI::Dropdown*>(info.Container);
                
                ssGUI::Dropdown::DropdownObjectCount--;
        
                if(ssGUI::Dropdown::DropdownObjectCount == 0)
                    dropdown->CleanUpDefaultResources();
            }
        );
    }

    int Dropdown::DropdownObjectCount = 0;
    ssGUI::ImageData* Dropdown::DefaultDropdownArrowImageData = nullptr;
    =================================================================
    */
    class Dropdown : public StandardButton
    {
        private:
            Dropdown& operator=(Dropdown const& other) = default;

        protected:
            ssGUI::ssGUIObjectIndex DropdownMenu;                                                   //See <GetDropdownMenu>
            int SelectedIndex;                                                                      //See <GetSelectedIndex>
            std::vector<std::pair<std::string, ssGUI::ssGUIObjectIndex>> Items;                     //See <GetItem>
            bool Toggle;                                                                            //(Internal variable) Used to set focus

            static int DropdownObjectCount;                                                         //(Internal variable) Used for deallocating default resources
            static ssGUI::ImageData* DefaultDropdownArrowImageData;                                 //(Internal variable) used to store default arrow image
                                                                                                    //TODO: Allow this to be configured
            Dropdown(Dropdown const& other);

            void AddItemListener(ssGUI::EventCallback* ecb, int index);

            virtual void MainLogic( ssGUI::Backend::BackendSystemInputInterface* inputInterface, 
                                    ssGUI::InputStatus& currentInputStatus, 
                                    ssGUI::InputStatus& lastInputStatus, 
                                    ssGUI::GUIObject* mainWindow) override;

        public:
            //string: ListenerKey
            static const std::string ListenerKey;

            Dropdown();
            virtual ~Dropdown() override;

            //function: GetSelectedIndex
            //Gets the index of the current selected item
            virtual int GetSelectedIndex() const;

            //function: SetSelectedItem
            //Sets the index of the current selected item
            virtual void SetSelectedItem(int index);
            
            //function: AddItem
            //Add an item to the dropdown menu
            virtual int AddItem(std::string item);

            //function: SetItem
            //Set an item to the dropdown menu
            virtual void SetItem(int index, std::string item);

            //function: GetItem
            //Gets the text content of an item according to the index. 
            //Returns empty string if index is not valid.
            virtual std::string GetItem(int index);
            
            //function: RemoveItem
            //Removes the item at the index position
            virtual void RemoveItem(int index);
            
            //function: ClearItems
            //Clears all items
            virtual void ClearItems();
            
            //function: GetItemsCount
            //Returns the number of items in the dropdown menu
            virtual int GetItemsCount() const;

            //function: GetDropdownMenu
            //Gets the dropdown menu GUI object
            virtual ssGUI::Menu* GetDropdownMenu();

            //function: SetDropdownMenu
            //Sets the dropdown menu GUI object.
            //The menu items in the new menu will be cleared 
            //and all the menu items from the old dropdown menu will moved to the new one.
            //Passing nullptr will unset the dropdown menu object.
            virtual void SetDropdownMenu(ssGUI::Menu* menu);

            //function: SetInteractable
            //See <Widget::SetInteractable>
            virtual void SetInteractable(bool interactable) override;
            
            //function: SetBlockInput
            //See <Widget::SetBlockInput>
            virtual void SetBlockInput(bool blockInput) override;
            
            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Dropdown* Clone(bool cloneChildren) override;

            //function: InitiateDefaultResources
            //See <GUIObject::InitiateDefaultResources>
            virtual void InitiateDefaultResources() override;
            
            //function: CleanUpDefaultResources
            //See <GUIObject::CleanUpDefaultResources>
            virtual void CleanUpDefaultResources() override;
    };
}

#endif