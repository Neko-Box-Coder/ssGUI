#ifndef OBJECTS_REFERENCES
#define OBJECTS_REFERENCES

#include <vector>
#include <queue>

namespace ssGUI
{
    typedef int ssGUIObjectIndex;

    class GUIObject;
    
    class ObjectsReferences
    {
        protected:
            std::vector<ssGUI::GUIObject*> ObjectsReferencesTable;
            std::queue<int> NextFreeObjectsReferenceIndices;
            int ObjectsReferencesCount;

        public:
            ObjectsReferences();
            ~ObjectsReferences() = default;

            virtual int GetObjectsReferencesCount() const;
            virtual ssGUIObjectIndex AddObjectReference(ssGUI::GUIObject* obj);
            virtual ssGUI::GUIObject* GetObjectReference(ssGUIObjectIndex index) const;
            virtual void SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj);
            virtual ssGUIObjectIndex GetObjectIndex(ssGUI::GUIObject* obj) const;
            virtual bool IsObjectReferenceExist(ssGUI::GUIObject* obj) const;
            virtual void RemoveObjectReference(ssGUIObjectIndex index);
    };
}

#endif