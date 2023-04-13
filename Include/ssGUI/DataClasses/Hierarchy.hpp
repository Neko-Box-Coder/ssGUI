#ifndef SSGUI_HIERARCHY_H
#define SSGUI_HIERARCHY_H

#include "ssGUI/Factory.hpp"
#include "ssGUI/DataClasses/ObjectsReferences.hpp"
#include <vector>
#include <tuple>
#include <type_traits>
#include <list>

//namespace: ssGUI
namespace ssGUI
{
    class Renderer;
    class EventCallbackManager;
    class Transform;
    class GUIObject;    

    /*class: ssGUI::Hierarchy
    This manages all the information related to the hierarchy status of a GUI Object

    Variables & Constructor:
    ============================== C++ ==============================
    public:
        struct ChildInfo                                                                                    //Holds infomation about a child, used internally
        {
            public:
                friend class Hierarchy;

                bool operator==(ChildInfo const& other)
                {
                    return ChildIndex == other.ChildIndex && CompositeChild == other.CompositeChild;
                };

            private:
                ssGUIObjectIndex ChildIndex = -1;
                bool CompositeChild = false;
        };
    
        using ChildToken = std::list<ChildInfo>::iterator;                                                  //See <GetCurrentChildToken>
    
    protected:
        ssGUIObjectIndex Parent;                                                                            //See <GetParent>
        std::list<ChildInfo> Children;                                                                      //See <GetListOfChildren>
        ChildToken CurrentChild;                                                                            //See <GetCurrentChild> and <GetCurrentChildToken> 
        bool CurrentChildIteratorFrontEnd;                                                                  //See <IsChildrenIteratorEnd>
        bool CurrentChildIteratorBackEnd;                                                                   //See <IsChildrenIteratorEnd>
        bool UserCreated;                                                                                   //See <IsUserCreated>
        bool ObjectDelete;                                                                                  //See <Delete>
        bool HeapAllocated;                                                                                 //See <IsHeapAllocated>
        ObjectsReferences CurrentObjectsReferences;                                                         //See <Internal_GetObjectsReferences>
        bool DestroyEventCalled;                                                                            //(Internal variable) Flag for calling <OnObjectDestroyEventCallback>
        std::vector<std::tuple<bool, bool, ChildToken>> StashedChildIterators;                              //See <StashChildrenIterator>
        bool Focused;                                                                                       //See <IsFocused>
    
        ssGUI::Renderer* CurrentRenderer;                                                                   //(Internal variable) Used to send redraw signal
        ssGUI::EventCallbackManager* CurrentEventCallbackManager;                                           //(Internal variable) Used to call any event callbacks listeners
        ssGUI::Transform* CurrentTransform;                                                                 //(Internal variable) Used to set the global position when the parent is being set
        ssGUI::GUIObject* CurrentObject;                                                                    //(Internal variable) Used to <Internal_AddChild> for the new parent when it is being set
    =================================================================
    ============================== C++ ==============================
    Hierarchy::Hierarchy() :    Parent(-1),
                                Children(),
                                CurrentChild(Children.end()),
                                CurrentChildIteratorFrontEnd(true),
                                CurrentChildIteratorBackEnd(true),
                                UserCreated(true),
                                ObjectDelete(false),
                                HeapAllocated(false),
                                CurrentObjectsReferences(),
                                DestroyEventCalled(false),
                                StashedChildIterators(),
                                Focused(false),
                                CurrentRenderer(nullptr),
                                CurrentEventCallbackManager(nullptr),
                                CurrentTransform(nullptr),
                                CurrentObject(nullptr)
    {}
    =================================================================
    */
    class Hierarchy
    {
        public:
            struct ChildInfo                                                                                    //Holds infomation about a child, used internally
            {
                public:
                    friend class Hierarchy;

                    bool operator==(ChildInfo const& other) const
                    {
                        return ChildIndex == other.ChildIndex && CompositeChild == other.CompositeChild;
                    };

                private:
                    ssGUIObjectIndex ChildIndex = -1;
                    bool CompositeChild = false;
            };
        
            using ChildToken = std::list<ChildInfo>::iterator;                                                  //See <GetCurrentChildToken>
        
        protected:
            ssGUIObjectIndex Parent;                                                                            //See <GetParent>
            std::list<ChildInfo> Children;                                                                      //See <GetListOfChildren>
            ChildToken CurrentChild;                                                                            //See <GetCurrentChild> and <GetCurrentChildToken> 
            bool CurrentChildIteratorFrontEnd;                                                                  //See <IsChildrenIteratorEnd>
            bool CurrentChildIteratorBackEnd;                                                                   //See <IsChildrenIteratorEnd>
            bool UserCreated;                                                                                   //See <IsUserCreated>
            bool ObjectDelete;                                                                                  //See <Delete>
            bool HeapAllocated;                                                                                 //See <IsHeapAllocated>
            ObjectsReferences CurrentObjectsReferences;                                                         //See <Internal_GetObjectsReferences>
            bool DestroyEventCalled;                                                                            //(Internal variable) Flag for calling <OnObjectDestroyEventCallback>
            std::vector<std::tuple<bool, bool, ChildToken>> StashedChildIterators;                              //See <StashChildrenIterator>
            bool Focused;                                                                                       //See <IsFocused>
        
            ssGUI::Renderer* CurrentRenderer;                                                                   //(Internal variable) Used to send redraw signal
            ssGUI::EventCallbackManager* CurrentEventCallbackManager;                                           //(Internal variable) Used to call any event callbacks listeners
            ssGUI::Transform* CurrentTransform;                                                                 //(Internal variable) Used to set the global position when the parent is being set
            ssGUI::GUIObject* CurrentObject;                                                                    //(Internal variable) Used to <Internal_AddChild> for the new parent when it is being set

            Hierarchy(Hierarchy const& other);

            virtual void NotifyAndRemoveOnObjectDestroyEventCallbackIfExist();

        public:
            Hierarchy();
            virtual ~Hierarchy();

            virtual void SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::EventCallbackManager* eventCallbackManager, ssGUI::Transform* transform, 
                ssGUI::GUIObject* obj);
            
            //function: GetParent
            //Gets the parent of the GUI Object. Nullptr will be returned if there's no parent set.
            virtual ssGUI::GUIObject* GetParent() const;
            
            //function: SetParent
            //Sets the parent of the GUI Object. Setting nullptr will unset the parent.
            //Consider using <ssGUI::ssGUIManager::AddGUIObjectAsChild> or <AddChild> instead if possible
            virtual void SetParent(ssGUI::GUIObject* newParent, bool compositeChild = false);

            //function: AddChild
            //Adds a GUI Object as a child of this GUI Object. 
            //The lifetime of the child GUI object is managed by <ssGUIManager> if this GUI object is (recursively) added to <ssGUIManager>.
            //Otherwise, the user needs to manage the lifetime of the child GUI object.
            template<typename T>
            T* AddChild(bool compositeChild = false)
            {
                if(std::is_base_of<ssGUI::GUIObject, T>::value)
                {
                    auto* guiObject = ssGUI::Factory::Create<T>();
                    guiObject->SetParent(CurrentObject);
                    return guiObject;
                }
                else
                {
                    ssGUI_WARNING(ssGUI_DATA_TAG, "You cannot add non GUI object");
                    return nullptr;
                }
            }

            //function: IsChildComposite
            //True if the current child (see <FindChild>) belongs to this composite object
            virtual bool IsChildComposite() const;

            //function: GetChildrenCount
            //Returns the number of children parented to this GUI Object. (Non recursive)
            virtual int GetChildrenCount() const;
            
            //function: MoveChildrenIteratorToFirst
            //Moves the children iterator to the first child. 
            //To preserve the current children iterator, use <StashChildrenIterator>.
            //
            //Please note that if the current children iterator points to a child that is removed 
            //then it will be invalid and <IsChildrenIteratorEnd> will return true.
            virtual void MoveChildrenIteratorToFirst();
            
            //function: MoveChildrenIteratorToLast
            //Moves the children iterator to the last child. 
            //To preserve the current children iterator, use <StashChildrenIterator>.
            //
            //Please note that if the current children iterator points to a child that is removed 
            //then it will be invalid and <IsChildrenIteratorEnd> will return true.
            virtual void MoveChildrenIteratorToLast();
            
            //function: MoveChildrenIteratorNext
            //Moves the children iterator to the next child. 
            //To preserve the current children iterator, use <StashChildrenIterator>.
            //
            //Please note that if the current children iterator points to a child that is removed 
            //then it will be invalid and <IsChildrenIteratorEnd> will return true.
            virtual void MoveChildrenIteratorNext();
            
            //function: MoveChildrenIteratorPrevious
            //Moves the children iterator to the previous child. 
            //To preserve the current children iterator, use <StashChildrenIterator>.
            //
            //Please note that if the current children iterator points to a child that is removed 
            //then it will be invalid and <IsChildrenIteratorEnd> will return true.
            virtual void MoveChildrenIteratorPrevious();
            
            //function: IsChildrenIteratorLast
            //Returns true if the iterator is on the last child
            virtual bool IsChildrenIteratorLast();
            
            //function: IsChildrenIteratorFirst
            //Returns true if the iterator is on the first child
            virtual bool IsChildrenIteratorFirst();
            
            //function: IsChildrenIteratorEnd
            //Returns true if the iterator is _beyond_ the first or last child
            virtual bool IsChildrenIteratorEnd();

            //function: StashChildrenIterator
            //Stores the current children iterator on the stack. Any modification will not affect the children iterator.
            //However, if the stashed children iterator points to a child that is removed then the stashed children iterator will be invalid
            //and <IsChildrenIteratorEnd> will be true when the stashed iterator is restored (popped).
            virtual void StashChildrenIterator();

            //function: PopChildrenIterator
            //Pops the most recent stashed children iterator back to the current children iterator.
            //
            //Please note that if the stashed children iterator points to a child that is removed then the stashed children iterator will be invalid,
            //meaning <IsChildrenIteratorEnd> will be true.
            virtual void PopChildrenIterator();

            //function: FindChild
            //If a child exists is parented to this GUI Object, this will move the children iterator to it and returns true.
            //Otherwise, the children iterator is untouched and this will return false.
            //To preserve the current children iterator, use <StashChildrenIterator>.
            virtual bool FindChild(ssGUI::GUIObject* child);

            //function: GetCurrentChild
            //Returns the object the children iterator is currently pointing to. This will remove nullptr if it is not pointing at any child.
            //To check if the current children iterator is valid, use <IsChildrenIteratorEnd>.
            virtual ssGUI::GUIObject* GetCurrentChild();

            //function: GetCurrentChildToken
            //Returns the underlying childrenIterator used for this GUI Object
            virtual ssGUI::Hierarchy::ChildToken GetCurrentChildToken();
            
            //function: ChangeChildOrderToBeforePosition
            //Changes the child's position to be in front of said position. Use <GetCurrentChildToken> to get the iterator.
            virtual void ChangeChildOrderToBeforePosition(ssGUI::Hierarchy::ChildToken child, 
                                                            ssGUI::Hierarchy::ChildToken position);
            
            //function: ChangeChildOrderToAfterPosition
            //Changes the child's position to be in behind of said position. Use <GetCurrentChildToken> to get the iterator.
            virtual void ChangeChildOrderToAfterPosition(ssGUI::Hierarchy::ChildToken child, 
                                                            ssGUI::Hierarchy::ChildToken position);

            //function: GetListOfChildren
            //Gets a list of children
            virtual std::vector<ssGUI::GUIObject*> GetListOfChildren() const;

            //function: Internal_AddChild
            //(Internal ssGUI function) Adds the GUI Object to the children record. Use <SetParent> instead for adding or removing child.
            virtual void Internal_AddChild(ssGUI::GUIObject* obj, bool compositeChild);
            
            //function: Internal_RemoveChild
            //(Internal ssGUI function) Removes the GUI Object to the children record. Use <SetParent> instead for adding or removing child.
            virtual void Internal_RemoveChild(ssGUI::GUIObject* obj);

            //TODO: This is probably not needed. I am not sure
            // virtual void Internal_SetChildAsComposite(bool compositeChild);

            //function: SetUserCreated
            //Sets the UserCreated flag. True if this GUI Object's lifetime is managed explcitly, otherwise managed by ssGUI object/extension.
            virtual void SetUserCreated(bool created);

            //function: IsUserCreated
            //Returns the UserCreated flag. True if this GUI Object's lifetime is managed explcitly, otherwise managed by ssGUI object/extension.
            virtual bool IsUserCreated() const;

            //function: IsFocused
            //Return if this GUI Object is focused
            virtual bool IsFocused() const;
            
            //function: SetFocus
            //Set this GUI Object to be focused
            virtual void SetFocus(bool focus);

            //function: Internal_SetSelfFocus
            //(Internal ssGUI function) Set the focus of the GUI object without affect any other GUI object
            virtual void Internal_SetSelfFocus(bool focus);

            //function: Delete 
            //Delete the current GUI Object. This will unset the parent of this GUI Object and all it's references.
            //This will also deletes all of the children GUI Objects. If this is allocated on the heap,
            //it will be deallocated automiactically. Any referencing to this GUI Object after deleted is undefined. 
            virtual void Delete();

            //function: Internal_IsDeleted
            //(Internal ssGUI function) This is used to determined if this GUI Object is "deleted" or not.
            virtual bool Internal_IsDeleted() const;

            //function: SetHeapAllocated
            //Sets the HeapAllocated flag of this GUI Object. If true, this object will be deleted from the heap automatically after the <Delete> function is called.
            virtual void SetHeapAllocated(bool heap);

            //function: IsHeapAllocated
            //Gets the HeapAllocated flag of this GUI Object. If true, this object will be deleted from the heap automatically after the <Delete> function is called.
            virtual bool IsHeapAllocated() const;

            //function: Internal_ChildrenManualDeletion
            //(Internal ssGUI function) Allow a composite GUI object to be able to deallocate itself with its generated GUI objects when the destructor is being called
            virtual void Internal_ChildrenManualDeletion(std::vector<ssGUI::ssGUIObjectIndex> generatedObjs);

            //function: Internal_GetObjectsReferences
            //(Internal ssGUI function) Returns the pointer of the ObjectReferences
            virtual ObjectsReferences* Internal_GetObjectsReferences();
    };
}

#endif