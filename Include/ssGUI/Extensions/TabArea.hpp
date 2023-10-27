#ifndef SSGUI_TAB_AREA_HPP
#define SSGUI_TAB_AREA_HPP

#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/CompositeClasses/Tab.hpp"

#include <utility>

namespace ssGUI
{
    //namespace: ssGUI::Extensions
    namespace Extensions
    {
        /*class: ssGUI::Extensions::TabArea

        Variables & Constructor:
        ============================== C++ ==============================
        protected:
            ssGUI::GUIObject* Container;        //See <BindToObject>
            bool Enabled;                       //See <IsEnabled>
        =================================================================
        ============================== C++ ==============================
        TabArea::TabArea() :    Container(nullptr),
                                                    Enabled(true)
        {}
        =================================================================
        */
        class TabArea : public Extension
        {
            public:
                friend class ssGUI::Factory;

            private:
                TabArea& operator=(const TabArea& other);

            protected:
                //See <BindToObject>
                ssGUI::GUIObject* Container;

                //See <IsEnabled>
                bool Enabled;

                float TabBarHeight;

                float NewTabWidth;

                //See <Internal_GetObjectsReferences>
                ObjectsReferences CurrentObjectsReferences;

                ssGUIObjectIndex TabBar;
                ssGUIObjectIndex CurrentTabContent;

                ssGUIObjectIndex LastClickedTab;

                ssGUIObjectIndex ContentsHolder;
                ssGUIObjectIndex TabsHolder;

                using TabId = ssGUIObjectIndex;
                using TabContentId = ssGUIObjectIndex;

                std::unordered_map<TabContentId, TabId> TabsInfos;

                ssGUIObjectIndex TabPreview;

                //See <GetDockPreviewColor>
                glm::u8vec4 PreviewColor;
                bool OverrideTabPreviewSize;
                glm::vec2 TabPreviewOverrideSize;

                //See <GetTabColor>
                glm::u8vec4 NewTabColor;

                static ssGUI::GUIObject* DefaultTabBarObject;
                static glm::u8vec4 DefaultTabBarColor;
                static ssGUI::Tab* DefaultTabObject;
                static glm::u8vec4 DefaultTabColor;
                static glm::u8vec4 DefaultPreviewColor;

                TabArea();
                virtual ~TabArea() override;
                TabArea(const TabArea& other);

                static void* operator new(size_t size)
                {
                    return ::operator new(size);
                };

                static void* operator new[](size_t size)
                {
                    return ::operator new(size);
                };

                static void operator delete(void* p)
                {
                    free(p);
                };

                static void operator delete[](void* p)
                {
                    free(p);
                };

                virtual void Initialize();

                virtual bool IsTabContentExist(ssGUI::GUIObject* content) const;

                using TabInfoIterator = std::unordered_map<TabContentId, TabId>::iterator;
                virtual TabInfoIterator GetMouseHoveredTab(glm::ivec2 mousePos);

                virtual void DrawTabPreview(TabId tab, glm::vec2 mousePos);

                virtual void DrawTabPreview();

                virtual void DiscardTabPreview();

                virtual void ValidateTabsAndOrders();

                virtual ssGUI::GUIObject* GetFirstLayoutChild(ssGUI::GUIObject* parent);

                virtual ssGUI::GUIObject* GetLastLayoutChild(ssGUI::GUIObject* parent);

                virtual ssGUI::Tab* AddTab(ssGUIObjectIndex contentId,
                                           ssGUI::Tab* tabToUse = nullptr);

                virtual void UpdateOrientationsForContentsAndPreviews();

                virtual bool TabbedIfNeeded(ssGUI::Backend::BackendSystemInputInterface*
                                                        inputInterface,
                                                    ssGUI::InputStatus& inputStatus,
                                                    ssGUI::InputStatus& lastInputStatus,
                                                    bool isMouseInsideTabBar,
                                                    glm::ivec2 mousePos);

                virtual void SetLastClickedTab(ssGUI::GUIObject* lastTab);

                virtual bool SanityCheck() const;

                virtual bool CanContentBeTabbed(ssGUI::GUIObject* contentToCheck,
                                                        ssGUI::GUIObject* contentTopLevelParent)
                    const;

                virtual void AddDeletionToCloseButton(ssGUI::Tab* tab);

                virtual void ConstructRenderInfo() override;
                virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface*
                                                     drawingInterface,
                                                 ssGUI::GUIObject* mainWindow,
                                                 glm::vec2 mainWindowPositionOffset) override;

            public:
                //====================================================================
                //Group: Constants
                //====================================================================

                //string: EXTENSION_NAME
                static const std::string EXTENSION_NAME;

                //string: ListenerKey
                static const std::string ListenerKey;

                //function: SetOverrideTabPreviewSize
                //By default, the tab preview size is half of the tab size.
                //If override is true, the tab preview size will be OverrideTabPreviewSize.
                virtual void SetOverrideTabPreviewSize(bool override, glm::vec2 size = glm::vec2());

                //function: IsOverrideTabPreviewSize
                virtual bool IsOverrideTabPreviewSize() const;

                //function: GetOverrideTabPreviewSize
                virtual glm::vec2 GetOverrideTabPreviewSize() const;

                //function: SetTabPreviewColor
                virtual void SetTabPreviewColor(glm::u8vec4 color);
                //function: GetTabPreviewColor
                virtual glm::u8vec4 GetTabPreviewColor() const;

                //function: GetTabBar
                virtual ssGUI::GUIObject* GetTabBar() const;

                //function: SetTabBarHeight
                virtual void SetTabBarHeight(float height);

                //function: GetTabBarHeight
                virtual float GetTabBarHeight() const;

                //function: SetNewTabWidth
                virtual void SetNewTabWidth(float width);

                //function: GetNewTabWidth
                virtual float GetNewTabWidth() const;

                //function: SetReverseTabsOrder
                virtual void SetReverseTabsOrder(bool reverse);

                //function: IsReverseTabsOrder
                virtual bool IsReverseTabsOrder() const;

                //function: SetNewTabColor
                virtual void SetNewTabColor(glm::u8vec4 color);

                //function: GetNewTabColor
                virtual glm::u8vec4 GetNewTabColor() const;

                //function: SetTabBarColor
                virtual void SetTabBarColor(glm::u8vec4 color);

                //function: GetTabBarColor
                virtual glm::u8vec4 GetTabBarColor() const;

                //function: AddContent
                virtual ssGUI::Tab* AddContent(ssGUI::GUIObject* contentToRegister,
                                               ssGUI::Tab* tabToUse = nullptr);

                //function: AddContentToFront
                virtual ssGUI::Tab* AddContentToFront(ssGUI::GUIObject* contentToRegister,
                                                      ssGUI::Tab* tabToUse = nullptr);

                //function: AddContentToEnd
                virtual ssGUI::Tab* AddContentToEnd(ssGUI::GUIObject* contentToRegister,
                                                    ssGUI::Tab* tabToUse = nullptr);

                //function: AddContentBefore
                virtual ssGUI::Tab* AddContentBefore(ssGUI::GUIObject* contentToRegister,
                                                     ssGUI::GUIObject* contentBefore,
                                                     ssGUI::Tab* tabToUse = nullptr);

                //function: AddContentAfter
                virtual ssGUI::Tab* AddContentAfter(ssGUI::GUIObject* contentToRegister,
                                                    ssGUI::GUIObject* contentAfter,
                                                    ssGUI::Tab* tabToUse = nullptr);

                //function: RemoveContent
                virtual void RemoveContent(ssGUI::GUIObject* contentToRemove);

                //function: GetTabFromContent
                virtual ssGUI::Tab* GetTabFromContent(ssGUI::GUIObject* registeredContent) const;

                //function: GetTabContentsHolder
                virtual ssGUI::GUIObject* GetTabContentsHolder() const;

                //function: MoveContentBefore
                virtual void MoveContentBefore(ssGUI::GUIObject* contentToMove,
                                               ssGUI::GUIObject* contentBefore);

                //function: MoveContentFront
                virtual void MoveContentFront(ssGUI::GUIObject* contentToMove);

                //function: MoveContentEnd
                virtual void MoveContentEnd(ssGUI::GUIObject* contentToMove);

                //function: MoveContentAfter
                virtual void MoveContentAfter(ssGUI::GUIObject* contentToMove,
                                              ssGUI::GUIObject* contentAfter);

                //function: SwitchContent
                virtual void SwitchContent(ssGUI::GUIObject* registeredContent);

                //function: GetCurrentTab
                virtual ssGUI::Tab* GetCurrentTab() const;

                //function: GetCurrentTabContent
                virtual ssGUI::GUIObject* GetCurrentTabContent() const;

                //function: SetTabBarHorizontal
                virtual void SetTabBarHorizontal(bool horizontal);

                //function: IsTabBarHorizontal
                virtual bool IsTabBarHorizontal() const;

                //function: SetDefaultTabBarObject
                static void SetDefaultTabBarObject(ssGUI::GUIObject* defaultTabBar);

                //function: GetDefaultTabBarObject
                static ssGUI::GUIObject* GetDefaultTabBarObject();

                //function: SetDefaultTabBarColor
                static void SetDefaultTabBarColor(glm::u8vec4 color);

                //function: GetDefaultTabBarColor
                static glm::u8vec4 GetDefaultTabBarColor();

                //function: SetDefaultTabObject
                static void SetDefaultTabObject(ssGUI::Tab* defaultTab);

                //function: GetDefaultTabObject
                static ssGUI::Tab* GetDefaultTabObject();

                //function: SetDefaultTabColor
                static void SetDefaultTabColor(glm::u8vec4 color);

                //function: GetDefaultTabColor
                static glm::u8vec4 GetDefaultTabColor();

                //function: SetDefaultPreviewColor
                static void SetDefaultPreviewColor(glm::u8vec4 color);

                //function: GetDefaultPreviewColor
                static glm::u8vec4 GetDefaultPreviewColor();

                //====================================================================
                //Group: Overrides
                //====================================================================

                //Override from Extension
                //function: SetEnabled
                //See <Extension::SetEnabled>
                virtual void SetEnabled(bool enabled) override;

                //function: IsEnabled
                //See <Extension::IsEnabled>
                virtual bool IsEnabled() const override;

                //function: Internal_Update
                //See <Extension::Internal_Update>
                virtual void Internal_Update(bool isPreUpdate,
                                             ssGUI::Backend::BackendSystemInputInterface*
                                                 inputInterface,
                                             ssGUI::InputStatus& currentInputStatus,
                                             ssGUI::InputStatus& lastInputStatus,
                                             ssGUI::GUIObject* mainWindow) override;

                //function: Internal_Draw
                //See <Extension::Internal_Draw>
                virtual void Internal_Draw(bool isPreRender,
                                           ssGUI::Backend::BackendDrawingInterface*
                                               drawingInterface,
                                           ssGUI::GUIObject* mainWindow,
                                           glm::vec2 mainWindowPositionOffset) override;

                //function: GetExtensionName
                //See <Extension::GetExtensionName>
                virtual std::string GetExtensionName() const override;

                //function: BindToObject
                //See <Extension::BindToObject>
                virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

                //function: Copy
                //See <Extension::Copy>
                virtual void Copy(const ssGUI::Extensions::Extension* extension) override;

                //function: Internal_GetObjectsReferences
                //See <Extension::Internal_GetObjectsReferences>
                virtual ObjectsReferences* Internal_GetObjectsReferences() override;

                //function: Clone
                //See <Extension::Clone>
                virtual TabArea* Clone() override;
        };
    }

}

#endif