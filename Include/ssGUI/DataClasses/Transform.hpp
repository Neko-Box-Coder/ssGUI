#ifndef SSGUI_TRANSFORM_H
#define SSGUI_TRANSFORM_H

#include "glm/vec2.hpp"
#include "ssGUI/Enums/AnchorType.hpp"

//namespace: ssGUI
namespace ssGUI
{
    class Hierarchy;
    class Renderer;
    class EventCallbackManager;
    class GUIObject;

    /*class: ssGUI::Transform
    This manages all the position and size related information for a GUI Object
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        //Widget transform
        glm::vec2 Position;                                         //See <GetPosition>
        glm::vec2 GlobalPosition;                                   //See <GetGlobalPosition>
        glm::vec2 Size;                                             //See <GetSize>
        glm::vec2 MinSize;                                          //See <GetMinSize>
        glm::vec2 MaxSize;                                          //See <GetMaxSize>
        ssGUI::Enums::AnchorType Anchor;                            //See <GetAnchorType>
        
        ssGUI::Hierarchy* CurrentHierarchy;                         //(Internal variable) Used to sync the position and global position
        ssGUI::Renderer* CurrentRenderer;                           //(Internal variable) Used to send redraw signal
        ssGUI::EventCallbackManager* CurrentEventCallbackManager;   //(Internal variable) Used to call event callbacks listeners 
        ssGUI::GUIObject* CurrentObject;                            //(Internal variable) used to register the container for event callbacks listeners
    =================================================================
    ============================== C++ ==============================
    Transform::Transform() :    Position(0, 0),
                                GlobalPosition(0, 0),
                                Size(25, 25),
                                MinSize(25, 25),
                                MaxSize(9999, 9999),
                                Anchor(ssGUI::Enums::AnchorType::TOP_LEFT),
                                CurrentHierarchy(nullptr),
                                CurrentRenderer(nullptr),
                                CurrentEventCallbackManager(nullptr),
                                CurrentObject(nullptr)
    {}
    =================================================================
    */
    class Transform
    {
        protected:
            //Widget transform
            glm::vec2 Position;                                         //See <GetPosition>
            glm::vec2 GlobalPosition;                                   //See <GetGlobalPosition>
            glm::vec2 Size;                                             //See <GetSize>
            glm::vec2 MinSize;                                          //See <GetMinSize>
            glm::vec2 MaxSize;                                          //See <GetMaxSize>
            ssGUI::Enums::AnchorType Anchor;                            //See <GetAnchorType>
            
            ssGUI::Hierarchy* CurrentHierarchy;                         //(Internal variable) Used to sync the position and global position
            ssGUI::Renderer* CurrentRenderer;                           //(Internal variable) Used to send redraw signal
            ssGUI::EventCallbackManager* CurrentEventCallbackManager;   //(Internal variable) Used to call event callbacks listeners 
            ssGUI::GUIObject* CurrentObject;                            //(Internal variable) used to register the container for event callbacks listeners

            Transform(Transform const& other);

            virtual void SyncPosition();
            virtual void SyncGlobalPosition();

        public:
            Transform();
            virtual ~Transform();

            virtual void SetDependentComponents(ssGUI::Hierarchy* hierarchy, ssGUI::Renderer* renderer, ssGUI::EventCallbackManager* eventCallbackManager, 
                ssGUI::GUIObject* obj);
        
            //function: GetPosition
            //Gets the local position against the parent from the anchor point. By default the anchor point is at the top-left corner of the parent.
            //Note that windows excludes title bar from the position calculation. Calling this function on MainWindow object will always return (0, 0)
            virtual glm::vec2 GetPosition() const;
            
            //function: SetPosition
            //Sets the local poisition against the parent from the anchor point
            virtual void SetPosition(glm::vec2 position);
            
            //function: GetGlobalPosition
            //Gets the global position against the top-left corner of MainWindow it is parented. Title bar is excluded from the position calculation.
            virtual glm::vec2 GetGlobalPosition();    //TODO : SyncGlobalPosition is called so can't be const. Try to turn this to const
            
            //function: SetGlobalPosition
            //Sets the global position against the top-left corner of MainWindow it is parented
            virtual void SetGlobalPosition(glm::vec2 position);

            //function: GetSize
            //Gets the size of the GUI Object
            virtual glm::vec2 GetSize() const;
            
            //function: SetSize
            //Sets the size of the GUI Object
            virtual void SetSize(glm::vec2 size);

            //function: GetMinSize
            //Gets the minimum size of the GUI Object. Setting the size to be below minimum size will set the size to minimum instead.
            virtual glm::vec2 GetMinSize() const;
            
            //function: SetMinSize
            //Sets the minimum size of the GUI Object. Setting minimum size to be negative infinte is possible.
            virtual void SetMinSize(glm::vec2 minSize);

            //function: GetMaxSize
            //Gets the maximum size of the GUI Object. Setting the size to be above maximum size will set the size to maximum instead.
            virtual glm::vec2 GetMaxSize() const;
            
            //function: SetMaxSize
            //Sets the maximum size of the GUI Object. Setting maximum size to be infinte is possible.
            virtual void SetMaxSize(glm::vec2 maxSize);

            //function: GetAnchorType
            //Returns the anchor type of this GUI Object
            virtual ssGUI::Enums::AnchorType GetAnchorType() const;
            
            //function: SetAnchorType
            //Sets the anchor type of this GUI Object
            virtual void SetAnchorType(ssGUI::Enums::AnchorType anchorType);
    };
}


#endif