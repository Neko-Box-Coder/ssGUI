#ifndef SSGUI_GUI_OBJECT
#define SSGUI_GUI_OBJECT

#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/DataClasses/Transform.hpp"
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/DataClasses/Hierarchy.hpp"
#include "ssGUI/DataClasses/ExtensionManager.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"
#include "ssGUI/EventCallbacks/EventCallback.hpp"

#include "glm/vec4.hpp"
#include <vector>
#include <list>
#include <limits>
#include <unordered_set>
#include <unordered_map>


//namespace: ssGUI
namespace ssGUI
{
    class Menu;

    /*class: ssGUI::GUIObject
    This is the implementation class for <ssGUI::GUIObject>. See <ssGUI::GUIObject> for more details about the functions
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        glm::vec2 LastGlobalPosition;                   //(Internal variable) Used for cache rendering
        std::unordered_set<std::string> CurrentTags;    //See <HasTag>
        ssGUI::Menu* RightClickMenu;                    //See <RegisterRightClickMenu>
    =================================================================
    ============================== C++ ==============================
    GUIObject::GUIObject() :    LastGlobalPosition(),
                                CurrentTags(),
                                RightClickMenu(nullptr)
    {
        SetupComponents();
    }
    =================================================================
    */
    class GUIObject : public Transform, public Renderer, public Hierarchy, public ExtensionManager, public EventCallbackManager
    {
        private:
            GUIObject& operator=(GUIObject const& other);

        protected:
            glm::vec2 LastGlobalPosition;                   //(Internal variable) Used for cache rendering
            std::unordered_set<std::string> CurrentTags;    //See <HasTag>
            ssGUI::Menu* RightClickMenu;                    //See <RegisterRightClickMenu>

            GUIObject(GUIObject const& other);

            virtual void SetupComponents();
            virtual ssGUI::GUIObject* CloneChildren(ssGUI::GUIObject* originalRoot, ssGUI::GUIObject* clonedRoot);
            virtual void CloneExtensionsAndEventCallbacks(ssGUI::GUIObject* clonedObj);
            virtual void CheckRightClickMenu(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                                ssGUI::GUIObject* mainWindow);
            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow);

        public:
            //TODO : Maybe make this thread safe?
            inline static std::vector<ssGUI::GUIObject*> ObjsToDelete = std::vector<ssGUI::GUIObject*>();

            GUIObject();
            virtual ~GUIObject();

            //function: GetType
            //Returns the type of this GUI Object. Note that <ssGUI::Enums::GUIObjectType> is a bit-operated enum class.
            virtual ssGUI::Enums::GUIObjectType GetType() const;
            
            //function: AddTag
            //Adds a tag to this GUI Object
            virtual void AddTag(std::string tag);
            
            //function: RemoveTag
            //Removes the tag on this GUI Object
            virtual void RemoveTag(std::string tag);
            
            //function: HasTag
            //Returns true if the tag exists on this GUI Object
            virtual bool HasTag(std::string tag) const;

            //function: RegisterRightClickMenu
            //Register this GUI Object to a menu that can be triggered by right click 
            virtual void RegisterRightClickMenu(ssGUI::Menu* menu);

            //function: ClearRightClickMenu
            //Clears the right click menu
            virtual void ClearRightClickMenu();

            //function: Internal_Draw
            //(Internal ssGUI function) Draw function called by <ssGUIManager>
            virtual void Internal_Draw(ssGUI::Backend::BackendDrawingInterface* drawingInterface, ssGUI::GUIObject* mainWindow, glm::vec2 mainWindowPositionOffset);
            
            //function: Internal_Update
            //(Internal ssGUI function) Update function called by <ssGUIManager>
            virtual void Internal_Update(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, ssGUI::GUIObject* mainWindow);

            //function: Clone
            //Clone function for cloning the object. Use this function instead of assignment operator or copy constructor.
            //The cloned object will be allocated on the heap and the clean up will be managed by ssGUI.
            //Setting cloneChildren will clone all the children recursively with all the ObjectsReferences respectively.
            virtual GUIObject* Clone(bool cloneChildren);

    };
}

#endif