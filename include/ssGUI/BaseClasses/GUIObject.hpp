#ifndef SSGUI_OBJECT_H
#define SSGUI_OBJECT_H

#include <list>
#include <vector>
#include <string>
#include "glm/vec2.hpp"
#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/Enums/AnchorType.hpp"
#include "ssGUI/Backend/Interfaces/BackendSystemInputInterface.hpp"
#include "ssGUI/Backend/Interfaces/BackendDrawingInterface.hpp"
#include "ssGUI/Extensions/Extension.hpp"
#include "ssGUI/EventCallbacks/EventCallback.hpp"
#include "ssGUI/BaseClasses/InputStatus.hpp"
#include "ssGUI/DebugAndBuild/ssGUIBuildAndDebugConfig.hpp"

//namespace: ssGUI
namespace ssGUI
{
    //class: GUIObject
    //Base class for all GUI Objects
    class GUIObject
    {       
        protected:
            virtual void SyncPosition() = 0;
            virtual void SyncGlobalPosition() = 0;
            virtual void ConstructRenderInfo() = 0;
            virtual void ConstructRenderInfo(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) = 0;
        
        public:
            //TODO : Maybe make this thread safe?
            inline static std::vector<ssGUI::GUIObject*> ObjsToDelete = std::vector<ssGUI::GUIObject*>();

            virtual ~GUIObject() = 0;

            //function: GetPosition
            //Gets the local position against the parent from the anchor point. By default the anchor point is at the top-left corner of the parent.
            //Note that windows excludes title bar from the position calculation. Calling this function on MainWindow object will always return (0, 0)
            virtual glm::vec2 GetPosition() const = 0;
            
            //function: SetPosition
            //Sets the local poisition against the parent from the anchor point
            virtual void SetPosition(glm::vec2 position) = 0;
            
            //function: GetGlobalPosition
            //Gets the global position against the top-left corner of MainWindow it is parented. Title bar is excluded from the position calculation.
            virtual glm::vec2 GetGlobalPosition() = 0;         //TODO : See BaseGUIObject.hpp for more details
            
            //function: SetGlobalPosition
            //Sets the global position against the top-left corner of MainWindow it is parented
            virtual void SetGlobalPosition(glm::vec2 position) = 0;
            
            //function: GetSize
            //Gets the size of the GUI Object
            virtual glm::vec2 GetSize() const = 0;
            
            //function: SetSize
            //Sets the size of the GUI Object
            virtual void SetSize(glm::vec2 size) = 0;

            //function: GetMinSize
            //Gets the minimum size of the GUI Object. Setting the size to be below minimum size will set the size to minimum instead.
            virtual glm::vec2 GetMinSize() const = 0;
            
            //function: SetMinSize
            //Sets the minimum size of the GUI Object. Setting minimum size to be negative infinte is possible.
            virtual void SetMinSize(glm::vec2 minSize) = 0;

            //function: GetMaxSize
            //Gets the maximum size of the GUI Object. Setting the size to be above maximum size will set the size to maximum instead.
            virtual glm::vec2 GetMaxSize() const = 0;
            
            //function: SetMaxSize
            //Sets the maximum size of the GUI Object. Setting maximum size to be infinte is possible.
            virtual void SetMaxSize(glm::vec2 maxSize) = 0;

            //function: GetParent
            //Gets the parent of the GUI Object. Nullptr will be returned if there's no parent set.
            virtual ssGUI::GUIObject* GetParent() const = 0;
            
            //function: SetParent
            //Sets the parent of the GUI Object. Setting nullptr will unset the parent.
            virtual void SetParent(ssGUI::GUIObject* newParent) = 0;

            //function: GetChildrenCount
            //Returns the number of children parented to this GUI Object. (Non recursive)
            virtual int GetChildrenCount() const = 0;

            //function: MoveChildrenIteratorToFirst
            //Moves the children iterator to the first child
            virtual void MoveChildrenIteratorToFirst() = 0;

            //function: MoveChildrenIteratorToLast
            //Moves the children iterator to the last child
            virtual void MoveChildrenIteratorToLast() = 0;

            //function: MoveChildrenIteratorNext
            //Moves the children iterator to the next child
            virtual void MoveChildrenIteratorNext() = 0;

            //function: MoveChildrenIteratorPrevious
            //Moves the children iterator to the previous child
            virtual void MoveChildrenIteratorPrevious() = 0;

            //function: IsChildrenIteratorLast
            //Returns true if the iterator is on the last child
            virtual bool IsChildrenIteratorLast() = 0;

            //function: IsChildrenIteratorFirst
            //Returns true if the iterator is on the first child
            virtual bool IsChildrenIteratorFirst() = 0;

            //function: IsChildrenIteratorEnd
            //Returns true if the iterator is _beyond_ the first or last child
            virtual bool IsChildrenIteratorEnd() = 0;

            //function: FindChild
            //Returns true if the child is parented to this GUI Object
            virtual bool FindChild(ssGUI::GUIObject* child) = 0;

            //function: GetCurrentChild
            //Returns the object the childrenIterator is currently on
            virtual ssGUI::GUIObject* GetCurrentChild() = 0;

            //function: GetCurrentChildReferenceIterator
            //Returns the underlying childrenIterator used for this GUI Object
            virtual std::list<ssGUIObjectIndex>::iterator GetCurrentChildReferenceIterator() = 0;

            //function: ChangeChildOrderToBeforePosition
            //Changes the child's position to be in front of said position. Use <GetCurrentChildReferenceIterator> to get the iterator.
            virtual void ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) = 0;

            //function: ChangeChildOrderToAfterPosition
            //Changes the child's position to be in behind of said position. Use <GetCurrentChildReferenceIterator> to get the iterator.
            virtual void ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) = 0;

            //function: Internal_AddChild
            //(Internal ssGUI function) Adds the GUI Object to the children record. Use <SetParent> instead for adding or removing child.
            virtual void Internal_AddChild(ssGUI::GUIObject* obj) = 0;
            
            //function: Internal_RemoveChild
            //(Internal ssGUI function) Removes the GUI Object to the children record. Use <SetParent> instead for adding or removing child.
            virtual void Internal_RemoveChild(ssGUI::GUIObject* obj) = 0;

            //function: GetType
            //Returns the type of this GUI Object. Note that <ssGUI::Enums::GUIObjectType> is a bit-operated enum class.
            virtual ssGUI::Enums::GUIObjectType GetType() const = 0;

            //function: GetAnchorType
            //Returns the anchor type of this GUI Object
            virtual ssGUI::Enums::AnchorType GetAnchorType() const = 0;
            
            //function: SetAnchorType
            //Sets the anchor type of this GUI Object
            virtual void SetAnchorType(ssGUI::Enums::AnchorType anchorType) = 0;

            //function: SetVisible
            //Sets the visibility of this GUI Object. Note that the logic of this GUI Object will not be called if not visible.
            virtual void SetVisible(bool visible) = 0;
            
            //TODO : Change IsVisible to IsEnabled

            //function: IsVisible
            //Returns the visibility of this GUI Object. Note that the logic of this GUI Object will not be called if not visible.
            virtual bool IsVisible() const = 0;

            //function: SetUserCreated
            //Sets the UserCreated flag. True if this GUI Object's lifetime is managed explcitly, otherwise managed by ssGUI object/extension.
            virtual void SetUserCreated(bool created) = 0;

            //function: IsUserCreated()
            //Returns the UserCreated flag. True if this GUI Object's lifetime is managed explcitly, otherwise managed by ssGUI object/extension.
            virtual bool IsUserCreated() const = 0;

            //function: SetBackgroundColor
            //Sets the background color of this GUI Object
            virtual void SetBackgroundColor(glm::u8vec4 color) = 0;
            
            //function: GetBackgroundColor
            //Gets the background color of this GUI Object.
            virtual glm::u8vec4 GetBackgroundColor() const = 0;

            //function: Delete
            //Delete the current GUI Object. This will unset the parent of this GUI Object and all it's references.
            virtual void Delete() = 0;

            //function: Internal_IsDeleted
            //(Internal ssGUI function) This is used to determined if this GUI Object is "deleted" or not.
            virtual bool Internal_IsDeleted() const = 0;
            
            //function: SetHeapAllocated
            //Sets the HeapAllocated flag of this GUI Object. If true, this object will be deleted from the heap automatically after the <Delete> function is called.
            virtual void SetHeapAllocated(bool heap) = 0;

            //function: IsHeapAllocated
            //Gets the HeapAllocated flag of this GUI Object. If true, this object will be deleted from the heap automatically after the <Delete> function is called.
            virtual bool IsHeapAllocated() const = 0;
        
            //function: Extension_GetDrawingVertices
            //This returns a list of vertices for all the shapes that will be drawn by this GUI object.
            //A shape is formed by having the vertices drawn in clockwise direction. Undefined behaviour if they are listed in anti-clockwise direction.
            //This function is mainly be called by <Extension::Draw>.
            virtual std::vector<glm::vec2>& Extension_GetDrawingVertices() = 0;
            
            //function: Extension_GetDrawingUVs
            //This returns the UVs that are mapped to each vertex at the same index location.
            //If no texture is used, you should still maintain the number of UVs same as the number of vertices.
            //This function is mainly be called by <Extension::Draw>.
            virtual std::vector<glm::vec2>& Extension_GetDrawingUVs() = 0;
            
            //function: Extension_GetDrawingColours
            //This returns the colors that are mapped to each vertex at the same index location.
            //If texture is used, this essentially affects the tint of the image, with white as no tinting at all.
            //This function is mainly be called by <Extension::Draw>.
            virtual std::vector<glm::u8vec4>& Extension_GetDrawingColours() = 0;
            
            //function: Extension_GetDrawingCounts
            //This returns the number of vertices each shape has, with the order same as vertices and others.
            //So for example if the first value is 4, then the first 4 vertices form a shape. So on and so forth...
            //This function is mainly be called by <Extension::Draw>.
            virtual std::vector<int>& Extension_GetDrawingCounts() = 0;
            
            //function: Extension_GetDrawingProperties
            //This returns the property of each shape, mapped to the same index location as <Extension_GetDrawingCounts>.
            //This indicates if the shape is just a colored shape, an image or font. 
            //This function is mainly be called by <Extension::Draw>.
            virtual std::vector<ssGUI::DrawingProperty>& Extension_GetDrawingProperties() = 0;
            
            //function: AddExtension
            //Adds the extension to this GUI Object. Note that the extension *must* be allocated on heap.
            virtual void AddExtension(ssGUI::Extensions::Extension* extension) = 0;
            
            //function: GetExtension
            //Gets the extension by the name of it. Nullptr will be returned if not found.
            virtual ssGUI::Extensions::Extension* GetExtension(std::string extensionName) = 0;

            //function: GetListOfExtensions
            //Returns all the extensions on the GUI Object
            virtual std::vector<ssGUI::Extensions::Extension*> GetListOfExtensions() = 0;
            
            //function: IsExtensionExist
            //Returns true if the extension exists on this GUI Object
            virtual bool IsExtensionExist(std::string extensionName) const = 0;
            
            //function: RemoveExtension
            //Removes the extension by the name of it
            virtual void RemoveExtension(std::string extensionName) = 0;
            
            //function: GetExtensionsCount
            //Returns the number of extensions on this GUI Object
            virtual int GetExtensionsCount() const = 0;
            
            //function: GetExtensionDrawOrder
            //Returns the draw order of the extension by the name of it
            virtual int GetExtensionDrawOrder(std::string extensionName) const = 0;
            
            //function: ChangeExtensionDrawOrder
            //Changes the draw order of the extension by the name of it
            virtual void ChangeExtensionDrawOrder(std::string extensionName, int order) = 0;
            
            //function: GetExtensionUpdateOrder
            //Returns the update order of the extension by the name of it
            virtual int GetExtensionUpdateOrder(std::string extensionName) const = 0;
            
            //function: ChangeExtensionUpdateOrder
            //Changes the update order of the extension by the name of it
            virtual void ChangeExtensionUpdateOrder(std::string extensionName, int order) = 0;

            //function: AddEventCallback
            //Adds an eventCallback to this GUI Object
            virtual void AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback) = 0;
            
            //function: GetEventCallback
            //Gets the eventCallback by the name of it
            virtual ssGUI::EventCallbacks::EventCallback* GetEventCallback(std::string eventCallbackName) = 0;
            
            //function: IsEventCallbackExist
            //Returns true if the eventCallback exists on this GUI Object
            virtual bool IsEventCallbackExist(std::string eventCallbackName) const = 0;
            
            //function: RemoveEventCallback
            //Removes the eventCallback by the name of it
            virtual void RemoveEventCallback(std::string eventCallbackName) = 0;

            //function: GetListOfEventCallbacks
            //Returns all the event callbacks on the GUI Object
            virtual std::vector<ssGUI::EventCallbacks::EventCallback*> GetListOfEventCallbacks() = 0;
            
            //function: AddTag
            //Adds a tag to this GUI Object
            virtual void AddTag(std::string tag) = 0;
            
            //function: RemoveTag
            //Removes the tag on this GUI Object
            virtual void RemoveTag(std::string tag) = 0;
            
            //function: HasTag
            //Returns true if the tag exists on this GUI Object
            virtual bool HasTag(std::string tag) const = 0;

            //function: RedrawObject
            //Forces the GUI Object to be redrawn. *Do not* call this in <Internal_Draw>.
            virtual void RedrawObject() = 0;

            //function: IsRedrawNeeded
            //Returns if the GUI Object needs to be redrawn
            virtual bool IsRedrawNeeded() const = 0;
            
            //function: Internal_GetObjectsReferences
            //(Internal ssGUI function) Returns the pointer of the ObjectReferences
            virtual ObjectsReferences* Internal_GetObjectsReferences() = 0;

            //function: Internal_Draw
            //(Internal ssGUI function) Draw function called by <ssGUIManager>
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset) = 0;
            
            //function: Internal_Update
            //(Internal ssGUI function) Update function called by <ssGUIManager>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) = 0;

            //function: Clone
            //Clone function for cloning the object. Use this function instead of assignment operator or copy constructor.
            //The cloned object will be allocated on the heap and the clean up will be managed by ssGUI.
            //Setting cloneChildren will clone all the children recursively with all the ObjectsReferences respectively.
            virtual GUIObject* Clone(bool cloneChildren) = 0;    
    };
    
    inline ssGUI::GUIObject::~GUIObject(){}     //Pure virtual destructor needs to be defined
}

#endif
