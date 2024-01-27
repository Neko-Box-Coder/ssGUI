#include "ssGUI/DataClasses/ObjectsReferences.hpp"

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

#include "ssGUI/HelperClasses/LogWithTagsAndLevel.hpp"

namespace ssGUI
{
    ObjectsReferences::ObjectsReferences() :    ObjectsReferencesTable(),
                                                ReverseObjectsReferencesTable(),
                                                NextFreeIndex(0),
                                                ExternalObjectsDependencies(),
                                                CleanedUp(false)
    {}

    ObjectsReferences::ObjectsReferences(ObjectsReferences const& other)
    {
        ssGUI_LOG_FUNC();
        
        ObjectsReferencesTable = other.ObjectsReferencesTable;
        ReverseObjectsReferencesTable = other.ReverseObjectsReferencesTable;
        NextFreeIndex = other.NextFreeIndex;
        ExternalObjectsDependencies = std::unordered_map<ObjectsReferences*, ssGUIObjectIndex>();
        CleanedUp = false;

        //Iterate all objects references and add external dependencies
        for(auto it : ObjectsReferencesTable)
        {
            if(it.second != nullptr)
            {
                ssGUI_DEBUG(ssGUI_TAG_DATA, "Adding external dependency to "<<it.second);
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid object reference found!");
                ssLOG_EXIT_PROGRAM();
            }
        }
    }

    ObjectsReferences::~ObjectsReferences()
    {
        CleanUp();
    }

    ObjectsReferences& ObjectsReferences::operator=(ObjectsReferences const& other)
    {
        ssGUI_LOG_FUNC();
        ObjectsReferencesTable = other.ObjectsReferencesTable;
        ReverseObjectsReferencesTable = other.ReverseObjectsReferencesTable;
        NextFreeIndex = other.NextFreeIndex;
        ExternalObjectsDependencies = std::unordered_map<ObjectsReferences*, ssGUIObjectIndex>();
        CleanedUp = false;

        //Iterate all objects references and add external dependencies
        for(auto it : ObjectsReferencesTable)
        {
            if(it.second != nullptr)
            {
                ssGUI_DEBUG(ssGUI_TAG_DATA, "Adding external dependency to "<<it.second);
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid object reference found!");
                ssLOG_EXIT_PROGRAM();
            }
        }
        
        return *this;
    }

    int ObjectsReferences::GetObjectsReferencesCount() const
    {
        return ObjectsReferencesTable.size();   
    }

    ssGUIObjectIndex ObjectsReferences::AddObjectReference(ssGUI::GUIObject* obj)
    {
        if(CleanedUp)
            return -1;
        
        if(obj == nullptr)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Attempted to add nullptr");
            ssLOG_EXIT_PROGRAM();
            return -1;
        }

        if(!IsObjectReferenceExist(obj))
        {
            ObjectsReferencesTable[NextFreeIndex] = obj;
            ReverseObjectsReferencesTable[obj] = NextFreeIndex;
            obj->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, NextFreeIndex);
            return NextFreeIndex++;
        }
        else
            return GetObjectIndex(obj);
    }

    ssGUI::GUIObject* ObjectsReferences::GetObjectReference(ssGUIObjectIndex index) const
    {
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end() && !CleanedUp)
            return ObjectsReferencesTable.at(index);
        else
            return nullptr;
    }

    void ObjectsReferences::SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj)
    {
        if(CleanedUp)
            return;

        if(obj == nullptr)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Attempted to set nullptr");
            ssLOG_EXIT_PROGRAM();
            return;
        }
        
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end() && !CleanedUp)
        {
            ObjectsReferencesTable[index]->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
            ReverseObjectsReferencesTable.erase(ObjectsReferencesTable[index]);
            ObjectsReferencesTable[index] = obj;
            ReverseObjectsReferencesTable[ObjectsReferencesTable[index]] = index;
            ObjectsReferencesTable[index]->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, index);
        }
    }

    void ObjectsReferences::RemoveObjectReference(ssGUIObjectIndex index, bool internalCleanUp)
    {
        ssGUI_LOG_FUNC();
        
        if(CleanedUp)
            return;

        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end())
        {
            ssGUI::GUIObject* obj = ObjectsReferencesTable[index];
            ObjectsReferencesTable.erase(index);
            ReverseObjectsReferencesTable.erase(obj);

            if(!internalCleanUp)
                obj->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }   
    }

    void ObjectsReferences::Internal_AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index)
    {
        if(CleanedUp)
            return;

        if(dependency == nullptr)
        {
            ssGUI_ERROR(ssGUI_TAG_DATA, "Adding null dependency found");
            ssLOG_EXIT_PROGRAM();
            return;
        }

        ExternalObjectsDependencies[dependency] = index;   
    }

    void ObjectsReferences::Internal_RemoveExternalDependency(ObjectsReferences* dependency)
    {
        if(CleanedUp)
            return;

        if(ExternalObjectsDependencies.find(dependency) != ExternalObjectsDependencies.end())
            ExternalObjectsDependencies.erase(dependency);
    }
    
    ssGUIObjectIndex ObjectsReferences::GetObjectIndex(ssGUI::GUIObject* obj) const
    {
        if(CleanedUp || obj == nullptr)
            return -1;

        if(ReverseObjectsReferencesTable.find(obj) != ReverseObjectsReferencesTable.end() && !CleanedUp)
            return ReverseObjectsReferencesTable.at(obj);
        else
            return -1;
    }

    bool ObjectsReferences::IsObjectReferenceExist(ssGUI::GUIObject* obj) const
    {
        if(CleanedUp || obj == nullptr)
            return false;

        return ReverseObjectsReferencesTable.find(obj) != ReverseObjectsReferencesTable.end();
    }

    std::vector<ssGUIObjectIndex> ObjectsReferences::GetListOfObjectsIndices()
    {
        std::vector<ssGUIObjectIndex> indices = std::vector<ssGUIObjectIndex>();

        for(auto it : ObjectsReferencesTable)
            indices.push_back(it.first);
        
        return indices;
    }

    void ObjectsReferences::CleanUp()
    {
        ssGUI_LOG_FUNC();
        
        if(CleanedUp)
            return;

        for(auto it : ExternalObjectsDependencies)
        {
            ssGUI_DEBUG(ssGUI_TAG_DATA, "Removing "<<it.first<<" external reference of this object");
            it.first->RemoveObjectReference(it.second, true);
        }

        for(auto it : ObjectsReferencesTable)
        {
            ssGUI_DEBUG(ssGUI_TAG_DATA, "Removing external depenency record stored on "<<it.second);
            it.second->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }
        
        CleanedUp = true;
    }

    void ObjectsReferences::CheckObjectsReferencesValidity()
    {
        ssGUI_LOG_FUNC();
        
        ssGUI_DEBUG(ssGUI_TAG_DATA, "Object Reference: "<<this);
        //Iterate all objects references and check
        for(auto it : ObjectsReferencesTable)
        {
            ssGUI_DEBUG(ssGUI_TAG_DATA, "Checking record: "<<it.second<<" with index "<<it.first);
            
            if(it.second == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid record found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.second->Internal_GetObjectsReferences();
        }

        //Iterate all external objects references and check
        for(auto it : ExternalObjectsDependencies)
        {
            ssGUI_DEBUG(ssGUI_TAG_DATA, "Checking dependency: "<<it.first);
            
            if(it.first == nullptr)
            {
                ssGUI_ERROR(ssGUI_TAG_DATA, "Invalid dependency found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.first->GetObjectsReferencesCount();
        }
    }
}