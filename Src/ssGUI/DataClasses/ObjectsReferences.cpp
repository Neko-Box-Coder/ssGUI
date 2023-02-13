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
        ssLOG_FUNC_ENTRY();
        
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
                ssGUI_DEBUG(ssGUI_DATA_TAG, "Adding external dependency to "<<it.second);
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid object reference found!");
                ssLOG_EXIT_PROGRAM();
            }
        }
        
        ssLOG_FUNC_EXIT();
    }

    ObjectsReferences::~ObjectsReferences()
    {
        CleanUp();
    }

    ObjectsReferences& ObjectsReferences::operator=(ObjectsReferences const& other)
    {
        ssLOG_FUNC_ENTRY();
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
                ssGUI_DEBUG(ssGUI_DATA_TAG, "Adding external dependency to "<<it.second);
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid object reference found!");
                ssLOG_EXIT_PROGRAM();
            }
        }
        
        ssLOG_FUNC_EXIT();
        return *this;
    }

    int ObjectsReferences::GetObjectsReferencesCount() const
    {
        return ObjectsReferencesTable.size();   
    }

    ssGUIObjectIndex ObjectsReferences::AddObjectReference(ssGUI::GUIObject* obj)
    {
        // ssLOG_FUNC_ENTRY();
        
        if(CleanedUp)
        {
            // ssLOG_FUNC_EXIT();
            return -1;
        }
        
        if(obj == nullptr)
        {
            ssGUI_ERROR(ssGUI_DATA_TAG, "Attempted to add nullptr");
            // ssLOG_FUNC_EXIT();
            ssLOG_EXIT_PROGRAM();
            return -1;
        }

        if(!IsObjectReferenceExist(obj))
        {
            ObjectsReferencesTable[NextFreeIndex] = obj;
            ReverseObjectsReferencesTable[obj] = NextFreeIndex;
            obj->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, NextFreeIndex);
            // ssLOG_FUNC_EXIT();
            return NextFreeIndex++;
        }
        else
        {
            // ssLOG_FUNC_EXIT();
            return GetObjectIndex(obj);
        }
    }

    ssGUI::GUIObject* ObjectsReferences::GetObjectReference(ssGUIObjectIndex index) const
    {
        // ssLOG_FUNC_ENTRY();
        
        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end() && !CleanedUp)
        {
            // ssLOG_FUNC_EXIT();
            return ObjectsReferencesTable.at(index);
        }
        else
        {
            // ssLOG_FUNC_EXIT();
            return nullptr;
        }
    }

    void ObjectsReferences::SetObjectReference(ssGUIObjectIndex index, ssGUI::GUIObject* obj)
    {
        // ssLOG_FUNC_ENTRY();

        if(CleanedUp)
        {
            // ssLOG_FUNC_EXIT();
            return;
        }

        if(obj == nullptr)
        {
            ssGUI_ERROR(ssGUI_DATA_TAG, "Attempted to set nullptr");
            // ssLOG_FUNC_EXIT();
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
        // ssLOG_FUNC_EXIT();
    }

    void ObjectsReferences::RemoveObjectReference(ssGUIObjectIndex index, bool internalCleanUp)
    {
        ssLOG_FUNC_ENTRY();
        
        if(CleanedUp)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        if(ObjectsReferencesTable.find(index) != ObjectsReferencesTable.end())
        {
            ssGUI::GUIObject* obj = ObjectsReferencesTable[index];
            ObjectsReferencesTable.erase(index);
            ReverseObjectsReferencesTable.erase(obj);

            if(!internalCleanUp)
                obj->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }   
        ssLOG_FUNC_EXIT();
    }

    void ObjectsReferences::Internal_AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index)
    {
        // ssLOG_FUNC_ENTRY();
        
        if(CleanedUp)
        {
            // ssLOG_FUNC_EXIT();
            return;
        }

        if(dependency == nullptr)
        {
            ssGUI_ERROR(ssGUI_DATA_TAG, "Adding null dependency found");
            // ssLOG_FUNC_EXIT();
            ssLOG_EXIT_PROGRAM();
            return;
        }

        ExternalObjectsDependencies[dependency] = index;   
        // ssLOG_FUNC_EXIT();
    }

    void ObjectsReferences::Internal_RemoveExternalDependency(ObjectsReferences* dependency)
    {
        // ssLOG_FUNC_ENTRY();
        
        if(CleanedUp)
        {
            // ssLOG_FUNC_EXIT();
            return;
        }

        if(ExternalObjectsDependencies.find(dependency) != ExternalObjectsDependencies.end())
            ExternalObjectsDependencies.erase(dependency);
        // ssLOG_FUNC_EXIT();

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
        ssLOG_FUNC_ENTRY();
        
        if(CleanedUp)
        {
            ssLOG_FUNC_EXIT();
            return;
        }

        for(auto it : ExternalObjectsDependencies)
        {
            ssGUI_DEBUG(ssGUI_DATA_TAG, "Removing "<<it.first<<" external reference of this object");
            it.first->RemoveObjectReference(it.second, true);
        }

        for(auto it : ObjectsReferencesTable)
        {
            ssGUI_DEBUG(ssGUI_DATA_TAG, "Removing external depenency record stored on "<<it.second);
            it.second->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }
        
        CleanedUp = true;

        ssLOG_FUNC_EXIT();
    }

    void ObjectsReferences::CheckObjectsReferencesValidity()
    {
        ssLOG_FUNC_ENTRY();
        
        ssGUI_DEBUG(ssGUI_DATA_TAG, "Object Reference: "<<this);
        //Iterate all objects references and check
        for(auto it : ObjectsReferencesTable)
        {
            ssGUI_DEBUG(ssGUI_DATA_TAG, "Checking record: "<<it.second<<" with index "<<it.first);
            
            if(it.second == nullptr)
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid record found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.second->Internal_GetObjectsReferences();
        }

        //Iterate all external objects references and check
        for(auto it : ExternalObjectsDependencies)
        {
            ssGUI_DEBUG(ssGUI_DATA_TAG, "Checking dependency: "<<it.first);
            
            if(it.first == nullptr)
            {
                ssGUI_ERROR(ssGUI_DATA_TAG, "Invalid dependency found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.first->GetObjectsReferencesCount();
        }

        ssLOG_FUNC_EXIT();
    }
}