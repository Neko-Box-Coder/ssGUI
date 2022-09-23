#include "ssGUI/DataClasses/ExtensionManager.hpp"

#include"ssGUI/DataClasses/Renderer.hpp"


namespace ssGUI
{
    ExtensionManager::ExtensionManager(ExtensionManager const&)
    {
        Extensions = std::unordered_map<std::string, ssGUI::Extensions::Extension*>(); 
        ExtensionsDrawOrder = std::vector<std::string>(); 
        ExtensionsUpdateOrder = std::vector<std::string>(); 
        CurrentRenderer = nullptr;
        CurrentObject = nullptr;
    }
    
    ExtensionManager::ExtensionManager() :  Extensions(),
                                            ExtensionsDrawOrder(),
                                            ExtensionsUpdateOrder(),
                                            CurrentRenderer(nullptr),
                                            CurrentObject(nullptr)
    {}

    ExtensionManager::~ExtensionManager()
    {
    }

    void ExtensionManager::SetDependentComponents(ssGUI::Renderer* renderer, ssGUI::GUIObject* obj)
    {
        CurrentRenderer = renderer;
        CurrentObject = obj;
    }

    void ExtensionManager::AddExtension(ssGUI::Extensions::Extension* extension)
    {
        if(IsExtensionExist(extension->GetExtensionName()))
            return;

        Extensions[extension->GetExtensionName()] = extension;
        ExtensionsDrawOrder.push_back(extension->GetExtensionName());
        ExtensionsUpdateOrder.push_back(extension->GetExtensionName());
        extension->BindToObject(CurrentObject);
        CurrentRenderer->RedrawObject();
    }

    ssGUI::Extensions::Extension* ExtensionManager::GetExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return nullptr;

        return Extensions[extensionName];
    }

    std::vector<ssGUI::Extensions::Extension*> ExtensionManager::GetListOfExtensions()
    {
        std::vector<ssGUI::Extensions::Extension*> returnVector = std::vector<ssGUI::Extensions::Extension*>();
        
        for(auto extension : Extensions)
            returnVector.push_back(extension.second);
        
        return returnVector;
    }
    

    bool ExtensionManager::IsExtensionExist(std::string extensionName) const
    {
        return Extensions.find(extensionName) != Extensions.end();
    }

    void ExtensionManager::RemoveExtension(std::string extensionName)
    {
        if(!IsExtensionExist(extensionName))
            return;
        ssGUI::Extensions::Extension* targetExtension = (*Extensions.find(extensionName)).second;
        // ssGUI::ObjectsReferences* ptr = targetExtension->Internal_GetObjectsReferences();
        // if(ptr != nullptr)
        //     ptr->CleanUp();
        ExtensionsDrawOrder.erase(ExtensionsDrawOrder.begin() + GetExtensionDrawOrder(extensionName));
        ExtensionsUpdateOrder.erase(ExtensionsUpdateOrder.begin() + GetExtensionUpdateOrder(extensionName));
        Extensions.erase(extensionName);
        ssGUI::Factory::Dispose(targetExtension);
        CurrentRenderer->RedrawObject();
    }

    int ExtensionManager::GetExtensionsCount() const
    {
        return ExtensionsDrawOrder.size();
    }

    int ExtensionManager::GetExtensionDrawOrder(std::string extensionName) const
    {
        if(!IsExtensionExist(extensionName))
            return -1;
        
        auto it = find(ExtensionsDrawOrder.begin(), ExtensionsDrawOrder.end(), extensionName);

        if(it == ExtensionsDrawOrder.end())
            return -1;
        else
            return it - ExtensionsDrawOrder.begin();
    }

    void ExtensionManager::ChangeExtensionDrawOrder(std::string extensionName, int order)
    {
        int drawIndex = GetExtensionDrawOrder(extensionName);

        if(drawIndex == -1)
            return;
        
        if(order < 0 || order >= GetExtensionsCount())
            return;

        //https://stackoverflow.com/questions/45447361/how-to-move-certain-elements-of-stdvector-to-a-new-index-within-the-vector
        if (drawIndex > order)
            std::rotate(ExtensionsDrawOrder.rend() - drawIndex - 1, ExtensionsDrawOrder.rend() - drawIndex, ExtensionsDrawOrder.rend() - order);
        else        
            std::rotate(ExtensionsDrawOrder.begin() + drawIndex, ExtensionsDrawOrder.begin() + drawIndex + 1, ExtensionsDrawOrder.begin() + order + 1);
        
        CurrentRenderer->RedrawObject();
    }

    int ExtensionManager::GetExtensionUpdateOrder(std::string extensionName) const
    {
        if(!IsExtensionExist(extensionName))
            return -1;
        
        auto it = find(ExtensionsUpdateOrder.begin(), ExtensionsUpdateOrder.end(), extensionName);

        if(it == ExtensionsUpdateOrder.end())
            return -1;
        else
            return it - ExtensionsUpdateOrder.begin();
    }

    void ExtensionManager::ChangeExtensionUpdateOrder(std::string extensionName, int order)
    {
        int updateIndex = GetExtensionUpdateOrder(extensionName);

        if(updateIndex == -1)
            return;
        
        if(order < 0 || order >= GetExtensionsCount())
            return;

        //https://stackoverflow.com/questions/45447361/how-to-move-certain-elements-of-stdvector-to-a-new-index-within-the-vector
        if (updateIndex > order)
            std::rotate(ExtensionsUpdateOrder.rend() - updateIndex - 1, ExtensionsUpdateOrder.rend() - updateIndex, ExtensionsUpdateOrder.rend() - order);
        else        
            std::rotate(ExtensionsUpdateOrder.begin() + updateIndex, ExtensionsUpdateOrder.begin() + updateIndex + 1, ExtensionsUpdateOrder.begin() + order + 1);
    
        CurrentRenderer->RedrawObject();
    }

}