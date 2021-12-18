#ifndef SSGUI_LAYOUT
#define SSGUI_LAYOUT

#include <unordered_map>
#include <unordered_set>
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/BaseClasses/Window.hpp"
#include "ssGUI/BaseClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"
#include "ssGUI/ssGUITags.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    //class: Layout
    class Layout : public Extension
    {
        private:
            Layout& operator=(Layout const& other);
        
        protected:
            bool HorizontalLayout;
            std::vector<float> PreferredSizeMultipliers;
            bool DisableChildrenResizing;
            bool OverrideChildrenResizeTypes;

            bool UpdateContainerMinMaxSize;
            bool ReverseOrder;
            bool CoverFullLength; 
            ssGUI::GUIObject* Container;
            bool Enabled;
            int Padding;
            int Spacing;

            int OnChildAddEventIndex;
            int ChildAddedEventIndex;
            int ChildRemovedEventIndex;
            int ChildPositionChangedEventIndex;

            ObjectsReferences CurrentObjectsReferences;

            std::unordered_map<ssGUIObjectIndex, glm::ivec2> LastUpdateChildrenSize;
            std::unordered_set<ssGUIObjectIndex> ObjectsToExclude;
            std::unordered_set<ssGUIObjectIndex> SpecialObjectsToExclude;  //note: subset of ObjectsToExclude that indicates for special objects that are not excluded by the user, which is matain by the extension itself.
            std::unordered_map<ssGUIObjectIndex, glm::ivec2> OriginalChildrenSize;
            std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType> OriginalChildrenResizeType;
            std::unordered_map<ssGUIObjectIndex, int> MinMaxSizeChangedEventIndices;


            void LayoutChildren(int startPos, int length, std::vector<int>& childrenPos, std::vector<int>& childrenLength, 
                                std::vector<int>& minChildrenLength, std::vector<int>& maxChildrenLength, int lastChildChangeIndex,
                                int sizeDiff);

            void UpdateChildrenResizeTypes();

            void SyncContainerMinMaxSize();

            Layout(Layout const& other);

        public:
            static const std::string EXTENSION_NAME;

            Layout();
            virtual ~Layout() override;

            //function: IsHorizontalLayout
            virtual bool IsHorizontalLayout() const;
            
            //function: SetHorizontalLayout
            virtual void SetHorizontalLayout(bool horizontal);

            //function: AddPreferredSizeMultiplier
            virtual void AddPreferredSizeMultiplier(float sizeMultiplier);
            
            //function: SetPreferredSizeMultiplier
            virtual void SetPreferredSizeMultiplier(int index, float sizeMultiplier);
            
            //function: GetPreferredSizeMultiplier
            virtual float GetPreferredSizeMultiplier(int index) const;
            
            //function: GerPreferredSizeMultiplierCount
            virtual int GerPreferredSizeMultiplierCount() const;

            //function: IsChildrenResizingDisabled
            virtual bool IsChildrenResizingDisabled() const;
            
            //function: SetDisableChildrenResizing
            virtual void SetDisableChildrenResizing(bool disableResizing);

            //function: IsReverseOrder
            virtual bool IsReverseOrder() const;
            
            //function: SetReverseOrder
            virtual void SetReverseOrder(bool reverseOrder);
            
            //function: IsCoverFullLength
            virtual bool IsCoverFullLength() const;
            
            //function: SetCoverFullLength
            virtual void SetCoverFullLength(bool fullLength);

            //function: GetOverrideChildrenResizeType
            virtual bool GetOverrideChildrenResizeType() const;
            
            //function: SetOverrideChildrenResizeType
            virtual void SetOverrideChildrenResizeType(bool override);

            //function: GetUpdateContainerMinMaxSize
            virtual bool GetUpdateContainerMinMaxSize() const;
            
            //function: SetUpdateContainerMinMaxSize
            virtual void SetUpdateContainerMinMaxSize(bool update);

            //function: GetPadding
            virtual int GetPadding() const;
            
            //function: SetPadding
            virtual void SetPadding(int padding);
            
            //function: GetSpacing
            virtual int GetSpacing() const;
            
            //function: SetSpacing
            virtual void SetSpacing(int spacing);

            virtual void ExcludeObject(ssGUI::GUIObject* obj);

            virtual void UnexcludeObject(ssGUI::GUIObject* obj);

            //function: Internal_OnRecursiveChildAdded
            virtual void Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child);
            
            //function: Internal_OnRecursiveChildRemoved
            virtual void Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child);
            
            //function: Internal_OnChildMinMaxSizeChanged
            virtual void Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child);

            //Override from Extension
            virtual void SetEnabled(bool enabled) override;

            virtual bool IsEnabled() const override;

            //function: Update
            virtual void Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}




#endif