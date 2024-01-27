#include "ssGUI/Extensions/Tabbable.hpp"
#include "ssGUI/GUIObjectClasses/GUIObject.hpp"
#include "ssGUI/GUIObjectClasses/MainWindow.hpp"

namespace ssGUI
{
    namespace Extensions
    {
        Tabbable::Tabbable() :
            Container(nullptr),
            Enabled(true),
            TabAreaObject(-1),
            CurrentObjectsReferences(),
            TopLevelParent(-1),
            Untabbable(false),
            LastMouseDragBeginPosition()
        {
        }

        Tabbable::~Tabbable()
        {
        }

        Tabbable::Tabbable(const Tabbable& other)
        {
            Container = nullptr;
            Copy(&other);
        }

        void Tabbable::ConstructRenderInfo()
        {
        }

        void Tabbable::ConstructRenderInfo(ssGUI::Backend::DrawingInterface*
                                               drawingInterface,
                                           ssGUI::GUIObject* mainWindow,
                                           glm::vec2 mainWindowPositionOffset)
        {
        }

        const std::string Tabbable::EXTENSION_NAME = "Tabbable";

        void Tabbable::SetTopLevelParent(ssGUI::GUIObject* topLevelParent)
        {
            if(topLevelParent == nullptr)
            {
                TopLevelParent = -1;
                return;
            }

            TopLevelParent = CurrentObjectsReferences.AddObjectReference(topLevelParent);
        }

        ssGUI::GUIObject* Tabbable::GetTopLevelParent() const
        {
            if(TopLevelParent < 0)
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference(TopLevelParent);
        }

        void Tabbable::SetUntabbable(bool untabbable)
        {
            Untabbable = untabbable;
        }

        bool Tabbable::IsUntabbable() const
        {
            return Untabbable;
        }

        glm::vec2 Tabbable::GetLastMouseDragBeginPosition() const
        {
            return LastMouseDragBeginPosition;
        }

        void Tabbable::SetLastMouseDragBeginPosition(glm::vec2 pos)
        {
            LastMouseDragBeginPosition = pos;
        }

        ssGUI::GUIObject* Tabbable::GetTabAreaObject() const
        {
            if(TabAreaObject < 0)
                return nullptr;

            return CurrentObjectsReferences.GetObjectReference(TabAreaObject);
        }

        void Tabbable::SetTabAreaObject(ssGUI::GUIObject* tabArea)
        {
            if(tabArea == nullptr)
            {
                TabAreaObject = -1;
                return;
            }

            TabAreaObject = CurrentObjectsReferences.AddObjectReference(tabArea);
        }

        bool Tabbable::IsEnabled() const
        {
            return Enabled;
        }

        void Tabbable::SetEnabled(bool enabled)
        {
            Enabled = enabled;
        }

        void Tabbable::Internal_Update(bool isPreUpdate,
                                       ssGUI::Backend::SystemInputInterface* inputInterface,
                                       ssGUI::InputStatus& currentInputStatus,
                                       ssGUI::InputStatus& lastInputStatus,
                                       ssGUI::GUIObject* mainWindow)
        {
            ssGUI_LOG_FUNC();

#define DISCARD_AND_RETURN() \
    do                       \
    {                        \
        return;              \
    }                        \
    while(0)

            //This is function is executed twice, one before the Container GUI object update and one
            //after. You can use the isPreUpdate variable to decide when to execute the extension
            //update
            if(isPreUpdate || Container == nullptr || Container->GetParent() == nullptr || !Enabled)
                return;

            if(Container->GetType() != ssGUI::Enums::GUIObjectType::WINDOW)
                DISCARD_AND_RETURN();

            {
                auto* window = static_cast<ssGUI::Window*>(Container);

                if(window->GetWindowDragState() == ssGUI::Enums::WindowDragState::STARTED)
                {
                    auto* mainWindowBackend =
                        static_cast<ssGUI::MainWindow*>(mainWindow)->GetBackendWindowInterface();
                    LastMouseDragBeginPosition =
                        inputInterface->GetCurrentMousePosition(mainWindowBackend);
                }

                //If the window is being dragged / finished being dragged, set the input status
                if(window->GetWindowDragState() == ssGUI::Enums::WindowDragState::DRAGGING ||
                   window->GetWindowDragState() == ssGUI::Enums::WindowDragState::ENDED)
                {
                    currentInputStatus.CurrentDragData.SetDragData(Container);
                }
            }
        }

        void Tabbable::Internal_Draw(bool isPreRender,
                                     ssGUI::Backend::DrawingInterface* drawingInterface,
                                     ssGUI::GUIObject* mainWindow,
                                     glm::vec2 mainWindowPositionOffset)
        {
        }

        std::string Tabbable::GetExtensionName() const
        {
            return EXTENSION_NAME;
        }

        void Tabbable::BindToObject(ssGUI::GUIObject* bindObj)
        {
            Container = bindObj;
        }

        void Tabbable::Copy(const ssGUI::Extensions::Extension* extension)
        {
            if(extension->GetExtensionName() != EXTENSION_NAME)
                return;

            auto* original = static_cast<const ssGUI::Extensions::Tabbable*>(extension);
            Enabled = original->IsEnabled();
            TabAreaObject = original->TabAreaObject;
            CurrentObjectsReferences = original->CurrentObjectsReferences;
            TopLevelParent = original->TopLevelParent;
            Untabbable = original->Untabbable;
            LastMouseDragBeginPosition = original->LastMouseDragBeginPosition;
        }

        ObjectsReferences* Tabbable::Internal_GetObjectsReferences()
        {
            return &CurrentObjectsReferences;
        }

        Tabbable* Tabbable::Clone()
        {
            Tabbable* temp = new Tabbable(*this);
            return temp;
        }
    }

}