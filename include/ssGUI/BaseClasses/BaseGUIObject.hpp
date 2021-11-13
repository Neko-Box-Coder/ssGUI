#ifndef SSGUI_BASE_GUI_OBJECT
#define SSGUI_BASE_GUI_OBJECT

#include "ssGUI/BaseClasses/GUIObject.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenAddedEventCallback.hpp"
#include "ssGUI/EventCallbacks/RecursiveChildrenRemovedEventCallback.hpp"
#include "ssGUI/EventCallbacks/MinMaxSizeChangedEventCallback.hpp"
#include "glm/vec4.hpp"
#include <vector>
#include <list>
#include <limits>

//namespace: ssGUI
namespace ssGUI
{
    //class: BaseGUIObject
    //This is the implementation class for <GUIObject>
    class BaseGUIObject : public GUIObject
    {
        private:
            ssGUI::GUIObject* ParentP;
            std::list<ssGUI::GUIObject*> Children;
            bool Visible;
            glm::u8vec4 BackgroundColour;
            bool UserCreated;

            //Widget transform
            glm::ivec2 Position;
            glm::ivec2 GlobalPosition;
            glm::ivec2 Size;
            glm::ivec2 MinSize;
            glm::ivec2 MaxSize;
            ssGUI::Enums::AnchorType Anchor;

            BaseGUIObject& operator=(BaseGUIObject const& other);

        protected:
            //Rendering
            std::vector<glm::ivec2> DrawingVerticies;
            std::vector<glm::ivec2> DrawingUVs;
            std::vector<glm::u8vec4> DrawingColours;
            std::vector<int> DrawingCounts;
            std::vector<ssGUI::DrawingProperty> DrawingProperties;

            std::vector<ssGUI::Extensions::Extension*> Extensions;
            std::vector<ssGUI::EventCallbacks::EventCallback*> EventCallbacks;

            BaseGUIObject(BaseGUIObject const& other);

            virtual void SyncPosition() override;
            virtual void SyncGlobalPosition() override;
            //virtual void DrawBorder(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset);

        public:
            BaseGUIObject();
            virtual ~BaseGUIObject() override;

            //Below are from GUIObject.hpp
            //function: GetPosition
            virtual glm::ivec2 GetPosition() const override;
            
            //function: SetPosition
            virtual void SetPosition(glm::ivec2 position) override;
            
            //function: GetGlobalPosition
            virtual glm::ivec2 GetGlobalPosition() override;    //TODO : SyncGlobalPosition is called so can't be const. Try to turn this to const
            
            //function: SetGlobalPosition
            virtual void SetGlobalPosition(glm::ivec2 position) override;

            //function: GetSize
            virtual glm::ivec2 GetSize() const override;
            
            //function: SetSize
            virtual void SetSize(glm::ivec2 size) override;

            //function: GetMinSize
            virtual glm::ivec2 GetMinSize() const override;
            
            //function: SetMinSize
            virtual void SetMinSize(glm::ivec2 minSize) override;

            //function: GetMaxSize
            virtual glm::ivec2 GetMaxSize() const override;
            
            //function: SetMaxSize
            virtual void SetMaxSize(glm::ivec2 maxSize) override;

            //function: GetParentP
            virtual ssGUI::GUIObject* GetParentP() const override;
            
            //function: SetParentP
            virtual void SetParentP(ssGUI::GUIObject* parentP) override;

            //function: GetChildrenCount
            virtual int GetChildrenCount() const override;
            
            //function: GetChildrenStartIterator
            virtual std::list<ssGUI::GUIObject*>::iterator GetChildrenStartIterator() override;
            
            //function: GetChildrenEndIterator
            virtual std::list<ssGUI::GUIObject*>::iterator GetChildrenEndIterator() override;
            
            //function: AddChild
            virtual void AddChild(ssGUI::GUIObject* obj) override;
            
            //function: RemoveChild
            virtual void RemoveChild(ssGUI::GUIObject* obj) override;

            //function: GetType
            virtual ssGUI::Enums::GUIObjectType GetType() const override;

            //function: GetAnchorType
            virtual ssGUI::Enums::AnchorType GetAnchorType() const override;
            
            //function: SetAnchorType
            virtual void SetAnchorType(ssGUI::Enums::AnchorType anchorType) override;

            //function: SetVisible
            virtual void SetVisible(bool visible) override;
            
            //function: IsVisible
            virtual bool IsVisible() const override;

            //function: SetUserCreated
            virtual void SetUserCreated(bool created) override;

            //function: IsUserCreated()
            virtual bool IsUserCreated() const override;
            
            //function: SetBackgroundColour
            virtual void SetBackgroundColour(glm::u8vec4 color) override;
            
            //function: GetBackgroundColour
            virtual glm::u8vec4 GetBackgroundColour() const override;
            
            //function: Extension_GetDrawingVerticies
            virtual std::vector<glm::ivec2>& Extension_GetDrawingVerticies() override;
            
            //function: Extension_GetDrawingUVs
            virtual std::vector<glm::ivec2>& Extension_GetDrawingUVs() override;
            
            //function: Extension_GetDrawingColours
            virtual std::vector<glm::u8vec4>& Extension_GetDrawingColours() override;
            
            //function: Extension_GetDrawingCounts
            virtual std::vector<int>& Extension_GetDrawingCounts() override;
            
            //function: Extension_GetDrawingProperties
            virtual std::vector<ssGUI::DrawingProperty>& Extension_GetDrawingProperties() override;
            
            //function: AddExtension
            virtual void AddExtension(ssGUI::Extensions::Extension* extension) override;
            
            //function: GetExtension
            virtual ssGUI::Extensions::Extension* GetExtension(std::string extensionName) override;
            
            //function: IsExtensionExist
            virtual bool IsExtensionExist(std::string extensionName) const override;
            
            //function: RemoveExtension
            virtual void RemoveExtension(std::string extensionName) override;

            
            //function: AddEventCallback
            virtual void AddEventCallback(ssGUI::EventCallbacks::EventCallback* eventCallback) override;
            
            //function: GetEventCallback
            virtual ssGUI::EventCallbacks::EventCallback* GetEventCallback(std::string eventCallbackName) override;
            
            //function: IsEventCallbackExist
            virtual bool IsEventCallbackExist(std::string eventCallbackName) const override;
            
            //function: RemoveEventCallback
            virtual void RemoveEventCallback(std::string eventCallbackName) override;

            //function: Draw
            virtual void Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindowP, glm::ivec2 mainWindowPositionOffset) override;
            
            //function: Internal_Update
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& globalInputStatus, ssGUI::InputStatus& windowInputStatus, ssGUI::GUIObject* mainWindow) override;
            
            //function: Clone
            virtual GUIObject* Clone(std::vector<GUIObject*>& originalObjs, bool cloneChildren) override;
    };
}

#endif