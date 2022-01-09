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
    class GUIObject
    {       
        protected:
            virtual void SyncPosition() = 0;
            virtual void SyncGlobalPosition() = 0;
        
        public:
            virtual ~GUIObject() = 0;

            //function: GetPosition
            virtual glm::ivec2 GetPosition() const = 0;
            
            //function: SetPosition
            virtual void SetPosition(glm::ivec2 position) = 0;
            
            //function: GetGlobalPosition
            virtual glm::ivec2 GetGlobalPosition() = 0;         //TODO : See BaseGUIObject.hpp for more details
            
            //function: SetGlobalPosition
            virtual void SetGlobalPosition(glm::ivec2 position) = 0;
            
            //function: GetSize
            virtual glm::ivec2 GetSize() const = 0;
            
            //function: SetSize
            virtual void SetSize(glm::ivec2 size) = 0;

            //function: GetMinSize
            virtual glm::ivec2 GetMinSize() const = 0;
            
            //function: SetMinSize
            virtual void SetMinSize(glm::ivec2 minSize) = 0;

            //function: GetMaxSize
            virtual glm::ivec2 GetMaxSize() const = 0;
            
            //function: SetMaxSize
            virtual void SetMaxSize(glm::ivec2 maxSize) = 0;

            //function: GetParent
            virtual ssGUI::GUIObject* GetParent() const = 0;
            
            //function: SetParent
            virtual void SetParent(ssGUI::GUIObject* newParent) = 0;

            //function: GetChildrenCount
            virtual int GetChildrenCount() const = 0;
            
            virtual void MoveChildrenIteratorToFirst() = 0;

            virtual void MoveChildrenIteratorToLast() = 0;

            virtual void MoveChildrenIteratorNext() = 0;

            virtual void MoveChildrenIteratorPrevious() = 0;

            virtual bool IsChildrenIteratorLast() = 0;

            virtual bool IsChildrenIteratorFirst() = 0;

            virtual bool IsChildrenIteratorEnd() = 0;

            //function: FindChild
            virtual bool FindChild(ssGUI::GUIObject* child) = 0;

            virtual ssGUI::GUIObject* GetCurrentChild() = 0;

            virtual std::list<ssGUIObjectIndex>::iterator GetCurrentChildReferenceIterator() = 0;
            
            virtual void ChangeChildOrderToBeforePosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) = 0;
            
            virtual void ChangeChildOrderToAfterPosition(std::list<ssGUIObjectIndex>::iterator child, std::list<ssGUIObjectIndex>::iterator position) = 0;

            //function: Internal_AddChild
            virtual void Internal_AddChild(ssGUI::GUIObject* obj) = 0;
            
            //function: Internal_RemoveChild
            virtual void Internal_RemoveChild(ssGUI::GUIObject* obj) = 0;

            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const = 0;

            //function: GetAnchorType
            virtual ssGUI::Enums::AnchorType GetAnchorType() const = 0;
            
            //function: SetAnchorType
            virtual void SetAnchorType(ssGUI::Enums::AnchorType anchorType) = 0;

            //function: SetVisible
            virtual void SetVisible(bool visible) = 0;
            
            //function: IsVisible
            virtual bool IsVisible() const = 0;

            //function: SetUserCreated
            virtual void SetUserCreated(bool created) = 0;

            //function: IsUserCreated()
            virtual bool IsUserCreated() const = 0;

            //function: SetBackgroundColour
            virtual void SetBackgroundColour(glm::u8vec4 color) = 0;
            
            //function: GetBackgroundColour
            virtual glm::u8vec4 GetBackgroundColour() const = 0;

            //function: Delete 
            virtual void Delete() = 0;

            //function: Internal_IsDeleted
            virtual bool Internal_IsDeleted() const = 0;

            virtual void SetHeapAllocated(bool heap) = 0;

            //function: Internal_NeedCleanUp
            virtual bool IsHeapAllocated() const = 0;
        
            //function: Extension_GetDrawingVerticies
            //This function should only be called by <Extension::Draw>.
            //This will return all the drawing verticies position that is already offset by the window titlebar. 
            //Therefore it *will not* match the global position.
            virtual std::vector<glm::ivec2>& Extension_GetDrawingVerticies() = 0;
            
            //function: Extension_GetDrawingUVs
            virtual std::vector<glm::ivec2>& Extension_GetDrawingUVs() = 0;
            
            //function: Extension_GetDrawingColours
            virtual std::vector<glm::u8vec4>& Extension_GetDrawingColours() = 0;
            
            //function: Extension_GetDrawingCounts
            virtual std::vector<int>& Extension_GetDrawingCounts() = 0;
            
            //function: Extension_GetDrawingProperties
            virtual std::vector<ssGUI::DrawingProperty>& Extension_GetDrawingProperties() = 0;
            
            //function: AddExtension
            virtual void AddExtension(ssGUI::Extensions::Extension* extension) = 0;
            
            //function: GetExtension
            virtual ssGUI::Extensions::Extension* GetExtension(std::string extensionName) = 0;

            virtual std::vector<ssGUI::Extensions::Extension*> GetListOfExtensions() = 0;
            
            //function: IsExtensionExist
            virtual bool IsExtensionExist(std::string extensionName) const = 0;
            
            //function: RemoveExtension
            virtual void RemoveExtension(std::string extensionName) = 0;

            virtual int GetExtensionsCount() const = 0;

            virtual int GetExtensionDrawOrder(std::string extensionName) const = 0;

            virtual void ChangeExtensionDrawOrder(std::string extensionName, int order) = 0;

            virtual int GetExtensionUpdateOrder(std::string extensionName) const = 0;

            virtual void ChangeExtensionUpdateOrder(std::string extensionName, int order) = 0;

            //function: AddEventCallback
            virtual void AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback) = 0;
            
            //function: GetEventCallback
            virtual ssGUI::EventCallbacks::EventCallback* GetEventCallback(std::string eventCallbackName) = 0;
            
            //function: IsEventCallbackExist
            virtual bool IsEventCallbackExist(std::string eventCallbackName) const = 0;
            
            //function: RemoveEventCallback
            virtual void RemoveEventCallback(std::string eventCallbackName) = 0;

            virtual void AddTag(std::string tag) = 0;

            virtual void RemoveTag(std::string tag) = 0;

            virtual bool HasTag(std::string tag) const = 0;

            virtual ObjectsReferences* Internal_GetObjectsReferences() = 0;

            //function: Draw
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) = 0;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) = 0;

            //function: Clone
            virtual GUIObject* Clone(bool cloneChildren) = 0;    
    };
    
    inline ssGUI::GUIObject::~GUIObject(){}     //Pure virtual destructor needs to be defined
}

#endif
