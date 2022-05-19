#ifndef SSGUI_BASE_GUI_OBJECT
#define SSGUI_BASE_GUI_OBJECT

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/EventCallbacks/OnRecursiveChildAddEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/OnRecursiveChildRemoveEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/OnObjectDestroyEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildPositionChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/SizeChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/ChildRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/BackgroundColorChangedEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusedEventCallback.hpp"
#include "ssGUI/EventCallbacks/FocusLostEventCallback.hpp"
#include "glm/vec4.hpp"
#include <vector>
#include <list>
#include <limits>
#include <unordered_set>
#include <unordered_map>

//TODO : Maybe replace GUIObject one day with this instead

//namespace: ssGUI
namespace ssGUI
{
    /*class: ssGUI::BaseGUIObject
    This is the implementation class for <ssGUI::GUIObject>. See <ssGUI::GUIObject> for more details about the functions
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        ssGUIObjectIndex Parent;
        std::list<ssGUIObjectIndex> Children;
        std::list<ssGUIObjectIndex>::iterator CurrentChild;
        bool CurrentChildIteratorFrontEnd;
        bool CurrentChildIteratorBackEnd;
        bool Visible;
        glm::u8vec4 BackgroundColour;
        bool UserCreated;
        bool ObjectDelete;
        bool HeapAllocated;
        ObjectsReferences CurrentObjectsReferences;
        bool DestroyEventCalled;
        bool Redraw;
        bool AcceptRedrawRequest;
        std::vector<std::tuple<bool, bool, std::list<ssGUIObjectIndex>::iterator>> StashedChildIterators;
        bool Focused;

        //Widget transform
        glm::vec2 Position;
        glm::vec2 GlobalPosition;
        glm::vec2 Size;
        glm::vec2 MinSize;
        glm::vec2 MaxSize;
        ssGUI::Enums::AnchorType Anchor;

        //Rendering
        std::vector<glm::vec2> DrawingVerticies;
        std::vector<glm::vec2> DrawingUVs;
        std::vector<glm::u8vec4> DrawingColours;
        std::vector<int> DrawingCounts;
        std::vector<ssGUI::DrawingProperty> DrawingProperties;

        //Cache rendering
        std::vector<glm::vec2> LastDrawingVerticies;
        std::vector<glm::vec2> LastDrawingUVs;
        std::vector<glm::u8vec4> LastDrawingColours;
        std::vector<int> LastDrawingCounts;
        std::vector<ssGUI::DrawingProperty> LastDrawingProperties;
        glm::vec2 LastGlobalPosition;

        std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;
        std::vector<std::string> ExtensionsDrawOrder;
        std::vector<std::string> ExtensionsUpdateOrder;
        std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*> EventCallbacks;

        std::unordered_set<std::string> CurrentTags;
    =================================================================
    ============================== C++ ==============================
    BaseGUIObject::BaseGUIObject() : Parent(-1), Children(), CurrentChild(Children.end()), CurrentChildIteratorFrontEnd(true), Visible(true),
                                        CurrentChildIteratorBackEnd(true), BackgroundColour(glm::u8vec4(255, 255, 255, 255)), UserCreated(true), 
                                        ObjectDelete(false), HeapAllocated(false), CurrentObjectsReferences(), DestroyEventCalled(false), Redraw(true), 
                                        AcceptRedrawRequest(true), StashedChildIterators(), Focused(false), Position(glm::vec2(0, 0)), 
                                        GlobalPosition(glm::vec2(0, 0)), Size(glm::vec2(150, 150)), MinSize(glm::vec2(25, 25)), 
                                        MaxSize(glm::vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max())),
                                        Anchor(ssGUI::Enums::AnchorType::TOP_LEFT), DrawingVerticies(), DrawingUVs(), DrawingColours(), 
                                        DrawingCounts(), DrawingProperties(), GUIObjectShapeIndex(-1), GUIObjectVertexIndex(-1), LastDrawingVerticies(), 
                                        LastDrawingUVs(), LastDrawingColours(), LastDrawingCounts(), LastDrawingProperties(), LastGlobalPosition(), 
                                        Extensions(), ExtensionsDrawOrder(), ExtensionsUpdateOrder(), EventCallbacks(), CurrentTags()
    {}
    =================================================================
    */
    class BaseGUIObject : public GUIObject
    {
        private:
            BaseGUIObject& operator=(BaseGUIObject const& other);

        protected:
            ssGUIObjectIndex Parent;
            std::list<ssGUIObjectIndex> Children;
            std::list<ssGUIObjectIndex>::iterator CurrentChild;
            bool CurrentChildIteratorFrontEnd;
            bool CurrentChildIteratorBackEnd;
            bool Visible;
            glm::u8vec4 BackgroundColour;
            bool UserCreated;
            bool ObjectDelete;
            bool HeapAllocated;
            ObjectsReferences CurrentObjectsReferences;
            bool DestroyEventCalled;
            bool Redraw;
            bool AcceptRedrawRequest;
            std::vector<std::tuple<bool, bool, std::list<ssGUIObjectIndex>::iterator>> StashedChildIterators;
            bool Focused;

            //Widget transform
            glm::vec2 Position;
            glm::vec2 GlobalPosition;
            glm::vec2 Size;
            glm::vec2 MinSize;
            glm::vec2 MaxSize;
            ssGUI::Enums::AnchorType Anchor;

            //Rendering
            std::vector<glm::vec2> DrawingVerticies;
            std::vector<glm::vec2> DrawingUVs;
            std::vector<glm::u8vec4> DrawingColours;
            std::vector<int> DrawingCounts;
            std::vector<ssGUI::DrawingProperty> DrawingProperties;
            int GUIObjectShapeIndex;
            int GUIObjectVertexIndex;

            //Cache rendering
            std::vector<glm::vec2> LastDrawingVerticies;
            std::vector<glm::vec2> LastDrawingUVs;
            std::vector<glm::u8vec4> LastDrawingColours;
            std::vector<int> LastDrawingCounts;
            std::vector<ssGUI::DrawingProperty> LastDrawingProperties;
            glm::vec2 LastGlobalPosition;

            std::unordered_map<std::string, ssGUI::Extensions::Extension*> Extensions;
            std::vector<std::string> ExtensionsDrawOrder;
            std::vector<std::string> ExtensionsUpdateOrder;
            std::unordered_map<std::string, ssGUI::EventCallbacks::EventCallback*> EventCallbacks;

            std::unordered_set<std::string> CurrentTags;

            BaseGUIObject(BaseGUIObject const& other);

            virtual void SyncPosition() override;
            virtual void SyncGlobalPosition() override;
            virtual void ConstructRenderInfo() override;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            //virtual void DrawBorder(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::vec2 mainWindowPositionOffset);
            virtual void NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();
            virtual ssGUI::GUIObject* CloneChildren(ssGUI::GUIObject* originalRoot, ssGUI::GUIObject* clonedRoot);
            virtual void CloneExtensionsAndEventCallbacks(ssGUI::GUIObject* clonedObj);
            virtual void CacheRendering();
            virtual void DisableRedrawObjectRequest();
            virtual void EnableRedrawObjectRequest();
            virtual void UpdateGUIObjectVertexAndShapeIndex();

        public:
            BaseGUIObject();
            virtual ~BaseGUIObject() override;

            //Below are from GUIObject.hpp
            //function: GetPosition
            //See <GUIObject::GetPosition>
            virtual glm::vec2 GetPosition() const override;
            
            //function: SetPosition
            //See <GUIObject::SetPosition>
            virtual void SetPosition(glm::vec2 position) override;
            
            //function: GetGlobalPosition
            //See <GUIObject::GetGlobalPosition>
            virtual glm::vec2 GetGlobalPosition() override;    //TODO : SyncGlobalPosition is called so can't be const. Try to turn this to const
            
            //function: SetGlobalPosition
            //See <GUIObject::SetGlobalPosition>
            virtual void SetGlobalPosition(glm::vec2 position) override;

            //function: GetSize
            //See <GUIObject::GetSize>
            virtual glm::vec2 GetSize() const override;
            
            //function: SetSize
            //See <GUIObject::SetSize>
            virtual void SetSize(glm::vec2 size) override;

            //function: GetMinSize
            //See <GUIObject::GetMinSize>
            virtual glm::vec2 GetMinSize() const override;
            
            //function: SetMinSize
            //See <GUIObject::SetMinSize>
            virtual void SetMinSize(glm::vec2 minSize) override;

            //function: GetMaxSize
            //See <GUIObject::GetMaxSize>
            virtual glm::vec2 GetMaxSize() const override;
            
            //function: SetMaxSize
            //See <GUIObject::SetMaxSize>
            virtual void SetMaxSize(glm::vec2 maxSize) override;

            //function: GetParent
            //See <GUIObject::GetParent>
            virtual ssGUI::GUIObject* GetParent() const override;
            
            //function: SetParent
            //See <GUIObject::SetParent>
            virtual void SetParent(ssGUI::GUIObject* newParent) override;

            //function: GetChildrenCount
            //See <GUIObject::GetChildrenCount>
            virtual int GetChildrenCount() const override;
            
            //function: MoveChildrenIteratorToFirst
            //See <GUIObject::MoveChildrenIteratorToFirst>
            virtual void MoveChildrenIteratorToFirst() override;
            
            //function: MoveChildrenIteratorToLast
            //See <GUIObject::MoveChildrenIteratorToLast>
            virtual void MoveChildrenIteratorToLast() override;
            
            //function: MoveChildrenIteratorNext
            //See <GUIObject::MoveChildrenIteratorNext>
            virtual void MoveChildrenIteratorNext() override;
            
            //function: MoveChildrenIteratorPrevious
            //See <GUIObject::MoveChildrenIteratorPrevious>
            virtual void MoveChildrenIteratorPrevious() override;
            
            //function: IsChildrenIteratorLast
            //See <GUIObject::IsChildrenIteratorLast>
            virtual bool IsChildrenIteratorLast() override;
            
            //function: IsChildrenIteratorFirst
            //See <GUIObject::IsChildrenIteratorFirst>
            virtual bool IsChildrenIteratorFirst() override;
            
            //function: IsChildrenIteratorEnd
            //See <GUIObject::IsChildrenIteratorEnd>
            virtual bool IsChildrenIteratorEnd() override;

            //function: StashChildrenIterator
            //See <GUIObject::StashChildrenIterator>
            virtual void StashChildrenIterator() override;

            //function: StashChildrenIterator
            //See <GUIObject::StashChildrenIterator>
            virtual void PopChildrenIterator() override;

            //function: FindChild
            //See <GUIObject::FindChild>
            virtual bool FindChild(ssGUI::GUIObject* child) override;

            //function: GetCurrentChild
            //See <GUIObject::GetCurrentChild>
            virtual ssGUI::GUIObject* GetCurrentChild() override;

            //function: GetCurrentChildReferenceIterator
            //See <GUIObject::GetCurrentChildReferenceIterator>
            virtual std::list<ssGUIObjectIndex>::iterator GetCurrentChildReferenceIterator() override;
            
            //function: ChangeChildOrderToBeforePosition
            //See <GUIObject::ChangeChildOrderToBeforePosition>
            virtual void ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) override;
            
            //function: ChangeChildOrderToAfterPosition
            //See <GUIObject::ChangeChildOrderToAfterPosition>
            virtual void ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) override;

            //function: GetListOfChildren
            //See <GUIObject::GetListOfChildren>
            virtual std::vector<ssGUI::GUIObject*> GetListOfChildren() const override;

            //function: Internal_AddChild
            //See <GUIObject::Internal_AddChild>
            virtual void Internal_AddChild(ssGUI::GUIObject* obj) override;
            
            //function: Internal_RemoveChild
            //See <GUIObject::Internal_RemoveChild>
            virtual void Internal_RemoveChild(ssGUI::GUIObject* obj) override;

            //function: GetType
            //See <GUIObject::GetType>
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: GetAnchorType
            //See <GUIObject::GetAnchorType>
            virtual ssGUI::Enums::AnchorType GetAnchorType() const override;
            
            //function: SetAnchorType
            //See <GUIObject::SetAnchorType>
            virtual void SetAnchorType(ssGUI::Enums::AnchorType anchorType) override;

            //function: SetVisible
            //See <GUIObject::SetVisible>
            virtual void SetVisible(bool visible) override;
            
            //function: IsVisible
            //See <GUIObject::IsVisible>
            virtual bool IsVisible() const override;

            //function: SetUserCreated
            //See <GUIObject::SetUserCreated>
            virtual void SetUserCreated(bool created) override;

            //function: IsUserCreated()
            //See <GUIObject::IsUserCreated>
            virtual bool IsUserCreated() const override;
            
            //function: SetBackgroundColor
            //See <GUIObject::SetBackgroundColor>
            virtual void SetBackgroundColor(glm::u8vec4 color) override;
            
            //function: GetBackgroundColor
            //See <GUIObject::GetBackgroundColor>
            virtual glm::u8vec4 GetBackgroundColor() const override;

            //function: IsFocused
            //See <GUIObject::IsFocused>
            virtual bool IsFocused() const override;
            
            //function: SetFocus
            //See <GUIObject::SetFocus>
            virtual void SetFocus(bool focus) override;

            //function: Delete 
            //See <GUIObject::Delete>
            virtual void Delete() override;

            //function: Internal_IsDeleted
            //See <GUIObject::Internal_IsDeleted>
            virtual bool Internal_IsDeleted() const override;

            //function: SetHeapAllocated
            //See <GUIObject::SetHeapAllocated>
            virtual void SetHeapAllocated(bool heap) override;

            //function: IsHeapAllocated
            //See <GUIObject::IsHeapAllocated>
            virtual bool IsHeapAllocated() const override;
            
            //function: Extension_GetDrawingVertices
            //See <GUIObject::Extension_GetDrawingVertices>
            virtual std::vector<glm::vec2>& Extension_GetDrawingVertices() override;
            
            //function: Extension_GetDrawingUVs
            //See <GUIObject::Extension_GetDrawingUVs>
            virtual std::vector<glm::vec2>& Extension_GetDrawingUVs() override;
            
            //function: Extension_GetDrawingColours
            //See <GUIObject::Extension_GetDrawingColours>
            virtual std::vector<glm::u8vec4>& Extension_GetDrawingColours() override;
            
            //function: Extension_GetDrawingCounts
            //See <GUIObject::Extension_GetDrawingCounts>
            virtual std::vector<int>& Extension_GetDrawingCounts() override;
            
            //function: Extension_GetDrawingProperties
            //See <GUIObject::Extension_GetDrawingProperties>
            virtual std::vector<ssGUI::DrawingProperty>& Extension_GetDrawingProperties() override;

            //function: Extension_GetGUIObjectFirstShapeIndex
            //See <GUIObject::Extension_GetGUIObjectFirstShapeIndex>
            virtual int Extension_GetGUIObjectFirstShapeIndex() const override;

            //function: Extension_GetGUIObjectFirstVertexIndex
            //See <GUIObject::Extension_GetGUIObjectFirstVertexIndex>
            virtual int Extension_GetGUIObjectFirstVertexIndex() const override;
            
            //function: AddExtension
            //See <GUIObject::AddExtension>
            virtual void AddExtension(ssGUI::Extensions::Extension* extension) override;
            
            //function: GetExtension
            //See <GUIObject::GetExtension>
            virtual ssGUI::Extensions::Extension* GetExtension(std::string extensionName) override;
            
            //function: GetListOfExtensions
            //See <GUIObject::GetListOfExtensions>
            virtual std::vector<ssGUI::Extensions::Extension*> GetListOfExtensions() override;

            //function: IsExtensionExist
            //See <GUIObject::IsExtensionExist>
            virtual bool IsExtensionExist(std::string extensionName) const override;
            
            //function: RemoveExtension
            //See <GUIObject::RemoveExtension>
            virtual void RemoveExtension(std::string extensionName) override;
            
            //function: GetExtensionsCount
            //See <GUIObject::GetExtensionsCount>
            virtual int GetExtensionsCount() const override;
            
            //function: GetExtensionDrawOrder
            //See <GUIObject::GetExtensionDrawOrder>
            virtual int GetExtensionDrawOrder(std::string extensionName) const override;
            
            //function: ChangeExtensionDrawOrder
            //See <GUIObject::ChangeExtensionDrawOrder>
            virtual void ChangeExtensionDrawOrder(std::string extensionName, int order) override;
            
            //function: GetExtensionUpdateOrder
            //See <GUIObject::GetExtensionUpdateOrder>
            virtual int GetExtensionUpdateOrder(std::string extensionName) const override;
            
            //function: ChangeExtensionUpdateOrder
            //See <GUIObject::ChangeExtensionUpdateOrder>
            virtual void ChangeExtensionUpdateOrder(std::string extensionName, int order) override;
            
            //function: AddEventCallback
            //See <GUIObject::AddEventCallback>
            virtual void AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback) override;
            
            //function: GetEventCallback
            //See <GUIObject::GetEventCallback>
            virtual ssGUI::EventCallbacks::EventCallback* GetEventCallback(std::string eventCallbackName) override;
            
            //function: IsEventCallbackExist
            //See <GUIObject::IsEventCallbackExist>
            virtual bool IsEventCallbackExist(std::string eventCallbackName) const override;
            
            //function: RemoveEventCallback
            //See <GUIObject::RemoveEventCallback>
            virtual void RemoveEventCallback(std::string eventCallbackName) override;

            //function: GetListOfEventCallbacks
            //See <GUIObject::RemoveEventCallback>
            virtual std::vector<ssGUI::EventCallbacks::EventCallback*> GetListOfEventCallbacks() override;
            
            //function: AddTag
            //See <GUIObject::AddTag>
            virtual void AddTag(std::string tag) override;
            
            //function: RemoveTag
            //See <GUIObject::RemoveTag>
            virtual void RemoveTag(std::string tag) override;
            
            //function: HasTag
            //See <GUIObject::HasTag>
            virtual bool HasTag(std::string tag) const override;

            //function: RedrawObject
            //See <GUIObject::RedrawObject>
            virtual void RedrawObject() override;

            //function: IsRedrawNeeded
            //See <GUIObject::IsRedrawNeeded>
            virtual bool IsRedrawNeeded() const override;
            
            //function: Internal_GetObjectsReferences
            //See <GUIObject::Internal_GetObjectsReferences>
            virtual ObjectsReferences* Internal_GetObjectsReferences() override;

            //function: Internal_Draw
            //See <GUIObject::Internal_Draw>
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            //See <GUIObject::Internal_Update>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;

            //function: Clone
            //See <GUIObject::Clone>
            virtual BaseGUIObject* Clone(bool cloneChildren) override;

    };
}

#endif