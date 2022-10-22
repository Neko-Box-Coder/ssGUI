#include "ssGUI/DataClasses/ObjectsReferences.hpp"

#include "ssGUI/GUIObjectClasses/GUIObject.hpp"

#include "ssLogger/ssLog.hpp"

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
                #if SSGUI_USE_DEBUG
                ssLOG_LINE("Adding external dependency to "<<it.second);
                #endif
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssLOG_LINE("Invalid object reference found!");
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
                #if SSGUI_USE_DEBUG
                ssLOG_LINE("Adding external dependency to "<<it.second);
                #endif
                it.second->Internal_GetObjectsReferences()->Internal_AddExternalDependency(this, it.first);
            }
            else
            {
                ssLOG_LINE("Invalid object reference found!");
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
            ssLOG_LINE("Attempted to add nullptr");
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
            ssLOG_LINE("Attempted to set nullptr");
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
            ssLOG_LINE("Adding null dependency found");
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
            #if SSGUI_USE_DEBUG
            ssLOG_LINE("Removing "<<it.first<<" external reference of this object");
            #endif
            it.first->RemoveObjectReference(it.second, true);
        }

        for(auto it : ObjectsReferencesTable)
        {
            #if SSGUI_USE_DEBUG
            ssLOG_LINE("Removing external depenency record stored on "<<it.second);
            #endif
            it.second->Internal_GetObjectsReferences()->Internal_RemoveExternalDependency(this);
        }
        
        CleanedUp = true;

        ssLOG_FUNC_EXIT();
    }

    void ObjectsReferences::CheckObjectsReferencesValidity()
    {
        ssLOG_FUNC_ENTRY();
        
        ssLOG_LINE("Object Reference: "<<this);
        //Iterate all objects references and check
        for(auto it : ObjectsReferencesTable)
        {
            ssLOG_LINE("Checking record: "<<it.second<<" with index "<<it.first);
            
            if(it.second == nullptr)
            {
                ssLOG_LINE("Invalid record found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.second->Internal_GetObjectsReferences();
        }

        //Iterate all external objects references and check
        for(auto it : ExternalObjectsDependencies)
        {
            ssLOG_LINE("Checking dependency: "<<it.first);
            
            if(it.first == nullptr)
            {
                ssLOG_LINE("Invalid dependency found");
                ssLOG_EXIT_PROGRAM();
            }
            else
                it.first->GetObjectsReferencesCount();
        }

        ssLOG_FUNC_EXIT();
    }
}