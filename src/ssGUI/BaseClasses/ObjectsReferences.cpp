#include "ssGUI/BaseClasses/ObjectsReferences.hpp"


#include "ssGUI/BaseClasses/GUIObject.hpp"


namespace ssGUI
{
    ObjectsReferences::ObjectsReferences() : ObjectsReferencesTable(), NextFreeObjectsReferenceIndices(), ObjectsReferencesCount(0)
    {

    }

    int ObjectsReferences::GetObjectsReferencesCount() const
    {
        return ObjectsReferencesCount;
    }

    ssGUIObjectIndex ObjectsReferences::AddObjectReference(ssGUI::GUIObject* obj)
    {
        int addedIndex = -1;
        
        if(NextFreeObjectsReferenceIndices.empty())
        {
            ObjectsReferencesTable.push_back(obj);
            addedIndex = ObjectsReferencesTable.size() - 1;
        }
        else
        {
            addedIndex = NextFreeObjectsReferenceIndices.front();
            ObjectsReferencesTable[NextFreeObjectsReferenceIndices.front()] = obj;
            NextFreeObjectsReferenceIndices.pop();
        }

        ObjectsReferencesCount++;

        return addedIndex;
    }

    ssGUI::GUIObject* ObjectsReferences::GetObjectReference(ssGUIObjectIndex index) const
    {
        return ObjectsReferencesTable[index];
    }

    void ObjectsReferences::SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj)
    {
        ObjectsReferencesTable[index] = obj;
    }

    ssGUIObjectIndex ObjectsReferences::GetObjectIndex(ssGUI::GUIObject* obj) const
    {
        for(int i = 0; i < ObjectsReferencesTable.size(); i++)
        {
            if(ObjectsReferencesTable[i] == obj)
                return i;
        }

        return -1;
    }

    bool ObjectsReferences::IsObjectReferenceExist(ssGUI::GUIObject* obj) const
    {
        for(int i = 0; i < ObjectsReferencesTable.size(); i++)
        {
            if(ObjectsReferencesTable[i] == obj)
                return true;
        }

        return false;
    }

    void ObjectsReferences::RemoveObjectReference(ssGUIObjectIndex index)
    {
        ObjectsReferencesTable[index] = nullptr;
        NextFreeObjectsReferenceIndices.push(index);

        ObjectsReferencesCount--;
    }



}