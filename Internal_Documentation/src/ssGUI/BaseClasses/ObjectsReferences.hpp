#ifndef OBJECTS_REFERENCES
#define OBJECTS_REFERENCES

#include <unordered_map>
#include <vector>

//namespace: ssGUI
namespace ssGUI
{
    typedef int ssGUIObjectIndex;

    class GUIObject;
    
    /*class: ObjectsReferences
    The purpose of this class is to track references between each GUI Objects. 
    So when cloning happens, the cloned version of the GUI Objects will be referenced (if present) instead of the original GUI Objects. 
    
    ============================ Text ============================
          A            A-copy
          │            │
          ├─►B         ├─►B-copy
          │            │
          ├─►C         ├─►C-copy
          │            │
          └─►D         └─►D-copy
    
    If B has a references to C and D.
    Then B-copy will be referencing C-copy and D-copy instead
    of C and D.
    ==============================================================
    
    If a GUI Object (Referencer) references another GUI Object (Referencee), 
    then the GUI Object it is referencing (Referencee) will be stored in this class inside the GUI Object (Referencer)
    
    Additionally, the GUI Object that is referencing the other object (Referencer) will be stored in ExternalObjectsDependencies 
    in the GUI Object which is being referenced (Referencee).
    
    This is do so to prevent invalid referencing when one of the referencing GUI Objects is deleted. 
    In other words, this is a 2-way referencing system.
    
    You normally don't need to deal with this
    
    Variables & Constructor:
    ============================== C++ ==============================
    protected:
        std::unordered_map<ssGUIObjectIndex, ssGUI::GUIObject*> ObjectsReferencesTable;
        std::unordered_map<ssGUI::GUIObject*, ssGUIObjectIndex> ReverseObjectsReferencesTable;

        ssGUIObjectIndex NextFreeIndex;
        std::unordered_map<ObjectsReferences*, ssGUIObjectIndex> ExternalObjectsDependencies;
        bool CleanedUp;
    =================================================================
    ============================== C++ ==============================
    ObjectsReferences::ObjectsReferences() : ObjectsReferencesTable(), ReverseObjectsReferencesTable(), NextFreeIndex(0), 
                                                ExternalObjectsDependencies(), CleanedUp(false)
    {}
    =================================================================
    */
    class ObjectsReferences
    {
        protected:
            std::unordered_map<ssGUIObjectIndex, ssGUI::GUIObject*> ObjectsReferencesTable;
            std::unordered_map<ssGUI::GUIObject*, ssGUIObjectIndex> ReverseObjectsReferencesTable;

            ssGUIObjectIndex NextFreeIndex;
            std::unordered_map<ObjectsReferences*, ssGUIObjectIndex> ExternalObjectsDependencies;
            bool CleanedUp;

        public:
            ObjectsReferences();
            ObjectsReferences(ObjectsReferences const& other);
            ~ObjectsReferences();

            ObjectsReferences& operator=(ObjectsReferences const& other);

            //function: GetObjectsReferencesCount
            //Returns the number of GUI Object this is referencing
            virtual int GetObjectsReferencesCount() const;

            //function: AddObjectReference
            //Adds the GUI Object to the referencing table. Returns the index of it.
            virtual ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj);

            //function: GetObjectReference
            //Gets the referencing GUI Object of the referencing index. Nullptr is returned if invalid index.
            virtual ssGUI::GUIObject* GetObjectReference(ssGUIObjectIndex index) const;

            //function: SetObjectReference
            //Sets the referencing GUI Object with the index 
            virtual void SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj);

            //function: RemoveObjectReference
            //Removes the referencing GUI Object with the index
            virtual void RemoveObjectReference(ssGUIObjectIndex index, bool internalCleanUp = false);

            //function: AddExternalDependency
            //Adds the external depending ObjectsReferences
            virtual void AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index);
            
            //function: RemoveExternalDependency           
            //Removes the external depending ObjectsReferences
            virtual void RemoveExternalDependency(ObjectsReferences* dependency);
            
            //function: GetObjectIndex
            //Gets the referencing index by the address of the GUI Object. Returns -1 if not found
            virtual ssGUIObjectIndex GetObjectIndex(ssGUI::GUIObject* obj) const;
            
            //function: IsObjectReferenceExist
            //Returns true if the GUI Object is being referenced     
            virtual bool IsObjectReferenceExist(ssGUI::GUIObject* obj) const;
            
            //function: GetListOfObjectsIndices
            //Gets all the valid referencing index
            virtual std::vector<ssGUIObjectIndex> GetListOfObjectsIndices();
            
            //function: CleanUp
            //Removes all references to this object
            virtual void CleanUp();
            
            //function: CheckObjectsReferencesValidity
            //Debug function to make sure all the references are valid
            virtual void CheckObjectsReferencesValidity();

    };
}

#endif