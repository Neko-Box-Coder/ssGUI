#ifndef SSGUI_LAYOUT_H
#define SSGUI_LAYOUT_H

#include <unordered_map>
#include <unordered_set>
#include "ssGUI/Enums/AlignmentHorizontal.hpp"
#include "ssGUI/Enums/AlignmentVertical.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/GUIObjectClasses/Window.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"  //This is needed as Extension is only forward declaring ssGUI::GUIObject


namespace ssGUI 
{ 

//namespace: ssGUI::Extensions
namespace Extensions
{
    /*class: ssGUI::Extensions::Layout
    This extensions allows the children of this GUI object to be positioned and sized in a row or column fashion.
    
    The size and resize type (window) will be recorded when a child is being added, 
    and will be set back to it when it is removed from this GUI object.
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        bool HorizontalLayout;                                                                          //See <IsHorizontalLayout>
        std::vector<float> PreferredSizeMultipliers;                                                    //See <GetPreferredSizeMultiplier>
        bool DisableChildrenResizing;                                                                   //See <IsChildrenResizingDisabled>
        bool OverrideChildrenResizeTypesAndOnTop;                                                       //See <IsOverrideChildrenResizeTypeAndOnTop>

        bool UpdateContainerMinMaxSize;                                                                 //See <GetUpdateContainerMinMaxSize>
        bool ReverseOrder;                                                                              //See <IsReverseOrder>
        bool CoverFullLength;                                                                           //See <IsCoverFullLength>
        ssGUI::GUIObject* Container;                                                                    //See <BindToObject>
        bool Enabled;                                                                                   //See <IsEnabled>
        float Padding;                                                                                  //See <GetPadding>
        float Spacing;                                                                                  //See <GetSpacing>
        bool Overflow;                                                                                  //See <GetOverflow>

        ObjectsReferences CurrentObjectsReferences;                                                     //(Internal variable) Used to keep track of all the children and event callbacks

        std::unordered_map<ssGUIObjectIndex, glm::vec2> LastUpdateChildrenSize;                         //(Internal variable) Used for figuring out if the child has change it size
        std::unordered_set<ssGUIObjectIndex> ObjectsToExclude;                                          //See <ExcludeObject>
        std::unordered_set<ssGUIObjectIndex> SpecialObjectsToExclude;                                   //(Internal variable) Used to exclude special objects from being layout
                                                                                                        //NOTE: subset of ObjectsToExclude that indicates for special objects 
                                                                                                        //that are not excluded by the user, which is maintain by the extension itself.

        std::unordered_map<ssGUIObjectIndex, glm::vec2> OriginalChildrenSize;                           //(Internal variable) Used to restore the size of the child when its parent being set to other GUI object
        std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType> OriginalChildrenResizeType;      //(Internal variable) Used to restore the resize type of the child when its parent being set to other GUI object
        std::unordered_map<ssGUIObjectIndex, bool> OriginalChildrenOnTop;                               //(Internal variable) Used to restore if the child moves to top if focused when its parent being set to other GUI object
    =================================================================
    ============================== C++ ==============================
    Layout::Layout() :  HorizontalLayout(false),
                        PreferredSizeMultipliers(),
                        DisableChildrenResizing(false),
                        OverrideChildrenResizeTypesAndOnTop(true),
                        UpdateContainerMinMaxSize(true),
                        ReverseOrder(false),
                        CoverFullLength(true),
                        Container(nullptr),
                        Enabled(true),
                        Padding(0),
                        Spacing(5),
                        Overflow(false),
                        CurrentObjectsReferences(),
                        LastUpdateChildrenSize(),
                        ObjectsToExclude(),
                        SpecialObjectsToExclude(),
                        OriginalChildrenSize(),
                        OriginalChildrenResizeType(),
                        OriginalChildrenOnTop()
    {}
    =================================================================
    */
    class Layout : public Extension
    {
        public:
            friend class ssGUI::Factory;
        
        private:
            Layout& operator=(Layout const& other);
        
        protected:
            bool HorizontalLayout;                                                                          //See <IsHorizontalLayout>
            std::vector<float> PreferredSizeMultipliers;                                                    //See <GetPreferredSizeMultiplier>
            bool DisableChildrenResizing;                                                                   //See <IsChildrenResizingDisabled>
            bool OverrideChildrenResizeTypesAndOnTop;                                                       //See <IsOverrideChildrenResizeTypeAndOnTop>

            bool UpdateContainerMinMaxSize;                                                                 //See <GetUpdateContainerMinMaxSize>
            bool ReverseOrder;                                                                              //See <IsReverseOrder>
            bool CoverFullLength;                                                                           //See <IsCoverFullLength>
            ssGUI::GUIObject* Container;                                                                    //See <BindToObject>
            bool Enabled;                                                                                   //See <IsEnabled>
            float Padding;                                                                                  //See <GetPadding>
            float Spacing;                                                                                  //See <GetSpacing>
            bool Overflow;                                                                                  //See <GetOverflow>

            ObjectsReferences CurrentObjectsReferences;                                                     //(Internal variable) Used to keep track of all the children and event callbacks

            std::unordered_map<ssGUIObjectIndex, glm::vec2> LastUpdateChildrenSize;                         //(Internal variable) Used for figuring out if the child has change it size
            std::unordered_set<ssGUIObjectIndex> ObjectsToExclude;                                          //See <ExcludeObject>
            std::unordered_set<ssGUIObjectIndex> SpecialObjectsToExclude;                                   //(Internal variable) Used to exclude special objects from being layout
                                                                                                            //NOTE: subset of ObjectsToExclude that indicates for special objects 
                                                                                                            //that are not excluded by the user, which is maintain by the extension itself.

            std::unordered_map<ssGUIObjectIndex, glm::vec2> OriginalChildrenSize;                           //(Internal variable) Used to restore the size of the child when its parent being set to other GUI object
            std::unordered_map<ssGUIObjectIndex, ssGUI::Enums::ResizeType> OriginalChildrenResizeType;      //(Internal variable) Used to restore the resize type of the child when its parent being set to other GUI object
            std::unordered_map<ssGUIObjectIndex, bool> OriginalChildrenOnTop;                               //(Internal variable) Used to restore if the child moves to top if focused when its parent being set to other GUI object

            Layout(Layout const& other);
            Layout();
            virtual ~Layout() override;
            static void* operator new(size_t size)      {return ::operator new(size);};
            static void* operator new[](size_t size)    {return ::operator new(size);};
            static void operator delete(void* p)        {free(p);};
            static void operator delete[](void* p)      {free(p);};

            void LayoutChildren(float startPos, float length, std::vector<float>& childrenPos, std::vector<float>& childrenLength, 
                                std::vector<float>& minChildrenLength, std::vector<float>& maxChildrenLength, int lastChildChangeIndex,
                                float sizeDiff);

            void UpdateChildrenResizeTypesAndOnTop();

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

            //NOTE: A little bit retarded but works well enough to shut the compiler up when "floats... sizeMultipliers" is empty
            inline void AddPreferredSizeMultipliers(){}

        public:
            static const std::string EXTENSION_NAME;

            //function: IsHorizontalLayout
            virtual bool IsHorizontalLayout() const;
            
            //function: SetHorizontalLayout
            virtual void SetHorizontalLayout(bool horizontal);

            //function: AddPreferredSizeMultiplier
            //Adds a new entry for representing the size of the child to be the size of sizeMultiplier.
            //0 = 0%, 1 = 100%
            virtual void AddPreferredSizeMultiplier(float sizeMultiplier);
            
            //function: AddPreferredSizeMultiplier
            template<typename... floats>
            inline void AddPreferredSizeMultipliers(float sizeMultiplier, floats... sizeMultipliers)
            {
                AddPreferredSizeMultiplier(sizeMultiplier);
                AddPreferredSizeMultipliers(sizeMultipliers...);
            }

            //function: AddPreferredSizeMultiplier
            virtual void AddPreferredSizeMultipliers(float sizeMultipliers[], int count);

            //function: SetPreferredSizeMultiplier
            //Sets the child with the index to be the size of sizeMultiplier.
            //0 = 0%, 1 = 100%
            virtual void SetPreferredSizeMultiplier(int index, float sizeMultiplier);
            
            //function: GetPreferredSizeMultiplier
            //Returns the sizeMultiplier of the child with the index.
            //0 = 0%, 1 = 100%
            virtual float GetPreferredSizeMultiplier(int index) const;

            //function: RemovePreferredSizeMultiplier
            virtual void RemovePreferredSizeMultiplier(int index);

            //function: ClearAllPreferredSizeMultiplier
            virtual void ClearAllPreferredSizeMultiplier();
            
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

            //function: IsOverrideChildrenResizeTypeAndOnTop
            //If true, if will override the window children's resize type. This should normally be true.
            virtual bool IsOverrideChildrenResizeTypeAndOnTop() const;
            
            //function: SetOverrideChildrenResizeTypeAndOnTop
            //If true, if will override the window children's resize type. This should normally be true.
            virtual void SetOverrideChildrenResizeTypeAndOnTop(bool override);

            //function: GetUpdateContainerMinMaxSize
            //If true, this will update the container's min max size to match the children's total min max size. This should normally be true.
            virtual bool GetUpdateContainerMinMaxSize() const;
            
            //function: SetUpdateContainerMinMaxSize
            //If true, this will update the container's min max size to match the children's total min max size. This should normally be true.
            virtual void SetUpdateContainerMinMaxSize(bool update);

            //function: GetPadding
            //Padding for layouting the children GUI objects. 
            //Padding will be on all 4 sides (widget) or only on left, right and bottom sides (window). 
            virtual float GetPadding() const;
            
            //function: SetPadding
            //Padding for layouting the children GUI objects. 
            //Padding will be on all 4 sides (widget) or only on left, right and bottom sides (window). 
            virtual void SetPadding(float padding);
            
            //function: GetSpacing
            //Spacing between each child
            virtual float GetSpacing() const;
            
            //function: SetSpacing
            //Spacing between each child
            virtual void SetSpacing(float spacing);

            //function: SetOverflow
            //Sets if allows the children to overflow the container. 
            //If true, the layout extension will not try to cover the full length of the container even if it is set so.
            virtual void SetOverflow(bool overflow);

            //function: GetOverflow
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
            virtual void Internal_Update(bool isPreUpdate, ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Internal_Draw
            //See <Extension::Internal_Draw>
            virtual void Internal_Draw(bool isPreRender, ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
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
            virtual Layout* Clone() override;
    };
}

}



#endif