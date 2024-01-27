#include "ssGUI/Extensions/TabArea.hpp"
#include "ssGUI/DataClasses/TabEventInfo.hpp"
#include "ssGUI/Extensions/AdvancedPosition.hpp"
#include "ssGUI/Extensions/AdvancedSize.hpp"
#include "ssGUI/Extensions/Layout.hpp"
#include "ssGUI/Extensions/Tabbable.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Tab.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"
#include "ssGUI/GUIObjectClasses/Widget.hpp"
#include "ssGUI/ssGUITags.hpp"

#include <vector>

namespace ssGUI
{
    namespace Extensions
    {
        ssGUI::GUIObject* TabArea::DefaultTabBarObject = nullptr;
        glm::u8vec4 TabArea::DefaultTabBarColor = glm::u8vec4(127, 127, 127, 255);
        ssGUI::Tab* TabArea::DefaultTabObject = nullptr;
        glm::u8vec4 TabArea::DefaultTabColor = glm::u8vec4(100, 100, 100, 255);
        glm::u8vec4 TabArea::DefaultPreviewColor = glm::u8vec4(255, 255, 255, 127);

        TabArea::TabArea() :
            Container(nullptr),
            Enabled(true),
            TabBarHeight(25),
            NewTabWidth(100),
            CurrentObjectsReferences(),
            TabBar(-1),
            CurrentTabContent(-1),
            LastClickedTab(-1),
            ContentsHolder(-1),
            TabsHolder(-1),
            TabsInfos(),
            TabPreview(-1),
            PreviewColor(DefaultPreviewColor),
            OverrideTabPreviewSize(false),
            TabPreviewOverrideSize(),
            NewTabColor(DefaultTabColor)
        {
        }

        TabArea::~TabArea()
        {
        }

        TabArea::TabArea(const TabArea& other)
        {
            Container = nullptr;
            Copy(&other);
        }

        void TabArea::Initialize()
        {
            if(Container == nullptr)
                return;

            //Create layout if there isn't one
            if(!Container->IsExtensionExist<ssGUI::Extensions::Layout>())
                Container->AddExtension<ssGUI::Extensions::Layout>(); //->SetCoverFullLength(false);

            Container->GetExtension<ssGUI::Extensions::Layout>()->SetSpacing(0);

            //Tab bar creation
            ssGUI::Widget* tabBar;

            if(DefaultTabBarObject != nullptr)
            {
                tabBar = static_cast<ssGUI::Widget*>(DefaultTabBarObject->Clone(true));
                tabBar->SetParent(Container);
            }
            else
            {
                tabBar = Container->AddChild<ssGUI::Widget>(true);
                tabBar->SetBackgroundColor(DefaultTabBarColor);
            }

            tabBar->SetUserCreated(false);
            tabBar->SetMaxSize(glm::vec2(9999, TabBarHeight));
            tabBar->AddExtension<ssGUI::Extensions::Layout>();
            Container->MoveChildToFirst(tabBar);

            TabBar = CurrentObjectsReferences.AddObjectReference(tabBar);

            //Tabs Holder
            ssGUI::GUIObject* tabsHolder = tabBar->AddChild<ssGUI::GUIObject>(true);
            tabsHolder->SetUserCreated(false);
            auto* tabsHolderLayout = tabsHolder->AddExtension<ssGUI::Extensions::Layout>();
            tabsHolderLayout->SetCoverFullLength(false);
            tabsHolderLayout->SetHorizontalLayout(true);
            TabsHolder = CurrentObjectsReferences.AddObjectReference(tabsHolder);

            //Contents Holder
            ssGUI::GUIObject* contentsHolder = Container->AddChild<ssGUI::GUIObject>(true);
            contentsHolder->SetUserCreated(false);
            contentsHolder->AddExtension<ssGUI::Extensions::Layout>();
            ContentsHolder = CurrentObjectsReferences.AddObjectReference(contentsHolder);

            //Create a preview object that we can use
            auto* tabPreview = tabsHolder->AddChild<ssGUI::Widget>(true);
            tabPreview->SetUserCreated(false);
            tabPreview->SetBlockInput(false);
            tabPreview->AddTag(ssGUI::Tags::FLOATING);
            tabPreview->AddExtension<ssGUI::Extensions::AdvancedPosition>()
                ->SetAlignment(ssGUI::Enums::AlignmentHorizontal::CENTER,
                               ssGUI::Enums::AlignmentVertical::CENTER);

            tabPreview->AddExtension<ssGUI::Extensions::AdvancedSize>()->SetSizingPercentage(1, 1);

            tabPreview->SetBackgroundColor(PreviewColor);
            tabPreview->SetEnabled(false);
            TabPreview = CurrentObjectsReferences.AddObjectReference(tabPreview);

            UpdateOrientationsForContentsAndPreviews();
        }

        bool TabArea::IsTabContentExist(ssGUI::GUIObject* content) const
        {
            ssGUIObjectIndex objectIndex = CurrentObjectsReferences.GetObjectIndex(content);

            if(objectIndex == -1)
                return false;

            return TabsInfos.find(objectIndex) != TabsInfos.end();
        }

        TabArea::TabInfoIterator TabArea::GetMouseHoveredTab(glm::ivec2 mousePos)
        {
            for(auto tabIt = TabsInfos.begin(); tabIt != TabsInfos.end(); ++tabIt)
            {
                ssGUI::GUIObject* tabObj =
                    CurrentObjectsReferences.GetObjectReference(tabIt->second);

                if(tabObj == nullptr)
                    continue;

                if(mousePos.x >= tabObj->GetGlobalPosition().x &&
                   mousePos.x < tabObj->GetGlobalPosition().x + tabObj->GetSize().x &&
                   mousePos.y >= tabObj->GetGlobalPosition().y &&
                   mousePos.y < tabObj->GetGlobalPosition().y + tabObj->GetSize().y)
                {
                    return tabIt;
                }
            }

            return TabsInfos.end();
        }

        void TabArea::DrawTabPreview(TabId tab, glm::vec2 mousePos)
        {
            if(CurrentObjectsReferences.GetObjectReference(tab) == nullptr)
                return;

            ssGUI::GUIObject* tabObj = CurrentObjectsReferences.GetObjectReference(tab);
            ssGUI::GUIObject* previewObject =
                CurrentObjectsReferences.GetObjectReference(TabPreview);

            previewObject->SetEnabled(true);

            bool isHoverLeftOrTop =
                IsTabBarHorizontal() ?
                    mousePos.x < tabObj->GetGlobalPosition().x + tabObj->GetSize().x / 2.f :
                    mousePos.y < tabObj->GetGlobalPosition().y + tabObj->GetSize().y / 2.f;

            //Set Preview parent
            previewObject->SetParent(CurrentObjectsReferences.GetObjectReference(TabsHolder));

            if(!previewObject->IsExtensionExist<ssGUI::Extensions::AdvancedPosition>() ||
               !previewObject->IsExtensionExist<ssGUI::Extensions::AdvancedSize>())
            {
                ssGUI_ERROR(ssGUI_TAG_EXT,
                            "Preview object does not have advanced size or advanced position");
                ssLOG_EXIT_PROGRAM();
                return;
            }

            //Disable advanced size and advanced position
            previewObject->GetExtension<ssGUI::Extensions::AdvancedPosition>()->SetEnabled(false);
            previewObject->GetExtension<ssGUI::Extensions::AdvancedSize>()->SetEnabled(false);

            //Set preview size
            if(OverrideTabPreviewSize)
                previewObject->SetSize(TabPreviewOverrideSize);
            else
            {
                if(IsTabBarHorizontal())
                    previewObject->SetSize(glm::vec2(tabObj->GetSize().x / 2.f, TabBarHeight));
                else
                    previewObject->SetSize(glm::vec2(TabBarHeight, tabObj->GetSize().y / 2.f));
            }

            //Tab preview position left/top hover tab
            if(isHoverLeftOrTop)
                previewObject->SetGlobalPosition(tabObj->GetGlobalPosition());
            //Tab preview position right/bottom hover tab
            else
            {
                glm::vec2 tabWidthOffset;

                if(OverrideTabPreviewSize)
                {
                    tabWidthOffset =
                        IsTabBarHorizontal() ?
                            glm::vec2(tabObj->GetSize().x - TabPreviewOverrideSize.x,
                                      TabBarHeight / 2.f - TabPreviewOverrideSize.y / 2.f) :
                            glm::vec2(TabBarHeight / 2.f - TabPreviewOverrideSize.x / 2.f,
                                      tabObj->GetSize().y - TabPreviewOverrideSize.x);
                }
                else
                {
                    tabWidthOffset = IsTabBarHorizontal() ?
                                         glm::vec2(tabObj->GetSize().x / 2.f, 0) :
                                         glm::vec2(0, tabObj->GetSize().y / 2.f);
                }

                previewObject->SetGlobalPosition(tabObj->GetGlobalPosition() + tabWidthOffset);
            }
        }

        void TabArea::DrawTabPreview()
        {
            ssGUI::GUIObject* previewObject =
                CurrentObjectsReferences.GetObjectReference(TabPreview);

            if(!previewObject->IsExtensionExist<ssGUI::Extensions::AdvancedPosition>() ||
               !previewObject->IsExtensionExist<ssGUI::Extensions::AdvancedSize>())
            {
                ssGUI_ERROR(ssGUI_TAG_EXT,
                            "Preview object does not have advanced size or advanced position");
                ssLOG_EXIT_PROGRAM();
                return;
            }

            previewObject->SetEnabled(true);

            //Set preview parent
            previewObject->SetParent(CurrentObjectsReferences.GetObjectReference(ContentsHolder));

            //Enable advanced size and advanced position
            previewObject->GetExtension<ssGUI::Extensions::AdvancedPosition>()->SetEnabled(true);
            previewObject->GetExtension<ssGUI::Extensions::AdvancedSize>()->SetEnabled(true);
        }

        void TabArea::DiscardTabPreview()
        {
            CurrentObjectsReferences.GetObjectReference(TabPreview)->SetEnabled(false);
        }

        void TabArea::ValidateTabsAndOrders()
        {
            if(!SanityCheck())
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Sanity check failed");
                ssLOG_EXIT_PROGRAM();
                return;
            }

            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);
            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);

            contentsHolder->StashChildrenIterator();
            tabsHolder->StashChildrenIterator();

            contentsHolder->MoveChildrenIteratorToFirst();
            tabsHolder->MoveChildrenIteratorToFirst();

            ssGUI::GUIObject* lastTab = nullptr;
            ssGUI::GUIObject* lastChild = nullptr;

            //For each child
            while(!contentsHolder->IsChildrenIteratorEnd())
            {
                //Skip if it is not a tab content
                if(!contentsHolder->GetCurrentChild()
                        ->IsExtensionExist<ssGUI::Extensions::Tabbable>())
                {
                    contentsHolder->MoveChildrenIteratorNext();
                    continue;
                }

                ssGUIObjectIndex currentChildIndex =
                    CurrentObjectsReferences.GetObjectIndex(contentsHolder->GetCurrentChild());

                //If it is a tab content, but not registered
                if(TabsInfos.find(currentChildIndex) == TabsInfos.end())
                {
                    //If this is the first child, add the current child as a tab content to the
                    //front
                    if(lastChild == nullptr)
                        lastTab = AddContentToFront(contentsHolder->GetCurrentChild());
                    //Otherwise, add it as a tab content tab bar after the last tab
                    else
                        lastTab = AddContentAfter(contentsHolder->GetCurrentChild(), lastTab);
                }
                //If it is a tab content and registered
                else
                {
                    //Get the next valid tab
                    while(!tabsHolder->IsChildrenIteratorEnd() &&
                          tabsHolder->GetCurrentChild()->GetType() !=
                              ssGUI::Enums::GUIObjectType::TAB)
                    {
                        tabsHolder->MoveChildrenIteratorNext();
                    }

                    ssGUI::GUIObject* currentContentTab =
                        CurrentObjectsReferences
                            .GetObjectReference(TabsInfos.at(currentChildIndex));
                    ssGUI::GUIObject* currentTab = tabsHolder->GetCurrentChild();

                    //If the tab we are currently referencing is invalid, create a new tab for this
                    //content
                    if(currentContentTab == nullptr)
                    {
                        currentContentTab = AddTab(currentChildIndex);
                        TabsInfos.at(currentChildIndex) =
                            CurrentObjectsReferences.AddObjectReference(currentContentTab);
                    }
                    //Otherwise, check if the tab is in the tab bar and parent the tab content if it
                    //is not
                    else if(currentContentTab->GetParent() != tabsHolder)
                        currentContentTab->SetParent(tabsHolder);

                    //If we can't find the current tab, move the content tab to after the last tab
                    if(currentTab == nullptr || currentTab != currentContentTab)
                    {
                        //If lastTab is nullptr, that means this is the first tab, move the tab to
                        //first
                        if(lastTab == nullptr)
                            tabsHolder->MoveChildToFirst(currentContentTab);
                        //Otherwise, move the tab after the last tab
                        else
                            tabsHolder->MoveChildAfterTargetChild(currentContentTab, lastTab);

                        ssGUI_ERROR(ssGUI_TAG_EXT, "Tab registered but not found");
                    }

                    lastTab = currentContentTab;
                }

                lastChild = contentsHolder->GetCurrentChild();

                contentsHolder->MoveChildrenIteratorNext();
                tabsHolder->MoveChildrenIteratorToChild(lastTab);
                tabsHolder->MoveChildrenIteratorNext();
            }

            contentsHolder->PopChildrenIterator();
            tabsHolder->PopChildrenIterator();
        }

        ssGUI::GUIObject* TabArea::GetFirstLayoutChild(ssGUI::GUIObject* parent)
        {
            std::vector<ssGUI::GUIObject*> children = parent->GetListOfChildren();

            for(int i = 0; i < children.size(); ++i)
            {
                if(!children.at(i)->HasTag(ssGUI::Tags::FLOATING) &&
                   !children.at(i)->HasTag(ssGUI::Tags::OVERLAY) && children.at(i)->IsEnabled())
                {
                    return children.at(i);
                }
            }

            return nullptr;
        }

        ssGUI::GUIObject* TabArea::GetLastLayoutChild(ssGUI::GUIObject* parent)
        {
            std::vector<ssGUI::GUIObject*> children = parent->GetListOfChildren();

            for(int i = children.size(); i >= 0; --i)
            {
                if(!children.at(i)->HasTag(ssGUI::Tags::FLOATING) &&
                   !children.at(i)->HasTag(ssGUI::Tags::OVERLAY) && children.at(i)->IsEnabled())
                {
                    return children.at(i);
                }
            }

            return nullptr;
        }

        ssGUI::Tab* TabArea::AddTab(ssGUIObjectIndex contentId, ssGUI::Tab* tabToUse)
        {
            if(!SanityCheck())
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Sanity check failed");
                ssLOG_EXIT_PROGRAM();
                return nullptr;
            }

            ssGUI::Tab* tab = nullptr;

            if(DefaultTabObject != nullptr)
                tabToUse = DefaultTabObject;

            if(tabToUse == nullptr)
            {
                tab =
                    CurrentObjectsReferences.GetObjectReference(TabsHolder)->AddChild<ssGUI::Tab>();
                tab->SetTabColor(NewTabColor);
                tab->GetTabTitleObject()->SetText("Tab");
            }
            else
            {
                if(tabToUse->IsEventCallbackExist(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED) &&
                   tabToUse->GetAssociatedContent() != nullptr &&
                   tabToUse->GetAssociatedContent()
                       ->IsExtensionExist<ssGUI::Extensions::Tabbable>())
                {
                    ssGUI::GUIObject* previousTabArea =
                        tabToUse->GetAssociatedContent()
                            ->GetExtension<ssGUI::Extensions::Tabbable>()
                            ->GetTabAreaObject();

                    //Remove event callback from old tab area if it exists
                    if(previousTabArea != nullptr)
                    {
                        tabToUse->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)
                            ->RemoveEventListener(ListenerKey, previousTabArea);
                    }
                }

                tab = tabToUse;
            }

            ssGUIObjectIndex tabAreaContainerId =
                tab->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)
                    ->AddObjectReference(Container);

            tab->GetEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED)
                ->AddEventListener(ListenerKey,
                                   Container,
                                   [tabAreaContainerId](EventInfo& info)
                                   {
                                       //Set LastClickedTab to the current tab when it is clicked
                                       ssGUI::Tab* currentTab =
                                           static_cast<ssGUI::Tab*>(info.Container);
                                       if(currentTab->GetButtonState() ==
                                          ssGUI::Enums::ButtonState::ON_CLICK)
                                       {
                                           ssGUI::GUIObject* tabAreaContainer =
                                               info.References
                                                   ->GetObjectReference(tabAreaContainerId);
                                           if(tabAreaContainer == nullptr ||
                                              !tabAreaContainer
                                                   ->IsExtensionExist<ssGUI::Extensions::TabArea>())
                                           {
                                               info.DeleteCurrentListener = true;
                                               return;
                                           }

                                           tabAreaContainer
                                               ->GetExtension<ssGUI::Extensions::TabArea>()
                                               ->SetLastClickedTab(info.Container);
                                       }
                                   });

            tab->SetUserCreated(false);

            if(tabToUse == nullptr)
            {
                if(IsTabBarHorizontal())
                    tab->SetSize(glm::vec2(NewTabWidth, TabBarHeight));
                else
                    tab->SetSize(glm::vec2(TabBarHeight, NewTabWidth));
            }

            tab->SetAssociatedContent(CurrentObjectsReferences.GetObjectReference(contentId));
            ssGUIObjectIndex tabIndex = CurrentObjectsReferences.AddObjectReference(tab);
            AddDeletionToCloseButton(tab);
            return tab;
        }

        void TabArea::UpdateOrientationsForContentsAndPreviews()
        {
            bool tabBarHorizontal = IsTabBarHorizontal();
            float minLength = 0;

            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(tabBarHorizontal)
            {
                CurrentObjectsReferences.GetObjectReference(TabsHolder)
                    ->SetMinSize(glm::vec2(25, TabBarHeight));
                CurrentObjectsReferences.GetObjectReference(TabsHolder)
                    ->SetMaxSize(glm::vec2(9999, TabBarHeight));
            }
            else
            {
                CurrentObjectsReferences.GetObjectReference(TabsHolder)
                    ->SetMinSize(glm::vec2(TabBarHeight, 25));
                CurrentObjectsReferences.GetObjectReference(TabsHolder)
                    ->SetMaxSize(glm::vec2(TabBarHeight, 9999));
            }
        }

        bool TabArea::TabbedIfNeeded(ssGUI::Backend::SystemInputInterface*
                                                 inputInterface,
                                             ssGUI::InputStatus& currentInputStatus,
                                             ssGUI::InputStatus& lastInputStatus,
                                             bool isMouseInsideTabBar,
                                             glm::ivec2 mousePos)
        {
#define DISCARD_AND_RETURN() \
    do                       \
    {                        \
        DiscardTabPreview(); \
        return true;         \
    }                        \
    while(0)

            //Check if there's any external tabs being dragged
            if(lastInputStatus.CurrentDragData.GetDragDataType() !=
                   ssGUI::Enums::DragDataType::GUI_OBJECT ||
               lastInputStatus.CurrentDragData.IsIntercepted())
            {
                return false;
            }

            ssGUI::GUIObject* draggedObj =
                lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>();

            //Check if mouse input is being block by external object
            if(currentInputStatus.MouseInputBlockedData.GetBlockDataType() ==
                   ssGUI::Enums::BlockDataType::GUI_OBJECT &&
               !currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>()
                    ->HasParentRecursively(Container) &&
               !currentInputStatus.MouseInputBlockedData.GetBlockData<ssGUI::GUIObject>()
                    ->HasParentRecursively(draggedObj))
            {
                return false;
            }

            //Check if it is possible to tab current dragged object to container
            if(!draggedObj->IsExtensionExist<ssGUI::Extensions::Tabbable>() ||
               !CanContentBeTabbed(draggedObj,
                                           draggedObj->GetExtension<ssGUI::Extensions::Tabbable>()
                                               ->GetTopLevelParent()))
            {
                return false;
            }

            //Check if the mouse is on any of the tabs
            if(isMouseInsideTabBar)
            {
                auto hoverIt = GetMouseHoveredTab(mousePos);

                if(hoverIt != TabsInfos.end() &&
                   CurrentObjectsReferences.GetObjectReference(hoverIt->first) != draggedObj &&
                   CurrentObjectsReferences.GetObjectReference(hoverIt->first) != nullptr &&
                   CurrentObjectsReferences.GetObjectReference(hoverIt->second) != nullptr)
                {
                    //Check if it is mouse up
                    if(inputInterface->IsButtonOrKeyUp(ssGUI::Enums::MouseButton::LEFT))
                    {
                        //If draggedObj parent has TabArea, call remove content
                        ssGUI::GUIObject* tabAreaObject =
                            draggedObj->GetExtension<ssGUI::Extensions::Tabbable>()
                                ->GetTabAreaObject();
                        ssGUI::Tab* tabClone = nullptr;
                        if(tabAreaObject != nullptr &&
                           tabAreaObject->IsExtensionExist<ssGUI::Extensions::TabArea>())
                        {
                            tabClone = tabAreaObject->GetExtension<ssGUI::Extensions::TabArea>()
                                           ->GetTabFromContent(draggedObj)
                                           ->Clone(true);

                            tabAreaObject->GetExtension<ssGUI::Extensions::TabArea>()
                                ->RemoveContent(draggedObj);
                        }

                        //Add it and intercept it
                        ssGUI::GUIObject* tabObj =
                            CurrentObjectsReferences.GetObjectReference(hoverIt->second);

                        ssGUI::GUIObject* tabsHolder =
                            CurrentObjectsReferences.GetObjectReference(TabsHolder);
                        if(tabsHolder == nullptr ||
                           !tabsHolder->IsExtensionExist<ssGUI::Extensions::Layout>())
                        {
                            ssGUI_ERROR(ssGUI_TAG_EXT, "Invalid tabs holder");
                            ssLOG_EXIT_PROGRAM();
                            return false;
                        }

                        bool addBefore = true;
                        addBefore =
                            IsTabBarHorizontal() ?
                                mousePos.x <
                                    tabObj->GetGlobalPosition().x + tabObj->GetSize().x / 2.f :
                                mousePos.y <
                                    tabObj->GetGlobalPosition().y + tabObj->GetSize().y / 2.f;

                        addBefore = tabsHolder->GetExtension<ssGUI::Extensions::Layout>()
                                            ->IsReverseOrder() ?
                                        !addBefore :
                                        addBefore;

                        if(addBefore)
                            AddContentBefore(draggedObj,
                                             CurrentObjectsReferences
                                                 .GetObjectReference(hoverIt->first),
                                             tabClone);
                        else
                            AddContentAfter(draggedObj,
                                            CurrentObjectsReferences
                                                .GetObjectReference(hoverIt->first),
                                            tabClone);

                        lastInputStatus.CurrentDragData.SetInterceptor(Container);
                        DISCARD_AND_RETURN();
                    }
                    //Otherwise we should show the preview
                    else
                    {
                        DrawTabPreview(hoverIt->second, mousePos);
                        return true;
                    }
                }
            }
            //Otherwise if outside of tab bar
            else
            {
                //If it is not inside container, return false
                if(mousePos.x < Container->GetGlobalPosition().x ||
                   mousePos.x >= Container->GetGlobalPosition().x + Container->GetSize().x ||
                   mousePos.y < Container->GetGlobalPosition().y ||
                   mousePos.y >= Container->GetGlobalPosition().y + Container->GetSize().y)
                {
                    return false;
                }

                //Check if the object being dragged is one of our contents, if so discard
                if(IsTabContentExist(draggedObj))
                    DISCARD_AND_RETURN();

                //Check if it is mouse up
                if(inputInterface->IsButtonOrKeyUp(ssGUI::Enums::MouseButton::LEFT))
                {
                    //If mouse up, add to end of tab bar
                    //If draggedObj parent has TabArea, call remove content
                    ssGUI::GUIObject* tabAreaObject =
                        draggedObj->GetExtension<ssGUI::Extensions::Tabbable>()->GetTabAreaObject();
                    ssGUI::Tab* tabClone = nullptr;
                    if(tabAreaObject != nullptr &&
                       tabAreaObject->IsExtensionExist<ssGUI::Extensions::TabArea>())
                    {
                        tabClone = tabAreaObject->GetExtension<ssGUI::Extensions::TabArea>()
                                       ->GetTabFromContent(draggedObj)
                                       ->Clone(true);

                        tabAreaObject->GetExtension<ssGUI::Extensions::TabArea>()
                            ->RemoveContent(draggedObj);
                    }

                    AddContent(draggedObj, tabClone);
                    lastInputStatus.CurrentDragData.SetInterceptor(Container);
                    DISCARD_AND_RETURN();
                }
                //Otherwise show preview
                else
                {
                    DrawTabPreview();
                    return true;
                }
            }

            return false;
        }

        void TabArea::SetLastClickedTab(ssGUI::GUIObject* lastTab)
        {
            if(Container == nullptr)
                return;

            LastClickedTab = CurrentObjectsReferences.GetObjectIndex(lastTab);
        }

        bool TabArea::SanityCheck() const
        {
            if(Container == nullptr || ContentsHolder < 0 || TabsHolder < 0 ||
               CurrentObjectsReferences.GetObjectReference(ContentsHolder) == nullptr ||
               CurrentObjectsReferences.GetObjectReference(TabsHolder) == nullptr ||
               !CurrentObjectsReferences.GetObjectReference(ContentsHolder)
                    ->IsExtensionExist<ssGUI::Extensions::Layout>() ||
               !CurrentObjectsReferences.GetObjectReference(TabsHolder)
                    ->IsExtensionExist<ssGUI::Extensions::Layout>())
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Sanity check failed");
                return false;
            }

            return true;
        }

        bool TabArea::CanContentBeTabbed(ssGUI::GUIObject* contentToCheck,
                                                 ssGUI::GUIObject* contentTopLevelParent) const
        {
            //If top level parent exists for the content
            if(contentTopLevelParent != nullptr)
            {
                //If top level parent is not this object or container's recursive parent, return
                if(contentTopLevelParent != Container &&
                   !Container->HasParentRecursively(contentTopLevelParent))
                {
                    return false;
                }
            }
            //Otherwise, check if we are under the same main window
            else
            {
                ssGUI::MainWindow* containerMainWindow = nullptr;
                ssGUI::GUIObject* currentParent = Container;

                while(currentParent != nullptr &&
                      currentParent->GetType() != ssGUI::Enums::GUIObjectType::MAIN_WINDOW)
                {
                    currentParent = Container->GetParent();
                }

                if(currentParent != nullptr)
                    containerMainWindow = static_cast<ssGUI::MainWindow*>(currentParent);
                else
                {
                    //This should not happen, but if it does, exit
                    ssGUI_ERROR(ssGUI_TAG_EXT, "We are not under Main Window, this is not handled");
                    ssLOG_EXIT_PROGRAM();
                    return false;
                }

                if(!containerMainWindow->HasChildRecursively(contentToCheck))
                    return false;
            }

            return true;
        }

        void TabArea::AddDeletionToCloseButton(ssGUI::Tab* tab)
        {
            if(tab->GetCloseButton() == nullptr)
                return;

            auto* ecb = tab->GetCloseButton()
                            ->AddEventCallback(ssGUI::Enums::EventType::BUTTON_STATE_CHANGED);

            ssGUIObjectIndex containerId = ecb->AddObjectReference(Container);
            ssGUIObjectIndex tabId = ecb->AddObjectReference(tab);
            ecb->AddEventListener(ListenerKey,
                                  Container,
                                  [containerId, tabId](ssGUI::EventInfo& info)
                                  {
                                      if(info.References->GetObjectReference(containerId) ==
                                             nullptr ||
                                         !info.References->GetObjectReference(containerId)
                                              ->IsExtensionExist<ssGUI::Extensions::TabArea>() ||
                                         info.References->GetObjectReference(tabId) == nullptr ||
                                         info.References->GetObjectReference<ssGUI::Tab>(tabId)
                                                 ->GetAssociatedContent() == nullptr)
                                      {
                                          info.DeleteCurrentListener = true;
                                          return;
                                      }

                                      ssGUI::Button* closeButton =
                                          static_cast<ssGUI::Button*>(info.Container);
                                      ssGUI::Tab* tab =
                                          info.References->GetObjectReference<ssGUI::Tab>(tabId);
                                      ssGUI::GUIObject* tabContent = tab->GetAssociatedContent();

                                      if(closeButton->GetButtonState() ==
                                         ssGUI::Enums::ButtonState::ON_CLICK)
                                      {
                                          info.References->GetObjectReference(containerId)
                                              ->GetExtension<ssGUI::Extensions::TabArea>()
                                              ->RemoveContent(tabContent);

                                          tabContent->Delete();
                                          tab->Delete();
                                          info.DeleteCurrentListener = true;
                                      }
                                  });
        }

        void TabArea::ConstructRenderInfo()
        {
        }

        void TabArea::ConstructRenderInfo(ssGUI::Backend::DrawingInterface* drawingInterface,
                                          ssGUI::GUIObject* mainWindow,
                                          glm::vec2 mainWindowPositionOffset)
        {
        }

        const std::string TabArea::EXTENSION_NAME = "Tab Area";
        const std::string TabArea::ListenerKey = "Tab Area";

        void TabArea::SetOverrideTabPreviewSize(bool override, glm::vec2 size)
        {
            OverrideTabPreviewSize = override;
            TabPreviewOverrideSize = size;
        }

        bool TabArea::IsOverrideTabPreviewSize() const
        {
            return OverrideTabPreviewSize;
        }

        glm::vec2 TabArea::GetOverrideTabPreviewSize() const
        {
            return TabPreviewOverrideSize;
        }

        void TabArea::SetTabPreviewColor(glm::u8vec4 color)
        {
            PreviewColor = color;
        }

        glm::u8vec4 TabArea::GetTabPreviewColor() const
        {
            return PreviewColor;
        }
        
        void TabArea::SetDefaultPreviewColor(glm::u8vec4 color)
        {
            DefaultPreviewColor = color;
        }

        glm::u8vec4 TabArea::GetDefaultPreviewColor()
        {
            return DefaultPreviewColor;
        }

        ssGUI::GUIObject* TabArea::GetTabBar() const
        {
            if(!SanityCheck())
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference(TabBar);
        }

        void TabArea::SetTabBarHeight(float height)
        {
            TabBarHeight = height;
        }

        float TabArea::GetTabBarHeight() const
        {
            return TabBarHeight;
        }
        
        void TabArea::SetTabBarColor(glm::u8vec4 color)
        {
            if(!SanityCheck())
                return;

            CurrentObjectsReferences.GetObjectReference(TabBar)->SetBackgroundColor(color);
        }

        glm::u8vec4 TabArea::GetTabBarColor() const
        {
            return CurrentObjectsReferences.GetObjectReference(TabBar)->GetBackgroundColor();
        }
        
        void TabArea::SetTabBarHorizontal(bool horizontal)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);
            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);
            ssGUI::GUIObject* tabBar = CurrentObjectsReferences.GetObjectReference(TabBar);

            tabsHolder->GetExtension<ssGUI::Extensions::Layout>()->SetHorizontalLayout(horizontal);
            tabBar->GetExtension<ssGUI::Extensions::Layout>()->SetHorizontalLayout(horizontal);
            Container->GetExtension<ssGUI::Extensions::Layout>()->SetHorizontalLayout(!horizontal);
            contentsHolder->GetExtension<ssGUI::Extensions::Layout>()
                ->SetHorizontalLayout(!horizontal);

            UpdateOrientationsForContentsAndPreviews();
        }

        bool TabArea::IsTabBarHorizontal() const
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return true;
            }

            return CurrentObjectsReferences.GetObjectReference(TabsHolder)
                ->GetExtension<ssGUI::Extensions::Layout>()
                ->IsHorizontalLayout();
        }

        void TabArea::SetDefaultTabBarObject(ssGUI::GUIObject* defaultTabBar)
        {
            DefaultTabBarObject = defaultTabBar;
        }

        ssGUI::GUIObject* TabArea::GetDefaultTabBarObject()
        {
            return DefaultTabBarObject;
        }

        void TabArea::SetDefaultTabBarColor(glm::u8vec4 color)
        {
            DefaultTabBarColor = color;
        }

        glm::u8vec4 TabArea::GetDefaultTabBarColor()
        {
            return DefaultTabBarColor;
        }

        void TabArea::SetNewTabWidth(float width)
        {
            NewTabWidth = width;
        }

        float TabArea::GetNewTabWidth() const
        {
            return NewTabWidth;
        }

        void TabArea::SetReverseTabsOrder(bool reverse)
        {
            if(!SanityCheck())
                return;

            CurrentObjectsReferences.GetObjectReference(TabsHolder)
                ->GetExtension<ssGUI::Extensions::Layout>()
                ->SetReverseOrder(reverse);
        }

        bool TabArea::IsReverseTabsOrder() const
        {
            if(!SanityCheck())
                return false;

            return CurrentObjectsReferences.GetObjectReference(TabsHolder)
                ->GetExtension<ssGUI::Extensions::Layout>()
                ->IsReverseOrder();
        }

        void TabArea::SetNewTabColor(glm::u8vec4 color)
        {
            NewTabColor = color;
        }

        glm::u8vec4 TabArea::GetNewTabColor() const
        {
            return NewTabColor;
        }
        
        void TabArea::SetDefaultTabObject(ssGUI::Tab* defaultTab)
        {
            DefaultTabObject = defaultTab;
        }

        ssGUI::Tab* TabArea::GetDefaultTabObject()
        {
            return DefaultTabObject;
        }

        void TabArea::SetDefaultTabColor(glm::u8vec4 color)
        {
            DefaultTabColor = color;
        }

        glm::u8vec4 TabArea::GetDefaultTabColor()
        {
            return DefaultTabColor;
        }

        ssGUI::Tab* TabArea::AddContent(ssGUI::GUIObject* contentToRegister, ssGUI::Tab* tabToUse)
        {
            if(!SanityCheck() || contentToRegister == nullptr)
                return nullptr;

            //If the content is already registered and the tab is valid, return the tab
            if(IsTabContentExist(contentToRegister) &&
               GetTabFromContent(contentToRegister) != nullptr)
                return GetTabFromContent(contentToRegister);

            //Remove invalid tab if content is registered but tab is invalid
            if(GetTabFromContent(contentToRegister) == nullptr)
                RemoveContent(contentToRegister);

            //Add tabbable extension to content, it will do nothing if it already exist
            auto* tabbableExt = contentToRegister->AddExtension<ssGUI::Extensions::Tabbable>();

            //If the content's top level parent outside/below the container, abort
            if(!CanContentBeTabbed(contentToRegister, tabbableExt->GetTopLevelParent()))
                return nullptr;

            //Set content parent
            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);

            ssGUIObjectIndex contentId =
                CurrentObjectsReferences.AddObjectReference(contentToRegister);
            ssGUI::Tab* tab = AddTab(contentId, tabToUse);
            TabsInfos[contentId] = CurrentObjectsReferences.AddObjectReference(tab);

            contentToRegister->GetExtension<ssGUI::Extensions::Tabbable>()
                ->SetTabAreaObject(Container);
            contentToRegister->SetParent(contentsHolder);

            //Call event callback
            ssGUI::TabEventInfo info;
            info.Tab = tab;
            info.TabAreaContainer = Container;

            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::NEW_TAB_CONTENT_ADDED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::NEW_TAB_CONTENT_ADDED)
                    ->Notify(contentToRegister, &info);
            }

            //Switch to newly registered content
            SwitchContent(contentToRegister);

            contentsHolder->GetExtension<ssGUI::Extensions::Layout>()->SetCoverFullLength(true);
            return tab;
        }

        ssGUI::Tab* TabArea::AddContentToFront(ssGUI::GUIObject* contentToRegister,
                                               ssGUI::Tab* tabToUse)
        {
            ssGUI::Tab* tab = AddContent(contentToRegister, tabToUse);
            MoveContentFront(contentToRegister);
            return tab;
        }

        ssGUI::Tab* TabArea::AddContentToEnd(ssGUI::GUIObject* contentToRegister,
                                             ssGUI::Tab* tabToUse)
        {
            ssGUI::Tab* tab = AddContent(contentToRegister, tabToUse);
            MoveContentEnd(contentToRegister);
            return tab;
        }

        ssGUI::Tab* TabArea::AddContentBefore(ssGUI::GUIObject* contentToRegister,
                                              ssGUI::GUIObject* contentBefore,
                                              ssGUI::Tab* tabToUse)
        {
            ssGUI::Tab* tab = AddContent(contentToRegister, tabToUse);
            MoveContentBefore(contentToRegister, contentBefore);
            return tab;
        }

        ssGUI::Tab* TabArea::AddContentAfter(ssGUI::GUIObject* contentToRegister,
                                             ssGUI::GUIObject* contentAfter,
                                             ssGUI::Tab* tabToUse)
        {
            ssGUI::Tab* tab = AddContent(contentToRegister, tabToUse);
            MoveContentAfter(contentToRegister, contentAfter);
            return tab;
        }

        void TabArea::RemoveContent(ssGUI::GUIObject* contentToRemove)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(contentToRemove == nullptr ||
               !contentToRemove->IsExtensionExist<ssGUI::Extensions::Tabbable>())
                return;

            ssGUIObjectIndex contentIndex =
                CurrentObjectsReferences.GetObjectIndex(contentToRemove);

            if(contentIndex < 0)
                return;

            auto foundIt = TabsInfos.find(contentIndex);

            if(foundIt == TabsInfos.end())
                return;

            contentToRemove->GetExtension<ssGUI::Extensions::Tabbable>()->SetTabAreaObject(nullptr);

            ssGUI::GUIObject* tabObj = CurrentObjectsReferences.GetObjectReference(foundIt->second);

            if(tabObj != nullptr)
                tabObj->Delete();

            TabsInfos.erase(foundIt);

            if(TabsInfos.empty())
            {
                ssGUI::GUIObject* contentsHolder =
                    CurrentObjectsReferences.GetObjectReference(ContentsHolder);
                contentsHolder->GetExtension<ssGUI::Extensions::Layout>()
                    ->SetCoverFullLength(false);
            }
        }

        ssGUI::Tab* TabArea::GetTabFromContent(ssGUI::GUIObject* registeredContent) const
        {
            if(Container == nullptr || registeredContent == nullptr)
                return nullptr;

            ssGUIObjectIndex contentIndex =
                CurrentObjectsReferences.GetObjectIndex(registeredContent);

            if(contentIndex < 0)
                return nullptr;

            auto foundIt = TabsInfos.find(contentIndex);

            if(foundIt == TabsInfos.end())
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference<ssGUI::Tab>(foundIt->second);
        }

        ssGUI::GUIObject* TabArea::GetTabContentsHolder() const
        {
            return CurrentObjectsReferences.GetObjectReference(ContentsHolder);
        }

        void TabArea::MoveContentBefore(ssGUI::GUIObject* contentToMove,
                                        ssGUI::GUIObject* contentBefore)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(contentToMove == nullptr || contentBefore == nullptr)
                return;

            if(!IsTabContentExist(contentToMove) || !IsTabContentExist(contentBefore))
                return;

            ssGUI::GUIObject* tabToMove = GetTabFromContent(contentToMove);
            ssGUI::GUIObject* tabBefore = GetTabFromContent(contentBefore);

            if(tabToMove == nullptr || tabBefore == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Tab object not found");
                return;
            }

            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);
            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);

            contentsHolder->MoveChildBeforeTargetChild(contentToMove, contentBefore);
            tabsHolder->MoveChildBeforeTargetChild(tabToMove, tabBefore);
        }

        void TabArea::MoveContentFront(ssGUI::GUIObject* contentToMove)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(contentToMove == nullptr || !IsTabContentExist(contentToMove))
                return;

            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);
            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);

            contentsHolder->MoveChildToFirst(contentToMove);

            ssGUI::GUIObject* tabToMove = GetTabFromContent(contentToMove);

            if(tabToMove == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Tab object not found");
                return;
            }

            tabsHolder->MoveChildToFirst(tabToMove);
        }

        void TabArea::MoveContentEnd(ssGUI::GUIObject* contentToMove)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(contentToMove == nullptr || !IsTabContentExist(contentToMove))
                return;

            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);
            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);

            contentsHolder->MoveChildToLast(contentToMove);

            ssGUI::GUIObject* tabToMove = GetTabFromContent(contentToMove);

            if(tabToMove == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Tab object not found");
                return;
            }

            tabsHolder->MoveChildToLast(tabToMove);
        }

        void TabArea::MoveContentAfter(ssGUI::GUIObject* contentToMove,
                                       ssGUI::GUIObject* contentAfter)
        {
            if(!SanityCheck())
            {
                ssLOG_EXIT_PROGRAM();
                return;
            }

            if(contentToMove == nullptr || contentAfter == nullptr ||
               !IsTabContentExist(contentToMove) || !IsTabContentExist(contentAfter))
            {
                return;
            }

            ssGUI::GUIObject* tabToMove = GetTabFromContent(contentToMove);
            ssGUI::GUIObject* tabAfter = GetTabFromContent(contentAfter);

            if(tabToMove == nullptr || tabAfter == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Tab object not found");
                return;
            }

            ssGUI::GUIObject* contentsHolder =
                CurrentObjectsReferences.GetObjectReference(ContentsHolder);
            ssGUI::GUIObject* tabsHolder = CurrentObjectsReferences.GetObjectReference(TabsHolder);

            contentsHolder->MoveChildAfterTargetChild(contentToMove, contentAfter);
            tabsHolder->MoveChildAfterTargetChild(tabToMove, tabAfter);
        }

        void TabArea::SwitchContent(ssGUI::GUIObject* registeredContent)
        {
            if(Container == nullptr)
                return;

            ssGUIObjectIndex contentId = -1;
            ssGUIObjectIndex originalContentId = CurrentTabContent;

            auto foundIt = TabsInfos.end();

            if(registeredContent != nullptr)
            {
                contentId = CurrentObjectsReferences.GetObjectIndex(registeredContent);

                if(contentId < 0)
                    return;

                foundIt = TabsInfos.find(contentId);

                if(foundIt == TabsInfos.end())
                    return;
            }

            CurrentTabContent = contentId;

            //Disable the rest of the tabs
            for(auto currentIt = TabsInfos.begin(); currentIt != TabsInfos.end(); ++currentIt)
            {
                ssGUI::GUIObject* contentObj =
                    CurrentObjectsReferences.GetObjectReference(currentIt->first);

                if(contentObj == nullptr)
                    continue;

                contentObj->SetEnabled(false);

                ssGUI::Tab* curTab =
                    CurrentObjectsReferences.GetObjectReference<ssGUI::Tab>(currentIt->second);
                if(curTab != nullptr)
                    curTab->SelectTab(false);
            }

            //If we switch to no content (i.e. registeredContent == nullptr), trigger event callback
            //and return here
            ssGUI::TabEventInfo info;
            info.Tab = nullptr;
            info.TabAreaContainer = Container;

            if(contentId < 0)
            {
                if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED))
                {
                    Container->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED)
                        ->Notify(registeredContent, &info);
                }

                return;
            }

            //Enable and switch to the tab we want to switch to
            registeredContent->SetEnabled(true);

            if(GetCurrentTab() != nullptr)
                GetCurrentTab()->SelectTab(true);

            //Trigger event callback
            info.Tab = GetCurrentTab();
            info.TabAreaContainer = Container;

            if(Container->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED))
            {
                Container->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED)
                    ->Notify(registeredContent, &info);
            }

            if(registeredContent
                   ->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED))
            {
                registeredContent->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED)
                    ->Notify(registeredContent, &info);
            }

            if(originalContentId >= 0)
            {
                ssGUI::GUIObject* originalContentObj =
                    CurrentObjectsReferences.GetObjectReference(originalContentId);

                if(originalContentId != CurrentTabContent && originalContentObj != nullptr &&
                   originalContentObj
                       ->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED))
                {
                    originalContentObj
                        ->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_SWITCHED)
                        ->Notify(registeredContent, &info);
                }
            }
        }

        ssGUI::Tab* TabArea::GetCurrentTab() const
        {
            if(CurrentTabContent < 0)
                return nullptr;

            auto tabIt = TabsInfos.find(CurrentTabContent);

            if(tabIt == TabsInfos.end())
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference<ssGUI::Tab>(tabIt->second);
        }

        ssGUI::GUIObject* TabArea::GetCurrentTabContent() const
        {
            if(CurrentTabContent < 0)
                return nullptr;

            auto tabIt = TabsInfos.find(CurrentTabContent);

            if(tabIt == TabsInfos.end())
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference(tabIt->first);
        }

        bool TabArea::IsEnabled() const
        {
            return Enabled;
        }

        void TabArea::SetEnabled(bool enabled)
        {
            Enabled = enabled;
        }

#undef DISCARD_AND_RETURN
#define DISCARD_AND_RETURN() \
    do                       \
    {                        \
        DiscardTabPreview(); \
        return;              \
    }                        \
    while(0)

        void TabArea::Internal_Update(bool isPreUpdate,
                                      ssGUI::Backend::SystemInputInterface* inputInterface,
                                      ssGUI::InputStatus& currentInputStatus,
                                      ssGUI::InputStatus& lastInputStatus,
                                      ssGUI::GUIObject* mainWindow)
        {
            ssGUI_LOG_FUNC();

            //This is function is executed twice, one before the Container GUI object update and one
            //after. You can use the isPreUpdate variable to decide when to execute the extension
            //update
            if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr)
                return;

            //Need to perform clean up if this is disabled. For example delete any objects created
            //by this extension
            if(!Enabled)
                DISCARD_AND_RETURN();

            //Perform order check
            ValidateTabsAndOrders();

            if(!SanityCheck())
                return;

#if 0
            ssLOG_LINE("Container: " << Container);
            ssLOG_LINE("Container->GetMinSize(): " << Container->GetMinSize());
            ssLOG_LINE("Container->GetMaxSize(): " << Container->GetMaxSize());
            ssLOG_LINE( "Container->GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength(): " << 
                        Container->GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength());
            ssLOG_LINE( "Container->GetExtension<ssGUI::Extensions::Layout>()->IsHorizontalLayout(): " << 
                        Container->GetExtension<ssGUI::Extensions::Layout>()->IsHorizontalLayout());


            ssGUI::GUIObject* contentsHolderObj = CurrentObjectsReferences.GetObjectReference(ContentsHolder);

            ssLOG_LINE("contentsHolderObj: " << contentsHolderObj);
            ssLOG_LINE("contentsHolderObj->GetMinSize(): " << contentsHolderObj->GetMinSize());
            ssLOG_LINE("contentsHolderObj->GetMaxSize(): " << contentsHolderObj->GetMaxSize());
            ssLOG_LINE( "contentsHolderObj->GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength(): " << 
                        contentsHolderObj->GetExtension<ssGUI::Extensions::Layout>()->IsCoverFullLength());
            ssLOG_LINE( "contentsHolderObj->GetExtension<ssGUI::Extensions::Layout>()->IsHorizontalLayout(): " << 
                        contentsHolderObj->GetExtension<ssGUI::Extensions::Layout>()->IsHorizontalLayout());
#endif

            //Check if the mouse is on any of the tabs
            auto* mainWindowBackend =
                static_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface();

            glm::ivec2 mousePos = inputInterface->GetCurrentMousePosition(mainWindowBackend);

            auto* tabBar = CurrentObjectsReferences.GetObjectReference(TabBar);
            if(tabBar == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_EXT, "Tab bar missing");
                DISCARD_AND_RETURN();
            }

            glm::vec2 tabBarMaxBound = tabBar->GetGlobalPosition() + tabBar->GetSize();

            bool isMouseInsideTabBar =
                mousePos.x >= tabBar->GetGlobalPosition().x && mousePos.x < tabBarMaxBound.x &&
                mousePos.y >= tabBar->GetGlobalPosition().y && mousePos.y < tabBarMaxBound.y;

            //Check if the mouse is dragging
            ssGUI::Enums::MouseButton leftMouseButton = ssGUI::Enums::MouseButton::LEFT;

            bool mouseInput =
                inputInterface->IsInputExistCurrentFrame(leftMouseButton) ||
                inputInterface->IsInputExistLastFrame(leftMouseButton);

            //Check if we are dragging one of our tab content
            ssGUI::DragData& lastDragData = lastInputStatus.CurrentDragData;

            bool ourTabBeingDragged =
                lastDragData.GetDragDataType() == ssGUI::Enums::DragDataType::GUI_OBJECT &&
                !lastDragData.IsIntercepted() &&
                IsTabContentExist(lastDragData.GetDragData<ssGUI::GUIObject>());

            if(ourTabBeingDragged && mouseInput)
            {
                //If so, continue setting it as being dragged and set the cursor
                currentInputStatus.CurrentDragData
                    .SetDragData(lastInputStatus.CurrentDragData.GetDragData<ssGUI::GUIObject>());

                inputInterface->SetCursorType(ssGUI::Enums::CursorType::MOVE);
            }

            //Check if any of our tabs are being tabbed by another tab
            if(TabbedIfNeeded(inputInterface,
                                      currentInputStatus,
                                      lastInputStatus,
                                      isMouseInsideTabBar,
                                      mousePos))
            {
                return;
            }

            //If left mouse button up for our tab for last frame and it is not tabbed against
            //anything,
            //  untab it if allowed
            auto hoverIt = GetMouseHoveredTab(mousePos);

            //Tab is still being counted as dragged when mouse up (no mouse input for that frame)
            //Therefore, if the tab is being dragged last frame and
            //there's no mouse input last frame,
            //that means it was mouse button up last frame for the tab.
            if(!mouseInput && ourTabBeingDragged && hoverIt == TabsInfos.end())
            {
                ssGUI::GUIObject* draggedContent = lastDragData.GetDragData<ssGUI::GUIObject>();

                if(draggedContent->IsExtensionExist<ssGUI::Extensions::Tabbable>() &&
                   !draggedContent->GetExtension<ssGUI::Extensions::Tabbable>()->IsUntabbable())
                {
                    //Remove the content and position it on the mouse
                    RemoveContent(draggedContent);
                    ssGUI::GUIObject* topLevelParent =
                        lastDragData.GetDragData<ssGUI::GUIObject>()
                            ->GetExtension<ssGUI::Extensions::Tabbable>()
                            ->GetTopLevelParent();

                    glm::vec2 mouseDownPos =
                        lastDragData.GetDragData<ssGUI::GUIObject>()
                            ->GetExtension<ssGUI::Extensions::Tabbable>()
                            ->GetLastMouseDragBeginPosition();

                    glm::vec2 mouseWindowPosDelta =
                        draggedContent->GetGlobalPosition() - mouseDownPos;

                    if(topLevelParent == nullptr)
                        draggedContent->SetParent(mainWindow);
                    else
                        draggedContent->SetParent(topLevelParent);

                    draggedContent->SetGlobalPosition(glm::vec2(mousePos) + mouseWindowPosDelta);

                    //Trigger event callbacks
                    ssGUI::TabEventInfo info;
                    info.Tab = nullptr;
                    info.TabAreaContainer = Container;
                    if(Container
                           ->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_UNTABBED))
                    {
                        Container->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_UNTABBED)
                            ->Notify(draggedContent, &info);
                    }

                    if(draggedContent
                           ->IsEventCallbackExist(ssGUI::Enums::EventType::TAB_CONTENT_UNTABBED))
                    {
                        draggedContent
                            ->GetEventCallback(ssGUI::Enums::EventType::TAB_CONTENT_UNTABBED)
                            ->Notify(draggedContent, &info);
                    }

                    DISCARD_AND_RETURN();
                }
            }

            if(LastClickedTab < 0)
                DISCARD_AND_RETURN();

            //Check last clicked tab if it is being clicked, if so switch to the content
            ssGUI::Tab* lastClickedTab =
                CurrentObjectsReferences.GetObjectReference<ssGUI::Tab>(LastClickedTab);

            if(lastClickedTab == nullptr ||
               lastClickedTab->GetButtonState() != ssGUI::Enums::ButtonState::ON_CLICK)
                DISCARD_AND_RETURN();

            ssGUI::GUIObject* content = lastClickedTab->GetAssociatedContent();
            auto contentIt = TabsInfos.find(CurrentObjectsReferences.GetObjectIndex(content));

            if(contentIt == TabsInfos.end() ||
               CurrentObjectsReferences.GetObjectReference(contentIt->first) == nullptr)
                DISCARD_AND_RETURN();

            ssGUI::GUIObject* contentObj =
                CurrentObjectsReferences.GetObjectReference(contentIt->first);
            SwitchContent(contentObj);

            //Set the drag object
            currentInputStatus.CurrentDragData.SetDragData(contentObj);

            //If content is window, get and tabbable extension and set last mouse drag begin
            //position
            if(contentObj->GetType() == ssGUI::Enums::GUIObjectType::WINDOW &&
               contentObj->IsExtensionExist<ssGUI::Extensions::Tabbable>())
            {
                static_cast<ssGUI::Window*>(contentObj)
                    ->GetExtension<ssGUI::Extensions::Tabbable>()
                    ->SetLastMouseDragBeginPosition(mousePos);
            }

            //Update content size
            CurrentObjectsReferences.GetObjectReference(ContentsHolder)
                ->GetExtension<ssGUI::Extensions::Layout>()
                ->ForceUpdateLayout(inputInterface,
                                    currentInputStatus,
                                    lastInputStatus,
                                    mainWindow);

            DISCARD_AND_RETURN();
        }

        void TabArea::Internal_Draw(bool isPreRender,
                                    ssGUI::Backend::DrawingInterface* drawingInterface,
                                    ssGUI::GUIObject* mainWindow,
                                    glm::vec2 mainWindowPositionOffset)
        {
        }

        std::string TabArea::GetExtensionName() const
        {
            return EXTENSION_NAME;
        }

        void TabArea::BindToObject(ssGUI::GUIObject* bindObj)
        {
            Container = bindObj;

            Initialize();
        }

        void TabArea::Copy(const ssGUI::Extensions::Extension* extension)
        {
            if(extension->GetExtensionName() != EXTENSION_NAME)
                return;

            auto* original = static_cast<const ssGUI::Extensions::TabArea*>(extension);
            
            Enabled = original->Enabled;
            TabBarHeight = original->TabBarHeight;
            NewTabWidth = original->NewTabWidth;
            CurrentObjectsReferences = original->CurrentObjectsReferences;
            TabBar = original->TabBar;
            CurrentTabContent = original->CurrentTabContent;
            LastClickedTab = -1;
            ContentsHolder = original->ContentsHolder;
            TabsHolder = original->TabsHolder;
            TabsInfos = original->TabsInfos;
            TabPreview = original->TabPreview;
            PreviewColor = original->PreviewColor;
            OverrideTabPreviewSize = original->OverrideTabPreviewSize;
            TabPreviewOverrideSize = original->TabPreviewOverrideSize;
            NewTabColor = original->NewTabColor;
        }

        ObjectsReferences* TabArea::Internal_GetObjectsReferences()
        {
            return &CurrentObjectsReferences;
        }

        TabArea* TabArea::Clone()
        {
            TabArea* temp = new TabArea(*this);
            return temp;
        }
    }

}