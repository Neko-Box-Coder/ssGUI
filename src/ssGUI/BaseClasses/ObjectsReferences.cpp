#include "ssGUI/BaseClasses/ObjectsReferences.hpp"


#include "ssGUI/BaseClasses/GUIObject.hpp"


namespace ssGUI
{
    ObjectsReferences::ObjectsReferences() : ObjectsReferencesTable(), ReverseObjectsReferencesTable(), NextFreeIndex(0), 
                                                ExternalObjectsDependencies(), CleanedUp(false)
    {}

    ObjectsReferences::ObjectsReferences(ObjectsReferences const& other)
    {
        ObjectsReferencesTable = other.ObjectsReferencesTable;
        ReverseObjectsReferencesTable = other.ReverseObjectsReferencesTable;
        NextFreeIndex = other.NextFreeIndex;
        ExternalObjectsDependencies = std::unordered_map<ObjectsReferences*, ssGUIObjectIndex>();

        //Iterate all objects references and add external dependencies
        for(auto it : ObjectsReferencesTable)
            it.second->Internal_GetObjectsReferences()->AddExternalDependency(this, it.first);
    }

    ObjectsReferences::~ObjectsReferences()
    {
        if(!CleanedUp)
            CleanUp();
    }

    ObjectsReferences& ObjectsReferences::operator=(ObjectsReferences const& other)
    {
        ObjectsReferencesTable = other.ObjectsReferencesTable;
        ReverseObjectsReferencesTable = other.ReverseObjectsReferencesTable;
        NextFreeIndex = other.NextFreeIndex;
        ExternalObjectsDependencies = std::unordered_map<ObjectsReferences*, ssGUIObjectIndex>();
        
        //Iterate all objects references and add external dependencies
        for(auto it : ObjectsReferencesTable)
            it.second->Internal_GetObjectsReferences()->AddExternalDependency(this, it.first);
        
        return *this;
    }

    int ObjectsReferences::GetObjectsReferencesCount() const
    {
        return ObjectsReferencesTable.size();   
    }

    ssGUIObjectIndex ObjectsReferences::AddObjectReference(ssGUI::GUIObject* obj)
    {
        if(!IsObjectReferenceExist(obj))
        {
            ObjectsReferencesTable[NextFreeIndex] = obj;
            ReverseObjectsReferencesTable[obj] = NextFreeIndex;
            obj->Internal_GetObjectsReferences()->AddExternalDependency(this, NextFreeIndex);
            return NextFreeIndex++;
        }
        else
            return GetObjectIndex(obj);
    }

    ssGUI::GUIObject* ObjectsReferences::GetObjectReference(ssGUIObjectIndex index) const
    {
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end())
            return ObjectsReferencesTable.at(index);
        else
            return nullptr;
    }

    void ObjectsReferences::SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj)
    {
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end())
            ObjectsReferencesTable[index] = obj;
    }

    void ObjectsReferences::RemoveObjectReference(ssGUIObjectIndex index)
    {
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end())
        {
            ssGUI::GUIObject* obj = ObjectsReferencesTable[index];
            ObjectsReferencesTable.erase(index);
            ReverseObjectsReferencesTable.erase(obj);
            obj->Internal_GetObjectsReferences()->RemoveExternalDependency(this);
        }   
    }

    void ObjectsReferences::AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index)
    {
        if(ExternalObjectsDependencies.find(dependency) != ExternalObjectsDependencies.end())
            ExternalObjectsDependencies[dependency] = index;   
    }

    void ObjectsReferences::RemoveExternalDependency(ObjectsReferences* dependency)
    {
        if(ExternalObjectsDependencies.find(dependency) != ExternalObjectsDependencies.end())
            ExternalObjectsDependencies.erase(dependency);   
    }
    
    ssGUIObjectIndex ObjectsReferences::GetObjectIndex(ssGUI::GUIObject* obj) const
    {
        if(ReverseObjectsReferencesTable.find(obj) != ReverseObjectsReferencesTable.end())
            return ReverseObjectsReferencesTable.at(obj);
        else
            return -1;
    }

    bool ObjectsReferences::IsObjectReferenceExist(ssGUI::GUIObject* obj) const
    {
        return ReverseObjectsReferencesTable.find(obj) != ReverseObjectsReferencesTable.end();
    }

    void ObjectsReferences::CleanUp()
    {
        for(auto it : ExternalObjectsDependencies)
            it.first->RemoveObjectReference(it.second);
        
        CleanedUp = true;
    }


}