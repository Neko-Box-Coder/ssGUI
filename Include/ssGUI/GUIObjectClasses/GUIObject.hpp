#ifndef SSGUI_GUI_OBJECT_H
#define SSGUI_GUI_OBJECT_H

#include "ssGUI/Enums/GUIObjectType.hpp"
#include "ssGUI/DataClasses/Transform.hpp"
#include "ssGUI/DataClasses/Renderer.hpp"
#include "ssGUI/DataClasses/Hierarchy.hpp"
#include "ssGUI/DataClasses/ExtensionManager.hpp"
#include "ssGUI/DataClasses/EventCallbackManager.hpp"

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
        ssGUI::ssGUIObjectIndex RightClickMenuId;       //See <RegisterRightClickMenu>
        std::string CurrentName;                        //See <GetName>
    =================================================================
    ============================== C++ ==============================
    GUIObject::GUIObject() :    LastGlobalPosition(),
                                CurrentTags(),
                                RightClickMenuId(-1),
                                CurrentName()
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
            ssGUI::ssGUIObjectIndex RightClickMenuId;       //See <RegisterRightClickMenu>
            std::string CurrentName;                        //See <GetName>

            GUIObject(GUIObject const& other);

            virtual void SetupComponents();
            virtual ssGUI::GUIObject* CloneChildren(ssGUI::GUIObject* originalRoot, ssGUI::GUIObject* clonedRoot);
            virtual void CloneExtensionsAndEventCallbacks(ssGUI::GUIObject* clonedObj);
            virtual void CheckRightClickMenu(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                                ssGUI::GUIObject* mainWindow);
            virtual void MainLogic(ssGUI::Backend::BackendSystemInputInterface* inputInterface, ssGUI::InputStatus& inputStatus, 
                                    ssGUI::GUIObject* mainWindow);

        public:
            static const std::string GUI_OBJECT_BG_SHAPE_NAME;
        
            //TODO : Maybe make this thread safe?
            static std::vector<ssGUI::GUIObject*> ObjsToDelete;

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

            //function: SetName
            virtual void SetName(std::string name);
            
            //function: GetName
            virtual std::string GetName() const;

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
            
            //function: InitiateDefaultResources
            //Initiate default resources that this GUI object needs, this is called automatically
            virtual void InitiateDefaultResources();
            
            //function: CleanUpDefaultResources
            //Deallocate default resources that this GUI Object needs, this is called automatically
            virtual void CleanUpDefaultResources();
    };
}

#endif