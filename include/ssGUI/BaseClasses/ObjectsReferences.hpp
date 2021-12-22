#ifndef OBJECTS_REFERENCES
#define OBJECTS_REFERENCES

#include <unordered_map>

namespace ssGUI
{
    typedef int ssGUIObjectIndex;

    class GUIObject;
    
    // External Dependency
    //     of B
    //     ┌────────┐  References  ┌─────────┐
    //     │        ├─────────────►│         │
    //     │    A   │              │    B    │
    //     │        │              │         │
    //     │        ├─────────────►│         │
    //     └────────┘Dependents On └─────────┘

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

            virtual int GetObjectsReferencesCount() const;

            virtual ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj);
            virtual ssGUI::GUIObject* GetObjectReference(ssGUIObjectIndex index) const;          
            virtual void SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj);
            virtual void RemoveObjectReference(ssGUIObjectIndex index);

            virtual void AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index);
            virtual void RemoveExternalDependency(ObjectsReferences* dependency);

            virtual ssGUIObjectIndex GetObjectIndex(ssGUI::GUIObject* obj) const;
            virtual bool IsObjectReferenceExist(ssGUI::GUIObject* obj) const;

            virtual void CleanUp();
    };
}

#endif