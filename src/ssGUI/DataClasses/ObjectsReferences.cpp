#include "ssGUI/DataClasses/ObjectsReferences.hpp"


#include "ssGUI/GUIObjectClasses/GUIObject.hpp"


namespace ssGUI
{
    ObjectsReferences::ObjectsReferences()
    {}

    ObjectsReferences::ObjectsReferences(ObjectsReferences const& other)
    {
        FUNC_DEBUG_ENTRY();
        
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
                #if USE_DEBUG
                DEBUG_LINE("Adding external dependency to "<<it.second);
                #endif
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                DEBUG_LINE("Invalid object reference found!");
                DEBUG_EXIT_PROGRAM();
            }
        }
        
        FUNC_DEBUG_EXIT();
    }

    ObjectsReferences::~ObjectsReferences()
    {
        CleanUp();
    }

    ObjectsReferences& ObjectsReferences::operator=(ObjectsReferences const& other)
    {
        FUNC_DEBUG_ENTRY();
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
                #if USE_DEBUG
                DEBUG_LINE("Adding external dependency to "<<it.second);
                #endif
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                DEBUG_LINE("Invalid object reference found!");
                DEBUG_EXIT_PROGRAM();
            }
        }
        
        FUNC_DEBUG_EXIT();
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
            DEBUG_LINE("Attempted to add nullptr");
            DEBUG_EXIT_PROGRAM();
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
            DEBUG_LINE("Attempted to set nullptr");
            DEBUG_EXIT_PROGRAM();
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
        FUNC_DEBUG_ENTRY();
        
        if(CleanedUp)
        {
            FUNC_DEBUG_EXIT();
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
        FUNC_DEBUG_EXIT();
    }

    void ObjectsReferences::Internal_AddExternalDependency(ObjectsReferences* dependency, ssGUIObjectIndex index)
    {
        if(CleanedUp)
            return;

        if(dependency == nullptr)
        {
            DEBUG_LINE("Adding null dependency found");
            DEBUG_EXIT_PROGRAM();
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
        FUNC_DEBUG_ENTRY();
        
        if(CleanedUp)
        {
            FUNC_DEBUG_EXIT();
            return;
        }

        for(auto it : ExternalObjectsDependencies)
        {
            #if USE_DEBUG
            DEBUG_LINE("Removing "<<it.first<<" external reference of this object");
            #endif
            it.first->RemoveObjectReference(it.second, true);
        }

        for(auto it : ObjectsReferencesTable)
        {
            #if USE_DEBUG
            DEBUG_LINE("Removing external depenency record stored on "<<it.second);
            #endif
            it.second->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }
        
        CleanedUp = true;

        FUNC_DEBUG_EXIT();
    }

    void ObjectsReferences::CheckObjectsReferencesValidity()
    {
        FUNC_DEBUG_ENTRY();
        
        DEBUG_LINE("Object Reference: "<<this);
        //Iterate all objects references and check
        for(auto it : ObjectsReferencesTable)
        {
            DEBUG_LINE("Checking record: "<<it.second<<" with index "<<it.first);
            
            if(it.second == nullptr)
            {
                DEBUG_LINE("Invalid record found");
                DEBUG_EXIT_PROGRAM();
            }
            else
                it.second->Internal_GetObjectsReferences();
        }

        //Iterate all external objects references and check
        for(auto it : ExternalObjectsDependencies)
        {
            DEBUG_LINE("Checking dependency: "<<it.first);
            
            if(it.first == nullptr)
            {
                DEBUG_LINE("Invalid dependency found");
                DEBUG_EXIT_PROGRAM();
            }
            else
                it.first->GetObjectsReferencesCount();
        }

        FUNC_DEBUG_EXIT();
    }
}