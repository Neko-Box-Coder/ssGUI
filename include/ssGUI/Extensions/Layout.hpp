#ifndef SSGUI_LAYOUT
#define SSGUI_LAYOUT

#include <unordered_map>
#include <unordered_set>
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"
#include "ssGUI/ssGUITags.hpp"

//namespace: ssGUI::Extensions
namespace ssGUI::Extensions
{
    /*class: Layout
    This extensions allows the children of this GUI object to be positioned and sized in a row or column fashion.
    
    The size and resize type (window) will be recorded when a child is being added, 
    and will be set back to it when it is removed from this GUI object.
    
    Variables & Constructor:
    ============================== C++ ==============================
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
        bool Overflow;

        int OnChildAddEventIndex;
        int ChildAddedEventIndex;
        int ChildRemovedEventIndex;
        int ChildPositionChangedEventIndex;

        ObjectsReferences CurrentObjectsReferences;

        std::unordered_map<ssGUIObjectIndex, glm::vec2> LastUpdateChildrenSize;
        std::unordered_set<ssGUIObjectIndex> ObjectsToExclude;
        std::unordered_set<ssGUIObjectIndex> SpecialObjectsToExclude;  //NOTE: subset of ObjectsToExclude that indicates for special objects that are not excluded by the user, which is matain by the extension itself.
        std::unordered_map<ssGUIObjectIndex, glm::vec2> OriginalChildrenSize;
        std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType> OriginalChildrenResizeType;
        std::unordered_map<ssGUIObjectIndex, int> MinMaxSizeChangedEventIndices;
    =================================================================
    ============================== C++ ==============================
    Layout::Layout() : HorizontalLayout(false), PreferredSizeMultipliers(), DisableChildrenResizing(false), 
                        OverrideChildrenResizeTypes(true), UpdateContainerMinMaxSize(true), ReverseOrder(false), CoverFullLength(true),
                        Container(nullptr), Enabled(true), Padding(0), Spacing(5), Overflow(false), OnChildAddEventIndex(-1), ChildAddedEventIndex(-1), 
                        ChildRemovedEventIndex(-1), ChildPositionChangedEventIndex(-1), CurrentObjectsReferences(), LastUpdateChildrenSize(), 
                        ObjectsToExclude(), SpecialObjectsToExclude(), OriginalChildrenSize(), OriginalChildrenResizeType(), MinMaxSizeChangedEventIndices()
    {}
    =================================================================
    */
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
            bool Overflow;

            int OnChildAddEventIndex;
            int ChildAddedEventIndex;
            int ChildRemovedEventIndex;
            int ChildPositionChangedEventIndex;

            ObjectsReferences CurrentObjectsReferences;

            std::unordered_map<ssGUIObjectIndex, glm::vec2> LastUpdateChildrenSize;
            std::unordered_set<ssGUIObjectIndex> ObjectsToExclude;
            std::unordered_set<ssGUIObjectIndex> SpecialObjectsToExclude;  //NOTE: subset of ObjectsToExclude that indicates for special objects that are not excluded by the user, which is matain by the extension itself.
            std::unordered_map<ssGUIObjectIndex, glm::vec2> OriginalChildrenSize;
            std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType> OriginalChildrenResizeType;
            std::unordered_map<ssGUIObjectIndex, int> MinMaxSizeChangedEventIndices;

            Layout(Layout const& other);

            void LayoutChildren(float startPos, float length, std::vector<float>& childrenPos, std::vector<float>& childrenLength, 
                                std::vector<float>& minChildrenLength, std::vector<float>& maxChildrenLength, int lastChildChangeIndex,
                                float sizeDiff);

            void UpdateChildrenResizeTypes();

            void SyncContainerMinMaxSize();

            void UpdateSpeicalObjectsToExclude();

            void UpdateExcludedObjects();

            void DisableChildrenResizingInUpdate();

            void GetAndValidateChildrenDetails(std::vector<float>& childrenPos, std::vector<float>& childrenSize, std::vector<float>& childrenMinSize,
                                    std::vector<float>& childrenMaxSize, glm::vec2 containerPos, glm::vec2 containerSize);
            
            void GetLastDifferentChild(std::vector<float>& childrenPos, std::vector<float>& childrenSize, float& sizeDiff, int& lastChildChangeIndex);

            void AssignPositionsAndSizesToChildren(std::vector<float>& childrenPos, std::vector<float>& childrenSize);

            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;


        public:
            static const std::string EXTENSION_NAME;

            Layout();
            virtual ~Layout() override;

            //function: IsHorizontalLayout
            virtual bool IsHorizontalLayout() const;
            
            //function: SetHorizontalLayout
            virtual void SetHorizontalLayout(bool horizontal);

            //function: AddPreferredSizeMultiplier
            //Adds a new entry for representing the size of the child to be the size of sizeMultiplier.
            //0 = 0%, 1 = 100%
            virtual void AddPreferredSizeMultiplier(float sizeMultiplier);
            
            //function: SetPreferredSizeMultiplier
            //Sets the child with the index to be the size of sizeMultiplier.
            //0 = 0%, 1 = 100%
            virtual void SetPreferredSizeMultiplier(int index, float sizeMultiplier);
            
            //function: GetPreferredSizeMultiplier
            //Returns the sizeMultiplier of the child with the index.
            //0 = 0%, 1 = 100%
            virtual float GetPreferredSizeMultiplier(int index) const;
            
            //function: GerPreferredSizeMultiplierCount
            //Returns the number of entries for PreferredSizeMultiplier. 
            //This will always be updated to the same number of children *after* each update.
            virtual int GerPreferredSizeMultiplierCount() const;

            //function: IsChildrenResizingDisabled
            //If true, children windows can be resized
            virtual bool IsChildrenResizingDisabled() const;
            
            //function: SetDisableChildrenResizing
            //If true, children windows can be resized
            virtual void SetDisableChildrenResizing(bool disableResizing);

            //function: IsReverseOrder
            //If false, the children are positioned left to right (horizontal) or top to bottom (vertical)
            virtual bool IsReverseOrder() const;
            
            //function: SetReverseOrder
            //If false, the children are positioned left to right (horizontal) or top to bottom (vertical)
            virtual void SetReverseOrder(bool reverseOrder);
            
            /*function: IsCoverFullLength
            If true, the children will be sized to cover the whole length of the container.
            
            If false, the children will just be sized according to their size or size multiplier.
            However, the children won't be allowed to overflow the length of the container. 
            (this is due to be changed by adding an option for allowing overflow or not)
            */
            virtual bool IsCoverFullLength() const;
            
            /*function: SetCoverFullLength
            If true, the children will be sized to cover the whole length of the container.
            
            If false, the children will just be sized according to their size or size multiplier.
            However, the children won't be allowed to overflow the length of the container. 
            (this is due to be changed by adding an option for allowing overflow or not)
            */
            virtual void SetCoverFullLength(bool fullLength);

            //function: GetOverrideChildrenResizeType
            //If true, if will override the window children's resize type. This should normally be true.
            virtual bool GetOverrideChildrenResizeType() const;
            
            //function: SetOverrideChildrenResizeType
            //If true, if will override the window children's resize type. This should normally be true.
            virtual void SetOverrideChildrenResizeType(bool override);

            //function: GetUpdateContainerMinMaxSize
            //If true, this will update the container's min max size to match the children's total min max size. This should normally be true.
            virtual bool GetUpdateContainerMinMaxSize() const;
            
            //function: SetUpdateContainerMinMaxSize
            //If true, this will update the container's min max size to match the children's total min max size. This should normally be true.
            virtual void SetUpdateContainerMinMaxSize(bool update);

            //function: GetPadding
            //Padding for layouting the children GUI objects. 
            //Padding will be on all 4 sides (widget) or only on left, right and bottom sides (window). 
            virtual int GetPadding() const;
            
            //function: SetPadding
            //Padding for layouting the children GUI objects. 
            //Padding will be on all 4 sides (widget) or only on left, right and bottom sides (window). 
            virtual void SetPadding(int padding);
            
            //function: GetSpacing
            //Spacing between each child
            virtual int GetSpacing() const;
            
            //function: SetSpacing
            //Spacing between each child
            virtual void SetSpacing(int spacing);

            //function: SetOverflow
            //Sets if allows the children to overflow the container. 
            //If true, the layout extension will not try to cover the full length of the container even if it is set so.
            virtual void SetOverflow(bool overflow);

            //function: SetOverflow
            //Gets if allows the children to overflow the container. 
            //If true, the layout extension will not try to cover the full length of the container even if it is set so.
            virtual bool GetOverflow() const;

            //function: ExcludeObject
            //If a GUI Object is excluded, it will be ignored
            virtual void ExcludeObject(ssGUI::GUIObject* obj);

            //function: UnexcludeObject
            //If a GUI Object is excluded, it will be ignored
            virtual void UnexcludeObject(ssGUI::GUIObject* obj);

            //function: Internal_OnRecursiveChildAdded
            //(Internal ssGUI function) Listener function when a child is being added
            virtual void Internal_OnRecursiveChildAdded(ssGUI::GUIObject* child);
            
            //function: Internal_OnRecursiveChildRemoved
            //(Internal ssGUI function) Listener function when a child is being removed
            virtual void Internal_OnRecursiveChildRemoved(ssGUI::GUIObject* child);
            
            //function: Internal_OnChildMinMaxSizeChanged
            //(Internal ssGUI function) Listener function when a child's min max size is changed
            virtual void Internal_OnChildMinMaxSizeChanged(ssGUI::GUIObject* child);

            //Override from Extension
            //function: SetEnabled
            //See <Extension::SetEnabled>
            virtual void SetEnabled(bool enabled) override;
            
            //function: IsEnabled
            //See <Extension::IsEnabled>
            virtual bool IsEnabled() const override;

            //function: Internal_Update
            //See <Extension::Internal_Update>
            virtual void Internal_Update(bool IsPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool IsPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: GetExtensionName
            //See <Extension::GetExtensionName>
            virtual std::string GetExtensionName() override;
            
            //function: BindToObject
            //See <Extension::BindToObject>
            virtual void BindToObject(ssGUI::GUIObject* bindObj) override;

            //function: Copy
            //See <Extension::Copy>
            virtual void Copy(ssGUI::Extensions::Extension* extension) override;

            //function: Internal_GetObjectsReferences
            //See <Extension::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Clone
            //See <Extension::Clone>
            virtual Extension* Clone(ssGUI::GUIObject* newContainer) override;
    };
}




#endif