#ifndef SSGUI_DROPDOWN
#define SSGUI_DROPDOWN

#include "ssGUI/GUIObjectClasses/CompositeClasses/StandardButton.hpp"
#include "ssGUI/HelperClasses/StaticDefaultWrapper.hpp"

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

        static ssGUI::StaticDefaultWrapper<ssGUI::ImageData>* DefaultDropdownArrowImageData;     //(Internal variable) used to store default arrow image
                                                                                                //TODO: Allow this to be configured
        static bool DefaultDropdownInitialized;                                                 //(Internal variable) Used to see if the default dropdown image needs initializing
    =================================================================
    ============================== C++ ==============================
    Dropdown::Dropdown() :  DropdownMenu(-1),
                            SelectedIndex(-1),
                            Items(),
                            Toggle(false)
    {
        InitializeDefaultDropdownArrowIfNeeded();

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

    ssGUI::StaticDefaultWrapper<ssGUI::ImageData>* Dropdown::DefaultDropdownArrowImageData = nullptr;
    bool Dropdown::DefaultDropdownInitialized = false;
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

            static ssGUI::StaticDefaultWrapper<ssGUI::ImageData>* DefaultDropdownArrowImageData;     //(Internal variable) used to store default arrow image
                                                                                                    //TODO: Allow this to be configured
            static bool DefaultDropdownInitialized;                                                 //(Internal variable) Used to see if the default dropdown image needs initializing

            Dropdown(Dropdown const& other);

            void AddItemListener(ssGUI::EventCallbacks::EventCallback* ecb, int index);

            static void InitializeDefaultDropdownArrowIfNeeded();

            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
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
            //Sets the dropdown menu GUI object
            virtual void SetDropdownMenu(ssGUI::Menu* menu);

            //function: CleanUpAllDefaultDropdownImage
            static void CleanUpAllDefaultDropdownImage();

            //function: GetType
            //See <Widget::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: Clone
            //See <Widget::Clone>
            virtual Dropdown* Clone(bool cloneChildren) override;
    };
}

#endif